#include "ch.h"
#include "hal.h"
#include <chprintf.h>
#include <usbcfg.h>

#include <assess_environment.h>
#include <main.h>
#include <camera/po8030.h>
#include <leds.h>







//static float distance_cm = 0;
static uint16_t line_position = IMAGE_BUFFER_SIZE/2;//middle
static uint8_t line_number=0;
//semaphore
static BSEMAPHORE_DECL(image_ready_sem, TRUE);

/*
 *  Returns the line's width extracted from the image buffer given
 *  Returns 0 if line not found
 *
 *
 */

void find_line(uint8_t *buffer){

	uint16_t i = 0, begin=0, end =0;
	uint8_t stop = 0, line_not_found = 0, n=0, continue_search=0;
	uint32_t mean = 0;

	//static uint16_t last_width = PXTOCM/GOAL_DISTANCE;

	//performs an average
	for(uint16_t i = begin ; i < IMAGE_BUFFER_SIZE ; i++){
		mean += buffer[i];
	}
	mean /= IMAGE_BUFFER_SIZE;





	do{

		//clear_leds();
		continue_search = 0;

		//search for a begin
		while(stop == 0 && i < (IMAGE_BUFFER_SIZE - WIDTH_SLOPE))
		{ 
			//the slope must at least be WIDTH_SLOPE wide and is compared
			//to the mean of the image
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

			while(stop == 0 && i < IMAGE_BUFFER_SIZE)
			{
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
		}
		else//if no begin was found
		{
			line_not_found = 1;


		}

		if(!line_not_found){
			//set_led(LED3, 2);
			//set_body_led(1);
			//chprintf((BaseSequentialStream *)&SDU1, "pos=%d\r\n", (begin + end)/2);
			if((end-begin) >MIN_LINE_WIDTH){
				n+=1;
				line_number=n;
				if(n==1){
					line_position=(begin + end)/2;
					//chprintf((BaseSequentialStream *)&SDU1, "posi=%d\r\n", get_line_position());
				}
			}

			i = end;
			begin = 0;
			end = 0;
			stop=0;
			continue_search = 1;


			//chprintf((BaseSequentialStream *)&SDU1, "pos=%d\r\n", line_position);
			//chprintf((BaseSequentialStream *)&SDU1, "line=%d\r\n", n);



		}


		//&& (end-begin) < MIN_LINE_WIDTH
	//}while(next_scan && n<3 );
}while(continue_search);


	//n=0;

	if(line_not_found){
		begin = 0;
		end = 0;
		//width = last_width;
	}

}

uint16_t extract_line_width(uint8_t *buffer){

	uint16_t i = 0, begin = 0, end = 0, width = 0;
	uint8_t stop = 0, wrong_line = 0, line_not_found = 0;
	uint32_t mean = 0;

	static uint16_t last_width = PXTOCM/GOAL_DISTANCE;

	//performs an average
	for(uint16_t i = 0 ; i < IMAGE_BUFFER_SIZE ; i++){
		mean += buffer[i];
	}
	mean /= IMAGE_BUFFER_SIZE;

	do{
		wrong_line = 0;
		//search for a begin
		while(stop == 0 && i < (IMAGE_BUFFER_SIZE - WIDTH_SLOPE))
		{
			//the slope must at least be WIDTH_SLOPE wide and is compared
			//to the mean of the image
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

			while(stop == 0 && i < IMAGE_BUFFER_SIZE)
			{
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
		}
		else//if no begin was found
		{
			line_not_found = 1;
		}

		//if a line too small has been detected, continues the search
		if(!line_not_found && (end-begin) < MIN_LINE_WIDTH){
			i = end;
			begin = 0;
			end = 0;
			stop = 0;
			wrong_line = 1;
		}
	}while(wrong_line);

	if(line_not_found){
		begin = 0;
		end = 0;
		width = last_width;
	}else{
		last_width = width = (end - begin);
		line_position = (begin + end)/2; //gives the line position.
	}

	//sets a maximum width or returns the measured width
	if((PXTOCM/width) > MAX_DISTANCE){
		return PXTOCM/MAX_DISTANCE;
	}else{
		return width;
	}

	//if line found, get line position and continue search
	//	}else{
	//		if(n==1)
	//		line_position = (begin + end)/2;
	//		if((PXTOCM/width) > MAX_DISTANCE){
	//			last_width =PXTOCM/MAX_DISTANCE;
	//		}else{
	//			last_width = width = (end - begin);
	//		}

	//i = end;
	//begin = 0;
	//end = 0;
	//stop = 0;
	//n+=1;
	//line_number=n;
	//chprintf((BaseSequentialStream *)&SDU1, "line_number=%d\r\n", line_number);


	//}

	//line_number=0;

}

//int process_line_number(void){
//
//	//get a sample of 5 line_number and extract the most displayed number of the sample
//
//	uint8_t n=0; // counts number of identical value
//	uint8_t k=0;//table index
//	uint8_t sample[5] = {0}; //5 value sample table
//
//
//
//	//fills table
//	for (uint8_t i = 0 ; i<4 ; i++){
//		sample[i]=get_line_number();
//	}
//	chSysLock();
//	//compares one line measure to the next in the table
//	while(k<4){
//		for(uint8_t j = k+1 ; j<5 ; j++){
//			if(sample[j]==sample[k]){
//				n+=1;
//			}
//		}
//		//if not enough equal number in data, look at next element of table
//		if(n<2){
//			k+=1;
//		}else{
//			return sample[k];
//		}
//
//	}
//	chSysUnlock();
//
//	return 0;
//}

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


static THD_WORKING_AREA(waProcessImage, 1024);
static THD_FUNCTION(ProcessImage, arg) {

	chRegSetThreadName(__FUNCTION__);
	(void)arg;

	uint8_t *img_buff_ptr;
	uint8_t image[IMAGE_BUFFER_SIZE] = {0};
	//uint16_t lineWidth = 0;



	bool send_to_computer = true;

	while(1){
		//waits until an image has been captured
		chBSemWait(&image_ready_sem);
		//gets the pointer to the array filled with the last image in RGB565    
		img_buff_ptr = dcmi_get_last_image_ptr();

		//Extracts only the red pixels
		for(uint16_t i = 0 ; i < (2 * IMAGE_BUFFER_SIZE) ; i+=2){
			//extracts first 5bits of the first byte
			//takes nothing from the second byte
			image[i/2] = (uint8_t)img_buff_ptr[i]&0xF8;
		}

		//uint16_t dummy = extract_line_width(image);

		find_line(image);


		//chprintf((BaseSequentialStream *)&SDU1, "line=%d\r\n", get_line_number());

		//converts the width into a distance between the robot and the camera
		//if(lineWidth){
		//	distance_cm = PXTOCM/lineWidth;
		//}

		if(send_to_computer){
			//sends to the computer the image
			SendUint8ToComputer(image, IMAGE_BUFFER_SIZE);
		}
		//invert the bool
		send_to_computer = !send_to_computer;


	}
}



uint16_t get_line_position(void){
	return line_position;
}
int get_line_number(void){
	return line_number;
}

void assess_environment_start(void){
	chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);
	chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL);
}
