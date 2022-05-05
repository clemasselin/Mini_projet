#ifndef MAIN_H
#define MAIN_H

#include "camera/dcmi_camera.h"
#include "msgbus/messagebus.h"
#include "parameter/parameter.h"

#define IMAGE_BUFFER_SIZE		640
#define MAX_DISTANCE 			25.0f
#define PXTOCM					1570.0f
#define GOAL_DISTANCE 			10.0f
#define MIN_LINE_WIDTH			40
#define WIDTH_SLOPE				5
#define ROTATION_THRESHOLD		5
#define ROTATION_COEFF			2
#define DEFAULT_SPEED			300
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
