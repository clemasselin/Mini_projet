#ifndef SIGN_DETECTION_H
#define SIGN_DETECTION_H




/* Declarations of functions directly used in the sign detection thread */
void sign_detection_start(void);
void detected(int calib);
int avoid_sign(int counter_motors_delay, int counter_leds_delay, int keep_running_delay);
int delay_leds_toggle(int counter_leds_delay);
void initialisation(void);
int get_detection_state(void);
bool get_initialized(void);
void signal_detection();
#endif /* SIGN_DETECTION_H */
