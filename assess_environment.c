#include "ch.h"
#include "hal.h"

#include <assess_environment.h>
#include <main.h>
#include <camera/po8030.h>
#include <leds.h>


// Declaration of static global variables
static uint16_t line_position = IMAGE_BUFFER_SIZE/TWO;//middle
static uint8_t line_number=ZERO;


//semaphore
static BSEMAPHORE_DECL(image_ready_sem, TRUE);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void find_line(uint8_t *buffer){

	uint16_t i = ZERO, begin=ZERO, end =ZERO;
	uint8_t stop = ZERO, line_not_found = ZERO, n=ZERO, continue_search=ZERO;
	uint32_t mean = ZERO;

	//performs an average
	for(uint16_t i = begin ; i < IMAGE_BUFFER_SIZE ; i++){
		mean += buffer[i];
	}
	mean /= IMAGE_BUFFER_SIZE;

	do{
		continue_search = 0;

		//search for a begin
		while(stop == 0 && i < (IMAGE_BUFFER_SIZE - WIDTH_SLOPE)){

			//looks for a wide enough downward slope in pixel line
			if(buffer[i] > mean && buffer[i+WIDTH_SLOPE] < mean)
			{
				begin = i;
				stop = 1;
			}
			i++;
		}

		//if a begin was found, search for an end
		if (i < (IMAGE_BUFFER_SIZE - WIDTH_SLOPE) && begin)
		{
			stop = 0;
			while(stop == 0 && i < IMAGE_BUFFER_SIZE){

				//looks for a wide enough upward slope in pixel line
				if(buffer[i] > mean && buffer[i-WIDTH_SLOPE] < mean)
				{
					end = i;
					stop = 1;
				}
				i++;
			}
			//if an end was not found
			if (i > IMAGE_BUFFER_SIZE || !end)
			{
				line_not_found = 1;
			}
		}//if no begin was found
		else{
			line_not_found = 1;
		}



		// if a line was found and is wide enough, add it to the line counter and continue the search until no more lines are found
		if(!line_not_found){
			if((end-begin) >MIN_LINE_WIDTH){
				n+=1;
				line_number=n;
				if(n==ONE){
					line_position=(begin + end)/TWO;
				}
			}
			i = end;
			begin = 0;
			end = 0;
			stop=0;
			continue_search = 1;
		}

}while(continue_search);


	if(line_not_found){
		begin = 0;
		end = 0;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static THD_WORKING_AREA(waCaptureImage, 256);
static THD_FUNCTION(CaptureImage, arg) {

	chRegSetThreadName(__FUNCTION__);
	(void)arg;

	//Takes pixels 0 to IMAGE_BUFFER_SIZE of the line 10 + 11 (minimum 2 lines because reasons)
	po8030_advanced_config(FORMAT_RGB565, 0, 10, IMAGE_BUFFER_SIZE, 2, SUBSAMPLING_X1, SUBSAMPLING_X1);
	dcmi_enable_double_buffering();
	dcmi_set_capture_mode(CAPTURE_ONE_SHOT);
	dcmi_prepare();

	while(1){
		//starts a capture
		dcmi_capture_start();
		//waits for the capture to be done
		wait_image_ready();
		//signals an image has been captured
		chBSemSignal(&image_ready_sem);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static THD_WORKING_AREA(waProcessImage, 1024);
static THD_FUNCTION(ProcessImage, arg) {

	chRegSetThreadName(__FUNCTION__);
	(void)arg;

	uint8_t *img_buff_ptr;
	uint8_t image[IMAGE_BUFFER_SIZE] = {ZERO};

	bool send_to_computer = true;

	while(1){
		//waits until an image has been captured
		chBSemWait(&image_ready_sem);
		//gets the pointer to the array filled with the last image in RGB565    
		img_buff_ptr = dcmi_get_last_image_ptr();

		//Extracts only the red pixels
		for(uint16_t i = ZERO ; i < (TWO * IMAGE_BUFFER_SIZE) ; i+=TWO){
			//extracts first 5bits of the first byte
			//takes nothing from the second byte
			image[i/TWO] = (uint8_t)img_buff_ptr[i]&0xF8;
		}


		find_line(image);


		if(send_to_computer){
			//sends to the computer the image
			SendUint8ToComputer(image, IMAGE_BUFFER_SIZE);
		}
		send_to_computer = !send_to_computer;


	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Getters to access static global variables

uint16_t get_line_position(void){
	return line_position;
}
uint16_t get_line_number(void){
	return line_number;
}
void set_line_number(uint16_t value){
	line_number=value;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void assess_environment_start(void){
	chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);
	chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL);
}
