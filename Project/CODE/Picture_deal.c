#include "headfile.h"
#include "math.h"




int16 X1[MT9V03X_W+20]= {0};        //�������������ŵƵ�����  X1  Y1
int16 Y1[MT9V03X_W+20]= {0};
int16 total_count = 0;
int16 total = 0;//�ڶ��α����������
int16 X =0,Y=0;

int16 beacon_x=0;
int16 beacon_y=0;
int16 control_x=0;
int16 y=0;
uint8 car_state = 0;//1:δ�ҵ���
uint8 distance = 0;//�����ж�

int16 sum = 0;
uint8 flag_circle = 0;//Բ��Ϊ1������Ϊ0
uint8 dotcnt = 0;
int8 left = 0,right = 0,top = 0,bottom = 0; 
uint8  light_off = 0; //��Ʊ�־λ


#define Error(a,b)   (a > b) ? (a - b) : (b - a)
int Center_W = MT9V03X_W/2;
#define Center_H     MT9V03X_H/2

int16 limit_s16(int16 Data,int16 Min,int16 Max)      
{
	if(Data<Min)Data=Min;
	else if(Data>Max)Data=Max;
	return Data;
}

int16 PaiXu(int16 Data0,int16 Data1,int16 Data2)//3 2 1
{
	int16 Temp=0;
	if(Data0>Data1)//2 3 1
	{
		Temp=Data0;
		Data0=Data1;
		Data1=Temp;
	}
	if(Data0>Data2)//1 3 2
	{
		Temp=Data0;
		Data0=Data2;
		Data2=Temp;
	}
	if(Data1>Data2)//1 2 3
	{
		Temp=Data1;
		Data1=Data2;
		Data2=Temp;
	}
	return Data1;
}


int16 Coordinate_Filter(int16 Channal,int16 Data_In)
{
	static int16	Data[3][3]={0};
	int16 Temp=0;
	Data[Channal][0]=Data[Channal][1];
	Data[Channal][1]=Data[Channal][2];
	Data[Channal][2]=Data_In;
	Temp=PaiXu(Data[Channal][0],Data[Channal][1],Data[Channal][2]);  //��С�������򷵻��м�ֵ
	
	return Temp;
}



void Scan_Point(void)
{	   
	int16 i = 0, j = 0;
	uint8 pot_1=0;
	uint8 pot_2=0;
	int16 count_x = 0;
	for(u8 z =0;z<MT9V03X_W+20;z++)    //�������
	{
		X1[z]=0;
		Y1[z]=0;
	}
	
	left = right = top = bottom =0; // ��������
	flag_circle = 0;
	dotcnt = 0;
/****************�����ҵ�*************************************/
	for(i = 0 ; i < MT9V03X_H;i++)  //80��
	{				
		for(j =0; j < MT9V03X_W;j++) //120��
		{
			if(bin_img[i][j] > nThershold)          
			{  
				count_x++;                       
			}
			else   
			{     
				if(count_x>=2)  //�˳���㣬�ҵ�2�������ϲŴ����ҵ���
				{
					  X = j-count_x/2;    //���һ���׵��������j��ȥ�׵�ĸ�����һ�룬���ǰ������ĵ������
				    Y = i; 				      //��¼�׵��������		
/***********************��������************************/										
							while(bin_img[Y][X - left]) //��¼�������
						 {	
								left++;
						 }
						 while(bin_img[Y][X + right]) //��¼�ұ�����
						 {
								right++;
						 }
							while(bin_img[Y - top][X]) //��¼�ϱ�����
						 {
								top++;
						 }
						 while(bin_img[Y + bottom][X]) //��¼�±�����
						 {
								bottom++;
						 }
						 if(((left-right)<=2)&&((right-left)<=2)&&((top-bottom)<=2)&&((bottom-top)<=2)&&(left > 0)) //�ж���״,����������Բ�������������
							{
									X1[pot_1++] = X;
					      	Y1[pot_2++] = Y;		
							} 
//							if (Error(X,X1[pot_1-1])<4&&Error(Y,Y1[pot_1-1])<=2)
//							{
//								X1[pot_1++] = X;
//								Y1[pot_2++] = Y;			
//							}							
				}
					count_x = 0;  //��¼һ�ΰ׵�����֮�󣬰׵��������
		  }
                                                                                                                      
		}
	}                                                        

			beacon_x=X1[pot_1>>1];   //������ֵ�˲�
		  beacon_y=Y1[pot_2>>1];
			beacon_x = beacon_x-Center_W;  // ��ͼ������Ϊ�������ĵ�   		
/**************************************************************************/
     	beacon_x=limit_s16(beacon_x,-MT9V03X_W/2,MT9V03X_W/2);    //�����޷�����֤������������ͼ��֮��
	    beacon_y=limit_s16(beacon_y,0,MT9V03X_H); 
	    beacon_x=Coordinate_Filter(0,beacon_x);      //�����ε�x���괫�룬���򣬷���һ���м�ֵ��x�����˲�
	    beacon_y=Coordinate_Filter(1,beacon_y);

	    if(beacon_x == -MT9V03X_W/2 && beacon_y == 0)		  //�Ƴ���ͼ��Χû���ҵ���
			{
		     beacon_x = 0;
			}
			if(beacon_x == -60) 
			{
				 beacon_x = 60;
			}
			if(beacon_x == 0 && beacon_y ==0)
			{
				car_state = 1;								  //û�ҵ��Ʊ�־λ
				sum = 0;
			}
			else  //�ҵ���
			{
				lose_light = 0;
				light_pwm = 0;
				car_state = 0;	
				Gyro_Turn_Angle = 0;						//��ת�Ƕ�����
	    }
}



//����--n,����Ĵ�С
//�ڻ���������������������Χ���Ȳ����㣬��ִ��ת�򻷣�ʹ�������
//ͨ�����㻭���ڵĵ�����beacon_y���ʹ��ʵ���˹���жϾ���
// left = 0,right = 0,top = 0,bottom = 0;  beacon_x beacon_y
void center_line(void)
{
	 int8 k;
	 int8 i,j;
	 int8 W_left = beacon_x + Center_W -left;
	 int8 W_right = beacon_x + Center_W +right;
	 int8 H_top = beacon_y - top;
	 int8 H_bottom = beacon_y + bottom;
	

		if(car_state == 0)  
		{
			  sum = 0;
#ifdef OPEN_LCD
			  for(k = 0;k<MT9V03X_H;k++) //������
				{
						 lcd_drawpoint(Center_W,k,RED);
				} 
			  for(k = H_top;k < H_bottom&&H_top>0&&W_left>0&&W_right>0;k++)//����
				{          
					  lcd_drawpoint(W_right,k,YELLOW);
				  	lcd_drawpoint(W_left, k,YELLOW);
				}
				for(k = W_left;k <W_right&&H_top>0&&W_left>0&&W_right>0;k++)  
				{
					  lcd_drawpoint(k, H_top,YELLOW);
				  	lcd_drawpoint(k, H_bottom,YELLOW);
				}
#endif
				for(i = W_left;i < W_right;i++) //����
				{
				   for(j = H_top;j < H_bottom;j++)
						{
								if(bin_img[j][i])
								{
									 sum++;
								}
						}   
				}
		}
}

/*
30--1m
90--0.5m--45
300--0.25m--70
<70 -- 0.0 --77


*/

float direction_p = 85.0;//����pid
float direction_i = 0;
float direction_d = 10.0;
int16 errorlight = 0;
int16 integral = 0;
int16 error_last = 0;
int16 direction_pwm = 0;

#define HIGH_SPEED

#ifdef  HIGH_SPEED
#define far_speed           3800 //3800
#define near_far_speed      3000 //3200
#define near_speed          2700 //2700
#define very_near_speed     2500 //2300
#define cor_speed           3100 //�����ٶ�
#define big_cor_speed       3600 //�����ٶ�
#define closs_speed         2500
#else
#define far_speed           3000 //3700
#define near_far_speed      2700 //3200
#define near_speed          2500 //2700
#define very_near_speed     2500 //2300
#define cor_speed           2500 //�����ٶ�
#define big_cor_speed       2700 //�����ٶ�
#define closs_speed         2500

#endif




//����ƫ-������ƫ+

void distance_control (void)
{
	    control_x = beacon_x;
	    BEEN_LOW;
    	lose_light = 0;
	   if(beacon_y > 0 && beacon_y <70&& sum < 30) //1m����
		 { 
			    ExpectSpeed = far_speed;
//			    if(beacon_x<-30 )
//					{
//							control_x = beacon_x - 6;
//					}
//					else if(beacon_x < -15 && beacon_x > -30)
//					{
//						  control_x = beacon_x - 3;
//					}
//					else if(beacon_x < 30 && beacon_x > 15)
//					{
//							control_x = beacon_x + 3;
//					}
//					else if(beacon_x > 30)
//					{
//						control_x = beacon_x + 6;
//					}
		 }
		 else if(sum > 30 && sum < 150) //1m--0.25m
		 {
		     ExpectSpeed = near_far_speed;
			   
			   if(beacon_x < -30)
				 {
				     ExpectSpeed = big_cor_speed ;
					   control_x = beacon_x - 13;			 
				 }
			 	 else if(beacon_x >-30 && beacon_x < -20 )  //λ��У��
				 {
				     ExpectSpeed = cor_speed;
					   control_x = beacon_x - 10;
				 }
				 else if(beacon_x >= -20 && beacon_x <= -10 )
				 {
				     ExpectSpeed = cor_speed;
					   control_x = beacon_x - 5;
				 }
				 else if(beacon_x > 10 && beacon_x <= 20)
				 {
				     ExpectSpeed = cor_speed;
					   control_x = beacon_x + 5;
				 }
				 else if(beacon_x > 20 && beacon_x < 30)
				 {
				 		 ExpectSpeed = cor_speed;
					   control_x = beacon_x + 10;
				 }
				 else if(beacon_x >= 30)
				 {
				     ExpectSpeed = big_cor_speed ;
					   control_x = beacon_x + 13;	
				 }
			 
		 }
		 else if(sum > 150&& sum < 300) //0.25m
		 {
		     ExpectSpeed = near_speed;
			   BEEN_HIGH;
			   if(beacon_x <= -30)
				 {
					   ExpectSpeed = big_cor_speed ;
				     control_x = beacon_x - 20;
				 }
			 	 else if(beacon_x <= -15 && beacon_x > -30)  //λ��У��
				 {
					   ExpectSpeed = cor_speed;
					    control_x = beacon_x - 16;
				 }
				 else if(beacon_x < -8 && beacon_x >-15)
				 {
				     control_x = beacon_x - 10;
				 }
				 else if(beacon_x > 8 && beacon_x < 15)
				 {
				     control_x = beacon_x + 10;
				 }
				 else if(beacon_x > 15 && beacon_x < 30)
				 {
					   ExpectSpeed = cor_speed;
				     control_x = beacon_x + 16;
				 }
				 else if( beacon_x >= 30)
				 {
					   ExpectSpeed = big_cor_speed ;
				     control_x = beacon_x + 20;
				 }

		 }
     else if(sum >= 300 ) //0.25m
		 {
		     ExpectSpeed = closs_speed;
			   BEEN_HIGH;				 
		 }
		 if( (beacon_x > 0 && control_x > 0) || (beacon_x < 0 && control_x < 0)) 
    			  control_x = control_x;
}
/*�е�֮��ת90 ����������*/
int32 light_pwm = 0;
#define turn_angle  -100.0f  //������ת�Ƕ�
float Kp = 133.0f;  
uint8_t lose_light = 0;
void string_light(void)
{
    Gyro_Turn_Angle+=(float)((int16)icm_gyro_z - 0)/16.4f*(0.002);  //�����ǻ���
	  light_pwm = Kp*(Gyro_Turn_Angle - turn_angle);
}

/********************************************************************************************************************/


/*************************************************************************
*  �������ƣ�void direction_pid(void)
*  ����˵����pidλ��ʽ���������
*  ����˵����beacon_x����ͼƬ����Ϊԭ�㽨������õ��ű��λ��
*  �������أ���
*  �޸�ʱ�䣺
*  ��    ע��λ��ʽPID:u(k)=u(k)-u(k-1)=kp*e(k)+ki*e(i)+kd*(e(k)-e(k-1))
*  �÷��򻷵�������������ٶ�
*************************************************************************/
void direction_pid(int control_x)
{
	errorlight = control_x;
	integral += errorlight;
  direction_pwm = direction_p*errorlight + direction_i*integral + direction_d*(errorlight-error_last);
  error_last = errorlight;	
}


/*���ƿ���*/
#define time_pit  0.002 
void lost_control(void)
{
			Gyro_Turn_Angle += (float)((int16)icm_gyro_z - 0)/16.4f*time_pit;  //�����ǻ���
	
				 if(Gyro_Turn_Angle > turn_angle && Gyro_Turn_Angle < 0)	
					{
							string_light();
					}	
				  else if(Gyro_Turn_Angle > -360.0 && Gyro_Turn_Angle < turn_angle)
					{
								 direction_pwm = 4000;	
					 
					}
					else if(Gyro_Turn_Angle < -360.0 && Gyro_Turn_Angle > -520.0)  
					{
								direction_pwm = 4000;
					}	
					else 
					{ 
								direction_pwm = 6000;
					}
}

