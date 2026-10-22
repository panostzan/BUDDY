#include <Servo.h>



const int potpin = A0;  // create a constant integer called potpin attached to A0
const int servopin = 9; // create a constnat integer at pinj 9 called servopin

unsigned long lastprinttime = 0;
const int interval = 150;

int currentangle = 20;


Servo myservo;      // create a servo object called myservo


void setup(){
  myservo.attach(servopin); // attachs myservo to servopin

  Serial.begin(9600);     // begins communicating with the arduino at a 9600 baud rate. (9600 bits per second)
  myservo.write(20);


}


void loop(){
  
  
  int potValue = analogRead(potpin);    // integer potValue is the analog reading of whatever is beign sent to potpin 
  int targetAngle = map(potValue,0,1023,0,180);


  unsigned long currentime = millis();

  if(currentime - lastprinttime >= interval){
    if(currentangle <targetAngle){

      currentangle = currentangle +1;
      myservo.write(currentangle);
    }

    

    Serial.print(potValue);
    Serial.print("    ");
    Serial.println(currentangle);

    lastprinttime = millis();

  
   if(currentangle > targetAngle){

      currentangle = currentangle -1;
      myservo.write(currentangle);
    }

  
    Serial.print(potValue);
    Serial.print("    ");
    Serial.println(currentangle);

    lastprinttime = millis();

  }
}

  







