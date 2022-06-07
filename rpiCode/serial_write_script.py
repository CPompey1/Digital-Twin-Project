#!/usr/bin/env python
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
    #data = input("Enter data to send: ")
    #data = data + '\n'
    ser.write(b'hi0')
    time.sleep(2)
