//#include "ch.h"
//#include "hal.h"
//#include <math.h>
//#include <usbcfg.h>
//#include <chprintf.h>
//#include <main.h>
//#include <motors.h>
//#include <follow_line.h>
//#include <process_image.h>
//
//
//
//
//static THD_WORKING_AREA(waFollowLine, 256);
//static THD_FUNCTION(FollowLine, arg) {
//
//    chRegSetThreadName(__FUNCTION__);
//    (void)arg;
//
//    systime_t time;
//
//    int16_t speed = 320;
//    int16_t deviation = 0;
//
//    while(1){
//        time = chVTGetSystemTime();
//
//        deviation = (get_line_position() - (IMAGE_BUFFER_SIZE/2));
//        if(abs(deviation) < ROTATION_THRESHOLD){
//        	deviation = 0;
//        }
//		right_motor_set_speed(speed - ROTATION_COEFF * deviation);
//		left_motor_set_speed(speed + ROTATION_COEFF * deviation);
//
//
////       if(deviation>DEV_TRESHOLD){
////
////        left_motor_set_speed(speed + ROTATION_COEFF * deviation);
////        right_motor_set_speed(speed - ROTATION_COEFF * deviation);
////        }
////        if(deviation<-DEV_TRESHOLD){
////        left_motor_set_speed(speed - ROTATION_COEFF * deviation);
////		right_motor_set_speed(speed + ROTATION_COEFF * deviation);
////       }
//
//
//        //100Hz
//        chThdSleepUntilWindowed(time, time + MS2ST((5)));
//    }
//}
//
//void follow_line_start(void){
//	chThdCreateStatic(waFollowLine, sizeof(waFollowLine), NORMALPRIO, FollowLine, NULL);
//}
