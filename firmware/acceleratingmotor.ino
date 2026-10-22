
//=======PIN settup ===
const int PWM_PIN = 9;
const int IO_PIN1 = 8;
const int IO_PIN2 = 7;
const int ENCODER_A = 2;

volatile long encoderCount = 0;

//=========time variables======

unsigned long lastPWMTime = 0;
unsigned long stateStartTime = 0;
const int interval = 15;
const int second = 1000;


//======== PWM interval=====

int pwmtotal = 0;

//===== state variables=======

int state = 0;           // 0 is paused bottom  1 is up 2 is paused top 3 is accelerate down.
bool forwardreverse = true;  // 1 is forward 0 is reversed (used for H bridge control)



void setup() {
  Serial.begin(9600);


  pinMode(IO_PIN1, OUTPUT);
  pinMode(IO_PIN2, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(ENCODER_A, INPUT);

  attachInterrupt(
    digitalPinToInterrupt(ENCODER_A),
    countPulse,
    RISING
  );


  digitalWrite(IO_PIN1, HIGH);
  digitalWrite(IO_PIN2, LOW);
}

void countPulse(){
    encoderCount ++;
  }

void loop() {


  // ==========STATE LOGIC ===========

//============ DIRECTION STATE LOGIC =========

if(state == 3){
  if(pwmtotal == 0){
    forwardreverse = !forwardreverse;
    Serial.println(forwardreverse);

  }
}

//=========== Speed State Logic ==========

  // pause for 1 second
  unsigned long currenttime = millis();

  if (pwmtotal == 0) {
    state = 0;
  }
  if (pwmtotal == 255) {
    state = 2;
  }
  if (state == 0) {
    if (currenttime - stateStartTime >= second) {
      stateStartTime = currenttime;
      state = 1;
    }
  }
  if (state == 2) {
    if (currenttime - stateStartTime >= second) {
      stateStartTime = currenttime;
      state = 3;
    }
  }





  //=======actuator logic =========

  if(forwardreverse == true){
  digitalWrite(IO_PIN1, HIGH);
  digitalWrite(IO_PIN2, LOW);

  }
  
  if(forwardreverse == false){
  digitalWrite(IO_PIN1, LOW);
  digitalWrite(IO_PIN2, HIGH);

  }

  if (state == 1) {
    if (currenttime - lastPWMTime >= interval) {
      analogWrite(PWM_PIN, pwmtotal);

      pwmtotal = pwmtotal + 1;
      lastPWMTime = currenttime;
      Serial.print(pwmtotal);
      Serial.print("     ");
      Serial.println(state);
    }
  }

  if (state == 3) {
    if (currenttime - lastPWMTime >= interval) {
      analogWrite(PWM_PIN, pwmtotal);

      pwmtotal = pwmtotal - 1;
      lastPWMTime = currenttime;
      Serial.print(pwmtotal);
      Serial.print("     ");
      Serial.println(state);
    }
  }
}
