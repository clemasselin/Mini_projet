#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"

#include <main.h>
#include <motors.h>
#include <sign_detection.h>
#include <msgbus/messagebus.h>
#include <spi_comm.h>
#include <camera/po8030.h>
#include <chprintf.h>
#include <process_image.h>
#include <follow_line.h>

void SendUint8ToComputer(uint8_t* data, uint16_t size)
{
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)"START", 5);
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)&size, sizeof(uint16_t));
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)data, size);
}

static void serial_start(void)
{
	static SerialConfig ser_cfg = {
	    115200,
	    0,
	    0,
	    0,
	};

	sdStart(&SD3, &ser_cfg); // UART3.
}

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);


int main(void)
{
    halInit();
    chSysInit();
    mpu_init();

    //starts the serial communication
    serial_start();
    //start the USB communication
    usb_start();
    //starts the camera
    dcmi_start();
	po8030_start();

    /* inits the spi communication bus. */
    spi_comm_start();

    /* inits the motors. */
	motors_init();

	/* Inits the Inter Process Communication bus. */
	messagebus_init(&bus, &bus_lock, &bus_condvar);

	/* start the threads for the detection of signs and the reaction of
	the robot in consequences with the distance sensor (IR). */
	sign_detection_start();
	process_image_start();
	follow_line_start();



    /* Infinite loop. */
    while (1) {
    	chThdSleepMilliseconds(1000);

    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
