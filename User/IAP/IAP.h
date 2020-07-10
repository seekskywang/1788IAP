#ifndef __IAP_H
#define	__IAP_H

//#include "config.h"
#include "lpc177x_8x.h"
#define     IAP_START_ADDR  0x00000000                                  // IAP开始地址
#define     IAP_LOCATION     0x1FFF1FF1                                   /* IAP升级的入口地址定义              */
/* The area will be erase and program */
#define APP_START_ADDR						0x018000//0x00001000UL                //4k  用户程序起始地址
#define APP_END_ADDR						  0x80000                //LPC1788 512K Flash          
//#define APP_SIZE						 0x10000  
/* 
 * Define the flash sectors used by the application 
 */
#define APP_START_SECTOR					17
#define APP_END_SECTOR						29                   // LPC1788  512K Flash扇区
typedef enum {
  IAP_WRITE_256  = 256,
  IAP_WRITE_512  = 512,
  IAP_WRITE_1024 = 1024,
  IAP_WRITE_4096 = 4096,
} IAP_WRITE_SIZE;

/* 定义IAP命令字 */
//											   命令		  参数
#define		IAP_SELECTOR		  50		  // 选择扇区		【起始扇区号、结束扇区号】
#define		IAP_RAMTOFLASH		51		  // 拷贝数据		【FLASH目标地址、RAM源地址、写入字节数、系统时钟频率】		
#define		IAP_ERASESECTOR		52		  // 擦除扇区		【起始扇区号、结束扇区号、系统时钟频率】
#define		IAP_BLANKCHK		  53		  // 查空扇区		【起始扇区号、结束扇区号】
#define		IAP_READPARTID		54		  // 读器件ID		【无】
#define		IAP_BOOTCODEID		55		  // 读Boot版本号	【无】
#define		IAP_COMPARE			  56		  // 比较命令		【FLASH起始地址、RAM起始地址、需要比较的字节数】
#define		IAP_REINVOKE_ISP	57		  // 重新调用ISP	【无】

/* 定义IAP返回状态字 */
#define		CMD_SUCCESS			0
#define		INVALID_COMMAND		1
#define		SRC_ADDR_ERROR		2
#define		DST_ADDR_ERROR		3
#define		DST_ADDR_NOT_MAPPED	5	 
#define		SRC_ADDR_NOT_MAPPED	4
#define		COUNT_ERROR			7
#define		SECTOR_NOT_BLANK	8
#define		SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION		9
#define		COMMPARE_ERROR		10
#define		BUSY				11

#define IAP_FLASH_PAGE_SIZE_BYTES							256
#define IAP_FLASH_PAGE_SIZE_WORDS							(IAP_FLASH_PAGE_SIZE_BYTES >> 2)



/*********************************************************************************************************
**函数名称:  SelSector
**函数功能:  IAP操作缓冲区选择，代码为50
**入口参数:  sec1	起始扇区
**			 sec2	终止扇区
**出口参数:  IAP 操作状态码
**			 IAP返回值（paramout缓冲区）
********************************************************************************************************/
extern	uint32_t	SelSector(uint8_t	sec1,uint8_t	sec2);


/*********************************************************************************************************
**函数名称:  RamToFlash 
**函数功能:  复制RAM的数据到FLASH，命令代码51
**入口参数:  dst		目标地址，即FLASH起始地址，以512字节为分界
**			 src		源地址，即RAM地址，地址必须字对其
**			 no		    复制字节个数，为256/512/1024/4096
**出口参数:  IAP 操作状态码
**			 IAP返回值（paramout缓冲区）
********************************************************************************************************/
extern	uint32_t	RamToFlash(uint32_t	dst, uint32_t	src, uint32_t	no);


/*********************************************************************************************************
**函数名称:  RamToFlash
**函数功能:  擦除扇区，命令代码52
**入口参数:  sec1	起始扇区
**			 sec2	终止扇区
**出口参数:  IAP 操作状态码
**			 IAP返回值（paramout缓冲区）
********************************************************************************************************/
extern	uint32_t	EraseSector(uint32_t	sec1, uint32_t	sec2);


/*********************************************************************************************************
**函数名称:  BlankCHK
**函数功能:  查空扇区，命令代码53
**入口参数:  sec1	起始扇区
**			 sec2	终止扇区
**出口参数:  IAP 操作状态码
**			 IAP返回值（paramout缓冲区）
********************************************************************************************************/
extern	uint32_t	BlankCHK(uint32_t	sec1,uint32_t	sec2);


/*********************************************************************************************************
**函数名称:  ReadParID
**函数功能:  读器件ID，命令代码54
**入口参数:  器件ID地址指针
**出口参数:  IAP 操作状态码
**			 IAP返回值（paramout缓冲区）
********************************************************************************************************/
extern	uint32_t	ReadParID(uint32_t *Device_ID);


/*********************************************************************************************************
**函数名称:  BootCodeID
**函数功能:  读boot代码ID，命令代码55
**入口参数:  boot代码ID地址指针
**出口参数:  IAP 操作状态码
**			 IAP返回值（paramout缓冲区）
********************************************************************************************************/
extern	uint32_t	BootCodeID(uint32_t *Boot_ID);
	

/*********************************************************************************************************
**函数名称:  Compare 
**函数功能： 校验数据，命令代码56
**入口参数:  dst		目标地址，即RAM/FLASH起始地址，地址必须字对齐
**			 src		源地址，即RAM/RAM地址，地址必须字对齐
**			 no		    复制字节个数，必须能被4整除
**出口参数:  IAP 操作状态码
**			 IAP返回值（paramout缓冲区）
********************************************************************************************************/
extern	uint32_t	Compare(uint32_t	dst, uint32_t	src, uint32_t	no);


/*********************************************************************************************************
**函数名称: Reinvoke_ISP 
**函数功能: 重新调用ISP，命令代码57。
**入口参数: 无
**出口参数: 无
********************************************************************************************************/
extern void  Reinvoke_ISP(void);


/*********************************************************************************************************
**函数名称:  WriteFlash
**函数功能:  向FLASH中写入数据
**入口参数:  dst		目标地址，即FLASH起始地址，地址必须以256字节对齐，即地址的低8位必须为0
**			 src		源地址，即RAM地址，地址必须字对齐，即地址的低两位必须为0
**			 no		    写入字节数，必须为256/512/1024/4096
**出口参数:  Compare的返回值，操作成功正确返回0
********************************************************************************************************/
extern	uint8_t	WriteFlash(uint32_t dst, uint32_t src, uint32_t no);
#endif
