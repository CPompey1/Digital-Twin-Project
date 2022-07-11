import serial_biDirectionalScript as serBds
import WSScript as wsBds
import time
import threading


def  main():
    print("starting main")
    #Controls access of json data to be sent to cloud
    dataLock = threading.Lock()

    print("initializing threads")
    serBdsT = threading.Thread(target=serBds.mainFunc, args=(dataLock,))
    print("initialized serbds thread")
    wsBdsT = threading.Thread(target=wsBds.mainFunc, args=((dataLock,)))
    running = True
    print("Initialized threads")
    serBdsT.start()
    wsBdsT.start()
    print("b4 loop")
    while(running):

        #Prompt for message while scripts run
        tempMessage = input("1)Enter message to send to serial(or exit): ")

        if tempMessage == "exit":
            running = False
        else:
            serBds.parseOutput(tempMessage)
    
    serBdsT.join()
    wsBdsT.join()
    

if __name__ == "__main__":
    main()
        
