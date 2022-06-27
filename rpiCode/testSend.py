#r/bin/env python
import time
import json
import serial
from websocket import create_connection

ws = create_connection("wss://cvyykl1zo6.execute-api.us-east-1.amazonaws.com/prod",header={'x-api-key':'zQXx6dS25g1osj74gXAL51nAmdAOBANL2gAvR3O8'})


ser = serial.Serial(
    port = '/dev/ttyS0',
    baudrate = 9600,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS
)

ser.write(b'-dumDat')
ser.flushInput()
bufferIn = ser.read_until(size = 2)
out = {
    "action" : "upload_data",
    "epoch_time": (int(time.time() * 1000),
    "data": {
        "temperature" : {
            "degrees_celsius" : int(bufferIn[1])
        }
    }
}
json_obj = json.dumps(out)
ws.send(json_obj)
print(json_obj)
response = ws.recv()
print(response)
ser.close()
