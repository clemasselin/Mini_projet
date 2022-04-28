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


messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);


int main(void)
{
    halInit();
    chSysInit();
    mpu_init();

    /* inits the spi communication bus. */
    spi_comm_start();

    /* inits the motors. */
	motors_init();

	/* Inits the Inter Process Communication bus. */
	messagebus_init(&bus, &bus_lock, &bus_condvar);

	/* start the threads for the detection of signs and the reaction of
	the robot in consequences with the distance sensor (IR). */
	sign_detection_start();



    /* Infinite loop. */
    while (1) {

    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
