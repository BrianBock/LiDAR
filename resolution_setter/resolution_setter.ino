#include <math.h>


int const stepmode = 4; //Defines the resolution of the system
float const multiplier = 360 / (200 * stepmode);


/*
  else if stepmode = 8{
  //...etc
  }


  else {
  Serial.println("StepMode can only have one of the following values: 4, 8, 16, 32");
  delay(100000);
  }*/

void setup() {

  if (stepmode == 4) {
    //Set pins for quarter step

    //
    float roundscan = 4.5; // approx time in seconds to scan a single round (from 0-360)
    int AZI_MAX = 200 * stepmode;
    float ALT_MAX = (1 / 3) * AZI_MAX;
    ALT_MAX=round(ALT_MAX);

    float runtime = roundscan * ALT_MAX;
    float runtime_minutes = runtime / 60;
    Serial.print("Running scan at 1 / 4 step. Estimated scan time: ");
    Serial.println(runtime_minutes);
    float starttime = millis();
  }


}

void loop() {}

