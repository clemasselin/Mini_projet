/*
 * motor_control.h
 *
 *  Created on: 13 mai 2022
 *      Author: margu
 */

#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_



void motor_set_position(float position_r, float position_l, float speed_r, float speed_l);
#define POSITION_CONTROL    1

#endif /* MOTOR_CONTROL_H_ */
