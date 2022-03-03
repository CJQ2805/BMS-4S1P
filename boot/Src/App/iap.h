#ifndef __IAP_H
#define __IAP_H
#include "bsp.h"


#define 	RX_DATA_MAX_LEN 		256
#define  	TX_DATA_MAX_LEN  		10

#define CMD_Rec_Data_Frame  0x31   //接收数据帧命令字，在调用Update函数时都要先发送该命令字
#define CMD_Erase           0x32          //擦除命令字
#define CMD_Start_Update    0x33       //开始升级命令字
#define CMD_Compel_Update   0x34     //强制升级命令字
#define CMD_Frame_End       0xBB     //frame end command
#define CMD_File_End        0x45     //file end command
#define CMD_ACK             0x36           //确认帧命令字
#define CMD_Exit_Update     0x37       //退出升级命令字
#define CMD_EraseOk         0x21          //擦除成功响应码
#define CMD_CRC_Ok          0x22       //帧校验正确响应码
#define CMD_CRC_Error       0x23      //帧校验错误响应码
#define CMD_NonApp          0x24          //没有应用程序响应码
#define CMD_Load_OK         0x25      //下载成功响应码
#define CMD_Compel_Ready    0x26   //可以强制升级响应码
#define CMD_Over_Time       0x27      //等待超时响应码
#define CMD_Error_Frame     0x28      //错误帧响应码
#define CMD_Update_Ready    0x29         //准备升级响应码
#define CMD_ExitOk          0x41         //退出升级响应码
#define CMD_Write_Error     0x42         //写入错误响应码



extern volatile unsigned int g_u32Seq;
extern volatile __align(4) u8  g_au8Rxbuffer[RX_DATA_MAX_LEN];

u8 iap_wire_app_hex(uint32_t u32addr, uint32_t u32len, uint8_t *pu8buf);
uint8_t BufferCmp(uint8_t *buf1,uint8_t *buf2,uint8_t size);
void Run_App_Check(void);
void iap_update(void);
#endif 

