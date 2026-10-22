#include <Wire.h>
#include <VL53L1X.h>


// ===============PIN SETTUP ============
const int RIGHT_IN1 = 7;
const int RIGHT_IN2 = 8;

const int LEFT_IN1 = 5;
const int LEFT_IN2 = 6;

const int PWM_RIGHT_PIN = 9;
const int PWM_LEFT_PIN = 10;

const int ENCODER_LEFT = 3;
const int ENCODER_RIGHT = 2;

VL53L1X sensor;

enum MotionState {
  STRAIGHT,
  TURN_LEFT,
  STOPPED
};

MotionState currentState = STOPPED;

unsigned long stateStartTime = 0;
const unsigned long TURN_DURATION = 700;



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
}


int readDistance() {
  uint16_t distance = sensor.read();

  if (sensor.timeoutOccurred()) {
    Serial.println("Sensor timeout!");
    return 0;
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" mm");
  }

  return distance;
}

void updateDriveState(int distance) {

  if (distance == 0) {
    currentState = STOPPED;
    return;
  }
  if (currentState == TURN_LEFT) {
    return;
  }
  if (currentState == STOPPED) {
    if (distance > 500) {
      currentState = STRAIGHT;
    }
  } else if (currentState == STRAIGHT) {
    if (distance <= 500) {
      currentState = TURN_LEFT;
      stateStartTime = millis();
    }
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

        analogWrite(PWM_LEFT_PIN, 180);
        analogWrite(PWM_RIGHT_PIN, 180);
        break;
      }

    case TURN_LEFT:
      {
        // Check how long we've been turning
        unsigned long elapsed = millis() - stateStartTime;

        if (elapsed >= TURN_DURATION) {
          currentState = STOPPED;
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

          analogWrite(PWM_LEFT_PIN, 80);
          analogWrite(PWM_RIGHT_PIN, 80);
        }

        break;
      }

    case STOPPED:
      analogWrite(PWM_LEFT_PIN, 0);
      analogWrite(PWM_RIGHT_PIN, 0);
      break;
  }
}



void loop() {
  int distance = readDistance();

  updateDriveState(distance);
  drivingState();
}
