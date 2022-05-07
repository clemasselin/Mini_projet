#include <assess_environment.h>
#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>
#include <main.h>
#include <motors.h>
#include <follow_line.h>


int16_t pi_regulator(void){

	float error = 0;
	float speed = 0;

	static float sum_error = 0;

	error = get_line_position() - (IMAGE_BUFFER_SIZE/2);

	//disables the PI regulator if the error is to small
	//this avoids to always move as we cannot exactly be where we want and
	//the camera is a bit noisy
	if(fabs(error) < ERROR_THRESHOLD){
		return 0;
	}

	sum_error += error;

	//we set a maximum and a minimum for the sum to avoid an uncontrolled growth
	if(sum_error > MAX_SUM_ERROR){
		sum_error = MAX_SUM_ERROR;
	}else if(sum_error < -MAX_SUM_ERROR){
		sum_error = -MAX_SUM_ERROR;
	}

	speed_correction = KP * error + KI * sum_error;

    return (int16_t)speed_correction;
}

static THD_WORKING_AREA(waFollowLine, 256);
static THD_FUNCTION(FollowLine, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    systime_t time;

    int16_t speed = 250;
    int16_t deviation = 0;

    while(1){
        time = chVTGetSystemTime();



//
//
//            case 1:
//
//              // find deviation from line and follow it
//
////            	deviation = (get_line_position() - (IMAGE_BUFFER_SIZE/2));
////            	if(abs(deviation) < ROTATION_THRESHOLD){
////            		deviation = 0;
////            	}
////
////            	right_motor_set_speed(speed - ROTATION_COEFF * deviation);
////            	left_motor_set_speed(speed + ROTATION_COEFF * deviation);
//
//              break;
//
//            case 2:
//              // stop, sign detection and turn
//
//              break;
//
//            case 3:
//              // U-turn
//
//              break;
//
//
//
//            default:
//              // default statements
//        }





//       if(deviation>DEV_TRESHOLD){
//
//        left_motor_set_speed(speed + ROTATION_COEFF * deviation);
//        right_motor_set_speed(speed - ROTATION_COEFF * deviation);
//        }
//        if(deviation<-DEV_TRESHOLD){
//        left_motor_set_speed(speed - ROTATION_COEFF * deviation);
//		right_motor_set_speed(speed + ROTATION_COEFF * deviation);
//       }


        //100Hz
        chThdSleepUntilWindowed(time, time + MS2ST((5)));
    }
}

void follow_line_start(void){
	chThdCreateStatic(waFollowLine, sizeof(waFollowLine), NORMALPRIO, FollowLine, NULL);
}
