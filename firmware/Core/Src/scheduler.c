/*
 * scheduler.c
 *
 *  Created on: Jun 6, 2026
 *      Author: artin59
 */


#include "scheduler.h"

static volatile uint8_t tick = 0;
SchedulerFlag flag = {0};

void Scheduler_Tick(){

	tick++;

	flag.imu = 1;

	if (tick %2 == 0)
		flag.temp = 1;

}
