#include "headfile.h"


float SpeKp = 0.004f; //速度环响应     \0.003
float SpeKi =  0.003f; //速度环积分    \0.003
int32 SpeILimit = 0;  //速度环积分限

/**********关于测速度变量******/
const float Ratio_Encoder = 200/(1175*0.1);//轮子速度=counter*轮子周长（mm）/（左轮转一圈对应的脉冲数*程序周期）  最后速度单位是mm/s ，100ms
float RealSpeed = 0;    //真实速度
int16 LeftRealSpeed = 0;
int16 RightRealSpeed = 0;

/**********关于速度偏差变量******/
float FilterSpeed = 0;   //阶梯滤波速度
float SpeedErrorTemp[5] = {0};
uint16 ExpectSpeed = 0;  //小车期望速度
float Distance = 0;   		//编码器累加值
	
/**********关于速度控制变量******/
float Spe_integral = 0;
	
float SpeedOut = 0 ;
float SpeedControlOut = 0 ;    //速度控制输出
float SpeedControlOutOld = 0;
float SpeedControlOutNew = 0;



/************************************************************************************************************
*函数名：void Encoder_init(void)
*输入：无
*输出：无
*作用：编码器初始化
*************************************************************************************************************/
void Encoder_init(void)
{
	tim_counter_init(ENCODER1_TIM, ENCODER1_PLUS);
	tim_counter_init(ENCODER2_TIM, ENCODER2_PLUS);

	gpio_init(ENCODER1_DIR, GPI, 1, GPI_PULL_UP);
	gpio_init(ENCODER2_DIR, GPI, 1, GPI_PULL_UP);


}


/************************************************************************************************************
*函数名：GetRealSpeed（）
*输入：无
*输出：无
*作用：通过编码器读取速度
*************************************************************************************************************/
void GetRealSpeed (void)
{
	static float SpeedOld, SpeedNew;
	if(gpio_get(ENCODER1_DIR))
		RightRealSpeed = -tim_counter_get_count(ENCODER1_TIM)*Ratio_Encoder;
	else
		RightRealSpeed = tim_counter_get_count(ENCODER1_TIM)*Ratio_Encoder;
	tim_counter_rst(ENCODER1_TIM);

	 if(gpio_get(ENCODER2_DIR))
		LeftRealSpeed = tim_counter_get_count(ENCODER2_TIM)*Ratio_Encoder;
	else
		LeftRealSpeed = -tim_counter_get_count(ENCODER2_TIM)*Ratio_Encoder;
	tim_counter_rst(ENCODER2_TIM);
	
	
    RealSpeed = (LeftRealSpeed + RightRealSpeed)/2;
	
	  SpeedOld = FilterSpeed;
    SpeedNew = RealSpeed;
	if(SpeedNew >= SpeedOld)
	{
			FilterSpeed = ((SpeedNew - SpeedOld) > 300 ? (SpeedOld + 300) : SpeedNew);
	}
	else
	{
			FilterSpeed = ((SpeedNew - SpeedOld) < -300 ? (SpeedOld - 300) : SpeedNew);
	}
	
}

/************************************************************************************************************
*函数名：SpeedControl（）
*输入：无
*输出：无
*作用：速度环pi核心算法
**************************************************************************************************************/
void SpeedControl(void)               //100ms执行一次
{
    GetRealSpeed();  //取得真实速度
	
	  SpeedErrorTemp[0] = ExpectSpeed - FilterSpeed;   //计算速度偏差
		
		SpeedControlOutOld = SpeedControlOutNew;		//值传递
	
		Spe_integral += SpeedErrorTemp[0];    //速度偏差积分
   	if (Spe_integral >= SpeILimit)        //这里的积分限幅为0，积分没有起作用，只是使用了P控制
			Spe_integral = SpeILimit;
		else if (Spe_integral < -SpeILimit)
			Spe_integral = -SpeILimit;
	 
    SpeedOut = SpeKp * SpeedErrorTemp[0] + SpeKi* Spe_integral;
}

/************************************************************************************************************
*函数名：SpeedControlFuncOut（）
*输入：无
*输出：无
*作用：速度环平滑输出
**************************************************************************************************************/

void SpeedControlFuncOut(void)     //10ms执行一次
{
		SpeedControlOutNew = SpeedOut ;
		static uint8 i = 1;
		if(i == 11)
		{
			i = 1;
		}
		SpeedControlOut = (SpeedControlOutNew - SpeedControlOutOld)*(i/10.0) + SpeedControlOutOld;
		i++;
//		SpeedControlOut = (SpeedControlOut > 15) ? 15:SpeedControlOut;
//	  SpeedControlOut = (SpeedControlOut <-15) ? -15:SpeedControlOut;
}

