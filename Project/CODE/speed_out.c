#include "headfile.h"


float SpeKp = 0.004f; //�ٶȻ���Ӧ     \0.003
float SpeKi =  0.003f; //�ٶȻ�����    \0.003
int32 SpeILimit = 0;  //�ٶȻ�������

/**********���ڲ��ٶȱ���******/
const float Ratio_Encoder = 200/(1175*0.1);//�����ٶ�=counter*�����ܳ���mm��/������תһȦ��Ӧ��������*�������ڣ�  ����ٶȵ�λ��mm/s ��100ms
float RealSpeed = 0;    //��ʵ�ٶ�
int16 LeftRealSpeed = 0;
int16 RightRealSpeed = 0;

/**********�����ٶ�ƫ�����******/
float FilterSpeed = 0;   //�����˲��ٶ�
float SpeedErrorTemp[5] = {0};
uint16 ExpectSpeed = 0;  //С�������ٶ�
float Distance = 0;   		//�������ۼ�ֵ
	
/**********�����ٶȿ��Ʊ���******/
float Spe_integral = 0;
	
float SpeedOut = 0 ;
float SpeedControlOut = 0 ;    //�ٶȿ������
float SpeedControlOutOld = 0;
float SpeedControlOutNew = 0;



/************************************************************************************************************
*��������void Encoder_init(void)
*���룺��
*�������
*���ã���������ʼ��
*************************************************************************************************************/
void Encoder_init(void)
{
	tim_counter_init(ENCODER1_TIM, ENCODER1_PLUS);
	tim_counter_init(ENCODER2_TIM, ENCODER2_PLUS);

	gpio_init(ENCODER1_DIR, GPI, 1, GPI_PULL_UP);
	gpio_init(ENCODER2_DIR, GPI, 1, GPI_PULL_UP);


}


/************************************************************************************************************
*��������GetRealSpeed����
*���룺��
*�������
*���ã�ͨ����������ȡ�ٶ�
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
*��������SpeedControl����
*���룺��
*�������
*���ã��ٶȻ�pi�����㷨
**************************************************************************************************************/
void SpeedControl(void)               //100msִ��һ��
{
    GetRealSpeed();  //ȡ����ʵ�ٶ�
	
	  SpeedErrorTemp[0] = ExpectSpeed - FilterSpeed;   //�����ٶ�ƫ��
		
		SpeedControlOutOld = SpeedControlOutNew;		//ֵ����
	
		Spe_integral += SpeedErrorTemp[0];    //�ٶ�ƫ�����
   	if (Spe_integral >= SpeILimit)        //����Ļ����޷�Ϊ0������û�������ã�ֻ��ʹ����P����
			Spe_integral = SpeILimit;
		else if (Spe_integral < -SpeILimit)
			Spe_integral = -SpeILimit;
	 
    SpeedOut = SpeKp * SpeedErrorTemp[0] + SpeKi* Spe_integral;
}

/************************************************************************************************************
*��������SpeedControlFuncOut����
*���룺��
*�������
*���ã��ٶȻ�ƽ�����
**************************************************************************************************************/

void SpeedControlFuncOut(void)     //10msִ��һ��
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

