
 // ============ MOTOR PINS ============
 const int RIGHT_IN1 = 7;
 const int RIGHT_IN2 = 8;
 const int LEFT_IN1 = 5;
 const int LEFT_IN2 = 6;

 const int PWM_RIGHT_PIN = 9;
 const int PWM_LEFT_PIN = 10;

 // ============ ENCODERS ============
 const int ENCODER_LEFT = 3;
 const int ENCODER_RIGHT = 2;

 volatile unsigned long leftCount = 0;
 volatile unsigned long rightCount = 0;

 // ============ TURN SETTINGS ============
 const float countsPerDegree = 3.05;
 const int targetDegrees = 90;
 const int turnPWM = 150;

 const unsigned long TURN_TIMEOUT_MS = 5000;

 // ============ STATES ============
 enum RobotState {
   IDLE,
   TURNING,
   DONE
 };

 RobotState robotState = IDLE;

 unsigned long leftStartCount = 0;
 unsigned long rightStartCount = 0;
 unsigned long turnStartTime = 0;

 // Encoder interrupt functions
 void countLeft() {
   leftCount++;
 }

 void countRight() {
   rightCount++;
 }

 // Safely copy encoder counts on the 8-bit Arduino Uno
 void readEncoders(unsigned long &left, unsigned long &right) {
   noInterrupts();
   left = leftCount;
   right = rightCount;
   interrupts();
 }

 // Stop both motors
 void stopMotors() {
   analogWrite(PWM_LEFT_PIN, 0);
   analogWrite(PWM_RIGHT_PIN, 0);
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

   stopMotors();

   attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT),
                   countLeft, RISING);
   attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT),
                   countRight, RISING);

   Serial.println("Buddy ready. Send 's' to start one 90-degree turn.");
 }

 void startTurn() {
   // Record starting encoder positions
   readEncoders(leftStartCount, rightStartCount);

   turnStartTime = millis();

   // Left wheel forward
   digitalWrite(LEFT_IN1, HIGH);
   digitalWrite(LEFT_IN2, LOW);

   // Right wheel backward
   digitalWrite(RIGHT_IN1, LOW);
   digitalWrite(RIGHT_IN2, HIGH);

   robotState = TURNING;

   Serial.println("Turning...");
 }

 void updateTurn() {
   unsigned long leftNow, rightNow;
   readEncoders(leftNow, rightNow);

   unsigned long leftTravel = leftNow - leftStartCount;
   unsigned long rightTravel = rightNow - rightStartCount;

   // Both encoder counters increment regardless of direction
   float averageCounts = (leftTravel + rightTravel) / 2.0;

   float targetCounts = targetDegrees * countsPerDegree;

   if (averageCounts >= targetCounts) {
     stopMotors();
     robotState = IDLE;

     Serial.println("=== TURN COMPLETE ===");
     Serial.print("Left encoder counts: ");
     Serial.println(leftTravel);
     Serial.print("Right encoder counts: ");
     Serial.println(rightTravel);
     Serial.print("Average counts: ");
     Serial.println(averageCounts);
     Serial.print("Target counts: ");
     Serial.println(targetCounts);
     Serial.print("Time (ms): ");
     Serial.println(millis() - turnStartTime);
   }
   else if (millis() - turnStartTime >= TURN_TIMEOUT_MS) {
     stopMotors();
     robotState = DONE;

     Serial.println("TURN TIMEOUT - check motors and encoders.");
     Serial.print("Left encoder counts: ");
     Serial.println(leftTravel);
     Serial.print("Right encoder counts: ");
     Serial.println(rightTravel);
   }
   else {
     analogWrite(PWM_LEFT_PIN, turnPWM);
     analogWrite(PWM_RIGHT_PIN, turnPWM);
   }
 }

 void loop() {
   switch (robotState) {
     case IDLE:
       stopMotors();

       // Start only after receiving 's' over Serial
       if (Serial.available() > 0) {
         char command = Serial.read();

         if (command == 's') {
           startTurn();
         }
       }
       break;

     case TURNING:
       updateTurn();
       break;

     case DONE:
       stopMotors();
       // Remain stopped until reset
       break;
   }
 }
