#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>


#include <main.h>
#include <motors.h>
#include <leds.h>
#include <sensors/proximity.h>
#include <sign_detection.h>
//#include <sensors/VL53L0X/VL53L0X.h>

static THD_WORKING_AREA(waSignDetection, 256);
static THD_FUNCTION(SignDetection, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    proximity_start();

    calibrate_ir();

    systime_t time;

    int speed = 0;
    //int distance_stop = 50;
    //int dist_mm = 0;


    while(1){

        time = chVTGetSystemTime();

//        VL53L0X_start();
//        dist_mm = VL53L0X_get_dist_mm();
//
//        if(dist_mm <= distance_stop){
//        	speed = 0;
//          }else{
//        	speed = 1000 ;
//        }

        if (get_calibrated_prox(1) > 0) {
        	speed = 0 ;
        } else {
        	speed = 1000 ;
        }

        //applies the speed from the sign  detection
        right_motor_set_speed(speed);
        left_motor_set_speed(speed);


        //100Hz
        chThdSleepUntilWindowed(time, time + MS2ST(10));
//        VL53L0X_stop();
    }
}

void sign_detection_start(void){
	chThdCreateStatic(waSignDetection, sizeof(waSignDetection), NORMALPRIO, SignDetection, NULL);
}
