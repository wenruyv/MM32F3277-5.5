#ifndef _spped_out_H_
#define _spped_out_H_

#include "headfile.h"

#define ENCODER1_TIM		TIM_3
#define ENCODER1_PLUS		TIM_3_ENC1_B04
#define ENCODER1_DIR		B5

#define ENCODER2_TIM		TIM_4
#define ENCODER2_PLUS		TIM_4_ENC1_B06
#define ENCODER2_DIR		B7


extern float RealSpeed;    //真实速度
extern uint16 ExpectSpeed;
extern float SpeKp;
extern float SpeKi;

extern int16 LeftRealSpeed;
extern int16 RightRealSpeed;


extern float SpeedControlOut;
extern float SpeedOut;
extern float SpeedControlOutOld;
extern float SpeedControlOutNew;
extern int32 SpeILimit;

void Encoder_init(void);
void GetRealSpeed (void);

void SpeedControl(void);
void GetRealSpeed(void);
void SpeedControlFuncOut(void);

#endif

