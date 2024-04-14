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
const byte BUTTON_4_IN = 36;
const byte BUTTON_5_IN = 35;
const byte BUTTON_6_IN = 25;
const byte BUTTON_7_IN = 26;
const byte BUTTON_0_OUT = 18;
const byte BUTTON_1_OUT = 19;
const byte BUTTON_2_OUT = 17;
const byte BUTTON_3_OUT = 16;
const byte BUTTON_4_OUT = 5;
const byte BUTTON_5_OUT = 4;
const byte BUTTON_6_OUT = 0;
const byte BUTTON_7_OUT = 2;


int delay_time = 100;
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
int button_3_pressed = 0;
int button_4_pressed = 0;
int button_5_pressed = 0;
int button_6_pressed = 0;
int button_7_pressed = 0;

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
  pinMode(BUTTON_5_OUT, OUTPUT);
  pinMode(BUTTON_6_OUT, OUTPUT);
  pinMode(BUTTON_7_OUT, OUTPUT);
  pinMode(BUTTON_0_IN, INPUT);
  pinMode(BUTTON_1_IN, INPUT);
  pinMode(BUTTON_2_IN, INPUT);
  pinMode(BUTTON_3_IN, INPUT);
  pinMode(BUTTON_4_IN, INPUT);
  pinMode(BUTTON_5_IN, INPUT);
  pinMode(BUTTON_6_IN, INPUT);
  pinMode(BUTTON_7_IN, INPUT);

  Serial.setTimeout(50);
  SerialBT.setTimeout(100);
}

void loop() {
 /* if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  */
  if (game > 0 && failed > 1) { game_over(); }
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
      Serial.println("Start 1 Player");
    }
    if (game == 2) {
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
          else if (recievedMessage == 4) {
            if (button != -1 && newButton) {
            failed++;
            }
            button = 3;
            Serial.println("button 4 on");
            i = 0;
          }
          else if (recievedMessage == 5) {
            if (button != -1 && newButton) {
            failed++;
            }
            button = 4;
            Serial.println("button 5 on");
            i = 0;
          }
          else if (recievedMessage == 6) {
            if (button != -1 && newButton) {
            failed++;
            }
            button = 5;
            Serial.println("button 6 on");
            i = 0;
          }
          else if (recievedMessage == 7) {
            if (button != -1 && newButton) {
            failed++;
            }
            button = 6;
            Serial.println("button 7 on");
            i = 0;
          }
          else if (recievedMessage == 8) {
            if (button != -1 && newButton) {
            failed++;
            }
            button = 7;
            Serial.println("button 8 on");
            i = 0;
          }
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
      Serial.println("Start 1 Player");
    }
    if (game == 2) {
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
        else if (recievedMessage == 4) {
          if (button != -1 && newButton) {
          failed++;
          }
          button = 3;
          Serial.println("button 4 on");
          i = 0;
        }
        else if (recievedMessage == 5) {
          if (button != -1 && newButton) {
          failed++;
          }
          button = 4;
          Serial.println("button 5 on");
          i = 0;
        }
        else if (recievedMessage == 6) {
          if (button != -1 && newButton) {
          failed++;
          }
          button = 5;
          Serial.println("button 6 on");
          i = 0;
        }
        else if (recievedMessage == 7) {
          if (button != -1 && newButton) {
          failed++;
          }
          button = 6;
          Serial.println("button 7 on");
          i = 0;
        }
        else if (recievedMessage == 8) {
          if (button != -1 && newButton) {
          failed++;
          }
          button = 7;
          Serial.println("button 8 on");
          i = 0;
        }
    }
  }

//1-player game
  if (game == 1 && failed < 2) {
    button_0_pressed = digitalRead(BUTTON_0_IN);
    button_1_pressed = digitalRead(BUTTON_1_IN);
    button_2_pressed = digitalRead(BUTTON_2_IN);
    button_3_pressed = digitalRead(BUTTON_3_IN);
    button_4_pressed = digitalRead(BUTTON_4_IN);
    button_5_pressed = digitalRead(BUTTON_5_IN);
    button_6_pressed = digitalRead(BUTTON_6_IN);
    button_7_pressed = digitalRead(BUTTON_7_IN);
    if (i < reaction_time) {
      button_lighting();
      
      if (button_0_pressed == HIGH) {
        if (button == 0) {
          digitalWrite(BUTTON_0_OUT, LOW);
          button_correct();
          button = random(8);
        }
        else {
          button_miss();
          button = random(8);
          delay(25);
        }
      }
      else if (button_1_pressed == HIGH) {
        if (button == 1) {
          i = 0;
          digitalWrite(BUTTON_1_OUT, LOW);
          button_correct();
          button = random(8);
        }
        else{
          button_miss();
          button = random(8);
          delay(25);
        }
      }
      else if (button_2_pressed == HIGH){
        if (button == 2) {
          i = 0;
          digitalWrite(BUTTON_2_OUT, LOW);
          button_correct();
          button = random(8);
        }
        else{
          button_miss();
          button = random(8);
          delay(25);
        }
      }
      else if (button_3_pressed == HIGH) {
        if (button == 3) {
          i = 0;
          digitalWrite(BUTTON_3_OUT, LOW);
          button_correct();
          button = random(8);
        }
        else{
          button_miss();
          button = random(8);
          delay(25);
        }
      }
      else if (button_4_pressed == HIGH) {
        if (button == 4) {
          i = 0;
          digitalWrite(BUTTON_4_OUT, LOW);
          button_correct();
          button = random(8);
        }
        else{
          button_miss();
          button = random(8);
          delay(25);
        }
      }
      else if (button_5_pressed == HIGH) {
        if (button == 5) {
          i = 0;
          digitalWrite(BUTTON_5_OUT, LOW);
          button_correct();
          button = random(8);
        }
        else{
          button_miss();
          button = random(8);
          delay(25);
        }
      }
      else if (button_6_pressed == HIGH) {
        if (button == 6) {
          i = 0;
          digitalWrite(BUTTON_6_OUT, LOW);
          button_correct();
          button = random(8);
        }
        else{
          button_miss();
          button = random(8);
          delay(25);
        }
      }
      else if (button_7_pressed == HIGH) {
        if (button == 7) {
          i = 0;
          digitalWrite(BUTTON_7_OUT, LOW);
          button_correct();
          button = random(8);
        }
        else{
          button_miss();
          button = random(8);
          delay(25);
        }
      }
        
      i++;
      //Serial.println(i);
    }
  }


//2 Player Game
  if (game == 2 && failed < 2) {
    button_0_pressed = digitalRead(BUTTON_0_IN);
    button_1_pressed = digitalRead(BUTTON_1_IN);
    button_2_pressed = digitalRead(BUTTON_2_IN);
    button_3_pressed = digitalRead(BUTTON_3_IN);
    button_4_pressed = digitalRead(BUTTON_4_IN);
    button_5_pressed = digitalRead(BUTTON_5_IN);
    button_6_pressed = digitalRead(BUTTON_6_IN);
    button_7_pressed = digitalRead(BUTTON_7_IN);
    
    if (i < reaction_time) {
      button_lighting();
      if (button_0_pressed == HIGH) {
        if (button == 0) {
          digitalWrite(BUTTON_0_OUT, LOW);
          button_correct();
          SerialBT.write(score);
        }
        else {
          button_miss();
          button = -1;
          delay(25);
        }
      }
      else if (button_1_pressed == HIGH) {
        if (button == 1) {
          digitalWrite(BUTTON_1_OUT, LOW);
          button_correct();
          SerialBT.write(score);
        }
        else {
          button_miss();
          button = -1;
          delay(25);
        }
      }
      else if (button_2_pressed == HIGH){
        if (button == 2) {
          digitalWrite(BUTTON_2_OUT, LOW);
          button_correct();
          SerialBT.write(score);
        }
        else {
          button_miss();
          button = -1;
          delay(25);
        }
      }
      else if (button_3_pressed == HIGH){
        if (button == 3) {
          digitalWrite(BUTTON_3_OUT, LOW);
          button_correct();
          SerialBT.write(score);
        }
        else {
          button_miss();
          button = -1;
          delay(25);
        }
      }
      else if (button_4_pressed == HIGH){
        if (button == 4) {
          digitalWrite(BUTTON_4_OUT, LOW);
          button_correct();
          SerialBT.write(score);
        }
        else {
          button_miss();
          button = -1;
          delay(25);
        }
      }
      else if (button_5_pressed == HIGH){
        if (button == 5) {
          digitalWrite(BUTTON_5_OUT, LOW);
          button_correct();
          SerialBT.write(score);
        }
        else {
          button_miss();
          button = -1;
          delay(25);
        }
      }
      else if (button_6_pressed == HIGH){
        if (button == 6) {
          digitalWrite(BUTTON_6_OUT, LOW);
          button_correct();
          SerialBT.write(score);
        }
        else {
          button_miss();
          button = -1;
          delay(25);
        }
      }
      else if (button_7_pressed == HIGH){
        if (button == 7) {
          digitalWrite(BUTTON_7_OUT, LOW);
          button_correct();
          SerialBT.write(score);
        }
        else {
          button_miss();
          button = -1;
          delay(25);
        }
      }
      i++;
      //Serial.println(i);
    }
  }
  
}

void button_miss() {
     failed += 1;
     SerialBT.write('m');
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

void game_over() {
  SerialBT.write('g');
  game = 0;
  digitalWrite(BUTTON_0_OUT, LOW);
  digitalWrite(BUTTON_1_OUT, LOW);
  digitalWrite(BUTTON_2_OUT, LOW);
  digitalWrite(BUTTON_3_OUT, LOW);
  digitalWrite(BUTTON_4_OUT, LOW);
  digitalWrite(BUTTON_5_OUT, LOW);
  digitalWrite(BUTTON_6_OUT, LOW);
  digitalWrite(BUTTON_7_OUT, LOW);
}

void button_lighting() {
  if (button == 0) {
        digitalWrite(BUTTON_0_OUT, HIGH);
        digitalWrite(BUTTON_1_OUT, LOW);
        digitalWrite(BUTTON_2_OUT, LOW);
        digitalWrite(BUTTON_3_OUT, LOW);
        digitalWrite(BUTTON_4_OUT, LOW);
        digitalWrite(BUTTON_5_OUT, LOW);
        digitalWrite(BUTTON_6_OUT, LOW);
        digitalWrite(BUTTON_7_OUT, LOW);
      }
      else if (button == 1) {
        digitalWrite(BUTTON_0_OUT, LOW);
        digitalWrite(BUTTON_1_OUT, HIGH);
        digitalWrite(BUTTON_2_OUT, LOW);
        digitalWrite(BUTTON_3_OUT, LOW);
        digitalWrite(BUTTON_4_OUT, LOW);
        digitalWrite(BUTTON_5_OUT, LOW);
        digitalWrite(BUTTON_6_OUT, LOW);
        digitalWrite(BUTTON_7_OUT, LOW);
      }
      else if (button == 2) {
        digitalWrite(BUTTON_0_OUT, LOW);
        digitalWrite(BUTTON_1_OUT, LOW);
        digitalWrite(BUTTON_2_OUT, HIGH);
        digitalWrite(BUTTON_3_OUT, LOW);
        digitalWrite(BUTTON_4_OUT, LOW);
        digitalWrite(BUTTON_5_OUT, LOW);
        digitalWrite(BUTTON_6_OUT, LOW);
        digitalWrite(BUTTON_7_OUT, LOW);
      }
      else if (button == 3) {
        digitalWrite(BUTTON_0_OUT, LOW);
        digitalWrite(BUTTON_1_OUT, LOW);
        digitalWrite(BUTTON_2_OUT, LOW);
        digitalWrite(BUTTON_3_OUT, HIGH);
        digitalWrite(BUTTON_4_OUT, LOW);
        digitalWrite(BUTTON_5_OUT, LOW);
        digitalWrite(BUTTON_6_OUT, LOW);
        digitalWrite(BUTTON_7_OUT, LOW);
      }
      else if (button == 4) {
        digitalWrite(BUTTON_0_OUT, LOW);
        digitalWrite(BUTTON_1_OUT, LOW);
        digitalWrite(BUTTON_2_OUT, LOW);
        digitalWrite(BUTTON_3_OUT, LOW);
        digitalWrite(BUTTON_4_OUT, HIGH);
        digitalWrite(BUTTON_5_OUT, LOW);
        digitalWrite(BUTTON_6_OUT, LOW);
        digitalWrite(BUTTON_7_OUT, LOW);
      }
      else if (button == 5) {
        digitalWrite(BUTTON_0_OUT, LOW);
        digitalWrite(BUTTON_1_OUT, LOW);
        digitalWrite(BUTTON_2_OUT, LOW);
        digitalWrite(BUTTON_3_OUT, LOW);
        digitalWrite(BUTTON_4_OUT, LOW);
        digitalWrite(BUTTON_5_OUT, HIGH);
        digitalWrite(BUTTON_6_OUT, LOW);
        digitalWrite(BUTTON_7_OUT, LOW);
      }
      else if (button == 6) {
        digitalWrite(BUTTON_0_OUT, LOW);
        digitalWrite(BUTTON_1_OUT, LOW);
        digitalWrite(BUTTON_2_OUT, LOW);
        digitalWrite(BUTTON_3_OUT, LOW);
        digitalWrite(BUTTON_4_OUT, LOW);
        digitalWrite(BUTTON_5_OUT, LOW);
        digitalWrite(BUTTON_6_OUT, HIGH);
        digitalWrite(BUTTON_7_OUT, LOW);
      }
      else if (button == 7) {
        digitalWrite(BUTTON_0_OUT, LOW);
        digitalWrite(BUTTON_1_OUT, LOW);
        digitalWrite(BUTTON_2_OUT, LOW);
        digitalWrite(BUTTON_3_OUT, LOW);
        digitalWrite(BUTTON_4_OUT, LOW);
        digitalWrite(BUTTON_5_OUT, LOW);
        digitalWrite(BUTTON_6_OUT, LOW);
        digitalWrite(BUTTON_7_OUT, HIGH);
      }
}
