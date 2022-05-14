#include "ch.h"
#include "hal.h"
#include <math.h>

#include <main.h>
#include <leds.h>
#include <robot_reactions.h>
#include <spi_comm.h>
#include <motors.h>

void turn_off_leds(){

	    clear_leds_rgb();
	    clear_leds();
}

void clear_leds_rgb(void)
{

		set_rgb_led(LED2, ZERO, ZERO, ZERO);
		set_rgb_led(LED8, ZERO, ZERO, ZERO);
}

void turn_signal_right(void){

	set_rgb_led(LED2, RED, GREEN, BLUE);
}

void turn_signal_left(void){

	set_rgb_led(LED8, RED, GREEN, BLUE);

}

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



void warning_rgb(void){
	set_rgb_led(LED2, RED, GREEN, BLUE);
	set_rgb_led(LED8, RED, GREEN, BLUE);

}

void traffic_light(int traffic_light_satut){

	if(traffic_light_satut == STOP){
		set_rgb_led(LED2, TRAFFIC_LIGHT, ZERO, ZERO);
		set_rgb_led(LED4, TRAFFIC_LIGHT, ZERO, ZERO);
		set_rgb_led(LED6, TRAFFIC_LIGHT, ZERO, ZERO);
		set_rgb_led(LED8, TRAFFIC_LIGHT, ZERO, ZERO);

		set_led(LED3, 2);
		set_led(LED7, 2);
	}else if(traffic_light_satut == GO){

		set_rgb_led(LED2, ZERO, TRAFFIC_LIGHT, ZERO);
		set_rgb_led(LED4, ZERO, TRAFFIC_LIGHT, ZERO);
		set_rgb_led(LED6, ZERO, TRAFFIC_LIGHT, ZERO);
		set_rgb_led(LED8, ZERO, TRAFFIC_LIGHT, ZERO);
	}
}


void keep_running(void){

	right_motor_set_speed(DEFAULT_SPEED);
	left_motor_set_speed(DEFAULT_SPEED);
}


void stop(void){

	right_motor_set_speed(ZERO);
	left_motor_set_speed(ZERO);
}

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






