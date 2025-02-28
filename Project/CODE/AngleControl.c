#include "headfile.h"

//74.9 8.05 13.0 0.2149
//80.25 8.625  14.0 0.2149 15.9
//65.0  1.0 17.0 7.5

float ANGLE_SPEED_CONTROL_P = 40.0f;  //50 ���ٶ���Ӧ \			200 \88\76.3\107\74.9
float ANGLE_SPEED_CONTROL_I = 1.5f;  //20 ���ٶȻ��� \30 L10000   \10(���ᶶ��11.9365\11.0\11.499\8.05
int32 AngSpeLim_fu = 45000;      //35000

float angle_acc = 0.0f;
float gyro_speed_y_ratio = 16.4f;   //��Ҫ����,4 3.5!
float gyro_speed_y = 0;
float icm_zero_y = -9.0;        //��С����ֹ����
float car_angle1 = 0.0f;
float k = 0.9605f;  //0.9605
float Gyro_Integral_Z=0.0f;
float Gyro_Turn_Angle=0.0f;
#define time_pit  0.002   //��ʱ���ж�4msһ��

float AngleControlOut = 0;
int32 Angle_Speed_Out = 0;

uint8 ang_stage;  //����ִ��״̬��־λ

int32 AngSpe_Pout;
int32 AngSpe_Iout;

float Angle_All_ERR;   //�ǶȻ�ƫ��

float AngleControlOutOld = 0,AngleControlOutNew = 0;

float car_angle_err; 
float Car_Angle_Speed_err = 0.0;

int32 ILimit = 50000;

Run_Type Wheel = 
{
	.Set_Angle = 18.0f,             //ƽ��Ƕ� 31.5f  \28.5\15.9
	.Angle_P = 22.6f,           //�ǶȻ���Ӧ12         \15\11.0\11.0\16.0
	.Angle_D = 9.0f,          //�ǶȻ�����            \0.03\0.2149\0.2149\6.5
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
*@��������gyro_x_ang_caculate(void)
*@���룺��
*@�������
*@�ؼ�������gyro_y_zero(�����Ǿ�ֹ)
*@���ã������С���ĸ����ǵĽǶ�car_angle1�͸����ǵĽ��ٶ�gyro_speed_y
*@��ע������ͨ���������ж���ִ��
*******************************/
void gyro_x_ang_caculate(void)
{
	 get_icm20602_accdata_spi();														// ��ȡICM20602�Ĳ�����ֵ
	 get_icm20602_gyro_spi();														    // ��ȡICM20602�Ĳ�����ֵ
	 angle_acc = (180.0 / 3.1415926 * (float)(atan2)(icm_acc_x, icm_acc_z));      //�ɼ��ٶȵõ��Ƕ�ֵ
	 gyro_speed_y = (float)(icm_gyro_y * 1.0 - icm_zero_y) / gyro_speed_y_ratio;  //������ٶ�
	 car_angle1 =(float) (k * (car_angle1 + gyro_speed_y * time_pit) + (1 - k) * (angle_acc));  //0.002=�ж�����2ms
//  Gyro_Integral_Z+=(float)((int16)icm_gyro_z - 0)/16.4f;  //�����ǻ���
//	Gyro_Turn_Angle=Gyro_Integral_Z*time_pit;   				//ת���ĽǶ�,ʹ����֮������Gyro_Integral_Z = 0.0;    
}

/******************************
*@��������AngleControl(void)
*@���룺��
*@�������
*@�ؼ�������AngleControlOut
*@���ã��ǶȻ�
*@��ע����
*******************************/
void AngleControl(void)    //10msִ��һ�� 
{
	static float AngleErrOld = 0;
	static float AngleErrNew = 0;
	static float AngleErrFilt = 0;
	
	static float SpeedControlOutOld = 0;
	static float SpeedControlOutNew = 0;
	static float SpeedControlOutFilt = 0;
	
	//SpeedControlOut = 0;	//�����ٶȻ�
	
	car_angle_err =  car_angle1 - Wheel.Set_Angle;//�����Ƕ�-�����Ƕ� 

	AngleErrOld = AngleErrFilt;
	AngleErrNew = car_angle_err;
   
	//�Ƕ��˲�����һ�εĽǶ�ƫ������һ�εıȽ�
	if(AngleErrOld <= AngleErrNew)
	{
		AngleErrFilt = (AngleErrNew - AngleErrOld > 7.0) ? AngleErrOld + 7.0 : AngleErrNew;
	}//ԭ���޷�Ϊ2
	else
	{
		AngleErrFilt = (AngleErrNew - AngleErrOld < -7.0) ? AngleErrOld - 7.0 : AngleErrNew;
	}	
	SpeedControlOutOld = SpeedControlOutFilt;
	SpeedControlOutNew = SpeedControlOut;
	
	//�ٶȻ�����˲�
	if(SpeedControlOutOld <= SpeedControlOutNew)
		SpeedControlOutFilt = (SpeedControlOutNew-SpeedControlOutOld > 20) ? SpeedControlOutOld + 20 : SpeedControlOutNew;
	else
		SpeedControlOutFilt = (SpeedControlOutNew-SpeedControlOutOld < -20) ? SpeedControlOutOld - 20 : SpeedControlOutNew;

	AngleControlOutOld = AngleControlOutNew;   //ֵ����
	
	//ԭ���޷�Ϊ20
	
	
	//�����Ƕ�-��ǰ�����Ƕ�-�ٶ�pid�������SpeedControlOut�˲�֮�� SpeedControlOut�����µ������Ƕ�
	
	Angle_All_ERR = AngleErrFilt + Wheel.Flag_SpeedControlOut * SpeedControlOutFilt;

	AngleControlOutNew = Angle_All_ERR * Wheel.Angle_P - gyro_speed_y * Wheel.Angle_D;
}


/******************************
*@��������AngleControFunlOut(void)
*@���룺��
*@�������
*@�ؼ�������AngleControlOut
*@���ã��ǶȻ��ֳ����ƽ�����
*@��ע����
*******************************/
void AngleControFunlOut(void)   //2msִ��һ��
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
*@��������Angle_Speed_Control(void)
*@���룺��
*@�������
*@�ؼ�������Angle_Speed_Out
*@���ã����ٶȻ�
*@��ע����
*******************************/
void Angle_Speed_Control(void)  //2msִ��1��,���ٶȿ���
{
	static float Car_Angle_Speed_Last_err = 0;
//	float Iout_temp = 0;
 
	gyro_x_ang_caculate(); //car_angle1--�õ������Ǻͽ��ٶ�

  //AngleControlOut = 0;       //���νǶȻ����

	Car_Angle_Speed_err = AngleControlOut - gyro_speed_y; //�ٶȻ����-��ǰ���ٶ�
	
	AngSpe_Pout += ANGLE_SPEED_CONTROL_P * (Car_Angle_Speed_err - Car_Angle_Speed_Last_err);
	AngSpe_Iout += ANGLE_SPEED_CONTROL_I * Car_Angle_Speed_err;
	
//    AngSpe_Iout = (AngSpe_Iout > ILimit) ?  ILimit : AngSpe_Iout;   //�����޷�
//    AngSpe_Iout = (AngSpe_Iout < -ILimit) ? -ILimit : AngSpe_Iout;  
	
//	Angle_Speed_Out=ANGLE_SPEED_CONTROL_P * (Car_Angle_Speed_err - Car_Angle_Speed_Last_err)+ANGLE_SPEED_CONTROL_I * Car_Angle_Speed_err;
	Angle_Speed_Out = AngSpe_Pout + AngSpe_Iout;
	
	Angle_Speed_Out = (Angle_Speed_Out > AngSpeLim_fu) ?  AngSpeLim_fu:Angle_Speed_Out;
  Angle_Speed_Out = (Angle_Speed_Out < -AngSpeLim_fu) ? -AngSpeLim_fu:Angle_Speed_Out; 

	Car_Angle_Speed_Last_err = Car_Angle_Speed_err;  //ֵ����
}

