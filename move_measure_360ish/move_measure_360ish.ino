#include <MultiStepper.h>
#include <AccelStepper.h>

#define LIDAR_TRIG_PIN 2
#define LIDAR_MON_PIN 3

//Motor 1 = Azi motor
//Motor 2 = Alti motor
#define ALTI_STEP_PIN 9 // UNO Pin 9 connected to STEP pin of Easy Driver
#define ALTI_DIR_PIN 8 // UNO Pin 8 connected to DIR pin of Easy Driver
#define ALTI_SLEEP_PIN 10
#define ALTI_MO_PIN 11
#define ALTI_M1_PIN 12

#define AZI_STEP_PIN 6
#define AZI_DIR_PIN 5
#define AZI_SLEEP_PIN 7
#define AZI_MO_PIN 4
#define AZI_M1_PIN 1

//Set Resolution by changing this section
final float resolution = .5; // half step resolution
final float steps = 200 / resolution; //# of steps per rotation

int AZI_MAX = steps;
int ALT_MAX = steps / 3; //Alti goes ~120 degrees. 

unsigned long pulseWidth;
int distance;
int i;
// AccelStepper Setup
AccelStepper StepperAlti(1, ALTI_STEP_PIN, ALTI_DIR_PIN);   // 1 = Step/Dir interface
AccelStepper StepperAzi(1, AZI_STEP_PIN, AZI_DIR_PIN);

int Alti;  // Used to store the X value entered in the Serial Monitor
int Azi;  // Used to store the Z value entered in the Serial Monitor

void setup() {
  Alti = 0;
  pinMode(AZI_MO_PIN, OUTPUT);
  pinMode(ALTI_MO_PIN, OUTPUT);
  digitalWrite(AZI_MO_PIN, LOW);
  digitalWrite(AZI_MO_PIN, LOW);


  pinMode(AZI_M1_PIN, OUTPUT);
  pinMode(ALTI_M1_PIN, OUTPUT);

  digitalWrite(AZI_M1_PIN, HIGH);
  digitalWrite(ALTI_M1_PIN, HIGH);



  pinMode(AZI_SLEEP_PIN, OUTPUT);
  digitalWrite(AZI_SLEEP_PIN, HIGH); //turns on driver

  pinMode(ALTI_SLEEP_PIN, OUTPUT);
  digitalWrite(ALTI_SLEEP_PIN, HIGH); //turns on driver

  pinMode(LIDAR_TRIG_PIN, OUTPUT); // Set pin 2 as trigger pin
  digitalWrite(LIDAR_TRIG_PIN, LOW); // Set trigger LOW for continuous read
  pinMode(LIDAR_MON_PIN, INPUT); // Set pin 3 as monitor pin

  Serial.begin(115200);  // Start the Serial monitor with speed of Moarspeed.bauds

  //  Set Max Speed and Acceleration of each Steppers
  StepperAlti.setMaxSpeed(500.0);      // Set Max Speed of X axis
  StepperAlti.setAcceleration(500.0);  // Acceleration of X axis

  StepperAzi.setMaxSpeed(250.0);      // Set Max Speed of Y axis slower for rotation
  StepperAzi.setAcceleration(250.0);  // Acceleration of Y axis

}

void loop() {
  if (abs(Alti) == ALT_MAX) {
    Serial.println("COMPLETED!");
    delay(10000);
  }
  else {
    for (Azi = 0; Azi > -AZI_MAX; Azi--) {
      __moveMotor(StepperAzi, Azi);

      Serial.print(abs(Azi));
      Serial.print(",");
      Serial.print(abs(Alti));
      Serial.print(",");
      distance = __getDistance();
      Serial.println(distance); // Print the distance measured by the LIDAR
      delay(2);
    }
    Alti--;
    __moveMotor(StepperAlti, Alti);

    for (Azi = AZI_MAX; Azi > 0; Azi--) {
      __moveMotor(StepperAzi, Azi);
      Serial.print(abs(Azi));
      Serial.print(",");
      Serial.print(abs(Alti));
      Serial.print(",");
      distance = __getDistance();
      Serial.println(distance); // Print the distance measured by the LIDAR
      delay(2);
    }
    Alti--;
    __moveMotor(StepperAlti, Alti);

  }
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





