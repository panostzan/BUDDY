#include <Servo.h>

const int potpin = A0;
const int servopin = 9;

unsigned long lasttime = 0;
const int second = 2000;
const int halfsecond = 500;
int targetangle = 20;
int state = 1;
int updown = 1;
int currentangle = 21;


// up is 1
// down is 0


// state = 1 is patrol
//state = 2 is pause

Servo servo;

void setup() {
  Serial.begin(9600);
  servo.attach(servopin);
  servo.write(21);
}

void loop() {

  int potValue = analogRead(potpin);
  int interval = map(potValue, 0,1023, 250,10);

  //DECISION LOGIC


  //Patrol logic

  if (state == 1) {
    if (updown == 1) {
      targetangle = 160;
      if (currentangle == 160) {
        updown = 0;
        state = 2;
      }
    }
    if (updown == 0) {
      targetangle = 20;
      if (currentangle == 20) {
        updown = 1;
        state = 3;
      }
    }
  }






  //ACTUATOR LOGIC
  unsigned long currenttime = millis();

  if (state == 1) {
    if (currenttime - lasttime >= interval) {
      if (updown == 1) {
        currentangle = currentangle + 1;
      } else {
        currentangle = currentangle - 1;
      }

      servo.write(currentangle);
      lasttime = currenttime;

      Serial.print(currentangle);
      Serial.print("    ");
      Serial.print(state);
      Serial.print("    ");
      Serial.print(updown);
      Serial.print("    ");
      Serial.print(targetangle);
      Serial.print("    ");
      Serial.println(interval);
    }
  }

  // pause logic
  else if (state == 2) {

    if (currenttime - lasttime >= second) {
      lasttime = millis();
      state = 1;

      Serial.print(currentangle);
      Serial.print("    ");
      Serial.print(state);
      Serial.print("    ");
      Serial.print(updown);
      Serial.print("     ");
      Serial.print(targetangle);
      Serial.print("    ");
      Serial.println(interval);
    }
  }
    else if (state == 3) {

    if (currenttime - lasttime >= halfsecond) {
      lasttime = millis();
      state = 1;

      Serial.print(currentangle);
      Serial.print("    ");
      Serial.print(state);
      Serial.print("    ");
      Serial.print(updown);
      Serial.print("     ");
      Serial.print(targetangle);
      Serial.print("    ");
      Serial.println(interval);
    }
  }
}
