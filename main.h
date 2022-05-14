#ifndef MAIN_H
#define MAIN_H


//#define IMAGE_BUFFER_SIZE 1000
//#define ROTATION_THRESHOLD 1000
//#define ROTATION_COEFF 1000
//#define PXTOCM 1000
//#define GOAL_DISTANCE 1000
//#define WIDTH_SLOPE 1000
//#define MIN_LINE_WIDTH 1000
//#define MAX_DISTANCE 1000


void delay(unsigned int n);

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
