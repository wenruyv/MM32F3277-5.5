#ifndef __ANGLECONTROL_H__
#define __ANGLECONTROL_H__
#include "headfile.h"

typedef struct 
{
	float Set_Angle;
	float Angle_P;
	float Angle_D;
	float ExpectSpeed;
	float Speed_P;
	float Dir_P;
	float Dir_D;
	char Flag_SpeedControlOut;
	float Angle_coe;
	uint16 Round_Angle;
	float Round_Out_Offset;
}Run_Type;

extern Run_Type Three_Wheel,Two_Wheel,Wheel;


extern float gyro_speed_y;
extern float car_angle_err;


extern int32 Angle_Speed_Out;
extern float AngleControlOut;
extern float angle_acc;
extern float car_angle;
extern float gyro_x_zero;
extern float icm_zero_y;

extern float Angle_All_ERR;

extern float gyro_speed_x;
extern float car_angle1;
extern float gyro_angle;
extern float Gyro_Turn_Angle;

extern int32 AngSpe_Pout;
extern int32 AngSpe_Iout;

extern uint8 ang_stage;
extern  float Car_Angle_Speed_err;

extern uint16 Filt_Acc_X;
extern int32 ILimit;

extern float ANGLE_SPEED_CONTROL_P;  //50 角速度响应
extern float ANGLE_SPEED_CONTROL_I;  //3.5 角速度积分

extern float AngleControlOutOld;
extern float AngleControlOutNew;
extern void gyro_x_ang_caculate(void);
extern void AngleControFunlOut(void);
extern void Angle_Speed_Control(void);
extern void AngleControl(void);


#endif

