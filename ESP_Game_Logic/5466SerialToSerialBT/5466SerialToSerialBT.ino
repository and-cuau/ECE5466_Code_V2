//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"

//#define USE_PIN // Uncomment this to use PIN during pairing. The pin is specified on the line below
const char *pin = "1234"; // Change this to more secure PIN.

String device_name = "ESP32-Game-Manager";

int game = 0;

const byte BUTTON_0 = 18;
const byte BUTTON_1 = 19;
const byte BUTTON_2 = 21;
const byte BUTTON_3 = 17;
const byte BUTTON_4 = 5;

int delay_time;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name); //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  //Serial.printf("The device with name \"%s\" and MAC address %s is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getMacString()); // Use this after the MAC method is implemented
  #ifdef USE_PIN
    SerialBT.setPin(pin);
    Serial.println("Using PIN");
  #endif
  pinMode(BUTTON_0, OUTPUT);
  pinMode(BUTTON_1, OUTPUT);
  pinMode(BUTTON_2, OUTPUT);
  pinMode(BUTTON_3, OUTPUT);
  pinMode(BUTTON_4, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    //Serial.write(SerialBT.read());
    String recievedMessage = SerialBT.readStringUntil('\n');
    if (recievedMessage.equals("1play")) {
      game = 1;
    }
  }

  if (game == 1) {
    int targetButton = random(5);
    if (targetButton == 0) {
      digitalWrite(BUTTON_0, HIGH);
    }
    else if (targetButton == 1) {
      digitalWrite(BUTTON_1, HIGH);
    }
    else if (targetButton == 2) {
      digitalWrite(BUTTON_2, HIGH);
    }
    else if (targetButton == 3) {
      digitalWrite(BUTTON_3, HIGH);
    }
    else if (targetButton == 4) {
      digitalWrite(BUTTON_4, HIGH);
    }
    
  }
  delay(200);
}
