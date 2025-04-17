#include <Stepper.h>

const int stepsPerRevolution = 2038;
Stepper myStepper = Stepper(stepsPerRevolution, 5, 7, 6, 8);

void setup() {
  // put your setup code here, to run once:
      myStepper.setSpeed(15);  // Speed in RPM
    myStepper.step(stepsPerRevolution * 11); 

}

void loop() {
  // put your main code here, to run repeatedly:

}