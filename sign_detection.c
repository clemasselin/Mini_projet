#include "ch.h"
#include "hal.h"
#include <math.h>


#include <main.h>
#include <motors.h>
#include <sensors/proximity.h>
#include <sign_detection.h>

static THD_WORKING_AREA(waSignDetection, 256);
static THD_FUNCTION(SignDetection, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    proximity_start();

    calibrate_ir();

    systime_t time;

    int speed = 0;


    while(1){

        time = chVTGetSystemTime();

        /* 0 set as the reference at the calibration so need to turn
         * on the robot with a sign as far as we want it to stop*/
        if (get_calibrated_prox(1) > 0) {
        	speed = 0 ;
        } else {
        	speed = 1000 ;
        }

        /*applies the speed from the sign  detection */
        right_motor_set_speed(speed);
        left_motor_set_speed(speed);


        /* 100Hz */
        chThdSleepUntilWindowed(time, time + MS2ST(10));
    }
}

void sign_detection_start(void){
	chThdCreateStatic(waSignDetection, sizeof(waSignDetection), NORMALPRIO+1, SignDetection, NULL);
}
