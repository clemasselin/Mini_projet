
#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>
#include <main.h>
#include <motors.h>
#include <stdbool.h>
#include <leds.h>
#include <control_reaction.h>
#include <assess_environment.h>

#include <sign_detection.h>
//#include <motor_control.h>





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


      // chprintf((BaseSequentialStream *)&SDU1, "signal=%d\r\n", get_detection_state());



        if(get_initialized()){
        	if(!get_detection_state()){
        	switch(get_line_number()){

           case 1:
            clear_leds();

           	deviation = ((int)get_line_position() - (IMAGE_BUFFER_SIZE/2));
          	if(abs(deviation) < ROTATION_THRESHOLD){
          		deviation = 0;
        	}
          	right_motor_set_speed(speed - ROTATION_COEFF * deviation);
          	left_motor_set_speed(speed + ROTATION_COEFF * deviation);

              break;

           case 2:
           	set_led(LED7, 2);


//            if (get_detection_state() == RIGHT){
//            	set_led(LED7, 2);
//            	//right_motor_set_speed(speed - SPEED_CORRECTION);
//            //	left_motor_set_speed(speed + SPEED_CORRECTION );
//            }
//
//           	right_motor_set_speed(0);
//         	left_motor_set_speed(0);
//           	chThdSleepMilliseconds(3000);
//          	while(get_line_number()!=1){
//           		right_motor_set_speed(speed - SPEED_CORRECTION);
//           		left_motor_set_speed(speed +SPEED_CORRECTION );
//         	}
//          	right_motor_set_speed(0);
//          	left_motor_set_speed(0);
//
              break;

           case 3:

//               set_led(LED3, 2);
              right_motor_set_speed(0);
              left_motor_set_speed(0);
              chThdSleepMilliseconds(3000);
              while(get_line_number()!=1){
              right_motor_set_speed(speed);
              left_motor_set_speed(speed);
              }
             break;

       }
        	}
        	else{
        		if(get_line_number()== 2){
        		// right_motor_set_speed(0);
        		 //left_motor_set_speed(0);

        		signal_detection();
        		}

        	}
        }


        //100Hz
        chThdSleepUntilWindowed(time, time + MS2ST((5)));
       //chThdSleep( time + MS2ST((5)));
    }
}

void control_reaction_start(void){
chThdCreateStatic(waFollowLine, sizeof(waFollowLine), NORMALPRIO, FollowLine, NULL);
}
