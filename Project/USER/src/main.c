/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				main
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.3 or MDK 5.24
* @Target core		MM32F3277
* @Taobao			https://seekfree.taobao.com/
* @date				2021-02-22


********************************************************************************************************************/

#include "headfile.h"

/*�ֶι̶���ֵ��ǰ30����ֵ�ߣ���30����ֵ��*/
int far_nThershold = 115;
int neal_nThershold = 70;//90
uint8 sub_row = 30; //��ʼ�ֶε�����
int nThershold;
float  show_angle[4]={0};
extern float k ;
extern int32 right_speed;
extern int32 left_speed;

//��255���� 0

int main(void)
{
	board_init(true);																// ��ʼ�� debug �������,usart1
  motor_init();
	icm20602_init_spi();
	seekfree_wireless_init();			
	Encoder_init();
	button_init();
	gpio_init(BUZZER_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
  lcd_init();
	lcd_showstr(0, 0, "camera init start.");
	mt9v03x_init();																	// ��ʼ��ģ��
	lcd_showstr(0, 1, "camera init success.");
	lcd_clear(WHITE);  //��������
	tim_interrupt_init_ms(TIM_8, 2, 0x00);         //��ʱ��8��ʼ����2ms�ж�һ�Σ�����ִ�����س���
	
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
			//nThershold = get_thr_ys(mt9v03x_image[0], MT9V03X_W,  MT9V03X_H);     //��ȡ��ֵ
			
			
			gray_to_2bw_otsu_ys(mt9v03x_image[0], bin_img[0],MT9V03X_W, MT9V03X_H,far_nThershold,neal_nThershold,sub_row);   //��ֵ�� 

			Scan_Point();
      center_line();						//������ڰ׵���
#ifdef OPEN_LCD
			lcd_displayimage032(bin_img[0], MT9V03X_W, MT9V03X_H);        				//��ӡ��ֵ��ͼ��
			lcd_showint8(0,5,beacon_y);/*��*/
			lcd_showint8(40,5,beacon_x);/*��*/  
			lcd_showuint8(0,6,far_nThershold);/*Զ��ֵ*/
			lcd_showuint8(40,6,neal_nThershold);/*����ֵ*/
			lcd_showint8(0,7,sub_row);/*��ʼ�ֶε�����*/
			for(u8 i = 0;i < 80; i++)
			{
			   lcd_drawpoint(i,sub_row,RED);
			}   
#endif			
   	  mt9v03x_finish_flag = 0;
		}

	}
}


