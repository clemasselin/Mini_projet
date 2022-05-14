#ifndef ROBOT_REACTIONS_H_
#define ROBOT_REACTIONS_H_

/* Definition of constants used in robot_reactions.c */
#define INSIDE_TURN_SPEED 100
#define OUTSIDE_TURN_SPEED 400
#define DEFAULT_SPEED 500
#define TIME_OFF 20
#define RED 100
#define GREEN 84
#define BLUE 0
#define TRAFFIC_LIGHT 100
#define ZERO 0
#define STOP 0
#define GO 1
#define PI                  3.1415926536f
#define WHEEL_DISTANCE      5.35f    //cm
#define WHEEL_PERIMETER    (PI * WHEEL_DISTANCE)
#define DISTANCE 30
#define NSTEP_ONE_TURN 50


/*Declarations of all functions used as reactions of the robot depending of the action called by the sign detection */


/*--------declarations of led's functions of the robot--------*/

void turn_off_leds(void);
void clear_leds_rgb(void);
void turn_signal_right(void);
void turn_signal_left(void);
void toggle_rgb(bool direction_statut);
void warning_rgb(void);
void traffic_light(int traffic_light_satut);

/*-------declarations of motors functions of the robot--------*/

void keep_running(void);
void stop(void);
void turn_right(void);
void turn_left(void);

#endif /* ROBOT_REACTIONS_H_ */
