/*
 * motor_control.c
 *
 *  Created on: 13 mai 2022
 *      Author: margu
 */
#include <stdlib.h>
#include <stdint.h>
#include <stm32f4xx.h>
//#include <gpio.h>
#include <motors.h>
#include <motor_control.h>


static int16_t counter_step_left = 0;
static int16_t counter_step_right = 0;          // in [step]
static int16_t position_to_reach_right = 0;	    // in [step]
static int16_t position_to_reach_left = 0;	    // in [step]
static uint8_t position_right_reached = 0;
static uint8_t position_left_reached = 0;
static uint8_t state_motor = 0;


void motor_set_position(float position_r, float position_l, float speed_r, float speed_l)
{
	//reinit global variable
	counter_step_left = 0;
	counter_step_right = 0;

    position_right_reached = 0;
    position_left_reached = 0;

	//Set global variable with position to reach in step
	position_to_reach_left = position_l * NSTEP_ONE_TURN / WHEEL_PERIMETER;
	position_to_reach_right = -position_r * NSTEP_ONE_TURN / WHEEL_PERIMETER;

	motor_set_speed(speed_r, speed_l);

	//flag for position control, will erase flag for speed control only
	state_motor = POSITION_CONTROL;
}

