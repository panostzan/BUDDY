#include <Servo.h>



Servo servo;
int currentangle = 90;
unsigned long lasttime = 0;
int targetangle = 90;
const int interval = 50;
const int servopin = 9;



void setup() {
  Serial.begin(9600);
  servo.attach(servopin);
  servo.write(90);

  // put your setup code here, to run once:
}

void loop() {

  if (Serial.available() > 0) {

    int servoAngle = Serial.parseInt();

    servo.write(servoAngle);
  }
}
