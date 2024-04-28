#include <ESP32Servo.h>
#include <HardwareSerial.h>

#define SERVO_PIN 19 // GPIO19 for rev servo
#define TRIGGER_PIN 14 //GPIO14 for trigger servo
#define BTN 0
#define REV_START 30
#define REV_END 0
#define TRIGGER_START 90
#define TRIGGER_END 0

// FC:A6:A7:68

HardwareSerial SerialPort(2);


Servo servoMotor;
Servo rev;
Servo trigger;
bool moved = false;
char number;
int pos = 0;

void setup() {
  Serial.begin(115200);
  SerialPort.begin(15200, SERIAL_8N1, 16, 17);
  pinMode(BTN, INPUT);
  trigger.write(0);
  trigger.attach(TRIGGER_PIN);
  servoMotor.write(REV_END);
  servoMotor.attach(SERVO_PIN);  // attaches the servo on ESP32 pin
}

// Push the rev trigger, wait for the motors to wind up, then pull the firing trigger
// Timing ends up releasing both servos / triggers around the same time
void fire() {
  servoMotor.write(REV_END);
  delay(2500);
  pushTrigger();
  servoMotor.write(REV_START);
}

void pushTrigger() {
  trigger.write(TRIGGER_END);
  delay(1500);
  trigger.write(TRIGGER_START);
}

void pushRev() {
  servoMotor.write(REV_END);
  delay(2500);
  servoMotor.write(REV_START);
}

void loop() {
  //Wait for a '1' to be received via UART from the game ESP, and call fire() when received
  if (SerialPort.available() > 0) {
    number = SerialPort.read();
    Serial.println(number);
    if (number == '1') {
      fire();
    }
  }
  //able to manually trigger with the BOOT button if desired, used for testing
  if(digitalRead(BTN) == LOW) {
    fire();
  }

}