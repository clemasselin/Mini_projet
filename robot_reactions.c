#include "ch.h"
#include "hal.h"
#include <math.h>

#include <main.h>
#include <leds.h>
#include <robot_reactions.h>
#include <spi_comm.h>
#include <motors.h>


/*----------------------------------------------LEDs reactions of the robot------------------------------------------------*/

/* Clear all the leds used in the project */
void turn_off_leds(){

	    clear_leds_rgb();
	    clear_leds();
}

/* Set at 0 both of RGB leds used */
void clear_leds_rgb(void)
{

		set_rgb_led(LED2, ZERO, ZERO, ZERO);
		set_rgb_led(LED8, ZERO, ZERO, ZERO);
}

/* Turn on the RGB LED2 with an orange color */
void turn_signal_right(void){

	set_rgb_led(LED2, RED, GREEN, BLUE);
}

/* Turn on the RGB LED8 with an orange color */
void turn_signal_left(void){

	set_rgb_led(LED8, RED, GREEN, BLUE);

}

/* Turn on the led wanted in function of the turn to do and then use a while loop to makes the led toggle*/
void  toggle_rgb(bool direction_statut){

	int i = 0;


	if(direction_statut == true){

		turn_signal_right();

	}else{

		turn_signal_left();
	}


	/* waits 0.3 second */
	chThdSleepMilliseconds(300);

	while(i <= TIME_OFF){

		clear_leds_rgb();
		i += 1;
	}

}

/* Turn on both RGB LED2 and LED8 with a orange color */
void warning_rgb(void){

	set_rgb_led(LED2, RED, GREEN, BLUE);
	set_rgb_led(LED8, RED, GREEN, BLUE);

}

/* Set all RGB leds green or red depending of the traffic light statut */
void traffic_light(int traffic_light_satut){

	if(traffic_light_satut == STOP){
		set_rgb_led(LED2, TRAFFIC_LIGHT, ZERO, ZERO);
		set_rgb_led(LED4, TRAFFIC_LIGHT, ZERO, ZERO);
		set_rgb_led(LED6, TRAFFIC_LIGHT, ZERO, ZERO);
		set_rgb_led(LED8, TRAFFIC_LIGHT, ZERO, ZERO);

	}else if(traffic_light_satut == GO){

		set_rgb_led(LED2, ZERO, TRAFFIC_LIGHT, ZERO);
		set_rgb_led(LED4, ZERO, TRAFFIC_LIGHT, ZERO);
		set_rgb_led(LED6, ZERO, TRAFFIC_LIGHT, ZERO);
		set_rgb_led(LED8, ZERO, TRAFFIC_LIGHT, ZERO);
	}
}


/*----------------------------------------------motors reactions of the robot------------------------------------------------*/

/* Set both motors at the default speed defined above to force the robot going forward */
void keep_running(void){

	right_motor_set_speed(DEFAULT_SPEED);
	left_motor_set_speed(DEFAULT_SPEED);
}

/* Set both motors speed equal to 0 to make the robot stops*/
void stop(void){

	right_motor_set_speed(ZERO);
	left_motor_set_speed(ZERO);
}

/* Set right motor's speed at a lower speed than the left one to make the robot turns right for a distance we choose */
void turn_right(void){

	int32_t count = 0;


	right_motor_set_pos(0);
	left_motor_set_pos(0);

	while(abs(count) < (DISTANCE * NSTEP_ONE_TURN/WHEEL_PERIMETER)){
		right_motor_set_speed(INSIDE_TURN_SPEED);
		left_motor_set_speed(OUTSIDE_TURN_SPEED);
		count = right_motor_get_pos();
	}

	right_motor_set_speed(ZERO);
	left_motor_set_speed(ZERO);
}

/* Set left motor's speed at a lower speed than the right one to make the robot turns left for a distance we choose*/
void turn_left(void){

	int32_t count = 0;


	right_motor_set_pos(0);
	left_motor_set_pos(0);

	while(abs(count) < (DISTANCE * NSTEP_ONE_TURN/WHEEL_PERIMETER)){
		right_motor_set_speed(OUTSIDE_TURN_SPEED);
		left_motor_set_speed(INSIDE_TURN_SPEED);
		count = right_motor_get_pos();
	}

	right_motor_set_speed(ZERO);
	left_motor_set_speed(ZERO);
}
