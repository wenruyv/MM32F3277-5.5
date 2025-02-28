#ifndef _Picture_deal_h
#define _Picture_deal_h


#define    normal  0
#define    far     1 
#define    near    2
#define    BUZZER_PIN			D12	
#define    BEEN_HIGH      gpio_set(BUZZER_PIN, GPIO_HIGH)
#define    BEEN_LOW       gpio_set(BUZZER_PIN, GPIO_LOW);



//#define OPEN_LCD   //打开屏幕

extern int16 X ,Y,y;
extern int16 beacon_x;
extern int16 beacon_y;
extern uint8 car_state;

extern int16 sum;
extern int8 left,right,top,bottom; 
extern uint8 flag_circle;
extern uint8 dotcnt;

/////////
extern float direction_p;//方向环pid
extern float direction_i;
extern float direction_d;
extern int16 errorlight;
extern int16 integral;
extern int16 error_last;
extern int16 direction_pwm;
extern int16 rotation_speed;
extern uint8 distance ;
extern uint8  light_off; 
extern int16 control_x;

extern int32 light_pwm ;
extern float Kp ;
extern uint8_t lose_light;

extern void Scan_Point(void);
extern int16 limit_s16(int16 Data,int16 Min,int16 Max);
extern int16 PaiXu(int16 Data0,int16 Data1,int16 Data2);
extern int16 Coordinate_Filter(int16 Channal,int16 Data_In);
extern void center_line(void);
void direction_pid(int beacon_x);
extern void distance_control (void);
void lost_control(void);

#endif

