#include "headfile.h"

#define KEY_1				D0														// 定义主板上按键对应引脚
#define KEY_2				D1														// 定义主板上按键对应引脚
#define KEY_3				D2														// 定义主板上按键对应引脚
#define KEY_4				D3														// 定义主板上按键对应引脚
#define SWITCH_1			D14														// 定义主板上拨码开关对应引脚
#define SWITCH_2			D15														// 定义主板上拨码开关对应引脚


uint8 key1_state = 0;																// 按键动作状态
uint8 key2_state = 0;																// 按键动作状态
uint8 key3_state = 0;																// 按键动作状态
uint8 key4_state = 0;																// 按键动作状态

uint8 switch1_state = 0;															// 按键动作状态
uint8 switch2_state = 0;															// 按键动作状态

uint8 key1_state_last = 0;															// 上一次按键动作状态
uint8 key2_state_last = 0;															// 上一次按键动作状态
uint8 key3_state_last = 0;															// 上一次按键动作状态
uint8 key4_state_last = 0;															// 上一次按键动作状态

uint8 switch1_state_last = 0;														// 上一次按键动作状态
uint8 switch2_state_last = 0;														// 上一次按键动作状态

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
*@函数名：void button_init(void)
*@输入：无
*@输出：无
*@关键参数：
*@作用：按键初始化,拨码开关初始化
*@备注：无
*******************************/
void button_init(void)
{
	gpio_init(KEY_1, GPI, GPIO_HIGH, GPI_PULL_UP);									// 初始化为GPIO浮空输入 默认上拉高电平
	gpio_init(KEY_2, GPI, GPIO_HIGH, GPI_PULL_UP);									// 初始化为GPIO浮空输入 默认上拉高电平
	gpio_init(KEY_3, GPI, GPIO_HIGH, GPI_PULL_UP);									// 初始化为GPIO浮空输入 默认上拉高电平
	gpio_init(KEY_4, GPI, GPIO_HIGH, GPI_PULL_UP);									// 初始化为GPIO浮空输入 默认上拉高电平
	gpio_init(SWITCH_1, GPI, GPIO_HIGH, GPI_PULL_UP);								 // 初始化为GPIO浮空输入 默认上拉高电平
	gpio_init(SWITCH_2, GPI, GPIO_HIGH, GPI_PULL_UP);								 // 初始化为GPIO浮空输入 默认上拉高电平
}

extern  float k ;
extern int far_nThershold;
extern int neal_nThershold;
extern uint8 sub_row;
/******************************
*@函数名：void button_change(void)
*@输入：无
*@输出：无
*@关键参数：
*@作用：按键调参
*@备注：无
*******************************/
void button_change(void)
{

//使用此方法优点在于，不需要使用while(1) 等待，避免处理器资源浪费
		//保存按键状态
		key1_state_last = key1_state;
		key2_state_last = key2_state;
		key3_state_last = key3_state;
		key4_state_last = key4_state;

		switch1_state_last = switch1_state;
		switch2_state_last = switch2_state;
		//读取当前按键状态
		key1_state = gpio_get(KEY_1);
		key2_state = gpio_get(KEY_2);
		key3_state = gpio_get(KEY_3);
		key4_state = gpio_get(KEY_4);

		switch1_state = gpio_get(SWITCH_1);
		switch2_state = gpio_get(SWITCH_2);
		//检测到按键按下之后  并放开置位标志位
		if(key1_state && !key1_state_last)    key1_flag = 1;
		if(key2_state && !key2_state_last)    key2_flag = 1;
		if(key3_state && !key3_state_last)    key3_flag = 1;
		if(key4_state && !key4_state_last)    key4_flag = 1;
		//标志位置位之后，可以使用标志位执行自己想要做的事件
		if(switch1_state==1)
		{
				if(key1_flag)   
				{
					key1_flag = 0;//使用按键之后，应该清除标志位
					far_nThershold+=2;  //角速度
				}

				if(key2_flag)   
				{
					key2_flag = 0;//使用按键之后，应该清除标志位
					far_nThershold-=2;
				}

				if(key3_flag)   
				{
					key3_flag = 0;//使用按键之后，应该清除标志位
					neal_nThershold+=2;  
				}

				if(key4_flag)   
				{
					key4_flag = 0;//使用按键之后，应该清除标志位
					neal_nThershold-=2;  
				}
		}
		else if(switch1_state==0)
		{

				if(key1_flag)   
				{
					key1_flag = 0;//使用按键之后，应该清除标志位
					sub_row += 2;
				}

				if(key2_flag)   
				{
					key2_flag = 0;//使用按键之后，应该清除标志位
					sub_row -= 2;
				}

				if(key3_flag)   
				{
					key3_flag = 0;//使用按键之后，应该清除标志位
					sub_row += 5;
				}

				if(key4_flag)   
				{
					key4_flag = 0;//使用按键之后，应该清除标志位
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

		//systick_delay_ms(10);//延时，按键程序应该保证调用时间不小于10ms
		//此处编写需要循环执行的代码


}


