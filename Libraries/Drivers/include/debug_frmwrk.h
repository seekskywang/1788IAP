/**********************************************************************
* $Id$		debug_frmwrk.h			2011-06-02
*//**
* @file		debug_frmwrk.h
* @brief	Contains some utilities that used for debugging through UART
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
* 
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
**********************************************************************/

#ifndef __DEBUG_FRMWRK_H_
#define __DEBUG_FRMWRK_H_
#include "Globalvalue/GlobalValue.h"
#include "lpc177x_8x_uart.h"

#define USED_UART_DEBUG_PORT	0

#define NUM_SKIPPED_ALLOWED			(10)

#define DBG_GETVAL_IN_DEC			(0)
#define DBG_GETVAL_IN_HEX			(1)
//#define DBG_GETVAL_IN_CHARS			(2) 

#if (USED_UART_DEBUG_PORT == 0)
#define DEBUG_UART_PORT	(LPC_UART_TypeDef *)LPC_UART0
#elif (USED_UART_DEBUG_PORT == 1)
#define DEBUG_UART_PORT	(LPC_UART_TypeDef *)LPC_UART1
#elif (USED_UART_DEBUG_PORT == 2)
#define DEBUG_UART_PORT	(LPC_UART_TypeDef *)LPC_UART2
#elif (USED_UART_DEBUG_PORT == 3)
#define DEBUG_UART_PORT	(LPC_UART_TypeDef *)LPC_UART3
#elif (USED_UART_DEBUG_PORT == 4)
#define DEBUG_UART_PORT	(LPC_UART_TypeDef *)LPC_UART4
#else
 #error "Invalid UART port selection!"
#endif

#define _DBG(x)								_db_msg(DEBUG_UART_PORT, x)
#define _DBG_(x)							_db_msg_(DEBUG_UART_PORT, x)

#define _DBC(x)								_db_char(DEBUG_UART_PORT, x)

#define _DBD(x)								_db_dec(DEBUG_UART_PORT, x)
#define _DBD16(x)							_db_dec_16(DEBUG_UART_PORT, x)
#define _DBD32(x)							_db_dec_32(DEBUG_UART_PORT, x)

#define _DBH(x)								_db_hex(DEBUG_UART_PORT, x)
#define _DBH16(x)							_db_hex_16(DEBUG_UART_PORT, x)
#define _DBH32(x)							_db_hex_32(DEBUG_UART_PORT, x)

#define _DBH_(x)							_db_hex_(DEBUG_UART_PORT, x)
#define _DBH16_(x)							_db_hex_16_(DEBUG_UART_PORT, x)
#define _DBH32_(x)							_db_hex_32_(DEBUG_UART_PORT, x)

#define _DG									_db_get_char(DEBUG_UART_PORT)
#define _DGV(option, numCh, val)			_db_get_val(DEBUG_UART_PORT, option, numCh, val)


//==========================================================
//调试配置
#define _HW_CONFIG_FALSE	(0)
#define _HW_CONFIG_TRUE		(1)

//==========================================================
//收发帧缓冲长度
#define REC_LEN_MAX  	(60)//接收帧长度
#define SEND_LEN_MAX 	(18)//发送帧长度

////串口发送结构
typedef struct
{
	int8_t begin;//发送开始标志
	int8_t ptr;//当前发送指针
	int8_t len;//发送数据长度
	int8_t buf[SEND_LEN_MAX];//数据发送缓冲
}ComSend_TypeDef;
#define TICKS_PER_SEC 100	//系统时钟每秒节拍数，100x10mS(T0)=1S
//接收溢出时间,3*1000/50=60mS
#define REC_TIME_OUT	 (3*TICKS_PER_SEC/50)
//串口接收结构
typedef struct
{
	int8_t end;//接收结束标志
	int8_t ptr;	//当前接收指针
	int8_t len;//接收数据长度
	int8_t buf[REC_LEN_MAX];//数据接收缓冲
	int8_t TimeOut;//接收超时(10ms周期)
}ComRec_TypeDef;

////串口收发结构
typedef struct
{
	ComSend_TypeDef send;
	ComRec_TypeDef  rec;
}Com_TypeDef;
extern Com_TypeDef ComBuf;//串口收发缓冲
extern Com_TypeDef ComBuf3;
//==========================================================
//通信协议，8位，9600波特率
//==========================================================
//起始符  总长度  目标地址  命令字  数据包  累加和  结束符
//1字节	  1字节	  1字节	    1字节	N字节   1字节   1字节
//==========================================================
#define UART_SEND_BEGIN 		(0xAB)
#define UART_SEND_END 			(0xAF)
#define FRAME_LEN_MAX 			(REC_LEN_MAX)//一帧最大字节
#define FRAME_LEN_MIN 			(3)	//一帧最小字节
#define UART_REC_BEGIN 			(0xAB)//AA
#define UART_REC_END 			(0xAF)//AF

//帧类型定义
#define FRAME_READ_RESULT		(0x00)	//读取结果
#define FRAME_START				(0x01)	//启动
#define FRAME_RESET				(0x02)	//复位
#define FRAME_DATA				(0xF1)	//数据帧
#define FRAME_CLR_BOOT_NUM		(0xF2)	//清开机次数帧
#define FRAME_WRITE_SN			(0xF3)	//写序列号帧

//帧位置定义
#define PFRAMEHEAD				(0)	//帧头位置
#define PFRAMEKIND				(1)	//帧类型位置
#define PDATASTART				(2)	//数据包首地址位置

//通信状态定义
#define COM_SUCCESS				(0x00)	//成功
#define COM_ERR_BUSY	 	 	(0x01)	//信道忙
#define COM_ERR_CRC				(0x02)	//校验错误
#define COM_ERR_NACK 			(0x03)	//无应答
#define COM_ERR_UNEXPECT 		(0x04)	//非期望帧
#define COM_ERR_FORMAT			(0x05)	//数据格式错误
#define COM_ERR_SAVE 			(0x06)	//无法保存数据
#define COM_ERR_ADDR_FIRE	 	(0x07)	//地址冲突
#define COM_ERR_UNDEFINE		(0x08)	//未定义错误
//void  _printf (const  char *format, ...);
#define SetRecTimeOut(time) (ComBuf.rec.TimeOut=time)


extern const vu8 READDATA[7];
extern void (*_db_msg)(LPC_UART_TypeDef *UARTx, const void *s);
extern void (*_db_msg_)(LPC_UART_TypeDef *UARTx, const void *s);
extern void (*_db_char)(LPC_UART_TypeDef *UARTx, uint8_t ch);
extern void (*_db_dec)(LPC_UART_TypeDef *UARTx, uint8_t decn);
extern void (*_db_dec_16)(LPC_UART_TypeDef *UARTx, uint16_t decn);
extern void (*_db_dec_32)(LPC_UART_TypeDef *UARTx, uint32_t decn);
extern void (*_db_hex)(LPC_UART_TypeDef *UARTx, uint8_t hexn);
extern void (*_db_hex_16)(LPC_UART_TypeDef *UARTx, uint16_t hexn);
extern void (*_db_hex_32)(LPC_UART_TypeDef *UARTx, uint32_t hexn);
extern void (*_db_hex_)(LPC_UART_TypeDef *UARTx, uint8_t hexn);
extern void (*_db_hex_16_)(LPC_UART_TypeDef *UARTx, uint16_t hexn);
extern void (*_db_hex_32_)(LPC_UART_TypeDef *UARTx, uint32_t hexn);

extern uint8_t (*_db_get_char)(LPC_UART_TypeDef *UARTx);
extern uint8_t (*_db_get_val)(LPC_UART_TypeDef *UARTx, uint8_t option, uint8_t numCh, uint32_t * val);

uint8_t UARTGetValue (LPC_UART_TypeDef *UARTx, uint8_t option,
											uint8_t numCh, uint32_t* val);
void UARTPutChar (LPC_UART_TypeDef *UARTx, uint8_t ch);
extern void UARTPuts(LPC_UART_TypeDef *UARTx, const void *str);
void UARTPuts_(LPC_UART_TypeDef *UARTx, const void *str);
void UARTPutDec(LPC_UART_TypeDef *UARTx, uint8_t decnum);
void UARTPutDec16(LPC_UART_TypeDef *UARTx, uint16_t decnum);
void UARTPutDec32(LPC_UART_TypeDef *UARTx, uint32_t decnum);
void UARTPutHex (LPC_UART_TypeDef *UARTx, uint8_t hexnum);
void UARTPutHex16 (LPC_UART_TypeDef *UARTx, uint16_t hexnum);
void UARTPutHex32 (LPC_UART_TypeDef *UARTx, uint32_t hexnum);
uint8_t UARTGetChar (LPC_UART_TypeDef *UARTx);
//void  _printf (const  char *format, ...);
void debug_frmwrk_init(void);
void Uart3_init(uint32_t freq);
//void Send_Freq(Send_Ord_Typedef *ord);

#endif /* __DEBUG_FRMWRK_H_ */
