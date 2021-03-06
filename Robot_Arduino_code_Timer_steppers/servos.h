#ifndef __SERVOS_H
#define __SERVOS_H

#include <Servo.h>
#include "globals.h"

#define CLAW_LEFT_PIN  10 //brown
#define CLAW_RIGHT_PIN 9  //white
#define CLAW_RAISE_PIN 6  //green

#define CLAW_OPEN_POS  100
#define CLAW_CLOSE_POS 30
#define CLAW_DOWN_POS  65
#define CLAW_UP_POS    170

Servo leftServo, rightServo, raiseServo;

void initializeServos()
{
    pinMode(CLAW_LEFT_PIN, OUTPUT);
    pinMode(CLAW_RIGHT_PIN, OUTPUT);
    pinMode(CLAW_RAISE_PIN, OUTPUT);
    leftServo.attach(CLAW_LEFT_PIN);
    rightServo.attach(CLAW_RIGHT_PIN);
    raiseServo.attach(CLAW_RAISE_PIN);

    leftServo.write(CLAW_OPEN_POS);
    rightServo.write(180 - CLAW_OPEN_POS);
    raiseServo.write(CLAW_DOWN_POS);
}

void raiseClaw()
{
    raiseServo.write(CLAW_UP_POS);
    return;
}

void lowerClaw()
{
    raiseServo.write(CLAW_DOWN_POS);
    return;
}

void closeClaw()
{
    leftServo.write(CLAW_CLOSE_POS);
    rightServo.write(180 - CLAW_CLOSE_POS);
    return;
}

void openClaw()
{
    leftServo.write(CLAW_OPEN_POS);
    rightServo.write(180 - CLAW_OPEN_POS);
    return;
}


#endif

