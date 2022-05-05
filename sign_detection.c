#include <assess_environment.h>
#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>


#include <main.h>
#include <motors.h>
#include <sensors/proximity.h>
#include <sign_detection.h>
#include <robot_reactions.h>
#include <leds.h>
#include <chprintf.h>


//void sign_reading(){
//	chprintf((BaseSequentialStream *)&SDU1, "line_width=%d\n", lineWidth);
//
//
//}


static THD_WORKING_AREA(waSignDetection, 256);
static THD_FUNCTION(SignDetection, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    /* start the thread for proximity sensor */
    proximity_start();

    /* start the calibration of the proximity sensor */
    calibrate_ir();

    /* waits 3 second */
    chThdSleepMilliseconds(3000);


    systime_t time;

    int speed = 0;


    while(1){

        time = chVTGetSystemTime();

        /* 0 set as the reference at the calibration so need to turn
         * on the robot with a sign as far as we want it to stop*/
        if (get_calibrated_prox(1) > 1) {

        	speed = 0 ;
            right_motor_set_speed(speed);
            left_motor_set_speed(speed);

            //sign_reading();


        	/* start the selection of reactions of the robot*/
        	//warning_rgb();
        	warning();

        	/* waits 0.1 second */
        	chThdSleepMilliseconds(100);

        } else {

        	speed = 1000 ;

        }

        /*applies the speed from the sign  detection */
        right_motor_set_speed(speed);
        left_motor_set_speed(speed);

        if(speed > 10){

        	/* Turn off all leds*/
        	//clear_leds_rgb();
        	clear_leds();
        }


        /* 100Hz */
        chThdSleepUntilWindowed(time, time + MS2ST(10));
    }
}



void sign_detection_start(void){
	chThdCreateStatic(waSignDetection, sizeof(waSignDetection), NORMALPRIO+1, SignDetection, NULL);
}
