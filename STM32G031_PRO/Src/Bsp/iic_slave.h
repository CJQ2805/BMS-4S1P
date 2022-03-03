#ifndef __IIC_H
#define __IIC_H
#include "bsp.h"

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;


/*CJQ2805  寄存器操作硬件IIC   参考地址:http://www.openedv.com/forum.php?mod=viewthread&tid=230248&extra= */

#define I2C_REG_TEST_SIZE                        10

//寄存器地址
#define I2C_REG_TEST_ADD                         (u8)0x02



/******************************************************************************
                              定义I2C总线操作结构体
******************************************************************************/
#define I2C_DEVICE_FLAG_DIR_SEND                 ((uint32_t)0x80000000)
#define I2C_DEVICE_FLAG_READ                     ((uint32_t)0x40000000)
#define I2C_DEVICE_FLAG_NACK                     ((uint32_t)0x20000000)
#define I2C_DEVICE_FLAG_WRITE                    ((uint32_t)0x10000000)
#define I2C_DEVICE_FLAG_ERROR                    ((uint32_t)0x08000000)


#define I2C_NOSTRETCH_EN                         1  //是否打开时钟延展	1是没打开时钟延展

#define I2C_DEVICE_DR_NULL_R                     0xFA
#define I2C_DEVICE_DR_NULL_S                     0xFB

typedef __packed struct _IIC_ISR_Register_t{
	
	u32  TXE        :   1;		
	u32  TXIS       :   1;	
	u32  RXNE       :   1;	
	u32  ADDR		:   1;
	u32  NACKF      :   1;	
	u32  STOPF      :   1;	
	u32  TC         :   1;	
	u32  TCR        :   1;
	u32  BERR		:   1;	
	u32  ARLO       :   1;
	u32  OVR        :   1;
	u32  PECERR     :   1;	
	u32  TIMEOUT    :   1;
	u32  ALERT      :   1;	
	u32  Reserved2  :   1;	
	u32  BUSY       :   1;	
	u32  DIR        :   1;
	u32  ADDCODE    :   7;	
	u32  Reserved1  :   8;

}t_IIC_ISR_Register;

typedef __packed union _IIC_ISR_Register_u{

	u32 u32val;
	t_IIC_ISR_Register  tIIC_ISR_Register;
	
}u_IIC_ISR_Register;


void MX_I2C1_Init(void);
void MX_I2C2_Init(void);

void IIC_DeInit(void);
#endif

