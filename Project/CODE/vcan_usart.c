#include "headfile.h"
//����1����1���ַ�
//c:Ҫ���͵��ַ�
void usart1_send_char(uint8 c)
{
//    while(USART_GetFlagStatus(UART_4,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������
    uart_putchar(WIRELESS_UART,c);
}

#define CMD_WARE     3   //3��������ʾ����,1����ͷ��2CCD

void usart1_putbuff (uint8_t *buff, uint32_t len)  //����1
{
    while(len--)
    {
      usart1_send_char(*buff);
       buff++;
    }
}

void vcan_sendware_wireless(uint8_t *wareaddr, uint32_t waresize)  //����1
{
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};
    usart1_putbuff(cmdf,sizeof(cmdf));
    usart1_putbuff(wareaddr,waresize);
    usart1_putbuff(cmdr,sizeof(cmdr));

}

//      vcan_sendware_wireless((uint8_t *)show_angle, sizeof(show_angle));

