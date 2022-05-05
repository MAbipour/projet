/*
 * color_detection.h
 *
 *  Created on: 3 mai 2022
 *      Author: misha
 */

#define CAMERA_OFF 0;
#define CAMERA_ON 1;

#ifndef COLOR_DETECTION_H_
#define COLOR_DETECTION_H_
#include <stdlib.h>


void set_camera_mode(uint8_t sel_mode);

void start_ThdMovement(void);

#endif /* COLOR_DETECTION_H_ */
