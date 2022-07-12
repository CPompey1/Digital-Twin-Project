
/***********************************************************************************
Program Name: biDirectional Communication via Serial Ports
Author: Cristian Pompey
Date Created: 6/8/2022
Last Edit: 6/27/2022
Description: This program will set up and run a basic bi-directional communication
             scheme with an expected raspberry pi device. The arduino will get information
             local sensors, and communicate this data to a raspberry pi device via serial 
             communication through the rx/tx pins.

             Initial implementation: At the time of creation it is not known what channel of 
             communication the sensors will use to communicate with the host arduino. Thus we abstract
             this through a function that will detect and start a process of response when  
             data from any local sensor is availiable. Each sensor will be identified by an integer
             that will be represented as a short data type in the code. When there is data availiable from 
             the RPi through the serial ports, we will parse it as a command or simple string to display.
***********************************************************************************************/

/********************************Includes*********************************************/
#include <string.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


/********************************Constants*********************************************/
#define WORD 64 
#define TCADRS 0x70  //Address for i2c perifphial
#define FCF    19.5  //calibration factor for flexiforce sensor
#define MPU0 1        
#define MPU1 2
#define MPU2 7

/********************************Flags*********************************************/
//ASCII charactors 1-10 will be reserved as flags for types of sensors
#define A2RD 11    //Arduino to Raspberry pi data object 
#define A2RC 22    //Arduino to Raspberry pi command
#define A2RS '>'  //Arduino to Raspberry pi String message
#define R2AD 14    //Raspberry pi to Arduino data object
#define R2AC '-'    //Raspberry pi to Arduinp command
#define R2AS '<'    //Raspberry pi to arduino string message

/******************************** Objects and Variables*********************************************/
//MPU sensors

Adafruit_MPU6050 mpu[3] = {Adafruit_MPU6050(), Adafruit_MPU6050(), Adafruit_MPU6050()};


int FS = A0;          //flexi force sensor
byte recBuf[WORD];               //64 byte buffer to hold a received data
byte sendBuf[WORD];               //Buffer to hold data to be sent
byte flag = 0;
short numBytesRec = 0;         //Number of bytes received data buffer
boolean dataAvailable = false;

//Commands
#define DUMD "dumDat"


void setup(){
    Serial.begin(9600);
    pinMode(FS,INPUT);
    //attatch hardware timer to detectData function (lib needs to be installed)
}

void loop(){

    //If there is sensor data available, format it and send it through serial
    while (dataAvailable){
      parseSensData(sendBuf);
      Serial.write(sendBuf[WORD]);
      flushBuf(sendBuf);
    }
    
   
}

/* Will parse data from sensors  and pack into a formatted series of bytes: sendBuf sendable to 
   RPi */
void parseSensData(byte *buf){

}  
/*
 * Method name: parceRecData
 * Description: Will check if received data on serial buffer is from an external device or the
 * serial moniter. If it is from an external device it will parse it as either a command or 
 * string message. If it is from the serial monitor, then it will send the data to the external device over
 * serial ports.
 */
void parseRecData(size_t len){
  char temp[WORD -1];

  //Read in bytes from buffer
  Serial.readBytes(recBuf,len);

  //Print incoming byte series as test line
  ////strncpy(temp,recBuf,len);
  //temp[len] = "\0";
  //Serial.print("Incoming Bytes: ");
  //Serial.println(temp);

  switch(recBuf[0]){

    //If its a string  and send to external device
    case A2RS:
      strncpy(temp,&recBuf[1],len);
      temp[len] = "\0";
      //Serial.print("Sending String: ");
      Serial.println(temp);
      

      //Echo to RPi
      Serial.write(recBuf,len);
      break;

    //If it is a command from the serial monitor, send it directly to external device
    case A2RC:
      Serial.write(recBuf,WORD);
      break;

    //If it is a data object from external device, parse it
    case R2AD:
      parseRpiDat(recBuf);
      break;

    //If it a string from external device, print it to serial monitor
    case R2AS:
      strncpy(temp,&recBuf[1],len);
      temp[len] = "\0";
      Serial.print("Received String: ");
      Serial.println(temp);
      break;
      
      
  //If it is a command from external device then parse it as such
    case R2AC:
      parseRpiCmd(&recBuf[1]);
      break;
    
  }

  flushBuf(recBuf);
         
}
  
void serialEvent(){
  delay(300);
  //Get num of bytes in buffer
  size_t numBytesRec = Serial.available();

  //Pass size to rec Data func
  parseRecData(numBytesRec);
}


/* Checks if there is any data available from local 
   sensors and packs it into send buffer */
void detectData(){
  sensors_event_t a,g,foo;
  
  //Get pressure data
  int fdata = analogRead(FS);
  float vout = fdata * FCF;

  //Pack into send buf
  *(float *)sendBuf = vout; 

  //Get first MPU data
  tcaselect(MPU0);
  mpu[0].getEvent(&a,&g,&foo);
  *(int *)(sendBuf+4) = a.acceleration.x;
  *(int *)(sendBuf+8) = a.acceleration.y;
  *(int *)(sendBuf+12) = a.acceleration.z;

  *(int *)(sendBuf+16) = g.gyro.x;
  *(int *)(sendBuf+20) = g.gyro.y;
  *(int *)(sendBuf+24) = g.gyro.z;

  //Get second MPU data
  tcaselect(MPU1);
  mpu[1].getEvent(&a,&g,&foo);
  *(int *)(sendBuf+28) = a.acceleration.x;
  *(int *)(sendBuf+32) = a.acceleration.y;
  *(int *)(sendBuf+36) = a.acceleration.z;

  *(int *)(sendBuf+40) = g.gyro.x;
  *(int *)(sendBuf+44) = g.gyro.y;
  *(int *)(sendBuf+48) = g.gyro.z;

  //Get 3rd MPU data
  tcaselect(MPU2);
  mpu[2].getEvent(&a,&g,&foo);
  *(int *)(sendBuf+28) = a.acceleration.x;
  *(int *)(sendBuf+32) = a.acceleration.y;
  *(int *)(sendBuf+36) = a.acceleration.z;

  *(int *)(sendBuf+40) = g.gyro.x;
  *(int *)(sendBuf+44) = g.gyro.y;
  *(int *)(sendBuf+48) = g.gyro.z;

  
  
  //Send data pver serial
  Serial.write(sendBuf,54);
  flushBuf(sendBuf);
}

//sets all entries in byte array to 0
void flushBuf(byte *arr){
  for (int i = 0; i < WORD; i++){
    arr[i] = 0;
  }
}

//Parse data from external device
void parseRpiDat(byte *buf){
  
}

//Parses command from external device
void parseRpiCmd(byte *buf){
  //Entering parse cmd
  short temp = 1;

  //Temporary fix 
  if (strcmp(DUMD,buf) == 0){
    dumD(); 
  }

  //Ideally would be a switching statement here
  //Search for valid command
  /*switch (buf[0]){
    case DUMD:
      dumD();
      break;

  }
  */
  flushBuf(sendBuf);
}

void dumD(){
  //Entering an receiving dummy dat
  //Serial.println("Entering dumm dat func");
  //Format dummy data
  byte temp = 42;
  sendBuf[0] = A2RD;
  sendBuf[1] = temp;
  Serial.write(sendBuf,2);
}

/**********************************************************
 * This function sets up each mpu sensor that is connected to the tca expander.
 * It seleects each individual mpu sensor by telling the tca expander which 
 * MPU it wants to communicate to, then sets the configuration settings of that MPU sensor.
 */
void setUpMPU(){
  tcaselect(MPU0);
  mpu[0].setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu[0].setGyroRange(MPU6050_RANGE_500_DEG);
  mpu[0].setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  tcaselect(MPU1);
  mpu[1].setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu[1].setGyroRange(MPU6050_RANGE_500_DEG);
  mpu[1].setFilterBandwidth(MPU6050_BAND_21_HZ);

  tcaselect(MPU2);
  mpu[2].setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu[2].setGyroRange(MPU6050_RANGE_500_DEG);
  mpu[2].setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void tcaselect(uint8_t selector) {
  
  if (selector > 7) return;
  Wire.beginTransmission(TCADRS);
  Wire.write(1 << selector);
  Wire.endTransmission();
}
  
