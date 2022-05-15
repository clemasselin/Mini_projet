#ifndef CIRCULATION_INSTRUCTIONS_H_
#define CIRCULATION_INSTRUCTIONS_H_


/* CIRCULATION CONSTANTS */

#define DEFAULT_SPEED 			250
#define RED 					100
#define GREEN 					84
#define BLUE 					0
#define TRAFFIC_LIGHT 			100
#define STOP 					0
#define GO 						1
#define SPEED_CORRECTION		100
#define ROTATION_THRESHOLD		5
#define ROTATION_COEFF			2


#define	TURN_RIGHT_RIGHT_SPEED	10
#define	TURN_RIGHT_LEFT_SPEED	500
#define	TURN_LEFT_RIGHT_SPEED	350
#define	TURN_LEFTT_LEFT_SPEED	150

/*--------declarations of led's functions of the robot--------*/

void turn_off_leds(void);
void clear_leds_rgb(void);
void turn_signal_right(void);
void turn_signal_left(void);
void warning_rgb(void);
void traffic_light(int traffic_light_satut);

/*-------declarations of motors functions of the robot--------*/

void keep_running(void);
void follow_line(int16_t deviation);
void stop(void);
void turn_left_90(void);
void turn_right_180(void);

#endif /* CIRCULATION_INSTRUCTIONS_H_ */
