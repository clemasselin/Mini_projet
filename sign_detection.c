#include "ch.h"
#include "hal.h"
#include <math.h>

#include <main.h>
#include <motors.h>
#include <sensors/proximity.h>
#include <sign_detection.h>
#include <leds.h>
#include <stdbool.h>


// Declaration of static global variables
static uint8_t detection_state = NONE;
static bool initialized = ZERO;
static int16_t calibration = ZERO;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void initialisation(void){

	set_rgb_led(LED2, LED_INIT, ZERO, ZERO);
	set_rgb_led(LED4, LED_INIT, ZERO, ZERO);
	set_rgb_led(LED6, LED_INIT, ZERO, ZERO);
	set_rgb_led(LED8, LED_INIT, ZERO, ZERO);

	/* waits 3 second to allow us to place the robot in front of the sign we used to calibrate the distance at which
	 * we want the robot stop for each sign  */
	 chThdSleepMilliseconds(LONG_TIME);

	 set_rgb_led(LED2, ZERO, LED_INIT, ZERO);
	 set_rgb_led(LED4, ZERO, LED_INIT, ZERO);
	 set_rgb_led(LED6, ZERO, LED_INIT, ZERO);
	 set_rgb_led(LED8, ZERO, LED_INIT, ZERO);

	 chThdSleepMilliseconds(500);

	 set_rgb_led(LED2, ZERO, ZERO, ZERO);
	 set_rgb_led(LED4, ZERO, ZERO, ZERO);
	 set_rgb_led(LED6, ZERO, ZERO, ZERO);
	 set_rgb_led(LED8, ZERO, ZERO, ZERO);

	detection_state = ZERO;
	initialized=ONE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Function use to verified if the robot detect a sign (right or left) or not*/
void detected(int16_t calibration){

	 if(get_prox(1)/calibration > ZERO){
		 detection_state = RIGHT;
	 }else if(get_prox(6)/calibration > ZERO){
		 detection_state = LEFT;
	 }else{
		 detection_state = NONE;
	 }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static THD_WORKING_AREA(waSignDetection, 256);
static THD_FUNCTION(SignDetection, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    /* start the thread for proximity sensor */
    proximity_start();

    /* start the calibration of the proximity sensor */
    calibrate_ir();

    /* Leds indication of time before the robot start to move, time used to calibrate the IR sensors */
	initialisation();

    systime_t time;

    //calibrates the IR back IR sensors to get a distance baseline for sign detection
    calibration = get_prox(3);

    while(1){

        time = chVTGetSystemTime();

        /* Comparing the value return by both proximity sensors IR2 and IR8 with baseline calibration distance*/
        detected(calibration);


        chThdSleepUntilWindowed(time, time + MS2ST(REFRESH_TIME));

    }
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Getters to access static global variables

uint8_t get_detection_state(void){
	return detection_state;
}

bool get_initialized(void){
	return initialized;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void sign_detection_start(void){
	chThdCreateStatic(waSignDetection, sizeof(waSignDetection), NORMALPRIO-ONE, SignDetection, NULL);
}
