#include <MultiStepper.h>
#include <AccelStepper.h>

#define LIDAR_TRIG_PIN 2
#define LIDAR_MON_PIN 3


//Motor 2 = Alti motor
#define ALTI_STEP_PIN 9 // UNO Pin 9 connected to STEP pin of Easy Driver
#define ALTI_DIR_PIN 8 // UNO Pin 8 connected to DIR pin of Easy Driver
#define ALTI_SLEEP_PIN 10

#define ALT_MAX 466

unsigned long pulseWidth;
int distance;
int i;
// AccelStepper Setup
AccelStepper StepperAlti(1, ALTI_STEP_PIN, ALTI_DIR_PIN);   // 1 = Step/Dir interface

int Alti;  // Used to store the X value entered in the Serial Monitor

void setup() {

  pinMode(ALTI_SLEEP_PIN, OUTPUT);
  digitalWrite(ALTI_SLEEP_PIN, HIGH); //turns on driver

  pinMode(LIDAR_TRIG_PIN, OUTPUT); // Set pin 2 as trigger pin
  digitalWrite(LIDAR_TRIG_PIN, LOW); // Set trigger LOW for continuous read
  pinMode(LIDAR_MON_PIN, INPUT); // Set pin 3 as monitor pin

  Serial.begin(115200);  // Start the Serial monitor with speed of Moarspeed.bauds

  //  Set Max Speed and Acceleration of each Steppers
  StepperAlti.setMaxSpeed(500.0);      // Set Max Speed of X axis
  StepperAlti.setAcceleration(500.0);  // Acceleration of X axis
}


void loop() {
  for (Alti = -ALT_MAX; Alti<=0; Alti++) {
    __moveMotor(StepperAlti, Alti);
    Serial.print("Alti Pos: ");
      Serial.print(abs(Alti));
      Serial.print(" ");
      distance = __getDistance();

      Serial.print("Distance: ");
      Serial.println(distance); // Print the distance measured by the LIDAR
      delay(15);
  }
  //__moveMotor(StepperAlti, 0);

  Serial.println("COMPLETED!");
}

void __moveMotor(AccelStepper motor, int position) {
  motor.moveTo(position);
  if (motor.distanceToGo()) {
    motor.run();
  }
}

int __getDistance() {
  pulseWidth = pulseIn(LIDAR_MON_PIN, HIGH); // Count how long the pulse is high in microseconds
  return pulseWidth / 10;
}


