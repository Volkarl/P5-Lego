# Bus Software for NXT
By SW501E17. You need to have NXT fantom drivers installed to be able to run BusRemoteTool and the flash tool for the NXT.

## nxtOSEK Compilation
Run ```make all``` to compile, ```make clean``` to clear the folder.
To push it to the NXT, make sure you have the USB cable attached and it is in flash mode (While in main menu, hold down enter and left until it shuts down - proceed to turn on), then run ```./appflash.sh```.
If you want to combine these you can chain them by doing ```make all && ./appflash.sh```.

## BusRemoteTool Compilation
Requires Qt to compile.