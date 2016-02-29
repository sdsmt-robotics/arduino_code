#ifndef __TIMED_DRIVE_H
#define __TIMED_DRIVE_H

#include "steppers.h"

volatile unsigned long leftStepsTarget = 0; //global vars, sets the target number of steps
volatile unsigned long leftSteps = 0;
volatile unsigned long rightStepsTarget = 0;
volatile unsigned long rightSteps = 0;

volatile unsigned long leftMicrosPerStep = 0;
volatile unsigned long rightMicrosPerStep = 0;


void stepLeft()
{
  Serial.println(leftSteps);
  if(leftSteps >= leftStepsTarget || leftStepsTarget == 0)
  {
    Timer1.pwm(STEPPER_LEFT_STEP, 0, leftMicrosPerStep);
    Timer1.detachInterrupt();
  }
  else
  {
    digitalWrite(STEPPER_LEFT_STEP, HIGH);
    delayMicroseconds(1);
    digitalWrite(STEPPER_LEFT_STEP, LOW);
    leftSteps++;
  }
}

void setLeftStepperStepsTime(int steps, int seconds)
{
  leftMicrosPerStep = (seconds * 1000000) / steps; //calculates the number of micros per step
  
  if(steps > 0)
    digitalWrite(STEPPER_LEFT_DIR, LOW);
  else if(steps < 0)
    digitalWrite(STEPPER_LEFT_DIR, HIGH);
  else
    return;

  noInterrupts();
  
  leftStepsTarget = abs(steps);
  Timer1.detachInterrupt();
  Timer1.attachInterrupt(stepLeft);
  Timer1.setPeriod(leftMicrosPerStep);
  
  interrupts();
  
}
#endif
