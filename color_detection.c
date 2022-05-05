/*
 * color_detection.c
 *
 *  Created on: 3 mai 2022
 *      Author: misha
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <color_detection.h>
#include "ch.h"
#include "hal.h"
#include <chprintf.h>
#include <usbcfg.h>

#include <main.h>
#include <camera/po8030.h>



#define THRESH_RED 10
#define THRESH_BLUE 10
#define THRESH_GREEN 10
#define IMAGE_BUFFER_SIZE 640

static uint8_t cam_mode=CAMERA_OFF;


static BSEMAPHORE_DECL(image_ready_sem, TRUE);


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
	//uint8_t image[IMAGE_BUFFER_SIZE] = {0};
	//uint16_t lineWidth = 0;
	int red_avg = 0;
	int green_avg = 0;
	int blue_avg = 0;

	//bool send_to_computer = true;

    while(1){
    	if(cam_mode==CAMERA_ON){
    	    //waits until an image has been captured
    		chBSemWait(&image_ready_sem);
    		//gets the pointer to the array filled with the last image in RGB565
    		img_buff_ptr = dcmi_get_last_image_ptr();

    		for(uint16_t i = 0 ; i < (2 * IMAGE_BUFFER_SIZE) ; i+=2){

    			//Extracts the red pixels
    			red_avg = red_avg+(((uint8_t)img_buff_ptr[i]&0xF8)/4);			//le img_buff_ptr pointe sur les bytes

    			//Extracts the green pixels
    			green_avg = green_avg+(((uint8_t)img_buff_ptr[i]&0xE0)/32);
    			green_avg = green_avg+(((uint8_t)img_buff_ptr[i]&0x07)*8);

    			//Extracts the blue pixels
    			blue_avg = blue_avg+((uint8_t)img_buff_ptr[i+1]&0x1F);

    			green_avg = green_avg/IMAGE_BUFFER_SIZE;
    			blue_avg = blue_avg/IMAGE_BUFFER_SIZE;
    			red_avg=red_avg/IMAGE_BUFFER_SIZE;

    			//green color detected
    			if ((green_avg>THRESH_GREEN) & (blue_avg<THRESH_BLUE) & (red_avg<THRESH_RED)){
    				//faire une action pour la couleur verte

    			}

    			//blue color detected
    			if ((green_avg<THRESH_GREEN) & (blue_avg>THRESH_BLUE) & (red_avg<THRESH_RED)){
    				//faire une action pour la couleur bleue
    			}

    			//red color dertected
    			if ((green_avg<THRESH_GREEN) & (blue_avg<THRESH_BLUE) & (red_avg>THRESH_RED)){
    				//faire une action pour la couleur rouge
    			}
    		}
    		cam_mode=CAMERA_OFF;
    	}
    }

	//il faut encore ajouter les gammes de couleurs:
	//le vert est codé sur 64 valeurs entre 0 et 63
	//bleu sur 32
	//rouge sur 32
}


void process_image_start(void){
	chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);
	chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL);
}



void activate_camera_mode(void){
	cam_mode=CAMERA_ON;
}
