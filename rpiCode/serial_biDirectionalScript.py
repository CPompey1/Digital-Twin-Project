#!/usr/bin/env python

import time
import serial
import json
import threading

WORD = 32
A2RD = 11
R2AS = '<'
DUMD = 1
A2RS = '>'
R2AC = '-'


ser = serial.Serial(
        port = '/dev/ttyS0',
        baudrate = 9600,
        parity = serial.PARITY_NONE,
        stopbits = serial.STOPBITS_ONE,
        bytesize = serial.EIGHTBITS
    )

                
def parseInput(wordSize, bufferIn,dataLock):


    #Check first byte of input buffer
    flag = bufferIn[0]

    #Switching statement for parsing different types of flags
    #if the arduino sent a string
    if (flag== A2RS):
        bufferInL = list(bufferIn,'ASCII')
        bufferInL.remove(ord('~'))
        bufferInB = bytes(bufferInL,'ASCII')
        print(bufferInB.decode('ASCII'))

    #If the arduino sent a data object
    elif(int(flag) == A2RD):
        #write to jsosn file
        out = {
            "action" : "upload_data",
            "epoch_time": int(time.time()) * 1000,
            "data": {
                "temperature" : {
                   "degrees_celsius" : int(bufferIn[1])
                }
            }
        }
        json_obj = json.dumps(out)

        dataLock.acquire()
        with open("dat.json","w") as outfile:
            outfile.write(json_obj)
        #Flush input buffer

        dataLock.release()
        
    
def parseOutput(message):
    #Get first byte
    flag = message[0]

    #convert string to series of byes
    messageB = bytes(message,'ASCII')
    

    #Switching for statement for how to pack input message based on flag
    #If its a simple string message 
    if flag == R2AS:
        # Send directly to serial ports
        ser.write(messageB)

        #Arduino echoes srting so wait and flush
        time.sleep(2)
        ser.flushInput()

    #If its a command
    elif flag == R2AC:
        #In future should parse convert typed command string
        #formatted command data obj instead of arduino
        #doing work.
        ser.write(messageB)
def mainFunc(dataLock):
    while True:
    #Check if there is any data from serial ports
        if(ser.in_waiting > 0):
            time.sleep(.5)
            print(ser.in_waiting)
            #If there is data, parse it
            temp = ser.read_until(size=ser.in_waiting)
            parseInput(WORD,temp,dataLock)
            ser.flushInput()

