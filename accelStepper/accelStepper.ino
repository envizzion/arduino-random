#include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper stepper(AccelStepper::DRIVER,10, 11);

int pos = 750;

void setup()
{ 
  stepper.setMaxSpeed(3000);
  stepper.setAcceleration(8000);
 
}

void loop()
{
  if (stepper.distanceToGo() == 0)
  {
    delay(500);
    pos = -pos;
    stepper.moveTo(pos);\
  }
  stepper.run();
}
