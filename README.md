# Digital Twin Project RPi/Arduino communication
---
## Introduction
This repository contains two different folders which will hold the code for the raspbrry pi layer(rpiCode) of the project and the code for the ardunio layer (arduiCode). Currently those folders hold simple scripts for sending and receiving data via the respective board's serial port. For the arduino these are titled serial_read.ino and serial_write.ino. For the raspberry pi these are titled serial_read_script.py and serial_write_script.py. The remaining files are currently being worked on to enable realtime bi-directional communication within one script for each board. -update needed

## Hierarchy
 -rpiCode  
<ul>
<li>serial_write_script.py  
<li>serial_read_script.py  
</ul>
-arduiCode
<ul>  
<li>serial_read.ino  
<li>serial_write.ino 
<li>serial_biDirectional.ino
</ul>

