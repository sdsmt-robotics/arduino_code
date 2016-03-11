#ifndef __SENSORS_H
#define __SENSORS_H

#define LEFT_IR_PIN  A2
#define RIGHT_IR_PIN A0
#define FRONT_IR_PIN A1
#define BACK_IR_PIN A3

unsigned int leftValue = 0;
unsigned int rightValue = 0;
unsigned int frontValue = 0;
unsigned int backValue = 0;

void initializeSensors()
{
    pinMode(LEFT_IR_PIN, INPUT);
    pinMode(RIGHT_IR_PIN, INPUT);
    pinMode(FRONT_IR_PIN, INPUT);
    pinMode(BACK_IR_PIN, INPUT);
}

void updateSensors()
{
    leftValue = analogRead(LEFT_IR_PIN);
    rightValue = analogRead(RIGHT_IR_PIN);
    frontValue = analogRead(FRONT_IR_PIN);
    backValue = analogRead(BACK_IR_PIN);
}

void sendSensorValues()
{
    //packet to be extracted with bitshifting evil on the C side
    //unsigned ints are 2 bytes on the Arduino
    unsigned int right_mask = 0xFF;
    unsigned int left_mask = 0xFF00;

    unsigned short left_byte = 0;
    unsigned short right_byte = 0;

    //Get and write each individual byte of sensor value
    //bit things with leftValue
    left_byte = leftValue & left_mask;
    left_byte = left_byte >> 8;
    right_byte = leftValue & right_mask;

    Serial.write( (unsigned char) left_byte);
    Serial.write( (unsigned char) right_byte);

    //bit things with rightValue
    left_byte = rightValue & left_mask;
    left_byte = left_byte >> 8;
    right_byte = rightValue & right_mask;

    Serial.write( (unsigned char) left_byte);
    Serial.write( (unsigned char) right_byte);

    //bit things with frontValue
    left_byte = frontValue & left_mask;
    left_byte = left_byte >> 8;
    right_byte = frontValue & right_mask;

    Serial.write( (unsigned char) left_byte);
    Serial.write( (unsigned char) right_byte);

    //bit things with backValue
    left_byte = backValue & left_mask;
    left_byte = left_byte >> 8;
    right_byte = backValue & right_mask;

    Serial.write( (unsigned char) left_byte);
    Serial.write( (unsigned char) right_byte);
}

void sendFrontSensorValues()
{
    //packet to be extracted with bitshifting evil on the C side
    //unsigned ints are 2 bytes on the Arduino
    unsigned int right_mask = 0xFF;
    unsigned int left_mask = 0xFF00;

    unsigned short left_byte = 0;
    unsigned short right_byte = 0;

    left_byte = frontValue & left_mask;
    left_byte = left_byte >> 8;
    right_byte = frontValue & right_mask;

    Serial.write( (unsigned char) left_byte);
    Serial.write( (unsigned char) right_byte);
}

void sendBackSensorValues()
{
    //packet to be extracted with bitshifting evil on the C side
    //unsigned ints are 2 bytes on the Arduino
    unsigned int right_mask = 0xFF;
    unsigned int left_mask = 0xFF00;

    unsigned short left_byte = 0;
    unsigned short right_byte = 0;

    left_byte = backValue & left_mask;
    left_byte = left_byte >> 8;
    right_byte = backValue & right_mask;

    Serial.write( (unsigned char) left_byte);
    Serial.write( (unsigned char) right_byte);
}

void sendLeftSensorValues()
{
    //packet to be extracted with bitshifting evil on the C side
    //unsigned ints are 2 bytes on the Arduino
    unsigned int right_mask = 0xFF;
    unsigned int left_mask = 0xFF00;

    unsigned short left_byte = 0;
    unsigned short right_byte = 0;

    left_byte = leftValue & left_mask;
    left_byte = left_byte >> 8;
    right_byte = leftValue & right_mask;

    Serial.write( (unsigned char) left_byte);
    Serial.write( (unsigned char) right_byte);
}

void sendRightensorValues()
{
    //packet to be extracted with bitshifting evil on the C side
    //unsigned ints are 2 bytes on the Arduino
    unsigned int right_mask = 0xFF;
    unsigned int left_mask = 0xFF00;

    unsigned short left_byte = 0;
    unsigned short right_byte = 0;

    left_byte = rightValue & left_mask;
    left_byte = left_byte >> 8;
    right_byte = rightValue & right_mask;

    Serial.write( (unsigned char) left_byte);
    Serial.write( (unsigned char) right_byte);
}

#endif
