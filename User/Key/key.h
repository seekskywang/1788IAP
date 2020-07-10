#ifndef __Key_H_
#define __Key_H_
typedef unsigned char u8;
typedef unsigned           int u32;
extern volatile unsigned long SysTickCnt;
#define 	Key_F1  	(9)
#define		Key_F2  	(17)
#define 	Key_F3  	(25)
#define 	Key_F4  	(1)
#define		Key_F5 		(10)
#define		Key_Disp 	 (19)//9
#define		Key_SETUP	 (11)
#define		Key_FAST	(26)
#define		Key_LEFT	(27)
#define		Key_RIGHT  	(3)
#define		Key_UP		(18)
#define		Key_DOWN	(2)
#define		Key_NUM7	(14)
#define		Key_NUM8	(13)
#define		Key_NUM9	(12)
#define		Key_NUM4	(22)
#define		Key_NUM5	(21)
#define		Key_NUM6	(20)
#define		Key_NUM1	(30)
#define		Key_NUM2	(29)
#define		Key_NUM3	(28)
#define		Key_NUM0	(6)
#define		Key_DOT		(5)
#define		Key_BACK	(4)
#define		Key_LOCK	(15)
#define		Key_BIAS	(23)
#define		Key_REST	(31)
#define		Key_TRIG	(7)
#define		Key_POWER	(55)
#define _Hc164CpL GPIO_ClearValue(1, (1<<1))
#define _Hc164CpH GPIO_SetValue(1, (1<<1))
#define _Hc164DL GPIO_ClearValue(1, (1<<10))
#define _Hc164DH GPIO_SetValue(1, (1<<10))
void HW_Sendvalueto164(u8 value);
void HW_keyInt(void);
u8 HW_KeyScsn(void);
void Delay_Key(void);
u8 HW_KeyScsn1(void);
void SysTick_Handler (void);
void Delay (unsigned long tick);
void GPIO_Led_Configuration(void);

void Beep_on(void);
void Beep_Off(void);
void Pass_Led(void);
void Fail_led(void);

void Lock_LedOn(void);
void Lock_LedOff(void);
void Lock_Control_Off(void);
void Lock_Control_On(void);

void Bais_LedOff(void);
void Bais_LedOn(void);

void Beep_Off(void);
void Beep_on(void);

void Power_Off_led(void);
void Power_On_led(void);
void All_LedOff(void);
	
void Turnoff_backlight(void);
void Turnon_backlight(void);
void Key_Beep(void);
void GPIO_Plc_Configuration(void);
void Power_Relay(unsigned char flag);

void No_Comp(void);
#endif

