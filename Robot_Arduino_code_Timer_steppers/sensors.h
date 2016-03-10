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
    //Serial.print("Left value: ");
    //Serial.println(leftValue);
    //write the two bytes for the value here
    //Serial.write(IR_RIGHT_TAG);
    //Serial.print("Right value: ");
    //Serial.println(rightValue);
    //write the two bytes for the value here
    //Serial.write(IR_FRONT_TAG);
    //Serial.print("Front value: ");
    //Serial.println(frontValue);
    //write the two bytes for the value here

    //packet to be extracted with bitshifting evil on the C side
    //unsigned ints are 2 bytes on the Arduino

    char left[4] = "";
    char right[4] = "";
    char front[4] = "";

    sprintf(left, "%d", leftValue);
    sprintf(right, "%d", rightValue);
    sprintf(front, "%d", frontValue);

    //add a leading 0 to the string if Values are two digits
    if ( strlen(left) == 2 )
    {
        //dest, src, num bytes
        memmove(&left[1], &left[0], sizeof(left) - sizeof(*left));
        left[0] = '0';
        left[3] = '\0';
    }

    if ( strlen(right) == 2 )
    {
        memmove(&right[1], &right[0], sizeof(right) - sizeof(*right));
        right[0] = '0';
        right[3] = '\0';
    }

    if ( strlen(front) == 2 )
    {
        memmove(&front[1], &front[0], sizeof(front) - sizeof(*front));
        front[0] = '0';
        front[3] = '\0';
    }
    
    
    Serial.print(left);
    Serial.print(right);
    Serial.print(front);

}



#endif
