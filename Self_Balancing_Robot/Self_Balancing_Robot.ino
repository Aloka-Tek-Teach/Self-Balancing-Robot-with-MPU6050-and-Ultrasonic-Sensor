#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
int motorLeft1 = 3;
int motorLeft2 = 4;
int motorRight1 = 5;
int motorRight2 = 6;
int trigPin = 8;
int echoPin = 9;
long duration;  
int distance;

#define PI 3.14159  // Define PI for angle calculations

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  pinMode(motorLeft1, OUTPUT);
  pinMode(motorLeft2, OUTPUT);
  pinMode(motorRight1, OUTPUT);
  pinMode(motorRight2, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }
}

void loop() {
  // Read MPU6050
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Calculate angle in degrees
  float angle = atan2(ay, az) * (180 / PI);
  Serial.print("Angle: ");
  Serial.println(angle);

  // Ultrasonic sensor reading
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  // Simple control logic based on angle and distance (tune for your robot)
  if (angle > 5) {
    // Move forward to balance
    digitalWrite(motorLeft1, HIGH);
    digitalWrite(motorLeft2, LOW);
    digitalWrite(motorRight1, HIGH);
    digitalWrite(motorRight2, LOW);
  } else if (angle < -5) {
    // Move backward to balance
    digitalWrite(motorLeft1, LOW);
    digitalWrite(motorLeft2, HIGH);
    digitalWrite(motorRight1, LOW);
    digitalWrite(motorRight2, HIGH);
  } else {
    // Stop motors when balanced
    digitalWrite(motorLeft1, LOW);
    digitalWrite(motorLeft2, LOW);
    digitalWrite(motorRight1, LOW);
    digitalWrite(motorRight2, LOW);
  }

  delay(100);
}
