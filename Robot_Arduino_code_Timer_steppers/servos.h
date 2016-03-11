#ifndef __SERVOS_H
#define __SERVOS_H

#include <Servo.h>

#define CLAW_LEFT_PIN  5
#define CLAW_RIGHT_PIN 6
#define CLAW_RAISE_PIN  9

#define CLAW_OPEN_POS  20
#define CLAW_CLOSE_POS 90
#define CLAW_DOWN_POS  20
#define CLAW_UP_POS    120

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
    rightServo.write(-CLAW_OPEN_POS);
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
    rightServo.write(-CLAW_CLOSE_POS);
    return;
}

void openClaw()
{
    leftServo.write(CLAW_OPEN_POS);
    rightServo.write(-CLAW_OPEN_POS);
    return;
}


#endif










