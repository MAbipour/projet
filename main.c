#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <motors.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <main.h>
#include <movement.h>




int main(void)
{
	halInit();
    chSysInit();
    mpu_init();
    start_ThdMovement();

    while (1) {
    	chThdSleepMilliseconds(1000);
    }
}


#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
