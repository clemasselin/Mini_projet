#ifndef SIGN_DETECTION_H
#define SIGN_DETECTION_H


#define LED_INIT		100

/* Declarations of functions */
void sign_detection_start(void);
void detected(int16_t calibration);
void initialisation(void);
uint8_t get_detection_state(void);
bool get_initialized(void);


#endif /* SIGN_DETECTION_H */
