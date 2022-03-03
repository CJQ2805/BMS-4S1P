#ifndef __IAP_H
#define __IAP_H
#include "bsp.h"


#define 	RX_DATA_MAX_LEN 		256
#define  	TX_DATA_MAX_LEN  		10

#define CMD_Rec_Data_Frame  0x31   //��������֡�����֣��ڵ���Update����ʱ��Ҫ�ȷ��͸�������
#define CMD_Erase           0x32          //����������
#define CMD_Start_Update    0x33       //��ʼ����������
#define CMD_Compel_Update   0x34     //ǿ������������
#define CMD_Frame_End       0xBB     //frame end command
#define CMD_File_End        0x45     //file end command
#define CMD_ACK             0x36           //ȷ��֡������
#define CMD_Exit_Update     0x37       //�˳�����������
#define CMD_EraseOk         0x21          //�����ɹ���Ӧ��
#define CMD_CRC_Ok          0x22       //֡У����ȷ��Ӧ��
#define CMD_CRC_Error       0x23      //֡У�������Ӧ��
#define CMD_NonApp          0x24          //û��Ӧ�ó�����Ӧ��
#define CMD_Load_OK         0x25      //���سɹ���Ӧ��
#define CMD_Compel_Ready    0x26   //����ǿ��������Ӧ��
#define CMD_Over_Time       0x27      //�ȴ���ʱ��Ӧ��
#define CMD_Error_Frame     0x28      //����֡��Ӧ��
#define CMD_Update_Ready    0x29         //׼��������Ӧ��
#define CMD_ExitOk          0x41         //�˳�������Ӧ��
#define CMD_Write_Error     0x42         //д�������Ӧ��



extern volatile unsigned int g_u32Seq;
extern volatile __align(4) u8  g_au8Rxbuffer[RX_DATA_MAX_LEN];

u8 iap_wire_app_hex(uint32_t u32addr, uint32_t u32len, uint8_t *pu8buf);
uint8_t BufferCmp(uint8_t *buf1,uint8_t *buf2,uint8_t size);
void Run_App_Check(void);
void iap_update(void);
#endif 

