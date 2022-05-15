#include "ch.h"
#include "hal.h"
#include <math.h>

#include <circulation_instructions.h>
#include <main.h>
#include <leds.h>
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

void follow_line(int16_t deviation){

	if(abs(deviation) < ROTATION_THRESHOLD){
	          		deviation = ZERO;
	        	}
	right_motor_set_speed(DEFAULT_SPEED - ROTATION_COEFF * deviation);
	left_motor_set_speed(DEFAULT_SPEED + ROTATION_COEFF * deviation);
}

void stop(void){

	right_motor_set_speed(ZERO);
	left_motor_set_speed(ZERO);
}

void turn_left_90(void){
	turn_signal_left();
	right_motor_set_speed(TURN_LEFT_RIGHT_SPEED);
	left_motor_set_speed(TURN_LEFTT_LEFT_SPEED);
	chThdSleepMilliseconds(LONG_TIME);
	clear_leds_rgb();

}

void turn_right_180(void){
	turn_signal_right();
	right_motor_set_speed(TURN_RIGHT_RIGHT_SPEED);
	left_motor_set_speed(TURN_RIGHT_LEFT_SPEED);
	chThdSleepMilliseconds(LONG_TIME);
	clear_leds_rgb();

}



