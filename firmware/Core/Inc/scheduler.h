/*
 * scheduler.h
 *
 *  Created on: Jun 6, 2026
 *      Author: artin59
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>

typedef struct {
	volatile uint8_t imu;
} SchedulerFlag;


extern SchedulerFlag flag;

void Scheduler_Tick();

#endif /* INC_SCHEDULER_H_ */
