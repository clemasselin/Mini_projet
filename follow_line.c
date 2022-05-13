
#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>
#include <main.h>
#include <motors.h>
#include <follow_line.h>
#include <stdbool.h>
#include <leds.h>
#include <assess_environment.h>


int16_t pi_regulator(float pos){

	float error = 0;
	float speed_correction = 0;

	static float sum_error = 0;

	error = pos- (IMAGE_BUFFER_SIZE/2);

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

    return (int16_t) speed_correction;
}

//int8_t process_line_number(void){
//
//	//get a sample of 5 line_number and extract the most displayed number of the sample
//
//	uint8_t n=0; // counts number of identical value
//	uint8_t k=0;//table index
//	uint8_t sample[5] = {0}; //5 value sample table
//
//
//
//	//fills table
//	for (uint8_t i = 0 ; i<4 ; i++){
//		sample[i]=get_line_number();
//	}
//	//compares one line measure to the next in the table
//	while(k<4){
//		for(uint8_t j = k+1 ; j<5 ; j++){
//			if(sample[j]==sample[k]){
//				n+=1;
//			}
//		}
//		//if not enough equal number in data, look at next element of table
//		if(n<2){
//			k+=1;
//		}else{
//			return sample[k];
//		}
//
//	}
//
//	return 0;
//}

static THD_WORKING_AREA(waFollowLine, 256);
static THD_FUNCTION(FollowLine, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    systime_t time;

    int16_t speed = 250; //250
    int16_t deviation = 0;
    //int16_t correction;

    while(1){
        time = chVTGetSystemTime();
        //right_motor_set_speed(speed);
       // left_motor_set_speed(speed);
        //

        //chprintf((BaseSequentialStream *)&SDU1, "line_number=%d\r\n", process_line_number());

       //chprintf((BaseSequentialStream *)&SDU1, "line=%d\r\n", get_line_number());



      //  switch(get_line_number()){




            //case 1:
            	//set_led(LED7, 2);

              // find deviation from line and follow it

           // clear_leds();
           	deviation = ((int)get_line_position() - (IMAGE_BUFFER_SIZE/2));
          	if(abs(deviation) < ROTATION_THRESHOLD){
          		deviation = 0;
        	}
          //chprintf((BaseSequentialStream *)&SDU1, "deviation=%d\r\n", deviation);
     right_motor_set_speed(speed - ROTATION_COEFF * deviation);
     left_motor_set_speed(speed + ROTATION_COEFF * deviation);



            //correction=pi_regulator(get_line_position()) ;
          // right_motor_set_speed(speed -correction);
         //  left_motor_set_speed(speed +correction );


              //break;

            //case 2:
            //	set_led(LED7, 2);
            	//right_motor_set_speed(0);
            	//left_motor_set_speed(0);

            	//set_body_led(1);
              // stop, sign detection and turn

             // break;

            //case 3:
              // set_led(LED3, 2);
            	//set_body_led(1);
              // right_motor_set_speed(0);
              // left_motor_set_speed(0);
              // U-turn

             // break;



 //           default:
  //             default statements
    //    }





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
       //chThdSleep( time + MS2ST((5)));
    }
}

void follow_line_start(void){
chThdCreateStatic(waFollowLine, sizeof(waFollowLine), NORMALPRIO, FollowLine, NULL);
}
