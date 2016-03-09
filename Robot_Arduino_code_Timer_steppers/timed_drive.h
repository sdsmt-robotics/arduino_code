#ifndef __TIMED_DRIVE_H
#define __TIMED_DRIVE_H

#include "steppers.h"

volatile unsigned long leftStepsTarget = 0; //global vars, sets the target number of steps
volatile unsigned long leftSteps = 0;
volatile unsigned long rightStepsTarget = 0;
volatile unsigned long rightSteps = 0;


void stepLeft()
{
    //Serial.println(leftSteps);
    if(leftSteps >= leftStepsTarget || leftStepsTarget == 0)
    {
        digitalWrite(STEPPER_LEFT_STEP, LOW);
        Timer1.detachInterrupt();
    }
    else
    {
        digitalWrite(STEPPER_LEFT_STEP, HIGH);
        /*for(int i = 0; i < 500; i++)
            int j = i;*/
        digitalWrite(STEPPER_LEFT_STEP, LOW);
        leftSteps++;
    }
}

void stepRight()
{
    //Serial.println(rightSteps);
    if(rightSteps >= rightStepsTarget || rightStepsTarget == 0)
    {
        digitalWrite(STEPPER_RIGHT_STEP, LOW);
        Timer3.detachInterrupt();
    }
    else
    {
        digitalWrite(STEPPER_RIGHT_STEP, HIGH);
        /*for(int i = 0; i < 500; i++)
            int j = i;*/
        digitalWrite(STEPPER_RIGHT_STEP, LOW);
        rightSteps++;
    }
}

void setLeftStepperStepsTime(int steps, int seconds)
{
    unsigned long leftMicrosPerStep = (seconds * 1000000) / steps; //calculates the number of micros per step
  
    if(steps > 0)
        digitalWrite(STEPPER_LEFT_DIR, HIGH);
    else if(steps < 0)
        digitalWrite(STEPPER_LEFT_DIR, LOW);
    else
        return;

    noInterrupts();
  
    leftStepsTarget = abs(steps);
    Timer1.detachInterrupt();
    Timer1.attachInterrupt(stepLeft);
    Timer1.setPeriod(leftMicrosPerStep);

    leftSteps = 0;
    Timer1.disablePwm(STEPPER_LEFT_STEP);

    interrupts();
  
}

void setRightStepperStepsTime(int steps, int seconds)
{
    unsigned long rightMicrosPerStep = (seconds * 1000000) / steps; //calculates the number of micros per step
  
    if(steps > 0)
        digitalWrite(STEPPER_RIGHT_DIR, LOW);
    else if(steps < 0)
        digitalWrite(STEPPER_RIGHT_DIR, HIGH);
    else
        return;

    noInterrupts();

    rightStepsTarget = abs(steps);
    Timer3.detachInterrupt();
    Timer3.attachInterrupt(stepRight);
    Timer3.setPeriod(rightMicrosPerStep);

    rightSteps = 0;
    Timer3.disablePwm(STEPPER_RIGHT_STEP);

    interrupts();
  
}
#endif

