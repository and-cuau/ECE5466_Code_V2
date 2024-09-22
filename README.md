To run the code, you will need an android phone, and two ESP-32 microcontrollers.

First, connect one of the ESP-32 board to Arduino IDE. 
In Arduino IDE, open up the file located in the "ESP_Game_Logic" subfolder of this folder called "5466SerialToBT.ino" and load it onto the ESP-32.

Now, disconnect the first ESP-32, and connect the second ESP-32.
Open up the file named "NerfCode.ino" and upload the code to the ESP-32.

The instructions for wiring the various componenets are located in the report.

To setup the Android app -
Open the Arduino program "SimpleBluetoothTerminal-master" in the Arduino IDE. 
Click on the arrow in the top left corner of the IDE (“Upload”). 
Click the green triangular play button in Android Studio to launch the app. 
Tap on the three dots in the top right corner of the screen. 
Tap on “Bluetooth settings” 
Tap on “Pair new device” 
Select “ESP32-Game-Manager” from the list of available devices.  
Select “ESP32-Game-Manager” from the list of Bluetooth devices. 

Once everything is connected and set-up, open up the android app on the android phone.
You are now ready to play!
