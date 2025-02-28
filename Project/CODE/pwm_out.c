#include "headfile.h"


int LeftPWM, RightPWM;//两个轮子电机输出变量
int LeftDeadPWM = 3000; //3000
int RightDeadPWM = 4200;  //4500 3300
int duty_max = 37500;  //75%占空比



/******************************
*@函数名：void motor_init(void)
*@输入：无
*@输出：无
*@关键参数：
*@作用：电机初始化
*@备注：无
*******************************/
void motor_init(void)
{	
// 请注意 这里接线时请务必不要接错 A0/A1一组 A2/A3一组 分别控制左右电机
	pwm_init(PWM_TIM, PWM_L_CH1, 16000, 0);											// PWM 通道1 初始化频率10KHz 占空比初始为0
	pwm_init(PWM_TIM, PWM_L_CH2, 16000, 0);											// PWM 通道2 初始化频率10KHz 占空比初始为0

	pwm_init(PWM_TIM, PWM_R_CH1, 16000, 0);											// PWM 通道3 初始化频率10KHz 占空比初始为0
	pwm_init(PWM_TIM, PWM_R_CH2, 16000, 0);											// PWM 通道4 初始化频率10KHz 占空比初始为0

}

/********************************************************************
*@函数名：void PwmOut(int32_t LeftPWM,int32_t RightPWM)
*@输入：LeftPWM,RightPWM
*@输出：无
*@关键参数：
*@作用：驱动电机
*@备注：*****这里因为是反跑，以下代码已做改动，和例程不一样，如不改变接线移植必须移植以下代码***********
******************************************************************/
void PwmOut(int32_t LeftPWM  ,int32_t RightPWM)
{
	//电机死区处理
	LeftPWM = LeftPWM + LeftDeadPWM;
	RightPWM = RightPWM + RightDeadPWM;
	
		if(RightPWM >= 0)																// 右侧正转
		{
			RightPWM = ((RightPWM > duty_max) ? duty_max: RightPWM);  
			pwm_duty_updata(PWM_TIM, PWM_L_CH1, RightPWM);			// 计算占空比
			pwm_duty_updata(PWM_TIM, PWM_L_CH2, 0);					
		}
		else																		// 右侧反转
		{
			RightPWM = ((RightPWM < - duty_max) ? -duty_max: RightPWM); 
			pwm_duty_updata(PWM_TIM, PWM_L_CH1, 0);									// 同一时间 一个电机只能输出一个PWM 另一通道保持低电平
			pwm_duty_updata(PWM_TIM, PWM_L_CH2, -RightPWM);		// 计算占空比
			
		}
		
		if(LeftPWM >= 0)																// 左侧正转
		{
			LeftPWM = ((LeftPWM > duty_max) ? duty_max: LeftPWM);  
			pwm_duty_updata(PWM_TIM, PWM_R_CH1, LeftPWM);			// 计算占空比
			pwm_duty_updata(PWM_TIM, PWM_R_CH2, 0);
		}
		else																		// 左侧反转
		{
			LeftPWM = ((LeftPWM <- duty_max) ? -duty_max: LeftPWM);  
			pwm_duty_updata(PWM_TIM, PWM_R_CH1, 0);									// 同一时间 一个电机只能输出一个PWM 另一通道保持低电平
			pwm_duty_updata(PWM_TIM, PWM_R_CH2, -LeftPWM);		// 计算占空比

		}		

}


