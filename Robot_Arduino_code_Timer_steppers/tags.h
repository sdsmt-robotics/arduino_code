#ifndef __TAGS_H
#define __TAGS_H

#include "globals.h"

////////////////////////////////////////////////////////////////////////////////
//    Packet Designations
////////////////////////////////////////////////////////////////////////////////

#define SENSOR_REQUEST 0x60
#define LEFT_SENSOR_REQUEST 0x61
#define RIGHT_SENSOR_REQUEST 0x62
#define FRONT_SENSOR_REQUEST 0x63
#define BACK_SENSOR_REQUEST 0x64

#define IR_RIGHT_TAG  0x71
#define IR_FRONT_TAG 0x72
#define IR_LEFT_TAG   0x73
#define IR_PACKET_END 0x74

#define MOTOR_LEFT_TAG  0x4C
#define MOTOR_RIGHT_TAG 0x52

#define MOTOR_LEFT_STEPS_TAG  0x39
#define MOTOR_RIGHT_STEPS_TAG 0x40

#define CLAW_CLOSE_TAG  0x41
#define CLAW_RAISE_TAG  0x42

#endif



