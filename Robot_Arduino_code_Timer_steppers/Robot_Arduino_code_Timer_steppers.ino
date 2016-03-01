////////////////////////////////////////////////////////////////////////////////
//    Library Includes
////////////////////////////////////////////////////////////////////////////////
#include "steppers.h"
#include "servos.h"
#include "timed_drive.h"

////////////////////////////////////////////////////////////////////////////////
//    Pin Defines
////////////////////////////////////////////////////////////////////////////////

//    IR Sensor Pins
#define IR_RIGHT  A0
#define IR_CENTER A1
#define IR_LEFT   A2

////////////////////////////////////////////////////////////////////////////////
//    Packet Designations
////////////////////////////////////////////////////////////////////////////////

#define IR_RIGHT_TAG  0x8000
#define IR_CENTER_TAG 0x8600
#define IR_LEFT_TAG   0x8800

#define MOTOR_LEFT_TAG  0x4C
#define MOTOR_RIGHT_TAG 0x52

#define MOTOR_LEFT_STEPS_TAG  0x39
#define MOTOR_RIGHT_STEPS_TAG 0x40

#define CLAW_CLOSE_TAG  0x41
#define CLAW_RAISE_TAG  0x42

////////////////////////////////////////////////////////////////////////////////
//    Global Variables
////////////////////////////////////////////////////////////////////////////////
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
unsigned int sensorValue = 0;
unsigned char input[8] = {0};

unsigned char cmdTarget = 0;
unsigned char cmdValue = 0;

////////////////////////////////////////////////////////////////////////////////
//    Global Objects
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//    Setup Function
////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  inputString.reserve(10);
  
  initializeSteppers();
  initializeServos();
}

////////////////////////////////////////////////////////////////////////////////
//    Main Loop
////////////////////////////////////////////////////////////////////////////////

void loop() {  
  
  if (Serial.available() > 0) {
    cmdTarget = Serial.read();
    if(cmdTarget == MOTOR_LEFT_STEPS_TAG || cmdTarget == MOTOR_RIGHT_STEPS_TAG)
    {
      for(int i = 0; i < 8; i++)
      {
        while(!Serial.available()){}
        input[i] = Serial.read();
      }
      stringComplete = true;
    }
    else
    {
      while(!Serial.available()){} 
      cmdValue = Serial.read();
      stringComplete = true;
    }
  }

  noInterrupts();
  if (stringComplete)
  {
    switch(cmdTarget)
    {
      case MOTOR_LEFT_TAG:
      {
        if(cmdValue != 127)
          setLeftStepperConstRPM(map(cmdValue, 0, 255, -MAX_SPEED, MAX_SPEED));
        else
          setLeftStepperConstRPM(0);
        break;
      }
      case MOTOR_RIGHT_TAG:
      {
        if(cmdValue != 127)
          setRightStepperConstRPM(map(cmdValue, 0, 255, -MAX_SPEED, MAX_SPEED));
        else
          setRightStepperConstRPM(0);
        break;
      }
      case MOTOR_LEFT_STEPS_TAG:
      {
        Serial.println("LEFT_STEPS received.");
        Serial.println("-steps-");
        int steps = assembleInt(input, 0);
        Serial.println(steps);
        Serial.println("-seconds-");
        int seconds = assembleInt(input, 4);
        Serial.println(seconds);
        setLeftStepperStepsTime(steps, seconds);
        break;
      }
      case MOTOR_RIGHT_STEPS_TAG:
      {
        Serial.println("RIGHT_STEPS received.");
        Serial.println("-steps-");
        int steps = assembleInt(input, 0);
        Serial.println(steps);
        Serial.println("-seconds-");
        int seconds = assembleInt(input, 4);
        Serial.println(seconds);
        setRightStepperStepsTime(steps, seconds);
        break;
      }
      case CLAW_RAISE_TAG:
      {
        if(cmdValue == 1)
          raiseClaw();
        else
          lowerClaw();
        break;
      }
      case CLAW_CLOSE_TAG:
      {
        if(cmdValue == 1)
          closeClaw();
        else
          openClaw();
        break;
      }
      default:
        Serial.println("FML");
        Serial.print("Failing cmdTarget: ");
        Serial.println(cmdTarget);
        Serial.print("Input: ");
        printInput(input);
    }
    for(int i = 0; i < 8; i++)
      input[i] = '\0';
    stringComplete = false;
  }
  interrupts();
}



int assembleInt(unsigned char *string, int index)
{
  unsigned char inBytes[4] = {0};
  int num = 0;
  for(int i = 0; i < 4; i++)
  {
    inBytes[i] = (unsigned char) string[index + i];
  }
  for(int i = 3; i > -1; i--)
  {
    num = num << 8;
    num = num | inBytes[i];
  }
  return num;
}

void clearBuffer()
{
  unsigned char nothing;
  while(Serial.available() > 0)
    nothing = Serial.read();
}

void printInput(unsigned char *input)
{
  for(int i = 0; i < 9; i++)
    Serial.println(input[i]);
}

void updateSensors()
{
  sensorValue = analogRead(IR_RIGHT);
  sensorValue |= IR_RIGHT_TAG;
  Serial.println(sensorValue);
  sensorValue = analogRead(IR_CENTER);
  sensorValue |= IR_CENTER_TAG;
  Serial.println(sensorValue);
  sensorValue = analogRead(IR_LEFT);
  sensorValue |= IR_LEFT_TAG;
  Serial.println(sensorValue);
}
