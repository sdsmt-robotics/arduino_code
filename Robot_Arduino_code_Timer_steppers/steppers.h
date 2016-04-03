#ifndef __STEPPERS_H
#define __STEPPERS_H

#include <TimerOne.h>
#include <TimerThree.h>
#include "globals.h"

#define STEPPING 4.0
#define GEAR_RATIO 5.2
#define STEPS_REV 200*STEPPING*GEAR_RATIO
#define MAX_SPEED 40 //rpms  
#define UPDATE_TIME 1000  //micros
#define DUTY_CYCLE 512 //out of 1024

//    Stepper Motor Pins
//    Remember to tie SLEEP to 5v

#define STEPPER_LEFT_DIR    23  //white
#define STEPPER_LEFT_MS1    25  //brown
#define STEPPER_LEFT_MS2    27  //green
#define STEPPER_LEFT_ENABLE 29  //orange

 
#define STEPPER_RIGHT_DIR    47  //white
#define STEPPER_RIGHT_MS1    49  //brown
#define STEPPER_RIGHT_MS2    51  //green
#define STEPPER_RIGHT_ENABLE 53  //orange

#define STEPPER_LEFT_STEP   11
#define STEPPER_RIGHT_STEP   2 

volatile unsigned long leftConstSpeed = 0; //global vars for speed control, checked by the interrupt every 1 mS. in uS/step
volatile unsigned long rightConstSpeed = 0;

volatile char rightDir = 0; // 1 = forward
volatile char leftDir = 0;

void updateLeftMotor()
{
    if(leftConstSpeed == 0)
        Timer1.pwm(STEPPER_LEFT_STEP, 0, leftConstSpeed);
    else
        Timer1.pwm(STEPPER_LEFT_STEP, DUTY_CYCLE, leftConstSpeed);
}

void updateRightMotor()
{
    if(rightConstSpeed == 0)
        Timer3.pwm(STEPPER_RIGHT_STEP, 0, rightConstSpeed);
    else
        Timer3.pwm(STEPPER_RIGHT_STEP, DUTY_CYCLE, rightConstSpeed);
}

void initializeSteppers()
{
    Timer1.initialize(UPDATE_TIME); //set it up!
    Timer3.initialize(UPDATE_TIME); //set it up!
    Timer1.attachInterrupt(updateLeftMotor); //attach the update function
    Timer3.attachInterrupt(updateRightMotor); //attach the update function
    Timer1.pwm(STEPPER_LEFT_STEP, 0, 1000000); //set up pwm on 9 and 10
    Timer3.pwm(STEPPER_RIGHT_STEP, 0, 1000000);

    // Left Stepper Pins
    pinMode(STEPPER_LEFT_DIR, OUTPUT);
    pinMode(STEPPER_LEFT_STEP, OUTPUT);
    pinMode(STEPPER_LEFT_MS1, OUTPUT);
    pinMode(STEPPER_LEFT_MS2, OUTPUT);
    pinMode(STEPPER_LEFT_ENABLE, OUTPUT); //should be hardwired to ground

    digitalWrite(STEPPER_LEFT_MS1, LOW); //enable 1/4 microstepping
    digitalWrite(STEPPER_LEFT_MS2, HIGH);
    digitalWrite(STEPPER_LEFT_ENABLE, LOW); //should be unnecessary

    // Right Stepper Pins
    pinMode(STEPPER_RIGHT_DIR, OUTPUT);
    pinMode(STEPPER_RIGHT_STEP, OUTPUT);
    pinMode(STEPPER_RIGHT_MS1, OUTPUT);
    pinMode(STEPPER_RIGHT_MS2, OUTPUT);
    pinMode(STEPPER_RIGHT_ENABLE, OUTPUT); //should be hardwired to ground

    digitalWrite(STEPPER_RIGHT_MS1, LOW); //enable 1/4 microstepping
    digitalWrite(STEPPER_RIGHT_MS2, HIGH);
    digitalWrite(STEPPER_RIGHT_ENABLE, LOW); //should be unnecessary

}

void setLeftStepperConstRPM(int rpm)
{
  //in->print("rpm in = ");
  //in->println(rpm);
    if(rpm > 0)
        digitalWrite(STEPPER_LEFT_DIR, HIGH); //back i say (but really, go forward)
    else if(rpm < 0)
        digitalWrite(STEPPER_LEFT_DIR, LOW); //tally ho, m8 (take it back now ya'll)
    else
    {
        noInterrupts();
        leftConstSpeed = 0; //just do nothing, m8
        interrupts();
        return;
    }
    
    unsigned long stepsSec = (abs(rpm) * STEPS_REV) / 60; //calcs the number of steps per sec from RPM
    //in->print("stepsSec = ");
    //in->println(stepsSec);

    noInterrupts();
    leftConstSpeed = 1000000 / stepsSec; //in this case "spped: is in uS per step. sorry about the stupid logic
    Timer1.attachInterrupt(updateLeftMotor); //get the interrupt on there in case it was detached/had a different function
    Timer1.setPeriod(UPDATE_TIME); //set the interrupt to update the stepper speed UPDATE_TIME micros
    interrupts();

    //in->print("leftConstSpeed = ");
    //in->println(leftConstSpeed);
}

void setRightStepperConstRPM(int rpm) //same as function above, just check it out
{
    //in->print("rpm in = ");
    //in->println(rpm);
    if(rpm > 0)
        digitalWrite(STEPPER_RIGHT_DIR, LOW);
    else if(rpm < 0)
        digitalWrite(STEPPER_RIGHT_DIR, HIGH);
    else
    {
        noInterrupts();
        rightConstSpeed = 0;
        interrupts();
        return;
    }

    unsigned long stepsSec = (abs(rpm) * STEPS_REV) / 60;
    //in->print("stepsSec = ");
    //in->println(stepsSec);

    noInterrupts();
    rightConstSpeed = 1000000 / stepsSec;
    Timer3.attachInterrupt(updateRightMotor);
    Timer3.setPeriod(UPDATE_TIME);
    interrupts();

    //in->print("rightConstSpeed = ");
    //in->println(rightConstSpeed);
}

#endif












