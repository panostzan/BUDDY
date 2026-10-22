#include <Servo.h>;


int state = 0;
unsigned long lasttime = 0;
const int interval = 15;

const int potpin = A0;
const int servopin = 9;

const int leftangle = 20;
const int rightangle = 160;
const int middleangle= 90;
  int targetAngle = 20;



int currentangle = 20;


Servo servo;



void setup() {

  servo.attach(servopin);

  Serial.begin(9600);
  servo.write(20);

 


}

void loop() {


//STATE MACHINE 
int potValue = analogRead(potpin);

//left to middle
if(state == 0){
  if(potValue >310){
  targetAngle = 90;
  state = 1;
  }
}

//middle to left
if(state == 1){
  if(potValue <290 ){
    targetAngle = 20;
    state = 0;

  }
}

//middle to right 
if(state ==1){
  if(potValue >810){
    targetAngle = 160;
    state = 2;
  }
}

//right to middle 
if(state == 2){
  if(potValue <790){
    targetAngle = 90;
    state =1;
  }
}


// PART TWO THE ACTUATOR 


unsigned long currenttime = millis();
if(currenttime - lasttime >= interval){

  if(currentangle > targetAngle){
    currentangle = currentangle -1;
    servo.write(currentangle);
    lasttime = millis();
    Serial.println(currentangle);

  }
  

  else if(currentangle < targetAngle){
    currentangle = currentangle +1;
    servo.write(currentangle);
    lasttime = millis();
    Serial.println(currentangle);
  }

}
}

  
