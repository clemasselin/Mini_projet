#ifndef MAIN_H
#define MAIN_H

#include "camera/dcmi_camera.h"
#include "msgbus/messagebus.h"
#include "parameter/parameter.h"


#define ZERO 					0
#define ONE						1
#define TWO 					2
#define RIGHT					1
#define LEFT 					2
#define NONE 					0

#define REFRESH_TIME			5
#define LONG_TIME 				3000
#define SHORT_TIME				1000
#define MAIN_REFRESH_TIME		1000


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
