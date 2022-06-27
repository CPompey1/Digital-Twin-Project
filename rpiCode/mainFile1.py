import multiprocessing
import time
import serial
import json
from websocket import create_connection

WORD = 32
A2RD = 11
R2AS = '<'
DUMD = 1
A2RS = '>'
R2AC = '-'

dataAval = False
data = "0"
ws = create_connection("wss://cvyykl1zo6.execute-api.us-east-1.amazonaws.com/prod",header={'x-api-key':'zQXx6dS25g1osj74gXAL51nAmdAOBANL2gAvR3O8'})

ser = serial.Serial(
        port = '/dev/ttyS0',
        baudrate = 9600,
        parity = serial.PARITY_NONE,
        stopbits = serial.STOPBITS_ONE,
        bytesize = serial.EIGHTBITS
    )


def main():
    with multiprocessing.Manager() as manager:
        dataAval = False
        bds_main = multiprocessing.Process(target = bdsMainFunc, args=(dataAval,))
        wss_main = multiprocessing.Process(target = wsMainFunc, args=(dataAval,))

        bds_main.start()
        wss_main.start()
        running = True
        while(running):
            #Ask to send a message
            tempClause = input("1)Send message \n2)exit \n")
            #If y, ask to enter message
            if (tempClause == "1"):
                bufferOut = input("Enter message:")
                
                #Pass input message to parseOutput
                
                parseOutput(bufferOut)
            elif(tempClause == "2"):
                running = False
            
        bds_main.kill()
        wss_main.kill()

def parseInput(wordSize, bufferIn,dataAval):


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
            "epoch_time": time.time(),
            "data": {
                "temperature" : int(bufferIn[1]),
            }
        }
        json_obj = json.dumps(out)
        with open("dat.json","w") as outfile:
            outfile.write(json_obj)
        #Flush input buffer

        dataAval = True
        
    
def parseOutput(message):
    #convert string to series of byes
    messageT = bytes(message,'ASCII')

    #Get first byte
    flag = messageT[0]

    #Switching for statement for how to pack input message based on flag
    #If its a simple string message 
    if flag == R2AS:
        # Send directly to serial ports
        ser.write(messageT)

        #Arduino echoes srting so wait and flush
        time.sleep(2)
        ser.flushInput()

    #If its a command
    elif flag == R2AC:
        #In future should parse convert typed command string
        #formatted command data obj instead of arduino
        #doing work.
        ser.write()
        
def wsMainFunc(dataAval):
    while True:
        if dataAval:
            with open('dat.json','r') as openfile:
                data = openfile.read()
                print(data)
            response = ws.recv()
            print(response)
def bdsMainFunc(dataAval):
    while True:
    #Check if there is any data from serial ports
        if(ser.in_waiting > 0):
            #If there is data, parse it
            temp = ser.read_until(size=ser.in_waiting)
            parseInput(WORD,temp,dataAval)
            ser.flushInput()

if __name__ == "__main__":
    main()
