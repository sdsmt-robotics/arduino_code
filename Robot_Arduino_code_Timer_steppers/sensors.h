#ifndef __SENSORS_H
#define __SENSORS_H

#include "globals.h"

#define LEFT_IR_PIN  A0
#define RIGHT_IR_PIN A1
#define FRONT_IR_PIN A2
#define BACK_IR_PIN A3

unsigned int leftValue = 0;
unsigned int rightValue = 0;
unsigned int frontValue = 0;
unsigned int backValue = 0;

unsigned long leftTemp, rightTemp, frontTemp, backTemp = 0;
unsigned int count = 0;
unsigned long lastMillis = 0;

void initializeSensors()
{
    pinMode(LEFT_IR_PIN, INPUT);
    pinMode(RIGHT_IR_PIN, INPUT);
    pinMode(FRONT_IR_PIN, INPUT);
    pinMode(BACK_IR_PIN, INPUT);
    lastMillis = millis();
}

void updateSensors()
{
    if((millis() - lastMillis) > 100) //averages and updates the "actual reportable" values erry 100 millis
    { 
        leftValue =  leftTemp / count;
        rightValue = rightTemp / count;
        frontValue = frontTemp / count;
        backValue =  backTemp / count;
    
        leftTemp =  0;
        rightTemp = 0;
        frontTemp = 0;
        backTemp =  0;

        count = 0;
        lastMillis = millis();
    }
    if((millis() - lastMillis) > 5) //updates the sensors every five millis
    {
      leftTemp += analogRead(LEFT_IR_PIN);
      rightTemp += analogRead(RIGHT_IR_PIN);
      frontTemp += analogRead(FRONT_IR_PIN);
      //backTemp += analogRead(BACK_IR_PIN);
      count++;
    }
}

void sendSensorValues()
{
    //packet to be extracted with bitshifting evil on the C side
    //unsigned ints are 2 unsigned chars on the Arduino
    unsigned int right_mask = 0xFF;
    unsigned int left_mask = 0xFF00;

    unsigned int left_byte = 0;
    unsigned int right_byte = 0;

    //Get and write each individual unsigned char of sensor value
    //bit things with leftValue
    left_byte = leftValue & left_mask;
    left_byte = left_byte >> 8;
    right_byte = leftValue & right_mask;

    out->write( (unsigned char) left_byte);
    out->write( (unsigned char) right_byte);

    //bit things with rightValue
    left_byte = rightValue & left_mask;
    left_byte = left_byte >> 8;
    right_byte = rightValue & right_mask;

    out->write( (unsigned char) left_byte);
    out->write( (unsigned char) right_byte);

    //bit things with frontValue
    left_byte = frontValue & left_mask;
    left_byte = left_byte >> 8;
    right_byte = frontValue & right_mask;

    out->write( (unsigned char) left_byte);
    out->write( (unsigned char) right_byte);

    //bit things with backValue
    left_byte = backValue & left_mask;
    left_byte = left_byte >> 8;
    right_byte = backValue & right_mask;

    out->write( (unsigned char) left_byte);
    out->write( (unsigned char) right_byte);
}

void sendFrontSensorValues()
{
    //packet to be extracted with bitshifting evil on the C side
    //unsigned ints are 2 unsigned chars on the Arduino
    unsigned int right_mask = 0xFF;
    unsigned int left_mask = 0xFF00;

    unsigned int left_byte = 0;
    unsigned int right_byte = 0;

    left_byte = frontValue & left_mask;
    left_byte = left_byte >> 8;
    right_byte = frontValue & right_mask;

    out->write( (unsigned char) left_byte);
    out->write( (unsigned char) right_byte);
}

void sendBackSensorValues()
{
    //packet to be extracted with bitshifting evil on the C side
    //unsigned ints are 2 unsigned chars on the Arduino
    unsigned int right_mask = 0xFF;
    unsigned int left_mask = 0xFF00;

    unsigned int left_byte = 0;
    unsigned int right_byte = 0;

    left_byte = backValue & left_mask;
    left_byte = left_byte >> 8;
    right_byte = backValue & right_mask;

    out->write( (unsigned char) left_byte);
    out->write( (unsigned char) right_byte);
}

void sendLeftSensorValues()
{
    //packet to be extracted with bitshifting evil on the C side
    //unsigned ints are 2 unsigned chars on the Arduino
    unsigned int right_mask = 0xFF;
    unsigned int left_mask = 0xFF00;

    unsigned int left_byte = 0;
    unsigned int right_byte = 0;

    left_byte = leftValue & left_mask;
    left_byte = left_byte >> 8;
    right_byte = leftValue & right_mask;

    out->write( (unsigned char) left_byte);
    out->write( (unsigned char) right_byte);
}

void sendRightSensorValues()
{
    //packet to be extracted with bitshifting evil on the C side
    //unsigned ints are 2 unsigned chars on the Arduino
    unsigned int right_mask = 0xFF;
    unsigned int left_mask = 0xFF00;

    unsigned int left_byte = 0;
    unsigned int right_byte = 0;

    left_byte = rightValue & left_mask;
    left_byte = left_byte >> 8;
    right_byte = rightValue & right_mask;

    out->write( (unsigned char) left_byte);
    out->write( (unsigned char) right_byte);
}

#endif


