
#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <main.h>
#include <motors.h>
#include <stdbool.h>
#include <leds.h>
#include <assess_environment.h>
#include <circulation_instructions.h>
#include <control_pannel.h>
#include <sign_detection.h>





static THD_WORKING_AREA(waControlPannel, 256);
static THD_FUNCTION(ControlPannel, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    systime_t time;
    int16_t deviation = ZERO;

    while(1){
        time = chVTGetSystemTime();

        //waits until IR is calibrated
        if(get_initialized()){
        	keep_running(); //leaves point of departure

        	//if no signal detected, look at number of lines detected by camera
        	if(!get_detection_state()){
        		switch(get_line_number()){

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        			//stops at the wide black strip
        			case 0:

        				stop();
        				warning_rgb();
        				chThdSleepMilliseconds(LONG_TIME);

        				break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        			//if one line detected, follow it
        			case 1:

        				clear_leds();
        				deviation = ((int)get_line_position() - (IMAGE_BUFFER_SIZE/TWO));
        				follow_line(deviation);

        				break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        			//traffic light at three-line-pattern
        			case 3:

        	   	   	   traffic_light(STOP);
        	   	   	   stop();
        	   	   	   chThdSleepMilliseconds(LONG_TIME);
        	   	   	   traffic_light(GO);
        	   	   	   while(get_line_number()!= ONE){
        	   	   		   chThdSleepMilliseconds(SHORT_TIME);
        	   	   		   keep_running();
        	   	   	   }

        	   	   	   break;
        			}
        		}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        		//if sign detected
        		else{
        			//if sign on the right, park the robot
        			if(get_detection_state() == RIGHT){
        				turn_left_90();
        			//if sign on the left, make a U-turn
        			}else{
        				turn_right_180();
        			}
        		}
        }
        chThdSleepUntilWindowed(time, time + MS2ST((REFRESH_TIME)));
    }
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void control_pannel_start(void){
chThdCreateStatic(waControlPannel, sizeof(waControlPannel), NORMALPRIO, ControlPannel, NULL);
}
