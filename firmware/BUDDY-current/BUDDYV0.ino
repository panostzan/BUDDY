#include <Wire.h>
#include <VL53L1X.h>

VL53L1X sensor;

// ===============PIN SETTUP ============
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

void countLeft() {
  leftCount++;
}
void countRight() {
  rightCount++;
}


//============ Proportional Control ========= //
const float Kp = 0.1;
unsigned long lastControlTime = 0;
const int CONTROL_INTERVAL = 200;

int baseCommandPWM = 0;

int leftMeasuredSpeed = 0;
int rightMeasuredSpeed = 0;
int speedDifference = 0;

int leftPWM = 0;
int rightPWM = 0;



enum MotionState {
  STRAIGHT,
  TURN_LEFT,
  STOPPED
};

MotionState currentState = STOPPED;

unsigned long stateStartTime = 0;
const unsigned long TURN_DURATION = 400;

const unsigned long TELEMETRY_INTERVAL = 500;
unsigned long lastTelemetryTime = 0;



void setup() {
  Serial.begin(9600);      //starts Serial which is how arduino comunicates with computer
  Wire.begin();            //    starts the wire which automatically wrires stuff i think
  sensor.setTimeout(500);  // not sure what this line does

  //MOTOR PIN SETTUPS
  pinMode(RIGHT_IN1, OUTPUT);
  pinMode(RIGHT_IN2, OUTPUT);

  pinMode(LEFT_IN1, OUTPUT);
  pinMode(LEFT_IN2, OUTPUT);

  pinMode(PWM_RIGHT_PIN, OUTPUT);
  pinMode(PWM_LEFT_PIN, OUTPUT);

  pinMode(ENCODER_LEFT, INPUT_PULLUP);
  pinMode(ENCODER_RIGHT, INPUT_PULLUP);

  // Motor direction
  digitalWrite(RIGHT_IN1, LOW);
  digitalWrite(RIGHT_IN2, HIGH);

  digitalWrite(LEFT_IN1, LOW);
  digitalWrite(LEFT_IN2, HIGH);

  // =========== INITIALIZE VL53L1X ============
  if (!sensor.init()) {
    Serial.println("Sensor not detected!");
    while (1)
      ;
  }

  // Configure distance measurements
  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(50000);

  // Take a measurement every 100 ms
  sensor.startContinuous(100);
  Serial.println("Sensor ready!");


  //==========ATTACH INTERRUPT TO THE TWO ENCODER PINS==========
  attachInterrupt(
    digitalPinToInterrupt(ENCODER_LEFT),
    countLeft,
    RISING);

  attachInterrupt(
    digitalPinToInterrupt(ENCODER_RIGHT),
    countRight,
    RISING);
}

void Telemetry(long leftCount, long rightCount, int distance) {
  unsigned long currentTime = millis();

  if (currentTime - lastTelemetryTime >= TELEMETRY_INTERVAL) {
    lastTelemetryTime = currentTime;

    Serial.print("Distance: ");
    Serial.print(distance);
    // Motor feedback
    Serial.print(" | Base PWM: ");
    Serial.print(baseCommandPWM);

    Serial.print(" | Speed Difference: ");
    Serial.print(speedDifference);
    Serial.println();
  }
}
int readDistance() {
  uint16_t distance = sensor.read();

  if (sensor.timeoutOccurred()) {
    Serial.println("Sensor timeout!");
    return 0;
  } else {

    return distance;
  }
}

void updateDriveState(int distance) {

  if (distance == 0) {
    currentState = STOPPED;
    baseCommandPWM = 0;
    return;
  }

  if (currentState == TURN_LEFT) {
    return;
  }

  if (distance <= 300) {
    currentState = TURN_LEFT;
    stateStartTime = millis();
    baseCommandPWM = 80;
  } else {
    currentState = STRAIGHT;
    baseCommandPWM = 180;
  }
}

void drivingState() {

  switch (currentState) {

    case STRAIGHT:
      {
        // Both motors forward
        digitalWrite(RIGHT_IN1, LOW);
        digitalWrite(RIGHT_IN2, HIGH);

        digitalWrite(LEFT_IN1, LOW);
        digitalWrite(LEFT_IN2, HIGH);

        analogWrite(PWM_LEFT_PIN, leftPWM);
        analogWrite(PWM_RIGHT_PIN, rightPWM);
        break;
      }

    case TURN_LEFT:
      {
        // Check how long we've been turning
        unsigned long elapsed = millis() - stateStartTime;

        if (elapsed >= TURN_DURATION) {
          currentState = STOPPED;
          baseCommandPWM = 0;
          leftPWM = 0;
          rightPWM = 0;

          analogWrite(PWM_LEFT_PIN, 0);
          analogWrite(PWM_RIGHT_PIN, 0);
        }

        else {
          // Left motor reverse
          digitalWrite(LEFT_IN1, HIGH);
          digitalWrite(LEFT_IN2, LOW);

          // Right motor forward
          digitalWrite(RIGHT_IN1, LOW);
          digitalWrite(RIGHT_IN2, HIGH);

          analogWrite(PWM_LEFT_PIN, leftPWM);
          analogWrite(PWM_RIGHT_PIN, rightPWM);
        }

        break;
      }

    case STOPPED:
      analogWrite(PWM_LEFT_PIN, 0);
      analogWrite(PWM_RIGHT_PIN, 0);
      break;
  }
}

void speedPorpotionalController(unsigned long currentTime) {
  if (currentTime - lastControlTime < CONTROL_INTERVAL) {
    return;
  }

  lastControlTime = currentTime;

  //measure wheel speed
  long leftSnapshot;
  long rightSnapshot;

  noInterrupts();
  leftSnapshot = leftCount;
  rightSnapshot = rightCount;
  interrupts();

  leftMeasuredSpeed = leftSnapshot - previousLeftCount;
  rightMeasuredSpeed = rightSnapshot - previousRightCount;

  //calculate the error
  speedDifference = leftMeasuredSpeed - rightMeasuredSpeed;

  if (currentState == STOPPED) {
    leftPWM = 0;
    rightPWM = 0;
  } else if (currentState == TURN_LEFT) {
    leftPWM = baseCommandPWM;
    rightPWM = baseCommandPWM;
  } else {
    float correction = Kp * speedDifference;

    leftPWM = constrain(baseCommandPWM - correction, 0, 255);
    rightPWM = constrain(baseCommandPWM + correction, 0, 255);
  }


  previousRightCount = leftSnapshot;
  previousLeftCount = rightSnapshot;
}



void loop() {
  int distance = readDistance();
  unsigned long currentTime = millis();

  updateDriveState(distance);
  speedPorpotionalController(currentTime);
  drivingState();

  Telemetry(leftCount, rightCount, distance);
}
