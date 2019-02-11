# Lego Projects
Software for an embedded real-time system for a self-driving bus buit with LEGO MINDSTORMS. The bus uses a nxt ultrasonic sensor and a nxtCamV4 to sense its surroundings and ensure it stays within its road lanes. It uses one motor to drive and another motor to control where it turns, and is implemented on the nxt brick component on the real-time operating system nxtOSEK. 

## Software 
The driving and sensor-near software implements classes for calibrating, using and communicating with the ultrasonic sensor, driving motor, turing motor and the nxtCamV4, as well as algorithms to drive the bus corresponding to the input it receives. 

## BusRemoteTool
The BusRemoteTool is a piece of software created to remote control the bus from a computer, and shares data with the nxt brick over a USB cable. It was highly useful to debug and troubleshoot the input of the nxt cam sensor and the output of the algorithm that decides which way to drive. 

## Scheduling
We have implemented the real-time requirement of the proejct by implementing the software running on the nxt brick with a fixed-priority approach, using the Osek Implementation Language (oil). Additionally, the real-time implementation has been simulated to never exceed any scheduling deadline using the modeling (validation and verification) software UPPAAL, meaning that it will always be able to stop in time for any sudden road obstructions (in theory, though this highly depends upon the quality of sensor results). 

## Compilation: Useful Info
Run ```make all``` to compile, ```make clean``` to clear the folder.
To push it to the NXT, make sure you have the USB cable attached and it is in flash mode (While in main menu, hold down enter and left until it shuts down - proceed to turn on), then run ```./appflash.sh```.
If you want to combine these you can chain them by doing ```make all && ./appflash.sh```.

------------------------------------------------------
Created as part of the 5th semester project of Software group 501e17, Comp. Sci, Aalborg University, during spring/summer of 2017. 

Group members:
- Andreas Laugård Hald 
- Jakob Bo Søndergaard Madsen 
- Jonathan Karlsson 
- Kim Larsen 
- Martin Fabrin Karkov
