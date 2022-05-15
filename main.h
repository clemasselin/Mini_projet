#ifndef MAIN_H
#define MAIN_H

#include "camera/dcmi_camera.h"
#include "msgbus/messagebus.h"
#include "parameter/parameter.h"

#define SPEED_CORRECTION		100
#define IMAGE_BUFFER_SIZE		640
#define MAX_DISTANCE 			25.0f
#define PXTOCM					1570.0f
#define GOAL_DISTANCE 			10.0f
#define MIN_LINE_WIDTH			20
#define WIDTH_SLOPE				5
#define ROTATION_THRESHOLD		5
#define ROTATION_COEFF			2
#define DEFAULT_MOVING_SPEED	300
#define ERROR_THRESHOLD			0.1f	//[cm] because of the noise of the camera
#define KP						800.0f
#define KI 						3.5f	//must not be zero
#define MAX_SUM_ERROR 			(MOTOR_SPEED_LIMIT/KI)


#define ZERO 0
#define LEDS_DELAY 5
#define MOTORS_DELAY 5
#define RUNNING_DELAY 1
#define RIGHT 1
#define LEFT 2
#define NONE 0

//#define IMAGE_BUFFER_SIZE 1000
//#define ROTATION_THRESHOLD 1000
//#define ROTATION_COEFF 1000
//#define PXTOCM 1000
//#define GOAL_DISTANCE 1000
//#define WIDTH_SLOPE 1000
//#define MIN_LINE_WIDTH 1000
//#define MAX_DISTANCE 1000


void delay(unsigned int n);
void SendUint8ToComputer(uint8_t* data, uint16_t size);

#ifdef __cplusplus
extern "C" {
#endif

#include "camera/dcmi_camera.h"
#include "msgbus/messagebus.h"
#include "parameter/parameter.h"


/** Robot wide IPC bus. */
extern messagebus_t bus;

extern parameter_namespace_t parameter_root;

#ifdef __cplusplus
}
#endif

#endif
