// ================= PINS =================

const int RIGHT_IN1 = 7;
const int RIGHT_IN2 = 8;

const int LEFT_IN1 = 5;
const int LEFT_IN2 = 6;

const int PWM_RIGHT_PIN = 9;
const int PWM_LEFT_PIN = 10;

const int ENCODER_LEFT = 3;
const int ENCODER_RIGHT = 2;


// ================= ENCODERS =================

volatile long leftCount = 0;
volatile long rightCount = 0;

long previousLeftCount = 0;
long previousRightCount = 0;

//===========TIME VARIABLES =======

unsigned long lastSpeedTime = 0;

//=========== SPEED VARIABLES========

const int pwmCommand = 150;

// ============DISTANCE VARIABLES ======

const float targetmm = 2000;

//========== STARTED VARIABLES ======
bool driveDistanceStarted = false;
bool turnDegreesStarted = false;

long leftStartCount = 0;
long rightStartCount = 0;

long turnLeftStartCount = 0;
long turnRightStartCount = 0;

// ========== CONTROL VARIABLES ============

float syncKp = 0.1;

// ===== CONVERSION VARIABLES ============
float countsPerDegree = 3.2;

// ============ FUNCTION STOPPED VARIABLES  =======

bool turnStopped = true;
bool driveStopped = true;
int sidesCompleted = 0;


//========== interupt service routine functions ISR

void countLeft() {
  leftCount++;
}
void countRight() {
  rightCount++;
}


// creates a type RobotState and the different values it can take
enum RobotState {
  DRIVE_1,
  TURN_1,
  DRIVE_2,
  DONE
};


RobotState robotState = DRIVE_1;  // sets the RobotState type robotState variable to drive_1


bool driveDistance(float targetmm) {  //drive distance funciton that takes how far we want to go
  // if we are starting
  if (driveDistanceStarted == false) {
    //update our starting value
    leftStartCount = leftCount;
    rightStartCount = rightCount;

    driveDistanceStarted = true;  //exit the settup loop
    driveStopped = false;         // show that we are now in the drivedistance function

    // set our wheel directions to straight
    digitalWrite(RIGHT_IN1, LOW);
    digitalWrite(RIGHT_IN2, HIGH);

    digitalWrite(LEFT_IN1, LOW);
    digitalWrite(LEFT_IN2, HIGH);
  }
  //continue on with everything else
  else {
    //do all the logic of the function

    // convert target mm to and encoder count value
    float convertedCount = (targetmm / 0.4);

    float leftTravelCount = leftCount - leftStartCount;  //how many encoder counts we have travelled
    float rightTravelCount = rightCount - rightStartCount;
    float averageTravelCount = (leftTravelCount + rightTravelCount) / 2;  // the average of the two wheels
    float syncError = leftTravelCount - rightTravelCount;                 // the difference between the two is the synchronization error (how much bigger left is)

    float correction = syncKp * syncError;  // how much we correct it by using proportional control

    float leftCommand = 150 - correction;  // we do - because of the way our sync error was calculated
    float rightCommand = 150 + correction;

    leftCommand = constrain(leftCommand, 0, 255);  // constrain the values
    rightCommand = constrain(rightCommand, 0, 255);

    //if our travel count is less than our target count
    if (averageTravelCount <= convertedCount) {
      analogWrite(PWM_RIGHT_PIN, rightCommand);
      analogWrite(PWM_LEFT_PIN, leftCommand);

    } else {
      analogWrite(PWM_RIGHT_PIN, 0);
      analogWrite(PWM_LEFT_PIN, 0);
      driveStopped = true;           // our global drivestopped variable knows we have stopped
      driveDistanceStarted = false;  //set this to false so the next time we call it it enters the start loop properly
    }
    Serial.print(" | L travel: ");
    Serial.print(leftTravelCount);

    Serial.print(" | R travel: ");
    Serial.print(rightTravelCount);

    Serial.print(" | Sync error: ");
    Serial.print(syncError);

    Serial.print(" | L cmd: ");
    Serial.print(leftCommand);

    Serial.print(" | R cmd: ");
    Serial.println(rightCommand);
  }
  return driveStopped;  // return the boolean drive stopped (should be true now because we are done the function)
}

bool turnDegrees(int targetDegrees) {
  //set up starting values
  if (turnDegreesStarted == false) {
    turnLeftStartCount = leftCount;
    turnRightStartCount = rightCount;

    turnDegreesStarted = true;  // leave our starting loop

    turnStopped = false;

  } else {

    // how far the wheels have travelled and there average
    long leftTurnTravelled = leftCount - turnLeftStartCount;
    long rightTurnTravelled = rightCount - turnRightStartCount;
    long averageTurnTravelled = (leftTurnTravelled + rightTurnTravelled) / 2;

    //target counts we want

    long targetTurnCounts = targetDegrees * countsPerDegree;

    if (averageTurnTravelled <= targetTurnCounts) {
      analogWrite(PWM_LEFT_PIN, 150);

      // set the right wheel to the opposite direction
      digitalWrite(RIGHT_IN1, HIGH);
      digitalWrite(RIGHT_IN2, LOW);

      analogWrite(PWM_RIGHT_PIN, 150);

    } else {
      //now that the function is done set the wheels to 0
      analogWrite(PWM_LEFT_PIN, 0);
      analogWrite(PWM_RIGHT_PIN, 0);
      Serial.println("turn Complete");
      //turn degrees started is false so the next time we call the funtion the settup loop happens correctly
      turnStopped = true;

      turnDegreesStarted = false;
    }
  }
  return turnStopped;
}

void setup() {

  Serial.begin(9600);

  pinMode(RIGHT_IN1, OUTPUT);
  pinMode(RIGHT_IN2, OUTPUT);

  pinMode(LEFT_IN1, OUTPUT);
  pinMode(LEFT_IN2, OUTPUT);

  pinMode(PWM_RIGHT_PIN, OUTPUT);
  pinMode(PWM_LEFT_PIN, OUTPUT);

  pinMode(ENCODER_LEFT, INPUT_PULLUP);
  pinMode(ENCODER_RIGHT, INPUT_PULLUP);

  // Motor direction
  digitalWrite(RIGHT_IN1, HIGH);
  digitalWrite(RIGHT_IN2, LOW);

  digitalWrite(LEFT_IN1, LOW);
  digitalWrite(LEFT_IN2, HIGH);

  attachInterrupt(
    digitalPinToInterrupt(ENCODER_LEFT),
    countLeft,
    RISING);

  attachInterrupt(
    digitalPinToInterrupt(ENCODER_RIGHT),
    countRight,
    RISING);
}

void printTelemetry(
  int pwmCommand,
  long leftSpeed,
  long rightSpeed) {

  // Serial.print("PWM: ");
  // Serial.print(pwmCommand);

  // Serial.print(" | L speed: ");
  // Serial.print(leftSpeed);

  // Serial.print(" | R speed: ");
  // Serial.print(rightSpeed);

  // Serial.print(" | R travel count ");
  // Serial.print(rightTravelCount);

  // Serial.print(" | left Travel count " );
  // Serial.print(leftTravelCount);

  // Serial.print(" | ");
  // Serial.print(rightCount);

  // Serial.print(" | StartL encoder value");
  // Serial.println(leftStartCount);


  // Serial.print(" | L encoder value");
  // Serial.println(leftCount);
}

void updateSpeedMeasurement(unsigned long currenttime) {

  if (currenttime - lastSpeedTime >= 500) {
    lastSpeedTime = currenttime;

    long leftSpeed = leftCount - previousLeftCount;
    long rightSpeed = rightCount - previousRightCount;

    previousRightCount = rightCount;
    previousLeftCount = leftCount;

    printTelemetry(pwmCommand, leftSpeed, rightSpeed);
  }
}

void loop() {
  unsigned long currenttime = millis();

  updateSpeedMeasurement(currenttime);

  switch (robotState) {
    case DRIVE_1:
      if (driveDistance(500)) {  // if this returns true
        sidesCompleted++;
        if (sidesCompleted < 4) {
          robotState = TURN_1;
        } else robotState = DONE;
      }

      break;
    case TURN_1:
      if (turnDegrees(90)) {
        robotState = DRIVE_1;
      }
      break;

      break;
    case DONE:
      analogWrite(PWM_LEFT_PIN, 0);
      analogWrite(PWM_RIGHT_PIN, 0);
      delay(10000);
      sidesCompleted = 0;
      robotState = DRIVE_1;
  }
}
