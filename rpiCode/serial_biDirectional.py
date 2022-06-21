#!/usr/bin/env python

import time
import serial
WORD = 32

ser = serial.Serial(
        port = '/dev/ttyS0',
        baudrate = 9600,
        parity = serial.PARITY_NONE,
        stopbits = serial.STOPBITS_ONE,
        bytesize = serial.EIGHTBITS
    )

                
def parseInput(wordSize, bufferIn):

    #convert string to series of bytes
    bytes(bufferIn)

    #Check first byte of input buffer
    flag = bufferIn[0]

    #Switching statement for parsing different types of flags
    if (flag== '~'):
        bufferInL = list(bufferIn)
        bufferInL.remove(ord('~'))
        bufferInB = bytes(bufferInL)
        print(bufferInB.decode)


    else:
        print("Invalid or Unimplemented message type")
        #Flush input buffer
    

def parseOutput(wordSize, message):
    #convert string to series of byes
    messageT = bytes(message)

    #Get first byte
    flag = messageT[0]

    #Switching for statement for how to pack input message based on flag
    #If its a simple string message send directly to serial ports
    if flag == '~':
        ser.write(messageT)

def main():
    while True:
    #Check if there is any data from serial ports
        if(ser.in_waiting > 0):
            print("test1")
            #If there is data, parse it
            temp = ser.read_until(size=WORD)
            parseInput(WORD,temp)
            
        #Ask to send a message
        tempClause = input("Do you want to send a nessage?(y/n)")

        #If y, ask to enter message
        if (tempClause == "Y" or tempClause == "y"):
            bufferOut = input("Enter message:")
            
            #Pass input message to parseOutput
            parseOutput(WORD,bufferOut)
            
if __name__ == '__main__':
    main()
