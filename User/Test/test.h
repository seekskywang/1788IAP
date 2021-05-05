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


//���Գ���
void Test_Process(void);

//�ļ��������
void File_Process(void);

//�������ó���
void Setup_Process(void);
//���ݱ������
void Data_StoreProcess(void) ;
//������ʾ
void Range_Process(void);
//��������ʾ
void Range_CountProcess(void);
 // �б���ʾ
void ItemProcess(void);
//�û�У��
void Use_DebugProcess(void) ;
unsigned char Uart_Process(void);
void Fac_DebugProcess(void);
void Sys_Process(void);//ϵͳ��Ϣ
void Use_SysSetProcess(void);//ϵͳ����
void Use_LimitSetProcess(void);//�����б�����
void Use_ITEMSetProcess(void);//�б�ɨ������
int8_t PackStandFrame(int8_t * framebuf, int8_t * datbuf, int8_t len);
Sort_TypeDef Disp_NumKeyboard_Set(Disp_Coordinates_Typedef *Disp_Coordinates);
Sort_TypeDef Disp_Set_Num(Disp_Coordinates_Typedef *Coordinates);
vu16 Freq_Set_Num(Disp_Coordinates_Typedef *Coordinates);
vu8 Avg_Set_Num(Disp_Coordinates_Typedef *Coordinates);//ƽ��������
Sort_TypeDef Disp_Set_InputNum(Disp_Coordinates_Typedef *Coordinates);
void Set_daot(vu8 *buff,vu8 dot);
Sort_TypeDef Disp_Set_InputpreNum(Disp_Coordinates_Typedef *Coordinates);
void Set_Compbcd_float(void);
void Test_Comp(All_Compvalue_Typedef *pt);
Sort_TypeDef Disp_Set_CompNum(Disp_Coordinates_Typedef *Coordinates);
int32_t BCDtoInt(int8_t *pt);
int32_t VBCDtoInt(int8_t *pt);
int8_t V_Test_Comp(int32_t value);//��ѹ�Ƚ�
int8_t R_Test_Comp(int32_t value);//����Ƚ�
void Comp_prompt(int8_t value);//�Ƚ���ʾ
void BCD_Int(int32_t data);
float Debug_Res(float a,float b,float c);
void V_BCD_Int(int32_t data);
int32_t Input_int(int8_t *pt);
void  Write_Usbdata (uint8_t  *buffer,uint32_t num );
void Soft_Turnon(void);//������ػ�
void input_password(Disp_Coordinates_Typedef *Coordinates );

void input_num(Disp_Coordinates_Typedef *Coordinates );
extern uint32_t keynum;

//void Send_Main_Ord(void);
#endif
