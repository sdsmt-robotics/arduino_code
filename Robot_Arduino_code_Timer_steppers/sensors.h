#ifndef __SENSORS_H
#define __SENSORS_H

#define LEFT_IR_PIN  A2
#define RIGHT_IR_PIN A0
#define FRONT_IR_PIN A1

unsigned int leftValue = 0;
unsigned int rightValue = 0;
unsigned int frontValue = 0;

void initializeSensors()
{
    pinMode(LEFT_IR_PIN, INPUT);
    pinMode(RIGHT_IR_PIN, INPUT);
    pinMode(FRONT_IR_PIN, INPUT);
}

void updateSensors()
{
    leftValue = analogRead(LEFT_IR_PIN);
    rightValue = analogRead(RIGHT_IR_PIN);
    frontValue = analogRead(FRONT_IR_PIN);
}

void sendSensorValues()
{
    //Serial.write(IR_LEFT_TAG);
    Serial.print("Left value: ");
    Serial.println(leftValue);
    //write the two bytes for the value here
    //Serial.write(IR_RIGHT_TAG);
    Serial.print("Right value: ");
    Serial.println(rightValue);
    //write the two bytes for the value here
    //Serial.write(IR_FRONT_TAG);
    Serial.print("Front value: ");
    Serial.println(frontValue);
    //write the two bytes for the value here
}



#endif
