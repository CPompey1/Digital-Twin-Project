#!/usr/bin/env python

import time
import serial
WORD = 32


def main():
    ser = serial.Serial(
        port = '/dev/ttyS0',
        baudrate = 9600,
        parity = serial.PARITY_NONE,
        stopbits = serial.STOPBITS_ONE,
        bytesize = serial.EIGHTBITS
    )
    while true:
        #Check if there is any data from serial ports
        if(ser.in_waiting > 0):
            #If there is data, parse it
            temp = bytes(ser.read_until(size=WORD))

            parseInput(WORD,temp)

        #Ask to send a message
        tempClause = input("Do you want to send a nessage?(y/n)")

        #If y, ask to enter message
        if (tempClause == "Y" or tempClause == "y"):
            bufferOut = input("Enter message:")

            #Pass input message to parseOutput
            parseOutput(WORD, bufferOut)
        #If anything else proceed on 


def parseInput(wordSize, bufferIn):
    #Check first byte of input buffer
    flag = bufferIn[0]

    #Switching statement for parsing different types of flags
    match flag:
        case '~':
            bufferInL = list(bufferIn)
            bufferInL.remove(ord('~'))
            print(bytes(bufferInL).decode())


        case default:
            print("Invalid or Unimplemented message type")
    #Flush input buffer
    

def parseOutput(wordSize, message):
    #Get first byte
    #Switching for statement for how to pack input message based on flag
    pass








if __name__ == "main":
    main()