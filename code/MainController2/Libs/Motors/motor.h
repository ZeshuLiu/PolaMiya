#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "main.h"

#define MAX_PWM_WIDTH (10000U-1U)
#define PWM_FSPEED (7000U-1U)
#define MT_PWM_STEP 700

void motor_init(void);
void motor_start(void);
void motor_end(void);

#endif