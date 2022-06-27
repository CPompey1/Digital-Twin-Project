import multiprocessing
import serial_biDirectionalScript as bds
import WSScript as wsS

def main():
    with multiprocessing.Manager() as manager:
        dataAval = False
        bds_main = multiprocessing.Process(target = bds.mainFunc, args=(dataAval,))
        wss_main = multiprocessing.Process(target = wsS.mainFunc, args=(dataAval,))

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
                
                bds.parseOutput(bufferOut)
            elif(tempClause == "2"):
                running = False
            
            bds_main.kill()
            wss_main.kill()
        

if __name__ == "__main__":
    main()
