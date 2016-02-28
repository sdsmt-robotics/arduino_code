////////////////////////////////////////////////////////////////////////////////
//    Library Includes
////////////////////////////////////////////////////////////////////////////////
#include "steppers.h"
#include "servos.h"

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

#define CLAW_CLOSE_TAG  0x41
#define CLAW_RAISE_TAG  0x42

////////////////////////////////////////////////////////////////////////////////
//    Global Variables
////////////////////////////////////////////////////////////////////////////////
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
unsigned int sensorValue = 0;
unsigned char cmdTarget = 0;

////////////////////////////////////////////////////////////////////////////////
//    Global Objects
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//    Setup Function
////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  inputString.reserve(2);
  
  initializeSteppers();
  initializeServos();
}

////////////////////////////////////////////////////////////////////////////////
//    Main Loop
////////////////////////////////////////////////////////////////////////////////

void loop() {  
  if (stringComplete)
  {
    cmdTarget = inputString[0];
    unsigned char cmdValue = inputString[1];
    //Serial.println(cmdValue);
    noInterrupts();
    switch(cmdTarget)
    {
      case MOTOR_LEFT_TAG:
      {
        if(cmdValue != 127)
          setLeftStepperRPM(map(cmdValue, 0, 255, -MAX_SPEED, MAX_SPEED));
        else
          setLeftStepperRPM(0);
        break;
      }
      case MOTOR_RIGHT_TAG:
      {
        if(cmdValue != 127)
          setRightStepperRPM(map(cmdValue, 0, 255, -MAX_SPEED, MAX_SPEED));
        else
          setRightStepperRPM(0);
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
    }
    interrupts();

    stringComplete = false;
    inputString = "";
  }
  //leftStepper.runSpeed();
  //rightStepper.runSpeed();
  //leftStepper.runSpeed();
  //rightStepper.runSpeed();
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  if (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    while (!Serial.available()) {}
    inChar = (char)Serial.read();
    inputString+= inChar;
    //Serial.println(inputString);
    stringComplete = true;
  }
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
