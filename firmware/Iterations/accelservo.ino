#include <Servo.h>

const int servopin = 9;

unsigned long lasttime = 0;
unsigned long speed = 0;
int currentangle = 21;
int state = 1;  // 1 = go 2 = stop



Servo servo;


void setup(){
  Serial.begin(9600);
  servo.attach(servopin);
  servo.write(21);

}


void loop(){
  //Decision logic 
  if(currentangle >=20){
    if(currentangle <=160){

      state = 1
    

    }

  }
  // actuator logic 
  while(currenttime - lasttime > interval){
    speed++
    currentangle = currentangle + speed;
    servo.write(currentangle);
    Serial.print(currentangle);
    Serial.print("     ");
    Serial.println(speed);
  }



}