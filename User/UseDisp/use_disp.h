#ifndef __use_disp_H_
#define __use_disp_H_
#include  "stdint.h"
#include  "Globalvalue/globalvalue.h"
#define FIRSTLINE	26
#define	SPACE1		22
#define LIST1		0
#define LIST2		250
#define SELECT_1END 192
#define SELECT_2END (410+32)
extern const uint32_t FreqNum[];
extern const uint8_t Cp_Button_Tip[][7+1];
extern const uint8_t Cs_Button_Tip[][7+1];
extern const uint8_t Lp_Button_Tip1[][7+1];
extern const uint8_t Ls_Button_Tip[][7+1];
extern const uint8_t Z_Button_Tip[][7+1];
extern const uint8_t Y_Button_Tip[][7+1];
extern const uint8_t R_Button_Tip[][7+1];
void Disp_Fastbutton(void);
void Disp_MidRect(void);
void Disp_TestScreen(void);

void Disp_Instruction(void);
void Disp_Test_Item(void);
void Disp_Range_Item(void);
void Disp_Range_Count_Item(void);
void Disp_List_Count_Item(void);
void Disp_Test_Set_Item(void);
void Disp_UserCheck_Item(void);
void Disp_LimitList_Item(void);
void Disp_ListScan_Item(void);
void Disp_Sys_Item(void);
void Disp_Fastbutton(void);
void Disp_Button_value1(uint32_t value);
void Disp_Button_TestSet(uint32_t value);
void DispSet_value(Button_Page_Typedef* Button_Page);
void Disp_Test_value(Button_Page_Typedef* Button_Page);
void Disp_RangeDispValue(Button_Page_Typedef *Button_Page);
void Disp_Debug_value(Button_Page_Typedef* Button_Page);
void Disp_Sys_value(Button_Page_Typedef* Button_Page);
void Disp_button_Num_time(void);
void Disp_button_Num_Freq(void);
Sort_TypeDef Time_Set_Cov(Sort_TypeDef *Time);
Sort_TypeDef Freq_Set_Cov(Sort_TypeDef *Freq);
void Disp_button_Num_Avg(void);
void Disp_button_Num_Input(vu8 page);
Sort_TypeDef Input_Set_Cov(Sort_TypeDef *Input_Ref);
void Disp_Cp(void);
void Disp_Button_Fun_Set(vu16 xpos,vu16 ypos,vu8 * Disp_Item,Button_Page_Typedef* Button_Page);
void Disp_LimitSEt_value(Button_Page_Typedef* Button_Page);
void Disp_Cp_D(void);
void Disp_Cp_Q(void);
void Disp_Cp_G(void);
void Disp_Cp_Rp(void);
void Disp_Cs_Rs(void);
void Disp_Cs_D(void);
void Disp_Cs_Q(void);
void Disp_Lp_Q(void);
void Disp_Lp_Rp(void);
void Disp_Lp_Rd(void);
void Disp_Lp_D(void);
void Disp_Lp_G(void);
void Disp_Ls_D(void);
void Disp_Ls_Q(void);
void Disp_Ls_Rs(void);
void Disp_Ls_Rd(void);
void Disp_Z_d(void);
void Disp_Z_r(void);
void Disp_Y_d(void);
void Disp_Y_r(void);
void Disp_R_X(void);
void Disp_Rs_Q(void);
void Disp_Rp_Q(void);
void Disp_Testvalue(int8_t chosen ,int32_t eee);//显示测试数据
void Disp_Big_MainUnit(vu8 unit,vu8 unit1);
void Disp_Big_SecondUnit(vu8 unit,vu8 unit1);
void Send_Freq(Send_Ord_Typedef *ord);
void Send_Main_Ord(void);
//测试时档号显示界面的显示
void Disp_Range_ComDispValue(Button_Page_Typedef *Button_Page);
void Disp_RangeDispvalue(vu8 data);
void Disp_Range_DispMainUnit(vu8 num,vu8 unit);
void Disp_Range_DispSecondUnit(vu8 num,vu8 unit);
void Disp_RangeTestvalue(void);
void Disp_RangeCount(void);
void Disp_LIMIT_ComDispValue(Button_Page_Typedef *Button_Page);
void Disp_Scan_Compvalue(vu8 set);
void Disp_Scan_SetCompvalue(Button_Page_Typedef* Button_Page);
void Disp_Button_ItemScanSet(uint32_t value);
void Disp_Button_ItemScan_no(void);
void Send_Request(void);
void Send_Uart3(uint8_t *buff);
void Disp_Set_Unit_12(uint32_t unit,uint32_t x,uint32_t y);
void Disp_Set_Unit_16(uint32_t unit,uint32_t x,uint32_t y);
Sort_TypeDef Input_Set_CovPre(Sort_TypeDef *Input_Ref);
void Disp_Test_Unit_v(uint32_t x,uint32_t y);
void Disp_Test_Unit_mA(uint32_t x,uint32_t y);
void Send_UartStart(void);
void Savetoeeprom(void);
void ReadSavedata(void);
void Disp_G_B(void);
void SetData_High_Low_Comp(void);
void Disp_Sys(void);
void Disp_Sys_Item1(void);
void Disp_Debug_Reference(Button_Page_Typedef* Button_Page,float ddd,float eee);
int16_t Debug_Value(Sort_TypeDef *Debug_value);
void Disp_Usbflag(uint8_t flag);
#endif
