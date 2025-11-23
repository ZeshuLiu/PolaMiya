#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "main.h"

#define MAX_PWM_WIDTH 10000-1
#define PWM_FSPEED 7000-1
#define MT_PWM_STEP 7

void motor_init(void);
void motor_start(void);
void motor_end(void);

#endif