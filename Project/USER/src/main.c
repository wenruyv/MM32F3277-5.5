/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
* 技术讨论QQ群：一群：179029047(已满)  二群：244861897
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file				main
* @company			成都逐飞科技有限公司
* @author			逐飞科技(QQ3184284598)
* @version			查看doc内version文件 版本说明
* @Software			IAR 8.3 or MDK 5.24
* @Target core		MM32F3277
* @Taobao			https://seekfree.taobao.com/
* @date				2021-02-22


********************************************************************************************************************/

#include "headfile.h"

/*分段固定阈值，前30行阈值高，后30行阈值低*/
int far_nThershold = 115;
int neal_nThershold = 70;//90
uint8 sub_row = 30; //开始分段的行数
int nThershold;
float  show_angle[4]={0};
extern float k ;
extern int32 right_speed;
extern int32 left_speed;

//白255，黑 0

int main(void)
{
	board_init(true);																// 初始化 debug 输出串口,usart1
  motor_init();
	icm20602_init_spi();
	seekfree_wireless_init();			
	Encoder_init();
	button_init();
	gpio_init(BUZZER_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
  lcd_init();
	lcd_showstr(0, 0, "camera init start.");
	mt9v03x_init();																	// 初始化模块
	lcd_showstr(0, 1, "camera init success.");
	lcd_clear(WHITE);  //清屏函数
	tim_interrupt_init_ms(TIM_8, 2, 0x00);         //定时器8初始化，2ms中断一次，用来执行主控程序
	
  while(1)
	{
		button_change();		
		show_angle[0]=AngSpe_Iout;
		show_angle[1]=car_angle1;
		show_angle[2]=AngSpe_Pout;
		show_angle[3]=Wheel.Set_Angle;
		vcan_sendware_wireless((uint8_t *)show_angle, sizeof(show_angle));
		    
		 if(mt9v03x_finish_flag)
		{
			//nThershold = get_thr_ys(mt9v03x_image[0], MT9V03X_W,  MT9V03X_H);     //获取阈值
			
			
			gray_to_2bw_otsu_ys(mt9v03x_image[0], bin_img[0],MT9V03X_W, MT9V03X_H,far_nThershold,neal_nThershold,sub_row);   //二值化 

			Scan_Point();
      center_line();						//计算框内白点数
#ifdef OPEN_LCD
			lcd_displayimage032(bin_img[0], MT9V03X_W, MT9V03X_H);        				//打印二值化图像
			lcd_showint8(0,5,beacon_y);/*行*/
			lcd_showint8(40,5,beacon_x);/*列*/  
			lcd_showuint8(0,6,far_nThershold);/*远阈值*/
			lcd_showuint8(40,6,neal_nThershold);/*近阈值*/
			lcd_showint8(0,7,sub_row);/*开始分段的行数*/
			for(u8 i = 0;i < 80; i++)
			{
			   lcd_drawpoint(i,sub_row,RED);
			}   
#endif			
   	  mt9v03x_finish_flag = 0;
		}

	}
}


