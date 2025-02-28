#include "headfile.h"

#define KEY_1				D0														// ���������ϰ�����Ӧ����
#define KEY_2				D1														// ���������ϰ�����Ӧ����
#define KEY_3				D2														// ���������ϰ�����Ӧ����
#define KEY_4				D3														// ���������ϰ�����Ӧ����
#define SWITCH_1			D14														// ���������ϲ��뿪�ض�Ӧ����
#define SWITCH_2			D15														// ���������ϲ��뿪�ض�Ӧ����


uint8 key1_state = 0;																// ��������״̬
uint8 key2_state = 0;																// ��������״̬
uint8 key3_state = 0;																// ��������״̬
uint8 key4_state = 0;																// ��������״̬

uint8 switch1_state = 0;															// ��������״̬
uint8 switch2_state = 0;															// ��������״̬

uint8 key1_state_last = 0;															// ��һ�ΰ�������״̬
uint8 key2_state_last = 0;															// ��һ�ΰ�������״̬
uint8 key3_state_last = 0;															// ��һ�ΰ�������״̬
uint8 key4_state_last = 0;															// ��һ�ΰ�������״̬

uint8 switch1_state_last = 0;														// ��һ�ΰ�������״̬
uint8 switch2_state_last = 0;														// ��һ�ΰ�������״̬

uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;
uint8 key4_flag;

uint8 key1_count;
uint8 key2_count;
uint8 key3_count;
uint8 key4_count;

extern int day_nThershold;

/******************************
*@��������void button_init(void)
*@���룺��
*@�������
*@�ؼ�������
*@���ã�������ʼ��,���뿪�س�ʼ��
*@��ע����
*******************************/
void button_init(void)
{
	gpio_init(KEY_1, GPI, GPIO_HIGH, GPI_PULL_UP);									// ��ʼ��ΪGPIO�������� Ĭ�������ߵ�ƽ
	gpio_init(KEY_2, GPI, GPIO_HIGH, GPI_PULL_UP);									// ��ʼ��ΪGPIO�������� Ĭ�������ߵ�ƽ
	gpio_init(KEY_3, GPI, GPIO_HIGH, GPI_PULL_UP);									// ��ʼ��ΪGPIO�������� Ĭ�������ߵ�ƽ
	gpio_init(KEY_4, GPI, GPIO_HIGH, GPI_PULL_UP);									// ��ʼ��ΪGPIO�������� Ĭ�������ߵ�ƽ
	gpio_init(SWITCH_1, GPI, GPIO_HIGH, GPI_PULL_UP);								 // ��ʼ��ΪGPIO�������� Ĭ�������ߵ�ƽ
	gpio_init(SWITCH_2, GPI, GPIO_HIGH, GPI_PULL_UP);								 // ��ʼ��ΪGPIO�������� Ĭ�������ߵ�ƽ
}

extern  float k ;
extern int far_nThershold;
extern int neal_nThershold;
extern uint8 sub_row;
/******************************
*@��������void button_change(void)
*@���룺��
*@�������
*@�ؼ�������
*@���ã���������
*@��ע����
*******************************/
void button_change(void)
{

//ʹ�ô˷����ŵ����ڣ�����Ҫʹ��while(1) �ȴ������⴦������Դ�˷�
		//���水��״̬
		key1_state_last = key1_state;
		key2_state_last = key2_state;
		key3_state_last = key3_state;
		key4_state_last = key4_state;

		switch1_state_last = switch1_state;
		switch2_state_last = switch2_state;
		//��ȡ��ǰ����״̬
		key1_state = gpio_get(KEY_1);
		key2_state = gpio_get(KEY_2);
		key3_state = gpio_get(KEY_3);
		key4_state = gpio_get(KEY_4);

		switch1_state = gpio_get(SWITCH_1);
		switch2_state = gpio_get(SWITCH_2);
		//��⵽��������֮��  ���ſ���λ��־λ
		if(key1_state && !key1_state_last)    key1_flag = 1;
		if(key2_state && !key2_state_last)    key2_flag = 1;
		if(key3_state && !key3_state_last)    key3_flag = 1;
		if(key4_state && !key4_state_last)    key4_flag = 1;
		//��־λ��λ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�
		if(switch1_state==1)
		{
				if(key1_flag)   
				{
					key1_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
					far_nThershold+=2;  //���ٶ�
				}

				if(key2_flag)   
				{
					key2_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
					far_nThershold-=2;
				}

				if(key3_flag)   
				{
					key3_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
					neal_nThershold+=2;  
				}

				if(key4_flag)   
				{
					key4_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
					neal_nThershold-=2;  
				}
		}
		else if(switch1_state==0)
		{

				if(key1_flag)   
				{
					key1_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
					sub_row += 2;
				}

				if(key2_flag)   
				{
					key2_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
					sub_row -= 2;
				}

				if(key3_flag)   
				{
					key3_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
					sub_row += 5;
				}

				if(key4_flag)   
				{
					key4_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
					sub_row -= 5;
				}

		 }

		if(switch1_state != switch1_state_last)   
		{
			printf("\r\nSWITCH_1: %d.", switch1_state);
			
		}

		if(switch2_state != switch2_state_last)   
		{
			printf("\r\nSWITCH_2: %d.", switch2_state);
		}

		//systick_delay_ms(10);//��ʱ����������Ӧ�ñ�֤����ʱ�䲻С��10ms
		//�˴���д��Ҫѭ��ִ�еĴ���


}


