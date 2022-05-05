#include "ch.h"
#include "hal.h"
#include <math.h>

#include <main.h>
#include <leds.h>
#include <robot_reactions.h>
#include <spi_comm.h>


void allumer_body(void){

	set_body_led(1);

}

void warning_rgb(void){

	set_rgb_led(LED2, 255, 215, 0);
	set_rgb_led(LED8, 255, 215, 0);

}

void warning(void){

	set_led(LED3, 2);
	set_led(LED7, 2);

}

void clear_leds_rgb(void)
{

	set_rgb_led(LED2, 0, 0, 0);
	set_rgb_led(LED8, 0, 0, 0);
}

//void turn_signal(void){
//
//
//
//}
