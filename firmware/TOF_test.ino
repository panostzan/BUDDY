
#include <Wire.h>
#include <VL53L1X.h>

// Create sensor object
VL53L1X sensor;

void setup() {
  Serial.begin(9600);

  // Start I2C communication on A4 (SDA), A5 (SCL)
  Wire.begin();

  // Set communication timeout
  sensor.setTimeout(500);

  // Initialize VL53L1X
  if (!sensor.init()) {
    Serial.println("Sensor not detected!");
    while (1);
  }

  // Configure distance measurements
  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(50000);

  // Take a measurement every 100 ms
  sensor.startContinuous(100);

  Serial.println("Sensor ready!");
}

void loop() {
  // Read distance in millimetres
  uint16_t distance = sensor.read();

  if (sensor.timeoutOccurred()) {
    Serial.println("Sensor timeout!");
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" mm");
  }

  delay(100);
}
