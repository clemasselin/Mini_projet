#ifndef SIGN_DETECTION_H
#define SIGN_DETECTION_H

/* Definition of constants used in sign_detection.c */
#define ZERO 0
#define LEDS_DELAY 5
#define MOTORS_DELAY 5
#define RUNNING_DELAY 1
#define RIGHT 1
#define LEFT 2
#define NONE 0

/* Declarations of functions directly used in the sign detection thread */
void sign_detection_start(void);
int detected(void);
int avoid_sign(int counter_motors_delay, int counter_leds_delay, int keep_running_delay, int calibration);
int delay_leds_toggle(int counter_leds_delay);
void initialisation(void);

#endif /* SIGN_DETECTION_H */
