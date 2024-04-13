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

const byte BUTTON_0_IN = 34;
const byte BUTTON_1_IN = 32;
const byte BUTTON_2_IN = 33;
const byte BUTTON_3_IN = 39;
const byte BUTTON_4_IN = 5;
const byte BUTTON_0_OUT = 18;
const byte BUTTON_1_OUT = 19;
const byte BUTTON_2_OUT = 17;
const byte BUTTON_3_OUT = 22;
const byte BUTTON_4_OUT = 5;


int delay_time = 500;
int reaction_time = 20000;
int score = 0;
int failed = 0;
bool correct = true;
bool newButton = false;
int i = 0;
int button = -1;
int recievedMessage = 0;
int old_button = -2;

int button_0_pressed = 0;
int button_1_pressed = 0;
int button_2_pressed = 0;

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
  pinMode(BUTTON_0_OUT, OUTPUT);
  pinMode(BUTTON_1_OUT, OUTPUT);
  pinMode(BUTTON_2_OUT, OUTPUT);
  pinMode(BUTTON_3_OUT, OUTPUT);
  pinMode(BUTTON_4_OUT, OUTPUT);
  pinMode(BUTTON_0_IN, INPUT);
  pinMode(BUTTON_1_IN, INPUT);
  pinMode(BUTTON_2_IN, INPUT);

  Serial.setTimeout(50);
  SerialBT.setTimeout(100);
}

void loop() {
 /* if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  */
  if (Serial.available()) {
    int recievedRead = Serial.parseInt();
    Serial.println(recievedRead);
    if (recievedRead != 0) {
      recievedMessage = recievedRead;
    }
    Serial.println(recievedMessage);
    if (recievedMessage == 10) {
      game = 2;
      failed = 0;
      i = 0;
      recievedMessage = 0;
      Serial.println("Start 2 Player");
    }
    if (recievedMessage == 9) {
      game = 1;
      failed = 0;
      i = 0;
      recievedMessage = 0;
    }
    if (recievedMessage == 1) {
        button = 0;
        i = 0;
        Serial.println("button 1 on");
      }
      else if (recievedMessage == 2) {
        i = 0;
        button = 1;
        Serial.println("button 2 on");
      }
      else if (recievedMessage == 3) {
        i = 0;
        button = 2;
        Serial.println("button 3 on");
      }
  }
  if (SerialBT.available()) {
    newButton = false;
    int recievedRead = SerialBT.parseInt();
    Serial.println(recievedRead);
    if (recievedRead != 0) {
      recievedMessage = recievedRead;
      newButton = true;
    }
    Serial.println(recievedMessage);
    if (recievedMessage == 10) {
      game = 2;
      failed = 0;
      i = 0;
      recievedMessage = 0;
      Serial.println("Start 2 Player");
    }
    if (recievedMessage == 9) {
      game = 1;
      failed = 0;
      i = 0;
      recievedMessage = 0;
    }
    if (recievedMessage == 1) {
      if (button != -1 && newButton) {
        failed++;
        }
        button = 0;
        Serial.println("button 1 on");
        i = 0;
       }
      else if (recievedMessage == 2) {
        if (button != -1 && newButton) {
        failed++;
        }
        button = 1;
        Serial.println("button 2 on");
        i = 0;
       }
      else if (recievedMessage == 3) {
        if (button != -1 && newButton) {
        failed++;
        }
        button = 2;
        Serial.println("button 3 on");
        i = 0;
      }
  }

//1-player game
  if (game == 1 && failed < 2) {
    button_0_pressed = digitalRead(BUTTON_0_IN);
    button_1_pressed = digitalRead(BUTTON_1_IN);
    button_2_pressed = digitalRead(BUTTON_2_IN);
    if (i < reaction_time) {
      if (button == 0) {
        digitalWrite(BUTTON_0_OUT, HIGH);
        digitalWrite(BUTTON_1_OUT, LOW);
        digitalWrite(BUTTON_2_OUT, LOW);
      }
      else if (button == 1) {
        digitalWrite(BUTTON_1_OUT, HIGH);
        digitalWrite(BUTTON_0_OUT, LOW);
        digitalWrite(BUTTON_2_OUT, LOW);
      }
      else if (button == 2) {
        digitalWrite(BUTTON_0_OUT, LOW);
        digitalWrite(BUTTON_1_OUT, LOW);
        digitalWrite(BUTTON_2_OUT, HIGH);
      }
      if (button_0_pressed == HIGH) {
        if (button == 0) {
          i = 0;
          digitalWrite(BUTTON_0_OUT, LOW);
          score++;
          delay(delay_time);
          button = random(3);
          delay(25);
        }
        else {
          button_miss();
          button = random(3);
          delay(25);
        }
      }
      if (button_1_pressed == HIGH) {
        if (button == 1) {
          i = 0;
          digitalWrite(BUTTON_1_OUT, LOW);
          delay(delay_time);
          score++;
          button = random(3);
          delay(25);
        }
        else{
          button_miss();
          button = random(3);
          delay(25);
        }
      }
      if (button_2_pressed == HIGH){
        if (button == 2) {
          i = 0;
          digitalWrite(BUTTON_2_OUT, LOW);
          delay(delay_time);
          score++;
          button = random(3);
          delay(25);
        }
        else{
          button_miss();
          button = random(3);
          delay(25);
        }
      }
      i++;
      //Serial.println(i);
    }
    else if (i == reaction_time) {
    SerialBT.write('2');
    digitalWrite(BUTTON_0_OUT, LOW);
    digitalWrite(BUTTON_1_OUT, LOW);
    digitalWrite(BUTTON_2_OUT, LOW);
    i++;
    }
  }


//2 Player Game
  if (game == 2 && failed < 2) {
    button_0_pressed = digitalRead(BUTTON_0_IN);
    button_1_pressed = digitalRead(BUTTON_1_IN);
    button_2_pressed = digitalRead(BUTTON_2_IN);
    
    if (i < reaction_time) {
      Serial.print(button);
      Serial.println(" should be on");
      if (button == 0) {
        digitalWrite(BUTTON_0_OUT, HIGH);
        digitalWrite(BUTTON_1_OUT, LOW);
        digitalWrite(BUTTON_2_OUT, LOW);
      }
      else if (button == 1) {
        digitalWrite(BUTTON_1_OUT, HIGH);
        digitalWrite(BUTTON_0_OUT, LOW);
        digitalWrite(BUTTON_2_OUT, LOW);
      }
      else if (button == 2) {
        digitalWrite(BUTTON_0_OUT, LOW);
        digitalWrite(BUTTON_1_OUT, LOW);
        digitalWrite(BUTTON_2_OUT, HIGH);
      }
      if (button_0_pressed == HIGH) {
        if (button == 0) {
          i = 0;
          button = -1;
          digitalWrite(BUTTON_0_OUT, LOW);
          score++;
          SerialBT.write(score);
          delay(500);
        }
        else {
          button_miss();
          button = -1;
        }
      }
      if (button_1_pressed == HIGH) {
        if (button == 1) {
          i = 0;
          button = -1;
          digitalWrite(BUTTON_1_OUT, LOW);
          delay(500);
          score++;
          SerialBT.write(score);
        }
        else {
          button_miss();
          button = -1;
        }
      }
      if (button_2_pressed == HIGH){
        if (button == 2) {
          i = 0;
          button = -1;
          digitalWrite(BUTTON_2_OUT, LOW);
          delay(500);
          score++;
          SerialBT.write(score);
        }
        else {
          button_miss();
          button = -1;
        }
      }
      i++;
      //Serial.println(i);
    }
    else if (i == reaction_time) {
    SerialBT.write('g');
    digitalWrite(BUTTON_0_OUT, LOW);
    digitalWrite(BUTTON_1_OUT, LOW);
    digitalWrite(BUTTON_2_OUT, LOW);
    i++;
    }
  }
  
}

void button_miss() {
     failed += 1;
     SerialBT.write('m');
     /*
     digitalWrite(BUTTON_0_OUT, LOW);
     digitalWrite(BUTTON_1_OUT, LOW);
     digitalWrite(BUTTON_2_OUT, LOW);
     delay(100);
     digitalWrite(BUTTON_0_OUT, HIGH);
     digitalWrite(BUTTON_1_OUT, HIGH);
     digitalWrite(BUTTON_2_OUT, HIGH);
     delay(100);
     digitalWrite(BUTTON_0_OUT, LOW);
     digitalWrite(BUTTON_1_OUT, LOW);
     digitalWrite(BUTTON_2_OUT, LOW);
     */
}

void button_correct() {
  i = 0;
  button = -1;
  score++;
  delay(delay_time);
  if (score % 5 == 0) {
    reaction_time -= 500;
  }
}
