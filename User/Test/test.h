#ifndef __test_h
#define __test_h
#include "stdint.h"
#include  "Globalvalue/globalvalue.h"
#define  MAX_BUFFER_SIZE             (256)/*(4000)*/
#define  WRITE_SIZE          (10 * 1000000)
#define  FILENAME_b  "jk2520b.TXT"
extern const vu8 Disp_Main_Ord[][3];

#define  CmdNumb         17
#define  CmdLen          12 

#define ChrStartR      0x3E
#define ChrStartS      0x3C

#define ChrEndR    	   0x0D
#define ChrEndS    	   0x0A

#define PARA_ERR       1
#define CMD_ERR        2
#define REM_ERR        3


void Power_Process(void);


//测试程序
void Test_Process(void);

//文件管理程序
void File_Process(void);

//参数设置程序
void Setup_Process(void);
//数据保存程序
void Data_StoreProcess(void) ;
//档号显示
void Range_Process(void);
//档计数显示
void Range_CountProcess(void);
 // 列表显示
void ItemProcess(void);
//用户校正
void Use_DebugProcess(void) ;
unsigned char Uart_Process(void);
void Fac_DebugProcess(void);
void Sys_Process(void);//系统信息
void Use_SysSetProcess(void);//系统设置
void Use_LimitSetProcess(void);//极限列表设置
void Use_ITEMSetProcess(void);//列表扫描设置
int8_t PackStandFrame(int8_t * framebuf, int8_t * datbuf, int8_t len);
Sort_TypeDef Disp_NumKeyboard_Set(Disp_Coordinates_Typedef *Disp_Coordinates);
Sort_TypeDef Disp_Set_Num(Disp_Coordinates_Typedef *Coordinates);
vu16 Freq_Set_Num(Disp_Coordinates_Typedef *Coordinates);
vu8 Avg_Set_Num(Disp_Coordinates_Typedef *Coordinates);//平均数设置
Sort_TypeDef Disp_Set_InputNum(Disp_Coordinates_Typedef *Coordinates);
void Set_daot(vu8 *buff,vu8 dot);
Sort_TypeDef Disp_Set_InputpreNum(Disp_Coordinates_Typedef *Coordinates);
void Set_Compbcd_float(void);
void Test_Comp(All_Compvalue_Typedef *pt);
Sort_TypeDef Disp_Set_CompNum(Disp_Coordinates_Typedef *Coordinates);
int32_t BCDtoInt(int8_t *pt);
int32_t VBCDtoInt(int8_t *pt);
int8_t V_Test_Comp(int32_t value);//电压比较
int8_t R_Test_Comp(int32_t value);//电阻比较
void Comp_prompt(int8_t value);//比较提示
void BCD_Int(int32_t data);
float Debug_Res(float a,float b,float c);
void V_BCD_Int(int32_t data);
int32_t Input_int(int8_t *pt);
void  Write_Usbdata (uint8_t  *buffer,uint32_t num );
void Soft_Turnon(void);//软键开关机
void input_password(Disp_Coordinates_Typedef *Coordinates );

void input_num(Disp_Coordinates_Typedef *Coordinates );
extern uint32_t keynum;

//void Send_Main_Ord(void);
#endif
