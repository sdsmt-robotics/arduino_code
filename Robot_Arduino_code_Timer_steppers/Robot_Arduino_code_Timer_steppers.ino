////////////////////////////////////////////////////////////////////////////////
//    Library Includes
////////////////////////////////////////////////////////////////////////////////
#include "steppers.h"
#include "servos.h"
#include "timed_drive.h"
#include "tags.h"
#include "sensors.h"
#include "globals.h"

////////////////////////////////////////////////////////////////////////////////
//    Global Variables
////////////////////////////////////////////////////////////////////////////////
bool stringComplete = false;
unsigned int sensorValue = 0;
unsigned char input[8] = {0};

unsigned char cmdTarget = 0;
unsigned char cmdValue = 0;

////////////////////////////////////////////////////////////////////////////////
//    Setup Function
////////////////////////////////////////////////////////////////////////////////

void setup() {
    in->begin(115200);
    out->begin(115200);
    initializeSteppers();
    initializeServos();
    initializeSensors();
}

////////////////////////////////////////////////////////////////////////////////
//    Main Loop
////////////////////////////////////////////////////////////////////////////////

void loop() 
{
    if (in->available() > 0)
    {
        cmdTarget = in->read(); //first byte is the identifying tag
        if(cmdTarget == MOTOR_LEFT_STEPS_TAG || cmdTarget == MOTOR_RIGHT_STEPS_TAG) //if the command is a # of steps in a time period, grab the next 8 bytes
        {
            for(int i = 0; i < 8; i++)
            {
                while(!in->available()){} //makes sure there's something to read
                input[i] = in->read();
            }
            stringComplete = true;
        }
        else if (cmdTarget == SENSOR_REQUEST || cmdTarget == LEFT_SENSOR_REQUEST || cmdTarget == RIGHT_SENSOR_REQUEST || cmdTarget == FRONT_SENSOR_REQUEST || cmdTarget == BACK_SENSOR_REQUEST)
        {
            //we're not expecting a second byte, so just move right along
            stringComplete = true;
        }
        else
        {
            while(!in->available()){} //if the second byte of two hasn't come in, chill and wait for it
            cmdValue = in->read(); //this is a speed value from 0-255 centered on 127 used for constant-speed driving
            stringComplete = true;
        }

        noInterrupts();
        if (stringComplete)
        {
            switch (cmdTarget)
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
                    //in->println("\tLEFT_STEPS received."); //lots of debug printing
                    //in->print("\tsteps: ");
                    int steps = assembleInt(input, 0); //grabs the first four bytes, makes a number
                    //in->println(steps);
                    //in->print("\tseconds: ");
                    int seconds = assembleInt(input, 4); //grabs the second 4 bytes, makes a number
                    //in->println(seconds);
                    setLeftStepperStepsTime(steps, seconds);
                    break;
                }
                case MOTOR_RIGHT_STEPS_TAG:
                {
                    //in->println("\tRIGHT_STEPS received."); //see above
                    //in->print("\tsteps: ");
                    int steps = assembleInt(input, 0);
                    //in->println(steps);
                    //in->print("\tseconds: ");
                    int seconds = assembleInt(input, 4);
                    //in->println(seconds);
                    setRightStepperStepsTime(steps, seconds);
                    break;
                }
                case CLAW_RAISE_TAG:
                {
                    //in->println("Raise tag");
                    if(cmdValue == 255) //raises the claw, pretty straightforward
                        raiseClaw();
                    else
                        lowerClaw();
                    break;
                }
                case CLAW_CLOSE_TAG: //closes the claw
                {
                    //in->println("Close tag");
                    if(cmdValue == 255)
                        closeClaw();
                    else
                        openClaw();
                    break;
                }
                case SENSOR_REQUEST:
                {
                    sendSensorValues();
                    break;
                }
                case FRONT_SENSOR_REQUEST:
                {
                    sendFrontSensorValues();
                    break;
                }
                case BACK_SENSOR_REQUEST:
                {
                    sendBackSensorValues();
                    break;
                }
                case LEFT_SENSOR_REQUEST:
                {
                    sendLeftSensorValues();
                    break;
                }
                case RIGHT_SENSOR_REQUEST:
                {
                    sendRightSensorValues();
                    break;
                }
                default:
                {
                    in->println("\tFML. Bad motor tag received.");
                    in->print("\tFailing cmdTarget: "); //you screwed up and the arduino got jibberish 
                    in->println(cmdTarget);
                    //in->println("\tInput: ");
                    unsigned char nothing = in->read();
                    clearBuffer();
                    //printInput(input);
                }
            }     
        }
        cmdTarget = '\0';
        for(int i = 0; i < 8; i++) //clears out the input buffer FOR SURE GOD DAMN IT BECAUSE I WAS SICK OF TROUBLESHOOTING
            input[i] = '\0';
        stringComplete = false; //it's time to look for a new input, fam
    }
    interrupts();
    updateSensors();
}



int assembleInt(unsigned char *input, int offset)
{
    unsigned char inBytes[4] = {0}; //zeroes the storage array
    int num = 0;
    for(int i = 0; i < 4; i++) //grab the first four bytes starting at the index
    {
        inBytes[i] = input[offset + i];
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
    while(in->available() > 0)
        nothing = in->read();
}

void printInput(unsigned char *input) //simple function for printing hte input string. useful for troubleshooting
{
    for(int i = 0; i < 8; i++)
        in->println(input[i]);
}


