#include <MultiStepper.h>
#include <AccelStepper.h>

#define LIDAR_TRIG_PIN 2
#define LIDAR_MON_PIN 3

//Motor 1 = Azi motor
//Motor 2 = Alti motor
#define ALTI_STEP_PIN 9 // UNO Pin 9 connected to STEP pin of Easy Driver
#define ALTI_DIR_PIN 8 // UNO Pin 8 connected to DIR pin of Easy Driver
#define ALTI_SLEEP_PIN 10

#define AZI_STEP_PIN 6
#define AZI_DIR_PIN 5
#define AZI_SLEEP_PIN 7


#define AZI_MAX 800
#define ALT_MAX 266

unsigned long pulseWidth;
int distance;
int i;
// AccelStepper Setup
AccelStepper StepperAlti(1, ALTI_STEP_PIN, ALTI_DIR_PIN);   // 1 = Step/Dir interface
AccelStepper StepperAzi(1, AZI_STEP_PIN, AZI_DIR_PIN);

int Alti;  // Used to store the X value entered in the Serial Monitor
int Azi;  // Used to store the Z value entered in the Serial Monitor

int move_finished = 1; // Used to check if move is completed

void setup() {

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

//Commented out all calls to altitude motor, don't run it if it's not going to change. Also need to give Alt and Azi values before you use them

// If running full program, nest for loops, outer loop incrementing alt, inner loop incrementing azi
void loop() {
   for (Azi = 0; Azi < AZI_MAX; Azi++) {
      StepperAlti.moveTo(Alti);  // Set new move position for X Stepper
      StepperAzi.moveTo(Azi);  // Set new move position for Z Stepper

      // Check if the Steppers have reached desired position
      if (StepperAlti.distanceToGo() !=0) {
         StepperAlti.run();  // Move Stepper X into position
      }

      if (StepperAzi.distanceToGo() != 0) {
         StepperAzi.run();  // Move Stepper Z into position
      }
      Serial.println("Done!"); // Debugging part 2


      Serial.print("Azi Pos: ");
      Serial.print(Azi);
      Serial.print(" ");
      distance = __getDistance();

      Serial.print("Distance: ");
      Serial.println(distance); // Print the distance measured by the LIDAR


      //Serial.print(Azi);
      //Serial.println(" Azi Travel ");

   }
   StepperAzi.moveTo(0); // Reset stepper position, should go in reverse, if not add reverse for loop with calls to moveTo
   while (StepperAzi.distanceToGo()) {
    StepperAzi.run();
   }
   Serial.println("COMPLETED!");
}

int __getDistance() {
   pulseWidth = pulseIn(LIDAR_MON_PIN, HIGH); // Count how long the pulse is high in microseconds
   return pulseWidth / 10;
}
