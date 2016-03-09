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
    cmdTarget = Serial.read(); //first byte is the identifying tag
    if(cmdTarget == MOTOR_LEFT_STEPS_TAG || cmdTarget == MOTOR_RIGHT_STEPS_TAG) //if the command is a # of steps in a time period, grab the next 8 bytes
    {
      for(int i = 0; i < 8; i++)
      {
        while(!Serial.available()){} //makes sure there's something to read
        input[i] = Serial.read();
      }
      stringComplete = true;
    }
    else
    {
      while(!Serial.available()){} //if the second byte of two hasn't come in, chill and wait for it
      cmdValue = Serial.read(); //this is a speed value from 0-255 centered on 127 used for constant-speed driving
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
          setLeftStepperConstRPM(map(cmdValue, 0, 255, -MAX_SPEED, MAX_SPEED)); //drive the left wheel at a constant speed
        else
          setLeftStepperConstRPM(0); //otherwise just don't move
        break;
      }
      case MOTOR_RIGHT_TAG:
      {
        if(cmdValue != 127)
          setRightStepperConstRPM(map(cmdValue, 0, 255, -MAX_SPEED, MAX_SPEED)); //set teh right wheel at a constant speed
        else
          setRightStepperConstRPM(0); //otherwise, so nothing
        break;
      }
      case MOTOR_LEFT_STEPS_TAG:
      {
        Serial.println("LEFT_STEPS received."); //lots of debug printing
        Serial.println("-steps-");
        int steps = assembleInt(input, 0); //grabs the first four bytes, makes a number
        Serial.println(steps);
        Serial.println("-seconds-");
        int seconds = assembleInt(input, 4); //grabs the second 4 bytes, makes a number
        Serial.println(seconds);
        setLeftStepperStepsTime(steps, seconds);
        break;
      }
      case MOTOR_RIGHT_STEPS_TAG:
      {
        Serial.println("RIGHT_STEPS received."); //see above
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
        if(cmdValue == 1) //raises the claw, pretty straightforward
          raiseClaw();
        else
          lowerClaw();
        break;
      }
      case CLAW_CLOSE_TAG: //closes the claw
      {
        if(cmdValue == 1)
          closeClaw();
        else
          openClaw();
        break;
      }
      default:
        Serial.println("FML");
        Serial.print("Failing cmdTarget: "); //you screwed up and the arduino got jibberish 
        Serial.println(cmdTarget);
        Serial.print("Input: ");
        printInput(input);
    }
    for(int i = 0; i < 8; i++) //clears out the input buffer FOR SURE GOD DAMN IT BECAUSE I WAS SICK OF TROUBLESHOOTING
      input[i] = '\0';
    stringComplete = false; //it's time to look for a new input, fam
  }
  interrupts();
}



int assembleInt(unsigned char *string, int index)
{
  unsigned char inBytes[4] = {0}; //zeroes the storage array
  int num = 0;
  for(int i = 0; i < 4; i++) //grab the first four bytes starting at the index
  {
    inBytes[i] = (unsigned char) string[index + i];
  }
  for(int i = 3; i > -1; i--) //assemble the bytes into an "int" in arduino (long errywhere else)
  {
    num = num << 8;
    num = num | inBytes[i];
  }
  return num;
}

void clearBuffer() //literally reads and dumps everything in the serial buffer
{
  unsigned char nothing;
  while(Serial.available() > 0)
    nothing = Serial.read();
}

void printInput(unsigned char *input) //simple function for printing hte input string. useful for troubleshooting
{
  for(int i = 0; i < 8; i++)
    Serial.println(input[i]);
}

void updateSensors() //not sure how this wull be implemented officially, but it'll be similar probs
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
