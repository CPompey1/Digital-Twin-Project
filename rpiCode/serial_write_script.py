#!/usr/bin/env python

#This script sends the string "Hello from RPi" through the serial port of the tx/rx pins. 
#The device name of the serial port is serial0. ttyS0 by default points to this port. 
import time
import serial

ser = serial.Serial(
    port = '/dev/ttyS0',
    baudrate = 9600,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS
)

while True:
    ser.write(b'Hi From RPi')
    time.sleep(2)
