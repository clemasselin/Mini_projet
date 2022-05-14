#include "ch.h"
#include "hal.h"
#include <math.h>

#include <main.h>
#include <motors.h>
#include <sensors/proximity.h>
#include <sign_detection.h>
#include <robot_reactions.h>
#include <leds.h>
#include <stdbool.h>

/* Declaration of static global variables */
static bool direction_statut = true;
static int turning_instruction_state = 0;

static THD_WORKING_AREA(waSignDetection, 256);
static THD_FUNCTION(SignDetection, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    /* start the thread for proximity sensor */
    proximity_start();

    /* start the calibration of the proximity sensor */
    calibrate_ir();;

    /* Leds indication of time before the robot start to move, time used to calibrate the IR sensors */
	initialisation();

    systime_t time;

    /* Declaration of local variables */
    int counter_leds_delay = LEDS_DELAY;
    int counter_motors_delay = MOTORS_DELAY;
    int keep_running_delay = ZERO;
    int calibration = get_prox(3);



    while(1){

    	/*A SUPPRIMER AVANT RENDU : juste pour afficher les valeurs*/
    	//chprintf((BaseSequentialStream *)&SD3, "variable=%d\r\n", variable);
    	/*---------------------------------------------------------*/

        time = chVTGetSystemTime();

        /* Function use to verified if the robot detect a sign (right or left) or not*/
        int detected(void){

        	 if(get_prox(1)/calibration > 0){
        		 return RIGHT;
        	 }else if(get_prox(6)/calibration > 0){
        		 return LEFT;
        	 }else{
        		 return NONE;
        	 }
        }


        /* Comparing the value return by both proximity sensors IR2 and IR8 with
         * the value we decided to use as a calibration at the beginning. If the result of the division is higher than 0 then  the robot
         * has detected a sign. The value returning by the function indicates the type of the turn wanted. 0 = no sign, 1 = right turn, 2 = left turn*/

        if (detected() == RIGHT){

        	/* Call the functions that force the robot to go again by ignoring the sign detection until it passe the sign and then turn
        	 *  in the direction wanted until the line research thread take the lead again*/
        	counter_motors_delay = avoid_sign(counter_motors_delay, counter_leds_delay, keep_running_delay, calibration);

        	/* Update the variables used to turn for a right turn */
        	direction_statut = true;
        	turning_instruction_state = RIGHT;

        	/* waits 0.4 second */
        	chThdSleepMilliseconds(400);

        }else if(detected() == LEFT){

        	counter_motors_delay = avoid_sign(counter_motors_delay, counter_leds_delay, keep_running_delay, calibration);

        	/* Update the variables used to turn for a left turn */
        	direction_statut = false;
        	turning_instruction_state = LEFT;


        	chThdSleepMilliseconds(400);

        }else{

        	/* Force the robot to continue along the line while it detects a sign */
        	keep_running();

        	/* Update the variable used to turn for no turn detected */
        	turning_instruction_state = NONE;
        }

        /* 100Hz */
        chThdSleepUntilWindowed(time, time + MS2ST(10));
    }
}



/* Sign detection thread starter */
void sign_detection_start(void){
	chThdCreateStatic(waSignDetection, sizeof(waSignDetection), NORMALPRIO+1, SignDetection, NULL);
}


/* Function that make the robot wait in front of the sign for a amount of time then let it go forward and ignoring the sign */
int avoid_sign(int counter_motors_delay, int counter_leds_delay, int keep_running_delay, int calibration){

	/* decrease the constant used as a delay to make the robot stops for a amount of time */
	if(counter_motors_delay > 0){

		stop();

		counter_leds_delay = delay_leds_toggle(counter_leds_delay);

	    counter_motors_delay -= 1;

	}else{

		/* While the last delay is over, the loop force the robot to move forward enough to go past the sign */
		while(keep_running_delay <= RUNNING_DELAY){


			keep_running();
			counter_leds_delay = delay_leds_toggle(counter_leds_delay);

			keep_running_delay += 1;

			/* waits 0.5 second */
			chThdSleepMilliseconds(500);
		}

		/* applies the action indicated by the sign */
		if(turning_instruction_state == RIGHT){

			/* Make the robot turn (here at right, below at left) for a small angle */
			turn_right();

		 }else if(turning_instruction_state == LEFT){

			 turn_left();
		 }

		 /* Turn off all leds if the robot doesn't detect any sign */
		 if(get_prox(1)/calibration < 1 && get_prox(6)/calibration < 1){

			 turn_off_leds();
		 }

			counter_motors_delay = MOTORS_DELAY;
	}

	counter_leds_delay += 1;

	return counter_motors_delay;
	return keep_running_delay;
	return turning_instruction_state;
}

int delay_leds_toggle(int counter_leds_delay){

	if(counter_leds_delay == LEDS_DELAY){

		toggle_rgb(direction_statut);

		/* waits 0.3 second */
		chThdSleepMilliseconds(300);

	    counter_leds_delay = ZERO;
	}

	return counter_leds_delay;
}

void initialisation(void){

	set_rgb_led(LED2, TRAFFIC_LIGHT, ZERO, ZERO);
	set_rgb_led(LED4, TRAFFIC_LIGHT, ZERO, ZERO);
	set_rgb_led(LED6, TRAFFIC_LIGHT, ZERO, ZERO);
	set_rgb_led(LED8, TRAFFIC_LIGHT, ZERO, ZERO);

	/* waits 3 second to allow us to place the robot in front of the sign we used to calibrate the distance at which
	 * we want the robot stop for each sign  */
	 chThdSleepMilliseconds(3000);

	 set_rgb_led(LED2, ZERO, TRAFFIC_LIGHT, ZERO);
	 set_rgb_led(LED4, ZERO, TRAFFIC_LIGHT, ZERO);
	 set_rgb_led(LED6, ZERO, TRAFFIC_LIGHT, ZERO);
	 set_rgb_led(LED8, ZERO, TRAFFIC_LIGHT, ZERO);

	 chThdSleepMilliseconds(500);

	 set_rgb_led(LED2, ZERO, ZERO, ZERO);
	 set_rgb_led(LED4, ZERO, ZERO, ZERO);
	 set_rgb_led(LED6, ZERO, ZERO, ZERO);
	 set_rgb_led(LED8, ZERO, ZERO, ZERO);
}
