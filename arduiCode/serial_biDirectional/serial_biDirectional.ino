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
void setup(){
    Serial.begin(9600);
}

void loop(){
    byte flag = 0;
    short i = 0;                //Counter
    short n = 0;                //Error holder
    byte buf[64];               //64 byte buffer to hold a received data
    byte buf[64];               //Buffer to hold data to be sent
    char str[64];               //Buffer to hold received string from RPi
    boolean dataAvailable = False;

    //If there is sensor data, call response function based on data flag
    while (flag = detectData() != 0){
        switch (flag){
        }
          
    }

    //If the RPi said something, parse it (command or string?)
    while (Serial.availiable() > 0){
            str = Serial.readString();
            switch


    }
    
}

/* Will parse data from a particular sensor and pack into a series of bytes sendable to 
   RPi */
void parseData1(byte flag){

}  

void 



/* Checks if there is any data available from local 
   sensors and returns appropriate flag based on
   type of sensor data */
byte detectData(){

}

//sets all entries in byte array to 0
void flushBuf(*byte){

}