#include "headfile.h"
//串口1发送1个字符
//c:要发送的字符
void usart1_send_char(uint8 c)
{
//    while(USART_GetFlagStatus(UART_4,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕
    uart_putchar(WIRELESS_UART,c);
}

#define CMD_WARE     3   //3代表虚拟示波器,1摄像头，2CCD

void usart1_putbuff (uint8_t *buff, uint32_t len)  //串口1
{
    while(len--)
    {
      usart1_send_char(*buff);
       buff++;
    }
}

void vcan_sendware_wireless(uint8_t *wareaddr, uint32_t waresize)  //串口1
{
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};
    usart1_putbuff(cmdf,sizeof(cmdf));
    usart1_putbuff(wareaddr,waresize);
    usart1_putbuff(cmdr,sizeof(cmdr));

}

//      vcan_sendware_wireless((uint8_t *)show_angle, sizeof(show_angle));

