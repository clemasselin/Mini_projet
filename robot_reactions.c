#include "ch.h"
#include "hal.h"
#include <math.h>

#include <main.h>
#include <leds.h>
#include <robot_reactions.h>


void allumer_body(void){

	set_body_led(1);

}

void warning(void){

		set_led(LED3, 2);
		set_led(LED7, 2);

}

//void turn_signal(void){
//
//
//
//}
