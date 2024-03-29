#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <usbcfg.h>
#include <main.h>
#include <motors.h>

#include <msgbus/messagebus.h>
#include <spi_comm.h>
#include <camera/po8030.h>


#include <assess_environment.h>
#include <control_pannel.h>
#include <sign_detection.h>

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

	sign_detection_start();
	assess_environment_start();
	control_pannel_start();

    /* Infinite loop. */
    while (1) {
    	chThdSleepMilliseconds(MAIN_REFRESH_TIME);

    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
