#include <Servo.h>


const int servopin = 9;
const int potpin = A0;

const int interval = 15;
unsigned long lasttime = 0;

int currentAngle = 20;

// leftState = 0;
// patrol = 1;
// rightState = 2;
//up = 0;
// down = 10;

int targetAngle = 20;
int state = 0;
int updown = 0;

Servo servo;


void setup() {
  Serial.begin(9600);
  servo.attach(servopin);
  servo.write(20);
  int targetAngle = 20;




  // put your setup code here, to run once:
}


void loop() {
  // put your main code here, to run repeatedly:
  int potValue = analogRead(potpin);


  //State logic
  //emerg stage to 20
  if (potValue < 300) {
    targetAngle = 20;
    state = 0;
    Serial.print(potValue);
    Serial.print("     ");
    Serial.print(currentAngle);
    Serial.print("      ");
    Serial.println(state);
  }

  //emerg stage to 160
  if (potValue > 700) {
    targetAngle = 160;
    state = 2;
    Serial.print(potValue);
    Serial.print("     ");
    Serial.print(currentAngle);
    Serial.print("      ");
    Serial.println(state);
  }

  //patrol stage
  if (potValue >= 300) {
    if (potValue <= 700) {
      Serial.print(potValue);
      Serial.print("     ");
      Serial.print(currentAngle);
      Serial.print("      ");
      Serial.println(state);
      state = 1;
      if (updown == 0) {
        targetAngle = 160;
        if (currentAngle == 160) {
          updown = 10;
        }

      } else if (updown == 10) {
        targetAngle = 20;
        if (currentAngle == 20) {
          updown = 0;
        }
      }
    }
  }

  unsigned long currenttime = millis();

  //Actuator logic
  if (currenttime - lasttime >= interval) {
    //Patrol state

    if (state == 1) {
      if (currentAngle < targetAngle) {
        currentAngle = currentAngle + 1;
        servo.write(currentAngle);
        lasttime = millis();

      } else if (state == 1) {
        if (currentAngle > targetAngle) {
          currentAngle = currentAngle - 1;
          servo.write(currentAngle);
          lasttime = millis();
        }
      }
    }


    // emerge left
    else if (state == 0) {
      if (currentAngle >= targetAngle) {

        currentAngle = currentAngle - 1;
        servo.write(currentAngle);
        lasttime = millis();
      }
    }
    // emerge right
    else if (state == 2) {
      if (currentAngle <= targetAngle) {
        currentAngle = currentAngle + 1;
        servo.write(currentAngle);
        lasttime = millis();
      }
    }
  }
}
