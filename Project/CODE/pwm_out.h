#ifndef _PWM_OUT_H_
#define _PWM_OUT_H_

#include "headfile.h"


#define MAX_DUTY			50														// 最大 MAX_DUTY% 占空比
#define PWM_TIM				TIM_5
#define PWM_L_CH1			TIM_5_CH1_A00
#define PWM_L_CH2			TIM_5_CH2_A01
#define PWM_R_CH1			TIM_5_CH3_A02
#define PWM_R_CH2			TIM_5_CH4_A03




extern int LeftPWM,RightPWM;

extern void motor_init(void);
extern void  PwmOut(int32_t LeftPWM,int32_t RightPWM);
#endif


