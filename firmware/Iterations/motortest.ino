// ================= PINS =================

const int RIGHT_IN1 = 7;
const int RIGHT_IN2 = 8;

const int LEFT_IN1 = 5;
const int LEFT_IN2 = 6;

const int PWM_RIGHT_PIN = 9;
const int PWM_LEFT_PIN = 10;

const int ENCODER_LEFT = 3;
const int ENCODER_RIGHT = 2;


// ================= TIMING =================

const unsigned long CONTROL_INTERVAL = 200;
const unsigned long STATE_INTERVAL = 2000;

unsigned long lastControlTime = 0;
unsigned long currentStateTime = 0;


// ================= ENCODERS =================

volatile long leftCount = 0;
volatile long rightCount = 0;

long previousLeftCount = 0;
long previousRightCount = 0;


// ================= DRIVETRAIN STATE =================

int leftTargetSpeed = 70;
int rightTargetSpeed = 70;

int leftMeasuredSpeed = 0;
int rightMeasuredSpeed = 0;

int leftError = 0;
int rightError = 0;

int baseLeftPWM = 75;
int baseRightPWM = 55;

int leftPWM = 80;
int rightPWM = 80;

const float Kp = 0.1;


// ================= MOTION STATE =================

enum MotionState {
  STRAIGHT,
  CURVE_LEFT,
  CURVE_RIGHT,
  STOPPED
};

MotionState currentState = STRAIGHT;


// ================= ENCODER INTERRUPTS =================

void countLeft() {
  leftCount++;
}

void countRight() {
  rightCount++;
}


// ================= DRIVETRAIN COMMAND =================

void setWheelSpeeds(int leftTarget, int rightTarget) {
  leftTargetSpeed = leftTarget;
  rightTargetSpeed = rightTarget;
}


// ================= MOTION STATE MACHINE =================

void updateMotionState(unsigned long currentTime) {

  if (currentTime - currentStateTime >= STATE_INTERVAL) {
    currentStateTime = currentTime;

    switch (currentState) {

      case STRAIGHT:
        currentState = CURVE_LEFT;
        break;

      case CURVE_LEFT:
        currentState = CURVE_RIGHT;
        break;

      case CURVE_RIGHT:
        currentState = STOPPED;
        break;

      case STOPPED:
        break;
    }
  }

  switch (currentState) {

    case STRAIGHT:
      setWheelSpeeds(80, 80);
      break;

    case CURVE_LEFT:
      setWheelSpeeds(60, 100);
      break;

    case CURVE_RIGHT:
      setWheelSpeeds(100, 60);
      break;

    case STOPPED:
      setWheelSpeeds(0, 0);
      break;
  }
}


// ================= SPEED CONTROLLER =================

void updateSpeedController(unsigned long currentTime) {

  if (currentTime - lastControlTime < CONTROL_INTERVAL) {
    return;
  }

  // Measure wheel speeds
  leftMeasuredSpeed = leftCount - previousLeftCount;
  rightMeasuredSpeed = rightCount - previousRightCount;

  // Calculate errors
  leftError = leftTargetSpeed - leftMeasuredSpeed;
  rightError = rightTargetSpeed - rightMeasuredSpeed;

  // Calculate PWM
  if (leftTargetSpeed == 0) {
    leftPWM = 0;
  } else {
    leftPWM = baseLeftPWM + Kp * leftError;
  }

  if (rightTargetSpeed == 0) {
    rightPWM = 0;
  } else {
    rightPWM = baseRightPWM + Kp * rightError;
  }

  // Keep PWM valid
  leftPWM = constrain(leftPWM, 0, 255);
  rightPWM = constrain(rightPWM, 0, 255);

  // Apply motor commands
  analogWrite(PWM_LEFT_PIN, leftPWM);
  analogWrite(PWM_RIGHT_PIN, rightPWM);

  // Update encoder memory
  previousLeftCount = leftCount;
  previousRightCount = rightCount;

  lastControlTime = currentTime;

  printTelemetry();
}


// ================= TELEMETRY =================

void printTelemetry() {

  Serial.print("State: ");
  Serial.print(currentState);

  Serial.print(" | L target: ");
  Serial.print(leftTargetSpeed);

  Serial.print(" R target: ");
  Serial.print(rightTargetSpeed);

  Serial.print(" | L speed: ");
  Serial.print(leftMeasuredSpeed);

  Serial.print(" R speed: ");
  Serial.print(rightMeasuredSpeed);

  Serial.print(" | L error: ");
  Serial.print(leftError);

  Serial.print(" R error: ");
  Serial.print(rightError);

  Serial.print(" | L PWM: ");
  Serial.print(leftPWM);

  Serial.print(" R PWM: ");
  Serial.println(rightPWM);
}


// ================= SETUP =================

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
    RISING
  );

  attachInterrupt(
    digitalPinToInterrupt(ENCODER_RIGHT),
    countRight,
    RISING
  );
}


// ================= LOOP =================

void loop() {

  unsigned long currentTime = millis();

  //updateMotionState(currentTime);

  updateSpeedController(currentTime);
}