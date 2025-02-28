#include "headfile.h"

//74.9 8.05 13.0 0.2149
//80.25 8.625  14.0 0.2149 15.9
//65.0  1.0 17.0 7.5

float ANGLE_SPEED_CONTROL_P = 40.0f;  //50 角速度响应 \			200 \88\76.3\107\74.9
float ANGLE_SPEED_CONTROL_I = 1.5f;  //20 角速度积分 \30 L10000   \10(不会抖）11.9365\11.0\11.499\8.05
int32 AngSpeLim_fu = 45000;      //35000

float angle_acc = 0.0f;
float gyro_speed_y_ratio = 16.4f;   //需要整定,4 3.5!
float gyro_speed_y = 0;
float icm_zero_y = -9.0;        //将小车静止放置
float car_angle1 = 0.0f;
float k = 0.9605f;  //0.9605
float Gyro_Integral_Z=0.0f;
float Gyro_Turn_Angle=0.0f;
#define time_pit  0.002   //定时器中断4ms一次

float AngleControlOut = 0;
int32 Angle_Speed_Out = 0;

uint8 ang_stage;  //函数执行状态标志位

int32 AngSpe_Pout;
int32 AngSpe_Iout;

float Angle_All_ERR;   //角度环偏差

float AngleControlOutOld = 0,AngleControlOutNew = 0;

float car_angle_err; 
float Car_Angle_Speed_err = 0.0;

int32 ILimit = 50000;

Run_Type Wheel = 
{
	.Set_Angle = 18.0f,             //平衡角度 31.5f  \28.5\15.9
	.Angle_P = 22.6f,           //角度环响应12         \15\11.0\11.0\16.0
	.Angle_D = 9.0f,          //角度环阻力            \0.03\0.2149\0.2149\6.5
	.ExpectSpeed = 3500,
	.Speed_P = 0.036,     //0.036
	.Flag_SpeedControlOut = -1,  //-1
	.Dir_P = 20000,
	.Dir_D = 2.6,
	.Angle_coe = 0.002154581f,
	.Round_Angle = 363,          //353
	.Round_Out_Offset = 0.3,
};




/******************************
*@函数名：gyro_x_ang_caculate(void)
*@输入：无
*@输出：无
*@关键参数：gyro_y_zero(陀螺仪静止)
*@作用：计算出小车的俯仰角的角度car_angle1和俯仰角的角速度gyro_speed_y
*@备注：测试通过，放在中断中执行
*******************************/
void gyro_x_ang_caculate(void)
{
	 get_icm20602_accdata_spi();														// 获取ICM20602的测量数值
	 get_icm20602_gyro_spi();														    // 获取ICM20602的测量数值
	 angle_acc = (180.0 / 3.1415926 * (float)(atan2)(icm_acc_x, icm_acc_z));      //由加速度得到角度值
	 gyro_speed_y = (float)(icm_gyro_y * 1.0 - icm_zero_y) / gyro_speed_y_ratio;  //算出角速度
	 car_angle1 =(float) (k * (car_angle1 + gyro_speed_y * time_pit) + (1 - k) * (angle_acc));  //0.002=中断周期2ms
//  Gyro_Integral_Z+=(float)((int16)icm_gyro_z - 0)/16.4f;  //陀螺仪积分
//	Gyro_Turn_Angle=Gyro_Integral_Z*time_pit;   				//转过的角度,使用完之后清零Gyro_Integral_Z = 0.0;    
}

/******************************
*@函数名：AngleControl(void)
*@输入：无
*@输出：无
*@关键参数：AngleControlOut
*@作用：角度环
*@备注：无
*******************************/
void AngleControl(void)    //10ms执行一次 
{
	static float AngleErrOld = 0;
	static float AngleErrNew = 0;
	static float AngleErrFilt = 0;
	
	static float SpeedControlOutOld = 0;
	static float SpeedControlOutNew = 0;
	static float SpeedControlOutFilt = 0;
	
	//SpeedControlOut = 0;	//屏蔽速度环
	
	car_angle_err =  car_angle1 - Wheel.Set_Angle;//期望角度-俯仰角度 

	AngleErrOld = AngleErrFilt;
	AngleErrNew = car_angle_err;
   
	//角度滤波：这一次的角度偏差与上一次的比较
	if(AngleErrOld <= AngleErrNew)
	{
		AngleErrFilt = (AngleErrNew - AngleErrOld > 7.0) ? AngleErrOld + 7.0 : AngleErrNew;
	}//原本限幅为2
	else
	{
		AngleErrFilt = (AngleErrNew - AngleErrOld < -7.0) ? AngleErrOld - 7.0 : AngleErrNew;
	}	
	SpeedControlOutOld = SpeedControlOutFilt;
	SpeedControlOutNew = SpeedControlOut;
	
	//速度环输出滤波
	if(SpeedControlOutOld <= SpeedControlOutNew)
		SpeedControlOutFilt = (SpeedControlOutNew-SpeedControlOutOld > 20) ? SpeedControlOutOld + 20 : SpeedControlOutNew;
	else
		SpeedControlOutFilt = (SpeedControlOutNew-SpeedControlOutOld < -20) ? SpeedControlOutOld - 20 : SpeedControlOutNew;

	AngleControlOutOld = AngleControlOutNew;   //值传递
	
	//原本限幅为20
	
	
	//期望角度-当前俯仰角度-速度pid的输出（SpeedControlOut滤波之后） SpeedControlOut当成新的期望角度
	
	Angle_All_ERR = AngleErrFilt + Wheel.Flag_SpeedControlOut * SpeedControlOutFilt;

	AngleControlOutNew = Angle_All_ERR * Wheel.Angle_P - gyro_speed_y * Wheel.Angle_D;
}


/******************************
*@函数名：AngleControFunlOut(void)
*@输入：无
*@输出：无
*@关键参数：AngleControlOut
*@作用：角度环分成五份平滑输出
*@备注：无
*******************************/
void AngleControFunlOut(void)   //2ms执行一次
{
	static uint8 i = 1;
	if(i == 6)
	{
		i = 1;
	}
	AngleControlOut = (AngleControlOutNew - AngleControlOutOld)*(i/5.0) + AngleControlOutOld;
	i++;
	
}

/******************************
*@函数名：Angle_Speed_Control(void)
*@输入：无
*@输出：无
*@关键参数：Angle_Speed_Out
*@作用：角速度环
*@备注：无
*******************************/
void Angle_Speed_Control(void)  //2ms执行1次,角速度控制
{
	static float Car_Angle_Speed_Last_err = 0;
//	float Iout_temp = 0;
 
	gyro_x_ang_caculate(); //car_angle1--得到俯仰角和角速度

  //AngleControlOut = 0;       //屏蔽角度环输出

	Car_Angle_Speed_err = AngleControlOut - gyro_speed_y; //速度环输出-当前角速度
	
	AngSpe_Pout += ANGLE_SPEED_CONTROL_P * (Car_Angle_Speed_err - Car_Angle_Speed_Last_err);
	AngSpe_Iout += ANGLE_SPEED_CONTROL_I * Car_Angle_Speed_err;
	
//    AngSpe_Iout = (AngSpe_Iout > ILimit) ?  ILimit : AngSpe_Iout;   //积分限幅
//    AngSpe_Iout = (AngSpe_Iout < -ILimit) ? -ILimit : AngSpe_Iout;  
	
//	Angle_Speed_Out=ANGLE_SPEED_CONTROL_P * (Car_Angle_Speed_err - Car_Angle_Speed_Last_err)+ANGLE_SPEED_CONTROL_I * Car_Angle_Speed_err;
	Angle_Speed_Out = AngSpe_Pout + AngSpe_Iout;
	
	Angle_Speed_Out = (Angle_Speed_Out > AngSpeLim_fu) ?  AngSpeLim_fu:Angle_Speed_Out;
  Angle_Speed_Out = (Angle_Speed_Out < -AngSpeLim_fu) ? -AngSpeLim_fu:Angle_Speed_Out; 

	Car_Angle_Speed_Last_err = Car_Angle_Speed_err;  //值传递
}

