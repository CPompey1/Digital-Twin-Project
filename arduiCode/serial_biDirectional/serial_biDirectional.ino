
/********************************Includes*********************************************/
#include <string.h>

/********************************Constants*********************************************/
#define WORD 32 

/********************************Flags*********************************************/
//ASCII charactors 1-10 will be reserved as flags for types of sensors
#define A2RD 11    //Arduino to Raspberry pi data object 
#define A2RC 22    //Arduino to Raspberry pi command
#define A2RS '>'  //Arduino to Raspberry pi String message
#define R2AD 14    //Raspberry pi to Arduino data object
#define R2AC 15    //Raspberry pi to Arduinp command
#define R2AS '<'    //Raspberry pi to arduino string message

/***********************************************************************************
Program Name: biDirectional Communication via Serial Ports
Author: Cristian Pompey
Date Created: 6/8/2022
Last Edit: 6/8/2022
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
byte recBuf[WORD];               //64 byte buffer to hold a received data
byte sendBuf[WORD];               //Buffer to hold data to be sent
byte flag = 0;
short numBytesRec = 0;         //Number of bytes received data buffer
boolean dataAvailable = false;


    
void setup(){
    Serial.begin(9600);
    //attatch hardware timer to detectData function (lib needs to be installed)
}

void loop(){

    //Check and Parse data received through serial
    while (numBytesRec = Serial.available() > 0){
      Serial.readBytes(recBuf,numBytesRec);
      Serial.write(byte(recBuf));
      parseRecData(recBuf);
      flushBuf(recBuf);
    }

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
void parseRecData(byte *buf){
  char temp[WORD -1];

  switch(buf[0]){

    //If its a string from the serial monitor, echo it and send to external device
    case A2RS:
      Serial.write(buf,WORD);
      strncpy(temp,buf[1],WORD -1);
      Serial.println(temp);

    //If it is a command from the serial monitor, send it directly to external device
    case A2RC:
      Serial.write(buf,WORD);

    //If it is a data object from external device, parse it
    case R2AD:
      parseRpiDat(buf);

    //If it a string from external device, print it to serial monitor
    case R2AS:
      strncpy(temp,buf[1],WORD - 1);
      Serial.println(temp);
      
      
  //If it is a command from external device then parse it as such
    case R2AC:
      parseRpiCmd(buf);
    
  }
         
}
  
  


/* Checks if there is any data available from local 
   sensors and packs it into send buffer */
void detectData(){

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
  
}
