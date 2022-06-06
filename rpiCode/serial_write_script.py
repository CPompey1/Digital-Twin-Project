#!/usr/bin/env python
import time
import serial

ser = serial.Serial(
    port = '/dev/ttyUSB0',
    baudrate = 9600,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS
)

while True:
    data = input("Enter data to send: ")
    ser.write(data,"/n")
    time.sleep(1)
