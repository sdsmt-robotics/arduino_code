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

Servo leftClaw, rightClaw, raiseClaw;

void intializeServos()
{
  pinMode(CLAW_LEFT_PIN, OUTPUT);
  pinMode(CLAW_RIGHT_PIN, OUTPUT);
  pinMode(CLAW_RAISE_PIN, OUTPUT);
  leftClaw.attach(CLAW_LEFT_PIN);
  rightClaw.attach(CLAW_RIGHT_PIN);
  raiseClaw.attach(CLAW_RAISE_PIN);

  leftClaw.write(CLAW_OPEN_POS);
  rightClaw.write(-CLAW_OPEN_POS);
  raiseClaw.write(CLAW_DOWN_POS);
}

void raiseClaw()
{
  raiseClaw.write(CLAW_UP_POS);
}

void lowerClaw()
{
  raiseClaw.write(CLAW_DOWN_POS);
}

void closeClaw()
{
  leftClaw.write(CLAW_CLOSE_POS);
  rightClaw.write(-CLAW_CLOSE_POS);
}

void openClaw()
{
  leftClaw.write(CLAW_OPEN_POS);
  rightClaw.write(-CLAW_OPEN_POS);
}


#endif
