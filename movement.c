/*
 * movement.c
 *
 *  Created on: 28 avr. 2022
 *      Author: misha
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <motors.h>
#include <color_detection.h>

#define MODE_MOT_ROTATION 0
#define MODE_MOT_STRAIGHT_LINE 1
#define SPEED_MOT 100
#define NB_STEP_ROT 7
#define SPEED_MOT_ROT 100
#define STOP 0

//misha
void start_motor_straight(int16_t motor_speed, int dist){
    	left_motor_set_speed(motor_speed);
    	right_motor_set_speed(motor_speed);
    	chThdSleepMilliseconds(abs(1000*dist/motor_speed));
}


void start_motor_rot(void){
  	left_motor_set_speed(SPEED_MOT_ROT);
   	right_motor_set_speed(-SPEED_MOT_ROT);
   	chThdSleepMilliseconds(1000*NB_STEP_ROT/SPEED_MOT);
}


void stop_motor(void){
   	left_motor_set_speed(STOP);
   	right_motor_set_speed(STOP);
}


static THD_WORKING_AREA(waThdMovement, 128);
static THD_FUNCTION(ThdMovement, arg) {
	static uint8_t mode_mot=MODE_MOT_ROTATION;
    static uint16_t angle=1;
    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    while(1){
        switch(mode_mot)
        {
        	case MODE_MOT_ROTATION:
        		if(angle>=191) {
        			stop_motor();
        		}
        		else{
        			start_motor_rot();
        			stop_motor();
        			chThdSleepMilliseconds(100);
        			angle=angle+1;
        		}
        		break;

        	case MODE_MOT_STRAIGHT_LINE:
        		start_motor_straight(SPEED_MOT, 1000);
        		stop_motor();
        		set_camera_mode(CAMERA_ON)
        		chThdSleepMilliseconds(1000);
//        		start_motor_straight(-SPEED_MOT, 1000);
//        		stop_motor();
//        		chThdSleepMilliseconds(500);
        		break;

        }
    }
}

void start_ThdMovement(void){
	chThdCreateStatic(waThdMovement, sizeof(waThdMovement), NORMALPRIO, ThdMovement, NULL);
}
