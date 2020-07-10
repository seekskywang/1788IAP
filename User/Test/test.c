#include "test.h"
#include "LCD/lpc177x_8x_lcd.h"
#include "Globalvalue/GlobalValue.h"
#include "use_disp.h"
#include "EX_SDRAM/EX_SDRAM.H"
#include "stdint.h"
#include "key/key.h"
#include <string.h>
#include "timer.h"
#include "lpc177x_8x_eeprom.h"
#include "math.h"
#include  "usbhost_fat.h"
#include "lpc177x_8x_clkpwr.h"
#include "LCD/logo.h"
#include "lpc177x_8x_rtc.h"
#include "debug_frmwrk.h"
#include "lpc177x_8x_gpio.h"
#include "open.h"
#include "IAP.h"
const char *PASSWORD="12345689";
const uint8_t USB_dISPVALUE[][9]=
{
	"RH_FAIL ",
	"RL_FAIL ",
	"VH_FAIL ",
	"VL_FAIL ",
	"ALL_FAIL",
	"ALL_PASS"
};
struct MODS_T g_tModS;
extern volatile uint32_t timer1_counter;
extern volatile uint32_t stable_counter;
extern vu8 vflag;
vu8 nodisp_v_flag=0;
vu16 stabletime;
vu32 rwatch;
vu32 vwatch;
vu32 testwatch[500];
vu8 u3sendflag;
vu32 Tick_10ms=0;
vu32 OldTick;
vu8 g_mods_timeout;
vu8 powerontest;
extern vu8 u3sendflag;
extern vu8 negvalm;
uint32_t keynum;
uint32_t returnwatch;
uint8_t upflag;
const u8 DOT_POS[6]=
{	
	2,
	1,
	3,
	2,
	0
};
//#include "debug_frmwrk.h"
const vu8 Uart_Ordel[]={0x60,0x70,0x71,0x80,0x90,0xa0,0xb0,0xc0,0xe0};
const vu8 READDATA[7]={0xAB,0x01,0x06,0x03,0x08,0xbf,'\0'};
const vu8 Disp_Main_Ord[][3]={
	{1,1,0},
	{1,1,1},
	{1,1,2},
	{1,1,3},//Cp
	
	{0,1,0},
	{0,1,1},
	{0,1,3},//Cs
	{1,0,0},
	{1,0,0},
	{1,0,0},
	{1,0,1},//Lp
	{1,0,3},
	{1,0,4},
	{1,0,0},
	{0,0,0},//Ls
	{0,0,1},
	{0,0,5},
	{0,0,4},
	{0,3,6},//Z
	{0,3,7},
	{0,4,6},//Y
	{0,4,7},
	{0,2,8},//R
	{1,2,1},
	{0,2,1},
	{1,1,0},
	{1,5,9},//GB
	//{1,1,0},
	};
//==========================================================
//函数名称：Power_Process
//函数功能：上电处理
//入口参数：无
//出口参数：无
//创建日期：2015.10.26
//修改日期：2015.10.26 08:53
//备注说明：开机长按SET进入校准调试模式
//==========================================================
void MODS_Poll(void)
{
	uint16_t addr;
	static uint16_t crc1;
    static vu32 testi;
	/* 超过3.5个字符时间后执行MODH_RxTimeOut()函数。全局变量 g_rtu_timeout = 1; 通知主程序开始解砿*/
//	if (g_mods_timeout == 0)	
//	{
//		return;								/* 没有超时，继续接收。不要清雿g_tModS.RxCount */
//	}

    testi=g_tModS.RxCount;
    testi=g_tModS.RxCount;
    testi=g_tModS.RxCount;
	if(testi>7)				/* ??????С?4???????? */
	{
		testi=testi;
	}
	testi=g_tModS.RxCount;
    if(testi==8)				/* ??????С?4???????? */
	{
		testi=testi+1;
	}
	//??????ˇ???
	if(OldTick!=Tick_10ms)
  	{  
	  OldTick=Tick_10ms;
	   if(g_mods_timeout>0)
      { 
	    g_mods_timeout--;
      }
	  if(g_mods_timeout==0 && g_tModS.RxCount>0)   //??????
      { 
		// goto err_ret;
	
      }
      else if(g_mods_timeout==0 && g_tModS.RxCount==0) //?????
         return;
      else //????ì???
         return;
	}
	else   //???10msì?????
		return;
	//g_mods_timeout = 0;	 					/* ??? */

	if (g_tModS.RxCount < 4)				/* ??????С?4???????? */
	{
		goto err_ret;
	}

	/* ??CRCУ?? */
// 	crc1 = CRC16(g_tModS.RxBuf, g_tModS.RxCount);
// 	if (crc1 != 0)
// 	{
// 		goto err_ret;
// 	}

// 	/* ??? (1??é */
// 	addr = g_tModS.RxBuf[0];				/* ?1?? ?? */
// 	if (addr != SADDR485)		 			/* ???????????ˇ??? */
// 	{
// 		goto err_ret;
// 	}

	/* 分析应用层协访*/
//    if(g_tModS.RxBuf[2] == 0xA5)
//    {
//        UART_Action();
//    }else{
//        usartocflag = 1;
        u3sendflag = 1;
        RecHandle();
        u3sendflag = 0;
//    }
							
	
err_ret:
#if 0										/* 此部分为了串口打印结枿实际运用中可不要 */
	g_tPrint.Rxlen = g_tModS.RxCount;
	memcpy(g_tPrint.RxBuf, g_tModS.RxBuf, g_tModS.RxCount);
#endif
	
 	g_tModS.RxCount = 0;					/* 必须清零计数器，方便下次帧同歿*/
}
void jumpboot(void)
{
//	 uint8_t tmp;

//	  /* Disable all interrupts */
//	  NVIC->ICER[0] = 0xFFFFFFFF;
//	  NVIC->ICER[1] = 0x00000001;
//	  /* Clear all pending interrupts */
//	  NVIC->ICPR[0] = 0xFFFFFFFF;
//	  NVIC->ICPR[1] = 0x00000001;

//	  /* Clear all interrupt priority */
//	  for (tmp = 0; tmp < 32; tmp++)
//	  {
//		NVIC->IP[tmp] = 0x00;
//	  }

//	  SCB->ICSR = 0x0A000000;
//	  SCB->VTOR = 0x00000000;
//	  SCB->AIRCR = 0x05FA0000;
//	  SCB->SCR = 0x00000000;
//	  SCB->CCR = 0x00000000;

//	  for (tmp = 0; tmp < 32; tmp++)
//	  {
//		SCB->SHP[tmp] = 0x00;
//	  }

//	  SCB->SHCSR = 0x00000000;
//	  SCB->CFSR = 0xFFFFFFFF;
//	  SCB->HFSR = 0xFFFFFFFF;
//	  SCB->DFSR = 0xFFFFFFFF;
//	  NVIC_DisableIRQ(MCI_IRQn);
//	  __disable_irq();
	  SCB->VTOR  =APP_START_ADDR & 0x1FFFFF80;
	  ExceuteApplication();
}
void  Bin_Read(void)
{
     static int32_t  fdr;
     uint32_t  bytes_read,writelen;
     static uint32_t dstaddr;
     returnwatch=SelSector(APP_START_SECTOR,APP_END_SECTOR);        //选择扇区
     returnwatch=EraseSector(APP_START_SECTOR,APP_END_SECTOR);                         
     returnwatch=BlankCHK(APP_START_SECTOR,APP_END_SECTOR);                            
     returnwatch=SelSector(APP_START_SECTOR,APP_END_SECTOR);    
//     PRINT_Log("\r\nstart file operations...\r\n");

   fdr = FILE_Open(FILENAME_R, RDONLY);
    if (fdr >0) {
//       PRINT_Log("Reading from %s...\n", FILENAME_R);
  for(writelen=0;writelen<(APP_END_ADDR-APP_START_ADDR)/1024;writelen++)
   {
//    bytes_read =FILE_Read(fdr, UserBuffer,MAX_BUFFER_SIZE);           
//    dstaddr=  (uint32_t)(APP_START_ADDR +(writelen)*1024);//dst address.  
//   SelSector(APP_START_SECTOR,APP_END_SECTOR);    
//   RamToFlash(dstaddr,(uint32_t)UserBuffer, 1024);
//   Compare(dstaddr, (uint32_t)UserBuffer, 1024);
	   
	   returnwatch =FILE_Read(fdr, UserBuffer,MAX_BUFFER_SIZE);           
       dstaddr=  (uint32_t)(APP_START_ADDR +(writelen)*1024);//dst address.  
	   returnwatch = SelSector(APP_START_SECTOR,APP_END_SECTOR);    
       returnwatch = RamToFlash(dstaddr,(uint32_t)UserBuffer, 1024);
       returnwatch = Compare(dstaddr, (uint32_t)UserBuffer, 1024);
   }  
  // printf("%x",writelen);
//  PRINT_Log("\r\n write filesuccessful\r\n");
//   SCB->VTOR  =APP_START_ADDR;                                             
   jumpboot();                                                      
   FILE_Close(fdr);                                                            
    } else{
//       PRINT_Log("\r\n write file failed\r\n");
    }
}   

void Power_Process(void)
{
	uint32_t  numBlks, blkSize;
	uint8_t  inquiryResult[INQUIRY_LENGTH],rc;
	vu16 i,key;
//	u8 buff[4096];
//	uint32_t  numBlks, blkSize;
//	uint8_t  inquiryResult[INQUIRY_LENGTH];
	Disp_Coordinates_Typedef Debug_Cood;
//	u8 rc;
//
	lcd_Clear(LCD_COLOR_TEST_BACK);
	Host_Init(); 
	rc = Host_EnumDev();       
	HW_keyInt();
    Beep_on();

	Beep_Off();
    i=0;
	powerontest = 1;
		
//UART_TxCmd(LPC_UART3, ENABLE);
	while(GetSystemStatus()==SYS_STATUS_POWER)
	{
        i++;
//        Delay(10);
//        if(i>5 && upflag == 0)
//		{
//            jumpboot();//待测状态
//		}else if(upflag == 1){
			Turnon_backlight();
			Disp_Instruction();
//		}
        key=HW_KeyScsn();
//		if(upflag == 1)
//		{
			Disp_Usbflag(usb_oenflag);
			
//		}
//		jumpboot();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{	
			//Disp_Flag=1;
			Key_Beep();
            switch(key)
			{
				case Key_FAST:
                    Bin_Read();
                break;
				case Key_F5:
				{
					 Host_Init();               /* Initialize the lpc17xx host controller                                    */
					 rc = Host_EnumDev();       /* Enumerate the device connected                                            */
					 if (rc == OK) 
					 {
						 /* Initialize the mass storage and scsi interfaces */
						 rc = MS_Init( &blkSize, &numBlks, inquiryResult );
						 if (rc == OK) {
						   	rc = FAT_Init();   /* Initialize the FAT16 file system */
							if (rc == OK)
							{
								usb_oenflag=1;  
							}								

						} 
					}else{
						usb_oenflag = 0;
					}						
				}break;
				case Key_SETUP:
				{					                                             
					upflag = 1;
				}break;
                default:
                    break;
                }
        }
//		i++;
//		if(i>POWERON_DISP_TIME)//延时20*100mS=2s后自动退出
			

//		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);//等待按键(100*10ms/10=100ms)
//		switch(key)
//		{
////			case KEY_SET:		//设置键
//			case L_KEY_SET:		//长按设置键
//				//SetSystemStatus(SYS_STATUS_DEBUG);//调试状态
//                
//				Beep_One();//蜂鸣器响一声
//				break;
//	
//			case KEY_UP:		//上键
//			case L_KEY_UP:		//长按上键
//				break;
//	
//			case KEY_DOWN:		//下键
//			case L_KEY_DOWN:	//长按下键
//				break;
//	
//			case KEY_LEFT:		//左键
//			case L_KEY_LEFT:	//长按左键
//				break;

//			case KEY_RIGHT:		//右键
//			case L_KEY_RIGHT:	//长按右键
//				break;

//			case KEY_ENTER:		//确认键
//			case L_KEY_ENTER:	//长按确认键
//				break;
//	
//			case KEY_START:		//启动键
//			case L_KEY_START:	//长按启动键
//				break;
//	
//			case KEY_RESET:		//复位键
//			case L_KEY_RESET:	//长按复位键
//				i=POWERON_DISP_TIME;//显示延时结束
//				break;
//			
//			default:
//				break;
//		}
		
		//Delay_1ms(50);
		//Range_Control(3);
	}
	
}

//测试程序
void Test_Process(void)
{

	static vu16 j;
	Button_Page_Typedef Button_Page;
//	Main_Second_TypeDed Main_Second;//主参数和幅参赛的序号
	vu32 keynum=0;
	float ddd,eee;
	vu8 key,i;

    vu8 timebuff[10];
    vu8 send_usbbuff[100];
	vu8 return_flag=0;
//	vu8 Usb_Sendbuff[30]={"       Ω"};
//    vu8 page=1;
	vu8 Disp_Flag=1;
//	vu8 uart_count;
	vu8 trip_flag=0;
	vu8 lock_flag=0;
//	vu8 rc;
	vu8 chosen=ALL_PASS,chosen1=ALL_PASS,comp=ALL_PASS;
	Send_Ord_Typedef Uart;
    vu8 sendtest[5] ={1,2,3,4,5};
    
    uint32_t  numBlks, blkSize;
	uint8_t  inquiryResult[INQUIRY_LENGTH],rc;
    GPIO_ClearInt(0, 1<<19);
    NVIC_EnableIRQ(GPIO_IRQn);
	Button_Page.page=0;
	Button_Page.index=0;
	Button_Page.third=0xff;
//     if(Save_Res.Sys_Setvalue.uart)
//         UART_TxCmd(LPC_UART3, ENABLE);
//     else
//         UART_TxCmd(LPC_UART3, DISABLE);
//     Uart3_init(Save_Res.Sys_Setvalue.buard);
    lcd_Clear(LCD_COLOR_TEST_BACK);
	Disp_Test_Item();
    //Button_Page.index=0;
//	Main_Second.Main_flag=0;
//	Main_Second.Second_falg=0;
	
	Delay_Key();
//	uart_count=0;
	clear_flag=0;
	Send_UartStart();//开始时的串口发送数据
	while(GetSystemStatus()==SYS_STATUS_TEST)
	{
        if(Rtc_intflag)
        {
            Rtc_intflag=0;
            Colour.Fword=LCD_COLOR_WHITE;
            Colour.black=LCD_COLOR_TEST_BACK;
            //sprintf((char *)timebuff,"%2d:%2d:%2d",RTC_TIME_DISP.HOUR,RTC_TIME_DISP.MIN,RTC_TIME_DISP.SEC);
            //Hex_Format(RTC_TIME_DISP.HOUR, 0, 2, 1);
            timebuff[0]=RTC_TIME_DISP.HOUR/10+'0';
            timebuff[1]=RTC_TIME_DISP.HOUR%10+'0';
            timebuff[2]=':';
            timebuff[3]=RTC_TIME_DISP.MIN/10+'0';
            timebuff[4]=RTC_TIME_DISP.MIN%10+'0';
            timebuff[5]=':';
            timebuff[6]=RTC_TIME_DISP.SEC/10+'0';
            timebuff[7]=RTC_TIME_DISP.SEC%10+'0';
            timebuff[8]=0;
            
            WriteString_16(LIST1+360, 2, timebuff,  0);
        }
        if(Disp_Flag==1)
		{
			Disp_Test_value(&Button_Page);
			
			Disp_Flag=0;
		
		}//
		//_printf("CoreClock: %s\n",READDATA); 
		
		Colour.black=LCD_COLOR_TEST_MID;
		
		if((Save_Res.Set_Data.trip==1&&trip_flag==1) || powerontest == 1)//单次触发
		{
			if(powerontest == 1)
			{
				delayMs(1,1000);
			}
			powerontest = 0;
			Send_Request();
			trip_flag=0;
			return_flag=1;
			delayMs(1,500);
		}
		Uart_Process();
		ddd=BCDtoInt((int8_t *)Test_Dispvalue.Main_valuebuff);//电阻
       
		eee=VBCDtoInt((int8_t *)Test_Dispvalue.Secondvaluebuff);//电压
		
        if(nodisp_v_flag)
            eee=0;
		if(timer0_counter>0)//请求数据
		{
			switch(Uart_Send_Flag)
			{
				case 0:
					if(Save_Res.Set_Data.trip==0 && u3sendflag ==0)
					{
						Send_Request();
						return_flag=1;
					}
					
					
					
					break;
				case 1:
					//if(uart_count++>5)
					Uart_Send_Flag=0;
				if(keynum<99)
				keynum++;
				else
					keynum++;
					Send_Main_Ord();
				
					break;
				case 2:
					
					//if(uart_count++>5)
					Uart_Send_Flag=0;
					Send_Freq(&Uart);
				
					break;
				default:
					//Send_Request();
					break;
			
			}
			
			
			timer0_counter=0;
		
		
		}
				//校正
		ddd=BCDtoInt((int8_t *)Test_Dispvalue.Main_valuebuff);//电阻
       
		eee=VBCDtoInt((int8_t *)Test_Dispvalue.Secondvaluebuff);//电压
		
		if(clear_flag)//清零
		{
			Bais_LedOff();
			clear_flag=0;
			if(Test_Dispvalue.Main_valuebuff[0]=='-')
			{
				Save_Res.clear=ddd;
				Save_Res.clear=-Save_Res.clear;
			}
			else
				Save_Res.clear=ddd;
			if(Test_Dispvalue.Main_valuebuff[0]=='-')
			{
				Save_Res.V_Clear=eee;
				Save_Res.V_Clear=-Save_Res.V_Clear;
				
			
			}
			else
				Save_Res.V_Clear=eee;
				
			Savetoeeprom();
		
		}else
		{
			if(!Test_Dispvalue.Rangedisp)
			{
				if(Test_Dispvalue.Main_valuebuff[0]=='-')
					ddd=0-ddd;
				ddd=fabs(ddd-Save_Res.clear);
				
				
			}
			if(Test_Unit.V_dot==3)
			{
				if(Test_Unit.V_Neg==0)//负
					eee=0-eee;
				eee=fabs(eee-Save_Res.V_Clear);
					
			
			
			}
		}
        
//         if(ddd==0xfffffff)
//            nodisp_v_flag=1;
//        else
//            nodisp_v_flag=0;
		
		//1125  1.171  8.84
		

		
		ddd=Debug_Res(ddd,Save_Res.Debug_Value[Test_Dispvalue.Rangedisp].standard,
		Save_Res.Debug_Value[Test_Dispvalue.Rangedisp].ad_value);
        if(ddd>32000000)
		{
           nodisp_v_flag=1;
			stable_counter = 0;
			j = 0;
        }else{
            nodisp_v_flag=0;
		}
		if(Test_Unit.V_dot==3)
			eee=Debug_Res(eee,Save_Res.Debug_Value[4].standard,Save_Res.Debug_Value[4].ad_value);
		else
			eee=Debug_Res(eee,Save_Res.Debug_Value[5].standard,Save_Res.Debug_Value[5].ad_value);
		
		if(nodisp_v_flag)
            eee=0;
			//ddd-=Save_Res.clear;
		//分选比较打开
		if(Save_Res.Set_Data.V_comp && Save_Res.Set_Data.dispvr != 2)//电压比较
		{
			chosen=V_Test_Comp(eee);
			
		
		
		}
		if(Save_Res.Set_Data.Res_comp && Save_Res.Set_Data.dispvr != 1)//电阻比较打开
		{
			chosen1=R_Test_Comp(ddd);
	//			if(R_Test_Comp(ddd)==ALL_PASS)
	//				;
		
		}
		if(chosen==ALL_PASS&&chosen1==ALL_PASS)
		{
			comp=ALL_PASS;
			//Pass_Led();
		} 
		else if(chosen!=ALL_PASS&&chosen1!=ALL_PASS)
		{
			comp=ALL_FAIL;
			//Fail_led();
		
		}
		else
		{
			if(chosen==VH_FAIL||chosen==VL_FAIL)
				comp=chosen;
			else if(chosen1==RH_FAIL||chosen1==RL_FAIL)
				comp=chosen1;
			//Fail_led();
				
		
		}
		#ifdef OVER_NO_ALARM
		if(nodisp_v_flag==0)
			Comp_prompt(comp);
		else
		{
			No_Comp();
		
		}
			
		#else
		if(Save_Res.Set_Data.V_comp || Save_Res.Set_Data.Res_comp || Save_Res.Set_Data.openbeep==1)
		{
			Comp_prompt(comp);
		}else{
			No_Comp();
		}
		#endif
		if(return_flag)	
		{
            rwatch = (int)ddd;
            vwatch = (int)eee * 10;
			testwatch[j] = rwatch;
			
			if(j < 499)
				j++;
			if(j == 5)
			{
				stabletime = stable_counter * 20;
			}
			BCD_Int(ddd);//DOT_POS	
			for(i=0;i<5;i++)
			{
				*(UserBuffer+i)=DispBuf[i];
				Send_buff2[i]=DispBuf[i];
			}
			if(Test_Unit.Res_dot)
			{
				*(UserBuffer+5)=' ';
				Send_buff2[5]=' ';
				
			}
			else
			{
				*(UserBuffer+5)='m';
				Send_buff2[5]='m';
				
			}
			*(UserBuffer+6)=0xa6;
			*(UserBuffer+7)=0xb8;
			*(UserBuffer+8)='	';
			Disp_R_X();//显示名称
			if(nodisp_v_flag == 1)
			{
				if(eee > 0 && eee < 200)
				{
					eee = 0;
					vflag = 1;
				}else if(eee <=0 && eee > -200){
					eee = 0;
					vflag = 1;
				}else{
					vflag = 0;
				}
			}else{
				vflag = 0;
			}
			Disp_Testvalue(comp,eee);
			
			if(Test_Unit.V_Neg)
			{
				*(UserBuffer+9)=' ';
				Send_buff2[6]=' ';
				
			}
			//WriteString_Big(100,92+55 ," ");
		else
		{
			*(UserBuffer+9)='-';
			Send_buff2[6]='-';
			
		}
			//WriteString_Big(100,92+55 ,"-");
		for(i=0;i<5;i++)
		{
			*(UserBuffer+10+i)=DispBuf[i];
			Send_buff2[7+i]=DispBuf[i];
			
		}
			Send_buff2[12]=comp;
			Send_buff2[13]=0;
			*(UserBuffer+15)='V';
			*(UserBuffer+16)='	';
			for(i=0;i<8;i++)
			*(UserBuffer+17+i)=USB_dISPVALUE[comp][i];
		
			*(UserBuffer+25)='\r';
			*(UserBuffer+26)='\n';
        *(UserBuffer+27)='\0';
			return_flag=0;
            
            strcpy((char *)send_usbbuff,(char *)timebuff);
            strcat((char *)send_usbbuff,(char *)"   ");
            strcat((char *)send_usbbuff,(char *)UserBuffer);
            if(Save_Res.Sys_Setvalue.u_flag)
            {
                Write_Usbdata ( send_usbbuff,38);//27
            }
            else
            {
                usb_oenflag=0;
            
            }
// 			if(Save_Res.Sys_Setvalue.uart)
// 				UARTPuts( LPC_UART3, sendtest);
	
		}
		Colour.Fword=LCD_COLOR_WHITE;
//		if(timer1_counter > 0)
//        {
            Tick_10ms ++;
            MODS_Poll();
//            timer1_counter = 0;
//        }
		//	Test_Comp(&Comp_Change);
        if(Button_Page.index==0)
        {
            if(usb_oenflag==1)
                {
                    //Write_Usbdata ( UserBuffer,27);
                    Disp_Usbflag(1);
                    
                }
                else
                    Disp_Usbflag(2);
            Colour.Fword=LCD_COLOR_WHITE;
            Colour.black=LCD_COLOR_TEST_BACK;
        }
		
	  key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{	
			Disp_Flag=1;
            Key_Beep();
			switch(key)
			{
				case Key_F1:
					
                    switch(Button_Page.index)
					{
						case 0:
							//if(Button_Page.page==0)
								SetSystemStatus(SYS_STATUS_TEST);
							break;
						case 1:
							Save_Res.Set_Data.trip=0;

							
							break;

						default:
							break;
					
					
					}
					Savetoeeprom();
				break;
				case Key_F2:
					switch(Button_Page.index)
					{
						case 0:
							//if(page==1)
								SetSystemStatus(SYS_STATUS_SETUPTEST);
							break;
						case 1:
							Save_Res.Set_Data.trip=1;

							
							break;

						
						default:
							break;
					
					
					}
					Savetoeeprom();
				break;
				case Key_F3:
					switch(Button_Page.index)
					{
						case 0:
							//if(page==1)
								SetSystemStatus(SYS_STATUS_SYSSET);
							break;

						case 6:
							
							break;
						
						default:
							break;
					
					
					}
//					Savetoeeprom();
				break;
				case Key_F4:
					switch(Button_Page.index)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SYS);
							break;
//						
						default:
							break;
					
					
					}	
//					Savetoeeprom();
				break;
				case Key_F5:
                    if(Save_Res.Sys_Setvalue.u_flag)
                    {
                        Host_Init();               /* Initialize the lpc17xx host controller                                    */
                        rc = Host_EnumDev();       /* Enumerate the device connected                                            */
                        if (rc == OK) {
                            /* Initialize the mass storage and scsi interfaces */
                            rc = MS_Init( &blkSize, &numBlks, inquiryResult );
                            if (rc == OK) {
                                rc = FAT_Init();   /* Initialize the FAT16 file system */   
														if(Save_Res.Sys_Setvalue.lanage )
															Write_Usbdata ( "Times   Resistance		Voltage		Sorting\r\n" ,27);	
														else
															Write_Usbdata ( "时间     电阻		电压		分选\r\n" ,27);	
                            usb_oenflag=1;                                
                    
                            } 
                        } 
                    }

				break;
				case Key_Disp:
                    //SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
                    //Savetoeeprom();
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
					if(Button_Page.index==0)
						Button_Page.index=1;
					else
					//if(Button_Page.index>3)
						Button_Page.index=0;
//					else
//						Button_Page.index+=2;
//					Button_Page.page=0;
				break;
				case Key_RIGHT:
					if(Button_Page.index==0)
						Button_Page.index=1;
					else
					//if(Button_Page.index<=3)
						Button_Page.index=0;
//					else
//						Button_Page.index-=2;
				//	Button_Page.page=0;
						
				break;
				case Key_DOWN:
					if(Button_Page.index>0)
						Button_Page.index=0;
					else
						Button_Page.index=1;
					//Button_Page.page=0;
					
				break;
				case Key_UP:
					if(Button_Page.index==0)
						Button_Page.index=1;
					else
						Button_Page.index=0;
					//Button_Page.page=0;
				break;
				
				case Key_NUM1:
				//break;
				case Key_NUM2:
				//break;
				case Key_NUM3:
				//break;
				case Key_NUM4:
				//break;
				case Key_NUM5:
				//break;
				case Key_NUM6:
				//break;
				case Key_NUM7:
				//break;
				case Key_NUM8:
				//break;
				case Key_NUM9:
				//break;
				case Key_NUM0:
					Uart_Send_Flag=1;
				break;
				case Key_DOT:
//					if(Button_Page.index==2)
//					{ 	Disp_Coordinates_Typedef Coordinates;
//						Coordinates.xpos=LIST1+88;
//						Coordinates.ypos=FIRSTLINE+SPACE1*1;
//						Coordinates.lenth=86;
//						SaveData.Main_Func.Freq=Freq_Set_Num(&Coordinates);
//						Uart.Ordel=Uart_Ordel[3];
//							Uart.name=SaveData.Main_Func.Freq;
//							Uart_Send_Flag=2;
//					
//					}
				break;
				case Key_BACK:
					
				break;
				case Key_LOCK:
					//clear_flag=1;
//					if(lock_flag)
//						lock_flag=0;
//					else
//						lock_flag=1;
//					if(lock_flag)
//					{
//						Lock_Control_On();
//						Lock_LedOn();
//						
//					}
//					else
//					{
//						Lock_LedOff();
//						Lock_Control_Off();
//					
//					
//					}
				break;
				case Key_BIAS:
					clear_flag=1;
					Bais_LedOn();
					Delay(500);
				break;
				case Key_REST:
					Power_Off_led();
				break;
				case Key_TRIG:
					trip_flag=1;
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}

}
//文件管理程序
void File_Process(void)
{
	 vu32 keynum=0;
	 vu8 key;
	Delay_Key();
  	while(GetSystemStatus()==SYS_STATUS_FILE)
	{
	 key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==2)
		{   
            Key_Beep();
			switch(key)
			{
				case Key_F1:
				break;
				case Key_F2:
				break;
				case Key_F3:
				break;
				case Key_F4:
				break;
				case Key_F5:
				break;
				case Key_Disp:
				break;
				case Key_SETUP:
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
				break;
				case Key_DOWN:
				break;
				case Key_NUM1:
				break;
				case Key_NUM2:
				break;
				case Key_NUM3:
				break;
				case Key_NUM4:
				break;
				case Key_NUM5:
				break;
				case Key_NUM6:
				break;
				case Key_NUM7:
				break;
				case Key_NUM8:
				break;
				case Key_NUM9:
				break;
				case Key_NUM0:
				break;
				case Key_DOT:
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	
	}

}
//参数设置程序
void Setup_Process(void)
{

	Button_Page_Typedef Button_Page;
	Disp_Coordinates_Typedef  Coordinates;
	Send_Ord_Typedef Uart;
	
	vu32 keynum=0;
	vu8 key;
//    vu8 page=1;
	vu8 Disp_Flag=1;
//	vu8 index=0;
//	vu32 *pt;
//	pt=(vu32 *)&SaveData.Main_Func;
	Button_Page.index=0;
	Button_Page.page=0;
    lcd_Clear(LCD_COLOR_TEST_BACK);
    Disp_Test_Set_Item();
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_SETUPTEST)
	{
	    
		if(Disp_Flag==1)
		{
			DispSet_value(&Button_Page);
			Disp_Flag=0;
			Delay_Key();
		
		}
		if(timer0_counter>0)//请求数据
		{
			switch(Uart_Send_Flag)
			{
				case 0:
					//Send_Request();
					break;
				case 1:
					Send_Main_Ord();
					break;
				case 2:
					Send_Freq(&Uart);
					Delay(100);
					break;
				default:
					Send_Request();
					break;
			
			}
			Uart_Send_Flag=0;
			
			timer0_counter=0;
		
		
		}
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==5)
		{	Disp_Flag=1;
            Key_Beep();
			switch(key)
			{
				case Key_F1:

					switch(Button_Page.index)
					{
						case 0:
							//if(Button_Page.page==0)
								SetSystemStatus(SYS_STATUS_TEST);//
//							else
//								SetSystemStatus(SYS_STATUS_FILE);
								
							break;
						case 1:
							Save_Res.Set_Data.trip=0;
							
							break;
						case 2:
							Save_Res.Set_Data.speed=0;
							
							Uart_Send_Flag=2;
								
							
							break;
						case 3:
							Save_Res.Set_Data.dispvr=0;
							
								
							
							break;
						case 3+1:
							Save_Res.Set_Data.Res_comp=0;
							Uart_Send_Flag=2;
							//SaveData.Main_Func.Level=0;
							break;
						case 4+1:
//							if(Save_Res.Set_Data.Res_low.Num<9000)
//								Save_Res.Set_Data.Res_low.Num+=1000;
//							else
//								Save_Res.Set_Data.Res_low.Num-=9000;
//							
							break;
						case 5+1:
							Save_Res.Set_Data.V_comp=0;
							Uart_Send_Flag=2;
							break;
						case 6+1:
							
//							if(Save_Res.Set_Data.V_low.Num<9000)
//								Save_Res.Set_Data.V_low.Num+=1000;
//							else
//								Save_Res.Set_Data.V_low.Num-=9000;
//							
							break;
						case 7+1:
							Save_Res.Set_Data.Range=0;
							Uart_Send_Flag=2;
							break;
						case 8+1:
							Save_Res.Set_Data.beep=0;
							break;
						case 9+1:
//							if(Save_Res.Set_Data.Nominal_Res.Num<9000)
//								Save_Res.Set_Data.Nominal_Res.Num+=1000;
//							else
//								Save_Res.Set_Data.Nominal_Res.Num-=9000;
//							Uart_Send_Flag=2;
							break;
						case 10+1:
//							if(Save_Res.Set_Data.High_Res.Num<9000)
//								Save_Res.Set_Data.High_Res.Num+=1000;
//							else
//								Save_Res.Set_Data.High_Res.Num-=9000;
//							Uart_Send_Flag=2;
							break;
						case 11+1:
//							if(Save_Res.Set_Data.Nominal_V.Num<9000)
//								Save_Res.Set_Data.Nominal_V.Num+=1000;
//							else
//								Save_Res.Set_Data.Nominal_V.Num-=9000;
//							Uart_Send_Flag=2;
							break;
						case 12+1:
//							if(Save_Res.Set_Data.V_high.Num<9000)
//								Save_Res.Set_Data.V_high.Num+=1000;
//							else
//								Save_Res.Set_Data.V_high.Num-=9000;
//							Uart_Send_Flag=2;
							
							break;
						
							
						default:
							break;
					
					
					}

				break;
				case Key_F2:
					

					switch(Button_Page.index)
					{
						case 0:
							//if(Button_Page.page==0)
								SetSystemStatus(SYS_STATUS_SETUPTEST);
//							else
//								SetSystemStatus(SYS_STATUS_SYSSET);
								
							break;
						case 1:
							
							Save_Res.Set_Data.trip=1;
							
							
							break;
						case 2:
							Save_Res.Set_Data.speed=1;
							
							Uart_Send_Flag=2;
							
							
							break;
						case 3:
							Save_Res.Set_Data.dispvr=1;
							
								
							
							break;
						case 3+1:
							Save_Res.Set_Data.Res_comp=1;
							Uart_Send_Flag=2;
							break;
						case 4+1:
//							if(Save_Res.Set_Data.Res_low.Num%1000<900)
//								Save_Res.Set_Data.Res_low.Num+=100;
//							else
//							{
//								//Save_Res.Set_Data.Res_low.Num%=100;
//								Save_Res.Set_Data.Res_low.Num-=900;
//								
//							}
							break;
						case 5+1:
							Save_Res.Set_Data.V_comp=1;
							Uart_Send_Flag=2;
							break;
						case 6+1:
//							if(Save_Res.Set_Data.V_low.Num%1000<900)
//								Save_Res.Set_Data.V_low.Num+=100;
//							else
//								Save_Res.Set_Data.V_low.Num-=900;
//							Uart_Send_Flag=2;
							break;
						case 7+1:
							Save_Res.Set_Data.Range=1;
							Uart_Send_Flag=2;
							break;
						case 8+1:
							Save_Res.Set_Data.beep=1;
							break;
//						case 9:
//							if(Save_Res.Set_Data.Nominal_Res.Num%1000<900)
//								Save_Res.Set_Data.Nominal_Res.Num+=100;
//							else
//								Save_Res.Set_Data.Nominal_Res.Num-=900;
//							Uart_Send_Flag=2;
//							break;
//						case 10:
//							if(Save_Res.Set_Data.High_Res.Num%1000<900)
//								Save_Res.Set_Data.High_Res.Num+=100;
//							else
//								Save_Res.Set_Data.High_Res.Num-=900;
//							Uart_Send_Flag=2;
//							break;
//						case 11:
//							if(Save_Res.Set_Data.Nominal_V.Num%1000<900)
//								Save_Res.Set_Data.Nominal_V.Num+=100;
//							else
//								Save_Res.Set_Data.Nominal_V.Num-=900;
//							Uart_Send_Flag=2;
//							break;
//						case 12:
//							if(Save_Res.Set_Data.V_high.Num%1000<900)
//								Save_Res.Set_Data.V_high.Num+=100;
//							else
//								Save_Res.Set_Data.V_high.Num-=900;
//							Uart_Send_Flag=2;
//							
//							

//							break;
						default:
							break;
					
					
					}				
				

				break;
				case Key_F3:
					switch(Button_Page.index)
					{
						case 0:
							//if(Button_Page.page==0)
								SetSystemStatus(SYS_STATUS_SYSSET);
//							else
//								SetSystemStatus(SYS_STATUS_TOOL);
							break;
						case 1:
							break;
						case 2:
							break;
						case 3:
							Save_Res.Set_Data.dispvr=2;
							
								
							
							break;
						case 3+1:
							break;
						case 4+1:
//							if(Save_Res.Set_Data.Res_low.Num%100<90)
//								Save_Res.Set_Data.Res_low.Num+=10;
//							else
//								Save_Res.Set_Data.Res_low.Num-=90;
//							Uart_Send_Flag=2;
							break;
						case 5+1:
							break;
						case 6+1:
//							if(Save_Res.Set_Data.V_low.Num%100<90)
//								Save_Res.Set_Data.V_low.Num+=10;
//							else
//								Save_Res.Set_Data.V_low.Num-=90;
//							Uart_Send_Flag=2;
							
							break;
						case 7+1:
							Save_Res.Set_Data.Range=2;
							Uart_Send_Flag=2;
							break;
						case 8+1:
							Save_Res.Set_Data.beep=2;
							break;
//						case 9:
//							if(Save_Res.Set_Data.Nominal_Res.Num%100<90)
//								Save_Res.Set_Data.Nominal_Res.Num+=10;
//							else
//								Save_Res.Set_Data.Nominal_Res.Num-=90;
//							Uart_Send_Flag=2;
//							break;
//						case 10://MAX_R_RANGE
//							if(Save_Res.Set_Data.High_Res.Num%100<90)
//								Save_Res.Set_Data.High_Res.Num+=10;
//							else
//								Save_Res.Set_Data.High_Res.Num-=90;
//							Uart_Send_Flag=2;
//							break;
//						case 11:
//							if(Save_Res.Set_Data.Nominal_V.Num%100<90)
//								Save_Res.Set_Data.Nominal_V.Num+=10;
//							else
//								Save_Res.Set_Data.Nominal_V.Num-=90;
//							Uart_Send_Flag=2;
//							break;
//						case 12:
//							if(Save_Res.Set_Data.V_high.Num%100<90)
//								Save_Res.Set_Data.V_high.Num+=10;
//							else
//								Save_Res.Set_Data.V_high.Num-=90;
//							Uart_Send_Flag=2;
							
//							break;
						
						default:
							break;
					
					
					}	
					
				break;
				case Key_F4:
					switch(Button_Page.index)
					{
						case 0:
//							if(Button_Page.page==0)
								SetSystemStatus(SYS_STATUS_SYS);
							break;
						case 1:
							
									
							break;
						case 2:
							
							break;
						case 3+1:
							break;
						case 4+1:
//							if(Save_Res.Set_Data.Res_low.Num%10<9)
//								Save_Res.Set_Data.Res_low.Num+=1;
//							else
//								Save_Res.Set_Data.Res_low.Num-=9;
//							Uart_Send_Flag=2;
							break;
						case 5+1:
							break;
						case 6+1:
							if(Save_Res.Set_Data.V_low.Num%10<9)
								Save_Res.Set_Data.V_low.Num+=1;
							else
								Save_Res.Set_Data.V_low.Num-=9;
							Uart_Send_Flag=2;
							break;
						case 7+1:
							Save_Res.Set_Data.Range=3;
							Uart_Send_Flag=2;
							break;
						case 8+1:
							if(Save_Res.Set_Data.openbeep==0)
							{
								Save_Res.Set_Data.openbeep=1;
							}else{
								Save_Res.Set_Data.openbeep=0;
							}
							break;
//						case 8:
//							break;
//						case 9:
//							if(Save_Res.Set_Data.Nominal_Res.Num%10<9)
//								Save_Res.Set_Data.Nominal_Res.Num+=1;
//							else
//								Save_Res.Set_Data.Nominal_Res.Num-=9;
//							Uart_Send_Flag=2;
//							break;
//						case 10:
//							if(Save_Res.Set_Data.High_Res.Num%10<9)
//								Save_Res.Set_Data.High_Res.Num+=1;
//							else
//								Save_Res.Set_Data.High_Res.Num-=9;
//							Uart_Send_Flag=2;
//							break;
//						case 11:
//							if(Save_Res.Set_Data.Nominal_V.Num%10<9)
//								Save_Res.Set_Data.Nominal_V.Num+=1;
//							else
//								Save_Res.Set_Data.Nominal_V.Num-=9;
//							Uart_Send_Flag=2;
//							break;
//						case 12:
//							if(Save_Res.Set_Data.V_high.Num%10<9)
//								Save_Res.Set_Data.V_high.Num+=1;
//							else
//								Save_Res.Set_Data.V_high.Num-=9;
//							Uart_Send_Flag=2;
//							break;
						
						default:
							break;					
					}	
				
				break;
				case Key_F5:
					switch(Button_Page.index)
					{
						case 4+1:
						case 9+1:
							break;
						case 7+1:	
							Save_Res.Set_Data.Range=4;
							Uart_Send_Flag=2;
						break;
						
						default:
							break;
					
					
					}
                    
					
				break;
				case Key_Disp:
                        SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                        //SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
					if(Button_Page.index==0)
						Button_Page.index=12;
					else
					if(Button_Page.index>7&&Button_Page.index < 10)
					{
						Button_Page.index-=7;
					}else if(Button_Page.index<3){
						Button_Page.index+=7;
					}else if(Button_Page.index >=3 && Button_Page.index <= 7){
						Button_Page.index+=6;
					}else if(Button_Page.index >=10){
						Button_Page.index-=6;
					}
						
				break;
				case Key_RIGHT:
					if(Button_Page.index==0)
						Button_Page.index=1;
					else
					if(Button_Page.index<=2)
						Button_Page.index+=7;
					else if(Button_Page.index>2 && Button_Page.index<=7)
						Button_Page.index+=6;
					else if(Button_Page.index>7 && Button_Page.index<=9)
						Button_Page.index-=7;
					else
						Button_Page.index-=6;
						
				break;
				case Key_DOWN:
					if(Button_Page.index>12)
						Button_Page.index=0;
					else
						Button_Page.index++;
					
					
				break;
				case Key_UP:
					if(Button_Page.index<1)
						Button_Page.index=13;
					else
						Button_Page.index--;
					
				break;
				case Key_DOT:
//					switch(Button_Page.index)
//					{
//						case 4:
//							if(Save_Res.Set_Data.Res_low.Dot>2)
//								Save_Res.Set_Data.Res_low.Dot=0;
//							else
//								Save_Res.Set_Data.Res_low.Dot++;
//							break;
//						case 6:
//							if(Save_Res.Set_Data.V_low.Dot>2)
//								Save_Res.Set_Data.V_low.Dot=0;
//							else
//								Save_Res.Set_Data.V_low.Dot++;
//							break;
//						case 9:
//							if(Save_Res.Set_Data.Nominal_Res.Dot>2)
//								Save_Res.Set_Data.Nominal_Res.Dot=0;
//							else
//								Save_Res.Set_Data.Nominal_Res.Dot++;
//							break;
//						case 10:
//							if(Save_Res.Set_Data.High_Res.Dot>2)
//								Save_Res.Set_Data.High_Res.Dot=0;
//							else
//								Save_Res.Set_Data.High_Res.Dot++;
//							break;
//						case 11:
//							if(Save_Res.Set_Data.Nominal_V.Dot>2)
//								Save_Res.Set_Data.Nominal_V.Dot=0;
//							else
//								Save_Res.Set_Data.Nominal_V.Dot++;
//							break;
//						case 12:
//							if(Save_Res.Set_Data.V_high.Dot>2)
//								Save_Res.Set_Data.V_high.Dot=0;
//							else
//								Save_Res.Set_Data.V_high.Dot++;
//							break;
//					
//					
//					}
					break;
				case Key_NUM1:
				//break;
				case Key_NUM2:
				//break;
				case Key_NUM3:
				//break;
				case Key_NUM4:
				//break;
				case Key_NUM5:
				//break;
				case Key_NUM6:
				//break;
				case Key_NUM7:
				//break;
				case Key_NUM8:
				//break;
				case Key_NUM9:
				//break;
				case Key_NUM0:
				//break;
				switch(Button_Page.index)
				{
					case 4+1:
						Coordinates.xpos=LIST1+88;
						Coordinates.ypos=FIRSTLINE+SPACE1*4;
						Coordinates.lenth=76;
						Save_Res.Set_Data.Res_low=Disp_Set_Num(&Coordinates);
						
						break;
					case 6+1:
						Coordinates.xpos=LIST1+88;
						Coordinates.ypos=FIRSTLINE+SPACE1*7;
						Coordinates.lenth=76;
						Save_Res.Set_Data.V_low=Disp_Set_CompNum(&Coordinates);
						
						break;
					case 9+1:
						Coordinates.xpos=LIST2+88;
						Coordinates.ypos=FIRSTLINE+SPACE1*3;
						Coordinates.lenth=76;
						Save_Res.Set_Data.Nominal_Res=Disp_Set_Num(&Coordinates);
						break;
					case 10+1:
						Coordinates.xpos=LIST2+88;
						Coordinates.ypos=FIRSTLINE+SPACE1*4;
						Coordinates.lenth=76;
						Save_Res.Set_Data.High_Res=Disp_Set_Num(&Coordinates);
						break;
					case 11+1:
						Coordinates.xpos=LIST2+88;
						Coordinates.ypos=FIRSTLINE+SPACE1*6;
						Coordinates.lenth=76;
						Save_Res.Set_Data.Nominal_V=Disp_Set_CompNum(&Coordinates);
						break;
					case 12+1:
						Coordinates.xpos=LIST2+88;
						Coordinates.ypos=FIRSTLINE+SPACE1*7;
						Coordinates.lenth=76;
						Save_Res.Set_Data.V_high=Disp_Set_CompNum(&Coordinates);
						break;
					default:
						break;
				
				
				}
																							
					break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	 	
	
	
	
	}
	Savetoeeprom();
}
//数据保存程序
void Data_StoreProcess(void)
{
	vu32 keynum=0;
	vu8 key;
//	Button_Page_Typedef Button_Page;
//	Button_Page.index=0;
//	Button_Page.page=0;
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_DATASTORE)
	{
	 key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==2)
		{
            Key_Beep();
			switch(key)
			{
				case Key_F1:
				break;
				case Key_F2:
				break;
				case Key_F3:
				break;
				case Key_F4:
				break;
				case Key_F5:
				break;
				case Key_Disp:
				break;
				case Key_SETUP:
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
				break;
				case Key_DOWN:
				break;
				case Key_NUM1:
				break;
				case Key_NUM2:
				break;
				case Key_NUM3:
				break;
				case Key_NUM4:
				break;
				case Key_NUM5:
				break;
				case Key_NUM6:
				break;
				case Key_NUM7:
				break;
				case Key_NUM8:
				break;
				case Key_NUM9:
				break;
				case Key_NUM0:
				break;
				case Key_DOT:
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	 	
	
	}
}
//档号显示
void Range_Process(void)
{
	
	vu32 keynum=0;
	vu8 key;
	Send_Ord_Typedef Uart;
//    vu8 page=1;
	vu8 Disp_flag=1;
//	vu8 index=0;
	Button_Page_Typedef Button_Page;
	Button_Page.index=0;
	Button_Page.page=0;
	Disp_Range_Item();
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_RANGE)
	{
		if(Disp_flag==1)
		{
			Disp_RangeDispValue(&Button_Page);
			Disp_flag=0;	
		}
		Uart_Process();
		
		if(timer0_counter>0)//请求数据
		{
			switch(Uart_Send_Flag)
			{
				case 0:
					Send_Request();
					break;
				case 1:
					Send_Main_Ord();
					break;
				case 2:
					Send_Freq(&Uart);
					break;
				default:
					Send_Request();
					break;
			
			}
			Uart_Send_Flag=0;
			
			timer0_counter=0;
		
		
		}
		if(SaveData.Limit_Tab.Comp)
			Test_Comp(&Comp_Change);
		Disp_RangeTestvalue();
	 	key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
            Key_Beep();
			Disp_flag=1;
			switch(key)
			{
				case Key_F1:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_TEST);
					}
					else
						SaveData.Limit_Tab.Comp=0;
				break;
				case Key_F2:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_RANGE);
					}
					else
						SaveData.Limit_Tab.Comp=1;
				break;
				case Key_F3:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_RANGECOUNT);
					}
				break;
				case Key_F4:
//					if(Button_Page.index==0)
//					{
//						if(Button_Page.page==0)
//							SetSystemStatus(SYS_STATUS_ITEM);
//					}
				break;
				case Key_F5:
//					if(Button_Page.index==0)
//					{
//						if(Button_Page.page==1)
//							Button_Page.page=0;
//						else
//							Button_Page.page=1;
//						Disp_Button_value1(Button_Page.page);
//					}
				break;
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
					
//				break;
				case Key_LEFT:
//				break;
				case Key_RIGHT:
//				break;
				case Key_UP:
//				break;
				case Key_DOWN:
					if(Button_Page.index)
						Button_Page.index=0;
					else
						Button_Page.index=1;
				break;
				case Key_NUM1:
				break;
				case Key_NUM2:
				break;
				case Key_NUM3:
				break;
				case Key_NUM4:
				break;
				case Key_NUM5:
				break;
				case Key_NUM6:
				break;
				case Key_NUM7:
				break;
				case Key_NUM8:
				break;
				case Key_NUM9:
				break;
				case Key_NUM0:
				break;
				case Key_DOT:
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
}
//档计数显示
void Range_CountProcess(void)
{
   	 vu32 keynum=0;
	vu32 uart_count=0;
	 vu8 key;
	u8 i;
	vu8 Disp_flag=1;
//	 vu8 page=1;
	Button_Page_Typedef Button_Page;
	Button_Page.index=0;
	Button_Page.page=0;
	Disp_Range_Count_Item();
	Delay_Key();
	Set_Compbcd_float();
	for(i=0;i<=10;i++)
		Count_buff[i]=0;
 	while(GetSystemStatus()==SYS_STATUS_RANGECOUNT)
	{
		key=HW_KeyScsn();
		if(Disp_flag==1)
		{
			Disp_Range_ComDispValue(&Button_Page);
			Disp_flag=0;
			
		}
		uart_count=Uart_Process();
		if(timer0_counter>0)//请求数据
		{
			switch(Uart_Send_Flag)
			{
				case 0:
					Send_Request();
					break;
				case 1:
					Send_Main_Ord();
					break;
				case 2:
					//Send_Freq(&Uart);
					break;
				default:
					Send_Request();
					break;
			
			}
			Uart_Send_Flag=0;
			
			timer0_counter=0;
		
		
		}
		if(SaveData.Limit_Tab.Comp&&uart_count==1)
			Test_Comp(&Comp_Change);
		//Disp_RangeTestvalue();
		if(SaveData.Limit_Tab.Param)//附属比较
			;
		Disp_RangeCount();//增加分选结果计数
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{	Disp_flag=1;
            Key_Beep();
			switch(key)
			{
				case Key_F1:
                    if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_TEST);
					}
					else
						SaveData.Limit_Tab.count=0;
				break;
				case Key_F2:
					if(Button_Page.index==0)
					{
                    if(Button_Page.page==0)
                        SetSystemStatus(SYS_STATUS_RANGE);
					}
					else
						SaveData.Limit_Tab.count=1;
				break;
				case Key_F3:
					if(Button_Page.index==0)
                    if(Button_Page.page==0)
                        SetSystemStatus(SYS_STATUS_RANGECOUNT);
				break;
				case Key_F4:
//					if(Button_Page.index==0)
//                    if(Button_Page.page==0)
//                        SetSystemStatus(SYS_STATUS_ITEM);
				break;
				case Key_F5:
//                    if(Button_Page.page==1)
//						Button_Page.page=0;
//					else
//						Button_Page.page=1;
//                    Disp_Button_value1(Button_Page.page);
				break;
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				//break;
				case Key_RIGHT:
				//break;
				case Key_UP:
				//break;
				case Key_DOWN:
					if(Button_Page.index==0)
						Button_Page.index=1;
					else
						Button_Page.index=0;
				break;
				case Key_NUM1:
				break;
				case Key_NUM2:
				break;
				case Key_NUM3:
				break;
				case Key_NUM4:
				break;
				case Key_NUM5:
				break;
				case Key_NUM6:
				break;
				case Key_NUM7:
				break;
				case Key_NUM8:
				break;
				case Key_NUM9:
				break;
				case Key_NUM0:
				break;
				case Key_DOT:
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
}
// 列表显示
void ItemProcess(void)
{
	
	vu32 keynum;
	vu8 key;
	vu8 Disp_flag=1;
//    vu8 page=1;
	
	Button_Page_Typedef Button_Page;
	SaveData.Limit_ScanValue.num=0;
	Button_Page.index=0;
	Button_Page.page=0;
	Disp_List_Count_Item();
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_ITEM)
	{
		if(Disp_flag)
		{
			Disp_flag=0;
			Disp_LIMIT_ComDispValue(&Button_Page);
		}
		
		Disp_Scan_Compvalue(0);
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==2)
		{
			Disp_flag=1;
            Key_Beep();
			switch(key)
			{
				case Key_F1:
                    if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_TEST);
					}
					else if(Button_Page.index==1)
						SaveData.Limit_ScanValue.fun=0;
					
				break;
				case Key_F2:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_RANGE);
					
					}else if(Button_Page.index==1)
						SaveData.Limit_ScanValue.fun=1;
					
				break;
				case Key_F3:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_RANGECOUNT);
					
					}
				break;
				case Key_F4:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_ITEM);
					}
					else if(Button_Page.index==2)
					{
						if(SaveData.Limit_ScanValue.num<1)
							SaveData.Limit_ScanValue.num=20;
						else
							SaveData.Limit_ScanValue.num--;
					
					}
				break;
				case Key_F5:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							Button_Page.page=1;
						else
							Button_Page.page=0;
						Disp_Button_value1(Button_Page.page);
					}
					else if(Button_Page.index==2)
					{
						if(SaveData.Limit_ScanValue.num>=20)
							SaveData.Limit_ScanValue.num=0;
						else
							SaveData.Limit_ScanValue.num++;
					
					}
				break;
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
					if(Button_Page.index>0)
						Button_Page.index--;
				break;
				case Key_DOWN:
					if(Button_Page.index<2)
						Button_Page.index++;
				break;
				case Key_NUM1:
				break;
				case Key_NUM2:
				break;
				case Key_NUM3:
				break;
				case Key_NUM4:
				break;
				case Key_NUM5:
				break;
				case Key_NUM6:
				break;
				case Key_NUM7:
				break;
				case Key_NUM8:
				break;
				case Key_NUM9:
				break;
				case Key_NUM0:
				break;
				case Key_DOT:
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
}
//列表扫描设置
void Use_ITEMSetProcess(void)
{
	Disp_Coordinates_Typedef  Coordinates;
	vu32 keynum=0;
	vu8 key;
//    vu8 page=0;
	vu8 Disp_flag=1;
	Button_Page_Typedef Button_Page;
	
	Button_Page.index=0;
	Button_Page.page=0;
	Button_Page.third=0;
	Button_Page.force=0;
	SaveData.Limit_ScanValue.num=0;
	Disp_ListScan_Item();
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_ITEMSET)
	{
	  	
		if(Disp_flag)
		{
			Disp_flag=0;
		
			Disp_Scan_SetCompvalue(&Button_Page);
		}
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			Disp_flag=1;
            Key_Beep();
			switch(key)
			{
				case Key_F1:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_SETUPTEST);
						else
							SetSystemStatus(SYS_STATUS_FILE);
					}
						
				break;
				case Key_F2:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_USERDEBUG);
						else
							SetSystemStatus(SYS_STATUS_SYSSET);
					}
				break;
				case Key_F3://
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_LIMITSET);
						else
							SetSystemStatus(SYS_STATUS_TOOL);
					}
				break;
				case Key_F4:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_ITEMSET);
					}else 
					if(Button_Page.index>1)
					{
						if(Button_Page.force>0)
							Button_Page.force--;
					
					}
					
				break;
				case Key_F5:
					if(Button_Page.index==0)
					{
						if(Button_Page.page)
							Button_Page.page=0;
						else
							Button_Page.page=1;
						Disp_Button_TestSet(Button_Page.page);
					}else
					if(Button_Page.force<20)
						Button_Page.force++;
				break;
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
					if(Button_Page.third>0)
						Button_Page.third--;
						
					
				break;
				case Key_RIGHT:
					if(Button_Page.third<5)
						Button_Page.third++;
					else
						Button_Page.third=5;
				break;
				case Key_UP:
					if(Button_Page.index>0)
						Button_Page.index--;
					else
						Button_Page.page=0;
						
				break;
				case Key_DOWN:
					if(Button_Page.index<12)
						Button_Page.index++;
					else
						Button_Page.index=12;
				break;
				case Key_NUM1:
				//break;
				case Key_NUM2:
				//break;
				case Key_NUM3:
				//break;
				case Key_NUM4:
				//break;
				case Key_NUM5:
				//break;
				case Key_NUM6:
				//break;
				case Key_NUM7:
				//break;
				case Key_NUM8:
				//break;
				case Key_NUM9:
				//break;
				case Key_NUM0:
				//break;
				case Key_DOT:
				//break;
				
					if(Button_Page.index>2)
					{
						if(Button_Page.third==0)
						{
							Coordinates.xpos=40;//FIRSTLINE+SPACE1+3+ (i-2)*16
							Coordinates.ypos=FIRSTLINE+SPACE1+3+(Button_Page.index-2)*16;
							
							SaveData.Limit_ScanValue.freq[Button_Page.force*10+
							(Button_Page.index-3)]=Freq_Set_Num(&Coordinates);
						}else 
						if(Button_Page.third==1)
						{
						
						}else
						if(Button_Page.third==2)
						{
						
						}else
						if(Button_Page.third==3)
						{
						
						}else
						if(Button_Page.third==4)
						{
							Coordinates.xpos=420;
							Coordinates.ypos=FIRSTLINE+SPACE1+3+(Button_Page.index-2)*16;
							Coordinates.lenth=60;
							SaveData.Limit_ScanValue.time[Button_Page.force*10+
							(Button_Page.index-3)]=Disp_Set_Num(&Coordinates);
						
						
						}
					
					}
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
}
//极限设置 
void Use_LimitSetProcess(void)
{
	
	vu32 keynum=0;
	vu8 key,i;
	Disp_Coordinates_Typedef Coordinates;
//    vu8 page=1;
	vu8 Disp_flag=1;
	Button_Page_Typedef Button_Page;
	Button_Page.index=0;
	Button_Page.page=0;
	Disp_LimitList_Item();
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_LIMITSET)
	{
	  	if(Disp_flag)
		{
			Disp_flag=0;
			Disp_LimitSEt_value(& Button_Page);
		}
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			Disp_flag=1;
            Key_Beep();
			switch(key)
			{
				case Key_F1:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_SETUPTEST);
						else
							SetSystemStatus(SYS_STATUS_FILE);
					
					}else if(Button_Page.index==1)
					;
					else if(Button_Page.index==2)
						;
					else if(Button_Page.index==3)
					{
						SaveData.Limit_Tab.Mode=0;
					
					}
					else if(Button_Page.index==4)
					{
						SaveData.Limit_Tab.Param=0;
					}
					else if(Button_Page.index==5)
					{
						SaveData.Limit_Tab.Comp=0;
					}else
					{
						if(Button_Page.index>15)
						{
						SaveData.Limit_Tab.Comp_Value[Button_Page.index-16].high.Num=0;
						SaveData.Limit_Tab.Comp_Value[Button_Page.index-16].low.Num=0;	
						}else if(Button_Page.index>5)
						{
						SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].high.Num=0;
						SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].low.Num=0;	
						
						}
					
					
					}
						
				break;
				case Key_F2:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_USERDEBUG);
						else
							SetSystemStatus(SYS_STATUS_SYSSET);
					}
					else if(Button_Page.index==1)
					;
					else if(Button_Page.index==2)
						;
					else if(Button_Page.index==3)
					{
						SaveData.Limit_Tab.Mode=1;
					
					}
					else if(Button_Page.index==4)
					{
						SaveData.Limit_Tab.Param=1;
					}
					else if(Button_Page.index==5)
					{
						SaveData.Limit_Tab.Comp=1;
					}
				break;
				case Key_F3:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_LIMITSET);
						else
							SetSystemStatus(SYS_STATUS_TOOL);
					}
				break;
				case Key_F4:
//					if(Button_Page.index==0)
//					{
//						if(Button_Page.page==0)
//							SetSystemStatus(SYS_STATUS_ITEMSET);
//					}
					
				break;
				case Key_F5:
					if(Button_Page.index==0)
					{
//						if(Button_Page.page)
//							Button_Page.page=0;
//						else
//						Button_Page.page=1;
//						Disp_Button_TestSet(Button_Page.page);
					}else if(Button_Page.index>5)
					{
						for(i=0;i<10;i++)
						{
							SaveData.Limit_Tab.Comp_Value[i].low.Num=0;
							SaveData.Limit_Tab.Comp_Value[i].high.Num=0;
							
						}
						Button_Page.index=0;
					
					
					}
				break;
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
					if(Button_Page.index!=0)
					{
						if(Button_Page.index<=5)
						{
							Button_Page.index--;
						
						}
						else
						{
							if(Button_Page.index>15)
								Button_Page.index-=10;
							else
								Button_Page.index+=9;
						}
					
					
					}
					
				break;
				case Key_RIGHT:
					if(Button_Page.index!=0)
					{
//						if(Button_Page.index<=5)
//						{
//							Button_Page.index++;
//						
//						}
//						else
						//if(Button_Page.index)
						{
							if(Button_Page.index>15)
								Button_Page.index-=9;
							else
								//if(Button_Page.index<)
								Button_Page.index+=10;
						}
					
					
					}
				break;
				case Key_UP:
					if(Button_Page.index>16)
						Button_Page.index--;
					else if(Button_Page.index==16)
						Button_Page.index=5;
					else
						
					if(Button_Page.index>0)
						Button_Page.index--;
				break;
				case Key_DOWN:
					if(Button_Page.index<15)
						Button_Page.index++;
					else if(Button_Page.index==15)
						Button_Page.index=0;
					else
						
					if(Button_Page.index<25)
						Button_Page.index++;
					else
						Button_Page.index=0;
						
				break;
				case Key_NUM1:
//				break;
				case Key_NUM2:
//				break;
				case Key_NUM3:
//				break;
				case Key_NUM4:
//				break;
				case Key_NUM5:
//				break;
				case Key_NUM6:
//				break;
				case Key_NUM7:
//				break;
				case Key_NUM8:
//				break;
				case Key_NUM9:
//				break;
				case Key_NUM0:
//				break;
				case Key_DOT:
					if(Button_Page.index==2)//LIST2-24, FIRSTLINE,
					{
							Coordinates.xpos=LIST2-24;//FIRSTLINE+SPACE1+3+ (i-2)*16
							Coordinates.ypos=FIRSTLINE;
							Coordinates.lenth=66;
							SaveData.Limit_Tab.Nom=Disp_Set_InputNum(&Coordinates);
					}else if(Button_Page.index>5&&Button_Page.index<=15)
						{
							//LIST2-90, 76+(i-6)*15
							Coordinates.xpos=LIST2-90;//FIRSTLINE+SPACE1+3+ (i-2)*16
							Coordinates.ypos=76+(Button_Page.index-6)*15;
							Coordinates.lenth=60;
							SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].low=Disp_Set_InputpreNum(&Coordinates);
							SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].high=SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].low;
							//Sort_TypeDef Disp_Set_InputpreNum(Disp_Coordinates_Typedef *Coordinates)
						}else if(Button_Page.index>15)
						{
							Coordinates.xpos=LIST2+70;//FIRSTLINE+SPACE1+3+ (i-2)*16
							Coordinates.ypos=76+(Button_Page.index-16)*15;
							Coordinates.lenth=60;
							SaveData.Limit_Tab.Comp_Value[Button_Page.index-16].high=Disp_Set_InputpreNum(&Coordinates);
						
						}
						do{
							key=HW_KeyScsn();
						}
						while(key!=0xff);
						
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
	Set_Compbcd_float();
	Savetoeeprom();
}
void Sys_Process(void)
{
	vu32 keynum=0;
	vu8 key;
	vu8 Disp_flag=1;
	Button_Page_Typedef Button_Page;
	Button_Page.index=0;
	Button_Page.page=0;
//	Disp_Sys_Screen();
//	Colour.Fword=White;
//	Colour.black=LCD_COLOR_TEST_BAR;
//	WriteString_16(0, 4, All_TopName[8],  0);
//	Disp_Sys_Item();
    lcd_Clear(LCD_COLOR_TEST_BACK);
	Delay_Key();
	while(GetSystemStatus()==SYS_STATUS_SYS)
	{
		if(Disp_flag==1)
		{
			Disp_Sys();
			Disp_flag=0;	
		}
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			Disp_flag=1;
            Key_Beep();
			switch(key)
			{
				case Key_F1:
					switch(Button_Page.index)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_TEST);
							break;
						
						default:
							break;
					
					}
                    
				break;
				case Key_F2:
					switch(Button_Page.index)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SETUPTEST);
							break;
						
					
					}
                    
				break;
				case Key_F3:
					switch(Button_Page.index)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SYSSET);
							break;
						
						default:
						break;
					
					}
						
				break;
				case Key_F4:
					switch(Button_Page.index)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SYS);
							break;
						
							
						
						default:
						break;
					
					}
				break;
				
				
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				
				
				default:
				break;
					
			}
		
		
		}
	
	
	
	
	}

}
//软件关机程序，放在工具状态里面
void Soft_Turnon(void)
{
	uint32_t i;
	uint8_t key;
	Set_Compbcd_float();
    
//	ReadSavedata();
//	SetData_High_Low_Comp();
//	Savetoeeprom();
	
	Power_On_led();

	All_LedOff();
	Beep_Off();
    GPIO_ClearInt(0, 1<<19);
	key=HW_KeyScsn();
	if(key==0xff)
	{
		keynum=0;
		jumpboot();
	}
	else{
		SetSystemStatus(SYS_STATUS_POWER);
		keynum++;
	}
//	if(keynum==1)
//	{	
//		//Disp_Flag=1;
//		Key_Beep();
//		switch(key)
//		{
//			case Key_SETUP:
//			{					                                             
				
//			}break;
//			default:
//				break;
//			}
//	}
	
	while(GetSystemStatus()==SYS_STATUS_TOOL)
	{
		
        for(i=0xfffff;i>0;i--)
		;
		NVIC_EnableIRQ(GPIO_IRQn);
		
//		if(Save_Res.softswitch)
//		{
//			SetSystemStatus(SYS_STATUS_POWER);
//		
//		
//		}
	
	
	}
    NVIC_DisableIRQ(GPIO_IRQn);
    GPIO_ClearInt(0, 1<<19);
	Power_Off_led();

}
//系统设置
void Use_SysSetProcess(void)
{	
	vu32 keynum=0;
	vu8 key,i;
//    uint8_t Disp_buff[12];
    char key_count=0;
	vu8 Disp_flag=1;
	Button_Page_Typedef Button_Page;
	Button_Page.index=0;
	Button_Page.page=0;
    lcd_Clear(LCD_COLOR_TEST_BACK);
	Disp_Sys_Item();
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_SYSSET)
	{
	  	
		if(Disp_flag==1||Rtc_intflag==1)
		{
            Rtc_intflag=0;
            if(key_count==1)
            {
                for(i=1;i<8;i++)
                Save_Res.Sys_Setvalue.textname[i]=0;
            
            }
            //Save_Res.Sys_Setvalue.textname[8]='/0';
			Disp_Sys_value(&Button_Page);
			Disp_flag=0;
            
		}
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			Disp_flag=1;
            Key_Beep();
			switch(key)
			{
				case Key_F1:
					switch(Button_Page.index)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_TEST);
							break;
						case 1:
							Save_Res.Sys_Setvalue.uart=0;
							break;
						case 2:
							Save_Res.Sys_Setvalue.buard=0;
							break;
						case 3:
                            Save_Res.Sys_Setvalue.u_flag=0;
							
							break;
						case 4:
							Save_Res.Sys_Setvalue.plc=0;
							break;
						case 5:
							Save_Res.Sys_Setvalue.lanage=0;
							break;
						case 6:
							if(RTC_TIME_DISP.YEAR<1)
								RTC_TIME_DISP.YEAR=RTC_YEAR_MAX;
							else
                                if(RTC_TIME_DISP.YEAR>=RTC_YEAR_MAX)
                                    RTC_TIME_DISP.YEAR=0;
                                else
								RTC_TIME_DISP.YEAR--;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_YEAR, RTC_TIME_DISP.YEAR);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 7:
							if(RTC_TIME_DISP.MONTH<=1)
								RTC_TIME_DISP.MONTH=RTC_MONTH_MAX;
							else
								RTC_TIME_DISP.MONTH--;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MONTH, RTC_TIME_DISP.MONTH);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 8:
							if(RTC_TIME_DISP.DOM<=1)
								RTC_TIME_DISP.DOM=RTC_DAYOFMONTH_MAX;
							else
								RTC_TIME_DISP.DOM--;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, RTC_TIME_DISP.DOM);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 9:
							if(RTC_TIME_DISP.HOUR<1)
								RTC_TIME_DISP.HOUR=RTC_HOUR_MAX;
							else
								RTC_TIME_DISP.HOUR--;
                             RTC_SetTime (LPC_RTC, RTC_TIMETYPE_HOUR, RTC_TIME_DISP.HOUR);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 10:
							if(RTC_TIME_DISP.MIN<1)
								RTC_TIME_DISP.MIN=RTC_MINUTE_MAX;
							else
								RTC_TIME_DISP.MIN--;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MINUTE, RTC_TIME_DISP.MIN);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 11:
							if(RTC_TIME_DISP.SEC<1)
								RTC_TIME_DISP.SEC=RTC_SECOND_MAX;
							else
								RTC_TIME_DISP.SEC--;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_SECOND, RTC_TIME_DISP.SEC);
                            
                            RTC_CalibConfig(LPC_RTC, 2, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 12://
							//SaveData.Sys_Setup.Bias=0;
							break;
						case 18:
							SaveData.Sys_Setup.Bus_Mode=0;
							break;
						case 13:
							if(SaveData.Sys_Setup.GP_Addr<1)
								SaveData.Sys_Setup.GP_Addr=99;
							else
								SaveData.Sys_Setup.GP_Addr--;
							break;
						case 14:
							SaveData.Sys_Setup.Talk_Only=1;
							break;
						case 15:
							if(SaveData.Sys_Setup.Timer_Value.Hour<1)
								SaveData.Sys_Setup.Timer_Value.Hour=12;
							else
								SaveData.Sys_Setup.Timer_Value.Hour--;
							break;
						case 16:
							if(SaveData.Sys_Setup.Timer_Value.Min<1)
								SaveData.Sys_Setup.Timer_Value.Min=59;
							else
								SaveData.Sys_Setup.Timer_Value.Min--;
							break;
						case 17:
							if(SaveData.Sys_Setup.Timer_Value.Sec<1)
								SaveData.Sys_Setup.Timer_Value.Sec=59;
							else
								SaveData.Sys_Setup.Timer_Value.Sec--;
							break;
						default:
							break;
					
					}
                    
				break;
				case Key_F2:
					switch(Button_Page.index)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SETUPTEST);
							break;
						case 1:
							Save_Res.Sys_Setvalue.uart=1;
							break;
						case 2:
							Save_Res.Sys_Setvalue.buard=1;
							break;
						case 3:
							Save_Res.Sys_Setvalue.u_flag=1;
							break;
						case 4:
							Save_Res.Sys_Setvalue.plc=1;
							break;
						case 5:
							Save_Res.Sys_Setvalue.lanage=1;
							break;
						case 6:
							if(RTC_TIME_DISP.YEAR>=RTC_YEAR_MAX)
								RTC_TIME_DISP.YEAR=0;
							else
								RTC_TIME_DISP.YEAR++;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_YEAR, RTC_TIME_DISP.YEAR);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 7:
							if(RTC_TIME_DISP.MONTH>=RTC_MONTH_MAX)
								RTC_TIME_DISP.MONTH=1;
							else
								RTC_TIME_DISP.MONTH++;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MONTH, RTC_TIME_DISP.MONTH);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 8:
							if(RTC_TIME_DISP.DOM>=RTC_DAYOFMONTH_MAX)
								RTC_TIME_DISP.DOM=1;
							else
								RTC_TIME_DISP.DOM++;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, RTC_TIME_DISP.DOM);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 9:
							if(RTC_TIME_DISP.HOUR>=RTC_HOUR_MAX)
								RTC_TIME_DISP.HOUR=0;
							else
								RTC_TIME_DISP.HOUR++;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_HOUR, RTC_TIME_DISP.HOUR);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 10:
							if(RTC_TIME_DISP.MIN>=RTC_MINUTE_MAX)
								RTC_TIME_DISP.MIN=0;
							else
								RTC_TIME_DISP.MIN++;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MINUTE, RTC_TIME_DISP.MIN);
                            
                            RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 11:
							if(RTC_TIME_DISP.SEC>=RTC_SECOND_MAX)
								RTC_TIME_DISP.SEC=0;
							else
								RTC_TIME_DISP.SEC++;
                            RTC_SetTime (LPC_RTC, RTC_TIMETYPE_SECOND, RTC_TIME_DISP.SEC);
                            
                            RTC_CalibConfig(LPC_RTC, 2, RTC_CALIB_DIR_FORWARD);
                            RTC_CalibCounterCmd(LPC_RTC, ENABLE);
							break;
						case 18:
							SaveData.Sys_Setup.Bias=1;
							break;
						case 12:
							SaveData.Sys_Setup.Bus_Mode=1;
							break;
						case 13:
							if(SaveData.Sys_Setup.GP_Addr>99)
								SaveData.Sys_Setup.GP_Addr=0;
							else
								SaveData.Sys_Setup.GP_Addr++;
							break;
						case 14:
							SaveData.Sys_Setup.Talk_Only=0;
							break;
						case 15:
							if(SaveData.Sys_Setup.Timer_Value.Hour>12)
								SaveData.Sys_Setup.Timer_Value.Hour=0;
							else
								SaveData.Sys_Setup.Timer_Value.Hour++;
							break;
						case 16:
							if(SaveData.Sys_Setup.Timer_Value.Min>69)
								SaveData.Sys_Setup.Timer_Value.Min=0;
							else
								SaveData.Sys_Setup.Timer_Value.Min++;
							break;
						case 17:
							if(SaveData.Sys_Setup.Timer_Value.Sec>59)
								SaveData.Sys_Setup.Timer_Value.Sec=0;
							else
								SaveData.Sys_Setup.Timer_Value.Sec++;
							break;
						default:
							break;
					
					}
                    
				break;
				case Key_F3:
					switch(Button_Page.index)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SYSSET);
							break;
						case 2:
							Save_Res.Sys_Setvalue.buard=2;
							
						break;
						default:
						break;
					
					}
						
				break;
				case Key_F4:
					switch(Button_Page.index)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SYS);
							break;
						case 2:
							Save_Res.Sys_Setvalue.buard=3;
							
						break;
						default:
						break;
					
					}
				break;
				case Key_F5:
					switch(Button_Page.index)
					{
						case 0:
							break;//恢复系统复位
						case 2:
							Save_Res.Sys_Setvalue.buard=4;
							
						break;
						default:
						break;
					
					}
				break;
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
                    if(Button_Page.index==12)
                    {
                        //dispflag=0;
//                        for(i=0;i<8;i++)
//                        {
//                            Save_Res.Sys_Setvalue.textname[i]=Disp_buff[i];
//                        
//                        }
                        key_count=0;
                        Button_Page.index=0;
                        Savetoeeprom();
                    }
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_DOWN:
					if(Button_Page.index>SYS_MAX-1)
						Button_Page.index=0;
					else
						Button_Page.index++;
						
				break;
				case Key_UP:
					if(Button_Page.index<1)
						Button_Page.index=SYS_MAX;
					else
						Button_Page.index--;
				break;
				case Key_NUM1:
                    if(Button_Page.index==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Save_Res.Sys_Setvalue.textname[key_count]='1';
                            key_count++;
                                
                        }
                    
                    }
                        //Save_Res.Sys_Setvalue
				break;
				case Key_NUM2:
                    if(Button_Page.index==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Save_Res.Sys_Setvalue.textname[key_count]='2';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM3:
                    if(Button_Page.index==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Save_Res.Sys_Setvalue.textname[key_count]='3';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM4:
                    if(Button_Page.index==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Save_Res.Sys_Setvalue.textname[key_count]='4';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM5:
                    if(Button_Page.index==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Save_Res.Sys_Setvalue.textname[key_count]='5';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM6:
                    if(Button_Page.index==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Save_Res.Sys_Setvalue.textname[key_count]='6';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM7:
                    if(Button_Page.index==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Save_Res.Sys_Setvalue.textname[key_count]='7';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM8:
                    if(Button_Page.index==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Save_Res.Sys_Setvalue.textname[key_count]='8';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM9:
                    if(Button_Page.index==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Save_Res.Sys_Setvalue.textname[key_count]='9';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM0:
                    if(Button_Page.index==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Save_Res.Sys_Setvalue.textname[key_count]='0';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_DOT:
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
	Savetoeeprom();
}

//用户校正
void Use_DebugProcess(void)
{
	
	vu32 keynum=0;
	vu8 key;
	float ddd,eee;
    Disp_Coordinates_Typedef Debug_Cood;
	Disp_Coordinates_Typedef Coordinates;
//    vu8 page=1;
	vu8 Disp_flag=1;
	Button_Page_Typedef Button_Page;
	Button_Page.index=0;
	Button_Page.page=0;
    lcd_Clear(LCD_COLOR_TEST_BACK);
	Disp_UserCheck_Item();
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_USERDEBUG)
	{
		Uart_Process();
		if(Disp_flag==1)
		{
			Disp_Debug_value(&Button_Page);
			Disp_flag=0;	
		}
		
		if(timer0_counter>0)//请求数据
		{
			switch(Uart_Send_Flag)
			{
				case 0:
					//if(Save_Res.Set_Data.trip==0)
						Send_Request();
					
					break;
				case 1:
					//if(uart_count++>5)
//					Uart_Send_Flag=0;
//				if(keynum<99)
//				keynum++;
//				else
//					keynum++;
//					Send_Main_Ord();
				
					break;
				case 2:
					
					//if(uart_count++>5)
//					Uart_Send_Flag=0;
//					Send_Freq(&Uart);
				
					break;
				default:
					//Send_Request();
					break;
				
			
			}
			//Disp_flag=1;
			
			
			timer0_counter=0;
		}
		ddd=BCDtoInt((int8_t *)Test_Dispvalue.Main_valuebuff);//电阻
		eee=VBCDtoInt((int8_t *)Test_Dispvalue.Secondvaluebuff);//电压
		if(!Test_Dispvalue.Rangedisp)
			{
				if(Test_Dispvalue.Main_valuebuff[0]=='-')
					ddd=0-ddd;
				ddd=fabs(ddd-Save_Res.clear);
				
				
			}
			if(Test_Unit.V_dot==3)
			{
				if(Test_Unit.V_Neg==0)//负
					eee=0-eee;
				eee=fabs(eee-Save_Res.V_Clear);
					
			
			
			}
			
			Disp_Debug_Reference(&Button_Page,eee,ddd);
			if(Button_Page.index-1<4)
				Save_Res.Debug_Value[Button_Page.index-1].ad_value=ddd/(pow(10,Button_Page.index));
				//Input_int((int8_t *)Test_Dispvalue.Main_valuebuff);
			else
				Save_Res.Debug_Value[Button_Page.index-1].ad_value=eee/pow(10,Button_Page.index-5);
				//Input_int((int8_t *)Test_Dispvalue.Secondvaluebuff);

		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			Disp_flag=1;
            Key_Beep();
			switch(key)
			{
				case Key_F1:
                    Savetoeeprom();
                    SetSystemStatus(SYS_STATUS_TEST);
//                    if(Button_Page.page==0)
//                        SetSystemStatus(SYS_STATUS_SETUPTEST);
//					else
//						SetSystemStatus(SYS_STATUS_FILE);
						
				break;
				case Key_F2:
//                    if(Button_Page.page==0)
//                        SetSystemStatus(SYS_STATUS_USERDEBUG);
//					else
//						SetSystemStatus(SYS_STATUS_SYSSET);
				break;
				case Key_F3:
//                    if(Button_Page.page==0)
//                        SetSystemStatus(SYS_STATUS_LIMITSET);
//					else
//						SetSystemStatus(SYS_STATUS_TOOL);
				break;
				case Key_F4:
//                    if(Button_Page.page==0)
//                        SetSystemStatus(SYS_STATUS_ITEMSET);
				break;
				case Key_F5:
//					if(Button_Page.page)
//						Button_Page.page=0;
//					else
//						Button_Page.page=1;
//                    Disp_Button_TestSet(Button_Page.page);
				break;
				case Key_Disp:
                    if(Button_Page.index==0)
                    {
                        Debug_Cood.xpos=70;
                        Debug_Cood.ypos =272-70;
                        Debug_Cood.lenth=120;
                        input_num(&Debug_Cood);
                    
                    }

				break;
				case Key_SETUP:
                   // SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
					
                    
				
				
				break;
				
				
                case Key_LEFT:
				case Key_UP:
						if(Button_Page.index>0)
							Button_Page.index--;
						else
							Button_Page.index=DEBUG_RANGE;
							
				break;
                        case Key_RIGHT:
				case Key_DOWN:
//					Save_Res.Debug_Value[Button_Page.index].standard=
//					Save_Res.Debug_Value[Button_Page.index].ad_value=
					if(Button_Page.index<DEBUG_RANGE)
						Button_Page.index++;
                    else
                        Button_Page.index=0;
                    
					
				break;
					case Key_DOT:
						break;
				case Key_NUM1:
				//break;
				case Key_NUM2:
				//break;
				case Key_NUM3:
				//break;
				case Key_NUM4:
				//break;
				case Key_NUM5:
				//break;
				case Key_NUM6:
				//break;
				case Key_NUM7:
				//break;
				case Key_NUM8:
				//break;
				case Key_NUM9:
				//break;
				case Key_NUM0:
				//break;
				
				Coordinates.xpos=LIST1+160;
				Coordinates.ypos=FIRSTLINE+SPACE1*(Button_Page.index);
				Coordinates.lenth=70;
				Save_Res.Debug_Value[Button_Page.index-1].standard=Freq_Set_Num(&Coordinates);
				
//					if(Button_Page.index==5)
//					{ 	
				//Disp_Coordinates_Typedef Coordinates;
//						Coordinates.xpos=LIST1+92;
//						Coordinates.ypos=FIRSTLINE+SPACE1*4;
//						Coordinates.lenth=86;
//						
//						SaveData.Main_Func.Freq=Freq_Set_Num(&Coordinates);
//					
//					}
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
					if(Save_Res.open == 0)
					{
						Save_Res.open = 1;
					}else{
						Save_Res.open = 0;
					}
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
}

//公司校正
void Fac_DebugProcess(void)
{
	vu32 keynum=0;
	vu8 key;
 	while(GetSystemStatus()==SYS_STATUS_FACRDEBUG)
	{
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==2)
		{
            Key_Beep();
			switch(key)
			{
				case Key_F1:
				break;
				case Key_F2:
				break;
				case Key_F3:
				break;
				case Key_F4:
				break;
				case Key_F5:
				break;
				case Key_Disp:
				break;
				case Key_SETUP:
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
				break;
				case Key_DOWN:
				break;
				case Key_NUM1:
				break;
				case Key_NUM2:
				break;
				case Key_NUM3:
				break;
				case Key_NUM4:
				break;
				case Key_NUM5:
				break;
				case Key_NUM6:
				break;
				case Key_NUM7:
				break;
				case Key_NUM8:
				break;
				case Key_NUM9:
				break;
				case Key_NUM0:
				break;
				case Key_DOT:
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
}	
//==========================================================
//函数名称：Uart_Process
//函数功能：串口处理
//入口参数：无
//出口参数：无
//创建日期：2015.10.26 
//修改日期：2015.10.26 10:02
//备注说明：无
//==========================================================
u8 Uart_Process(void)
{
	vu8 i;
#if HW_UART_SUPPORT
	u8 kind=0xff;
	u8 data=0;
	u8 str[(FRAME_LEN_MAX-FRAME_LEN_MIN)+1];//收发数据缓冲
//	if(SaveData.Sys_Setup.Bus_Mode==0)//串口有效
	{
		if (ComBuf.rec.end)//接收数据结束
		{data=1;
			memset(str,'\0',(FRAME_LEN_MAX-FRAME_LEN_MIN+1));//清空缓冲
			{
				//memcpy(str,&ComBuf.rec.buf[PDATASTART-1],ComBuf.send.len-FRAME_LEN_MIN);//数据包
				kind=ComBuf.rec.buf[PFRAMEKIND];//命令字
				for(i=0;i<6;i++)
				{
					Test_Dispvalue.Main_valuebuff[i]=ComBuf.rec.buf[1+i];
					Test_Dispvalue.Secondvaluebuff[i]=ComBuf.rec.buf[8+i];
					
				}
				//Test_Dispvalue.Test_R=
				if(ComBuf.rec.buf[7])
					Test_Dispvalue.Unit[0]=1;
				else
					Test_Dispvalue.Unit[0]=0;
				
				Test_Dispvalue.Rangedisp=ComBuf.rec.buf[14];
					

			}
			//准备接收下一帧数据sprintf
			ComBuf.rec.end=0;//接收缓冲可读标志复位
			ComBuf.rec.ptr=0;//接收指针清零
		}
	}
//	WriteString_Big(0, 150, (uint8_t *)&ComBuf.rec.buf[1]);
	ComBuf.rec.end=0;

//	switch(kind)
//	{
//		case FRAME_READ_RESULT://读取结果
//			//串口发送测试数据:电压(5)+电阻(6)+时间(4)+分选(1)=16字节
//			switch (GetSystemMessage())//系统信息
//			{
//				case MSG_ABORT:
//					kind=0x9B;//测试中止
//					break;
//				case MSG_PASS:
//					kind=0x91;//测试通过
//					break;
//				case MSG_HIGH:
//					kind=0x92;//上限报警
//					break;
//				case MSG_LOW:
//					kind=0x92;//下限报警
//					break;
//				default:
//					kind=0x90;//正常测试
//					break;
//			}		
//			ComBuf.send.buf[1+5+6+4]=kind;
//			ComBuf.send.begin=0;
//			ComBuf.send.len=PackStandFrame(ComBuf.send.buf , &ComBuf.send.buf[1] , 16  );
////			if(SendDataToCom()==0)//发送成功判别
////			{
////			//	Delay_1ms(100);//延时
////			//	SendDataToCom();//发送
////			}
//			break;
//		
//		case FRAME_START://启动
//			SetSystemStatus(SYS_STATUS_TEST);//系统状态-启动测试
//			break;

//		case FRAME_RESET://复位
//			//SetSystemStatus(SYS_STATUS_IDLE);//系统状态-待机
//			break;

//		case FRAME_WRITE_SN://写序列号
//			break;
//		
//		case FRAME_CLR_BOOT_NUM://清开机次数
//			break;
//		
//		case FRAME_DATA://数据帧
//			break;

//		default:
//			break;
//	}
	return data;
#endif
}

//==========================================================
//函数名称：Uart_Process
//函数功能：串口处理
//入口参数：无
//出口参数：无
//创建日期：2015.10.26 
//修改日期：2015.10.26 10:02
//备注说明：无
//==========================================================
u8 Uart3_Process(void)
{
	vu8 i;
#if HW_UART_SUPPORT
	u8 kind=0xff;
	u8 data=0;
	u8 str[(FRAME_LEN_MAX-FRAME_LEN_MIN)+1];//收发数据缓冲
//	if(SaveData.Sys_Setup.Bus_Mode==0)//串口有效
	{
		if (ComBuf3.rec.end)//接收数据结束
		{data=1;
			memset(str,'\0',(FRAME_LEN_MAX-FRAME_LEN_MIN+1));//清空缓冲
			{
				memcpy(str,&ComBuf.rec.buf[PDATASTART-1],ComBuf.send.len-FRAME_LEN_MIN);//数据包
				kind=ComBuf3.rec.buf[PFRAMEKIND];//命令字
                switch(kind)
                {
                    case 0:// 设置数据
                        
                    break;
                    case 1://单次触发
                        
                    break;
                    case 2://请求发送
                        
                    break;
                    default:
                        
                    break;
                
                }
//				for(i=0;i<6;i++)
//				{
//					Test_Dispvalue.Main_valuebuff[i]=ComBuf.rec.buf[1+i];
//					Test_Dispvalue.Secondvaluebuff[i]=ComBuf.rec.buf[8+i];
//					
//				}
//				//Test_Dispvalue.Test_R=
//				if(ComBuf.rec.buf[7])
//					Test_Dispvalue.Unit[0]=1;
//				else
//					Test_Dispvalue.Unit[0]=0;
//				
//				Test_Dispvalue.Rangedisp=ComBuf.rec.buf[14];
					

			}
			//准备接收下一帧数据sprintf
			ComBuf3.rec.end=0;//接收缓冲可读标志复位
			ComBuf3.rec.ptr=0;//接收指针清零
		}
	}
//	WriteString_Big(0, 150, (uint8_t *)&ComBuf.rec.buf[1]);
	ComBuf3.rec.end=0;

//	switch(kind)
//	{
//		case FRAME_READ_RESULT://读取结果
//			//串口发送测试数据:电压(5)+电阻(6)+时间(4)+分选(1)=16字节
//			switch (GetSystemMessage())//系统信息
//			{
//				case MSG_ABORT:
//					kind=0x9B;//测试中止
//					break;
//				case MSG_PASS:
//					kind=0x91;//测试通过
//					break;
//				case MSG_HIGH:
//					kind=0x92;//上限报警
//					break;
//				case MSG_LOW:
//					kind=0x92;//下限报警
//					break;
//				default:
//					kind=0x90;//正常测试
//					break;
//			}		
//			ComBuf.send.buf[1+5+6+4]=kind;
//			ComBuf.send.begin=0;
//			ComBuf.send.len=PackStandFrame(ComBuf.send.buf , &ComBuf.send.buf[1] , 16  );
////			if(SendDataToCom()==0)//发送成功判别
////			{
////			//	Delay_1ms(100);//延时
////			//	SendDataToCom();//发送
////			}
//			break;
//		
//		case FRAME_START://启动
//			SetSystemStatus(SYS_STATUS_TEST);//系统状态-启动测试
//			break;

//		case FRAME_RESET://复位
//			//SetSystemStatus(SYS_STATUS_IDLE);//系统状态-待机
//			break;

//		case FRAME_WRITE_SN://写序列号
//			break;
//		
//		case FRAME_CLR_BOOT_NUM://清开机次数
//			break;
//		
//		case FRAME_DATA://数据帧
//			break;

//		default:
//			break;
//	}
	return data;
#endif
}

////全局变量
//u8 WaitRecTimeOver;

//==========================================================
//函数名称：PackStandFrame
//函数功能：将数据打包成帧
//入口参数：*framebuf:帧数据缓冲
//			*datbuf:数据缓冲
//			len:数据长度
//出口参数：一帧数据长度
//创建日期：2014.04.11
//修改日期：2014.04.11 10:28
//备注说明：旧的发送协议
//开始(0xAB)，电压(5) 电阻(6) 时间(4)，分选(1)，结束(0xAF)=18字节
//==========================================================
int8_t PackStandFrame(int8_t * framebuf, int8_t * datbuf, int8_t len)
{
	if(len>(SEND_LEN_MAX-2))//数据的最大长度
		len=(SEND_LEN_MAX-2);
	framebuf[0]=UART_SEND_BEGIN;//帧头
	memcpy(&framebuf[1], datbuf, len);//数据
	framebuf[len+1]=UART_SEND_END;//帧尾
	return (len+1);//返回一帧数据长度
}

const u32 UNIT_VALUE[15]=
{
	1,1E3,1E6

};
Sort_TypeDef Input_compvalue(Disp_Coordinates_Typedef *Coordinates)
{
	u8 key;
	u8 page=0;
	u8 disp_flag=1;
//	u8 index=0;
	u8 disp_cound=0;
	u8 disp_pow=0;
//	u8 input_flag=0;
//	u32 unit_c;
	u8 dot=5;//小数点
	u8 dot_num=0;
	vu8 While_flag=1;
	vu32 keynum=0;
	vu8 Disp_buff[10];
	vu8 key_count;
	vu8 dot_num1;
	vu8 del_flag=0;
	Sort_TypeDef   Sort_set;
	
//	u8 unit=0;//单位
	u32 Word;
	
	float conp_value=0;
//	float old_value;
	key_count=0;
//	old_value=SaveData.SetParameter.Nominal_value.comp_value;
//	if(SaveData.SysParameter.language==0)
//		Disp_Inputback("输入数值");//背景色
//	else
//		Disp_Inputback("INPUTNUM");//背景色
	LCD_DrawRect( Coordinates->xpos, Coordinates->ypos,Coordinates->xpos+Coordinates->lenth , 
	Coordinates->ypos+16 , Red );
	while(While_flag)
	{
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			disp_flag=1;
            Key_Beep();
			switch(key)
			{
				case Key_F1:
					//unit_c=UNIT_VALUE[DispBuf[disp_cound]-'0'];
						conp_value*=pow(10,5-(disp_cound-dot_num));
						//Sort_set.Num=	conp_value *unit_c/pow(10,dot);
						Sort_set.Num=conp_value;
						Sort_set.Dot=dot;
						if(page==0)
							Sort_set.Unit=0;
						else
							Sort_set.Unit=4;
							
						While_flag=0;
					break;
				case Key_F2:
						conp_value*=pow(10,5-(disp_cound-dot_num));
						//Sort_set.Num=	conp_value *unit_c/pow(10,dot);
						Sort_set.Num=conp_value;
						Sort_set.Dot=dot;
						if(page==0)
							Sort_set.Unit=1;
						else
							Sort_set.Unit=5;
						While_flag=0;
					break;
				case Key_F3:
					conp_value*=pow(10,5-(disp_cound-dot_num));
						//Sort_set.Num=	conp_value *unit_c/pow(10,dot);
						Sort_set.Num=conp_value;
						Sort_set.Dot=dot;
						if(page==0)
							Sort_set.Unit=2;
						else
							Sort_set.Unit=6;
						While_flag=0;
					break;
				case Key_F4:
						if(page==0)
						{
						conp_value*=pow(10,5-(disp_cound-dot_num));
						//Sort_set.Num=	conp_value *unit_c/pow(10,dot);
						Sort_set.Num=conp_value;
						Sort_set.Dot=dot;
						Sort_set.Unit=3;
						While_flag=0;
						}
					break;
				case Key_F5:
					if(page)
						page=0;
					else
						page=1;
				
					break;
				case Key_Disp:
					SetSystemStatus(SYS_STATUS_TEST);
					While_flag=0;
					Sort_set.Updata_flag=0;
				break;
				case Key_SETUP:
					While_flag=0;
					Sort_set.Updata_flag=0;
				SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
				break;
				case Key_DOWN:
				break;
				case Key_NUM1:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='1';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
							
					}
				break;
				case Key_NUM2:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='2';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM3:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='3';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM4:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='4';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
					
				break;
				case Key_NUM5:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='5';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM6:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='6';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM7:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='7';
						
						Word=Disp_buff[disp_cound]-'0';
						key_count++;
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM8:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='8';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM9:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='9';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM0:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='0';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_DOT:
					if(dot_num==0)
					{
						dot_num1=key_count;
						Disp_buff[key_count]='.';
						dot_num=1;
						key_count++;
						disp_cound++;
					}
					break;
				case Key_BACK:
					if(key_count>0)
					{	key_count--;
						Disp_buff[key_count]=' ';
						if(dot_num1==key_count)
						{
							dot_num=0;
							dot_num1=0;
							
						}
						del_flag=1;
					
					}
//					else
//					{
//						if(Disp_buff[key_count]==0)
//								Disp_buff[key_count]='-';
//							else if(Disp_buff[key_count]=='-')
//								Disp_buff[key_count]='+';
//							else
//								Disp_buff[key_count]='-';
//							key_count++;
//					}
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
			if(disp_flag)
			{
				disp_flag=0;
				Disp_button_Num_Input(page);
				Colour.Fword=White;
				Colour.black=Red;
				if(del_flag)
				{
					PutChar( Coordinates->xpos+(key_count+1)*10, Coordinates->ypos, 
					Disp_buff[key_count], Colour.Fword, Colour.black ); 
					del_flag=0;
					
				}
				else if(key_count>0)				
					PutChar( Coordinates->xpos+key_count*10, Coordinates->ypos, 
					Disp_buff[key_count-1], Colour.Fword, Colour.black ); 
			
			}
				
				
			
			
		}
			
	}
		
		
	return Sort_set;
}
	
	

//数字键输入显示
Sort_TypeDef Disp_NumKeyboard_Set(Disp_Coordinates_Typedef *Coordinates )
{
	vu8 While_flag=1;
	vu8 Disp_buff[10];
	vu8 key,i;
	vu8 dispflag=1;
	vu8 dot_num=0,dot_num1=0;
//	vu8 page=0;
	vu32 keynum=0;
	vu8 key_count=0;
//	vu32 Num[6]={1,10,100,1e3,1e4,1e5};
	Sort_TypeDef   Sort_set;
	Sort_set.Dot=0;
	Sort_set.Num=0;
	Sort_set.Unit=0;
	Sort_set.Num=0;
	for(i=0;i<6;i++)
	Disp_buff[i]=' ';
	Disp_buff[7]=0;
	
	while(While_flag)
	{
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			dispflag=1;
            Key_Beep();
			switch(key)
			{
				case Key_F1:
					
					Sort_set.Unit=0;
					
					While_flag=0;
					if(key_count<NUM_LENTH)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					
					}
						
					
					Sort_set.Updata_flag=1;
					
				break;
				case Key_F2:
					Sort_set.Unit=1;
					
					While_flag=0;
					if(key_count<NUM_LENTH)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					
					}
						
					
					Sort_set.Updata_flag=1;
					
				break;
				case Key_F3:
					
				break;
				case Key_F4:
					
				break;
				case Key_F5:
					
				break;
				case Key_Disp:
					SetSystemStatus(SYS_STATUS_TEST);
					While_flag=0;
					Sort_set.Updata_flag=0;
				break;
				case Key_SETUP:
					While_flag=0;
					Sort_set.Updata_flag=0;
				SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
				break;
				case Key_DOWN:
				break;
				case Key_NUM1:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='1';
						
						
							
						
						key_count++;
							
					}
				break;
				case Key_NUM2:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='2';
						key_count++;
					}
				break;
				case Key_NUM3:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='3';
						key_count++;
					}
				break;
				case Key_NUM4:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='4';
						key_count++;
					}
					
				break;
				case Key_NUM5:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='5';
						key_count++;
					}
				break;
				case Key_NUM6:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='6';
						key_count++;
					}
				break;
				case Key_NUM7:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='7';
						key_count++;
					}
				break;
				case Key_NUM8:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='8';
						key_count++;
					}
				break;
				case Key_NUM9:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='9';
						key_count++;
					}
				break;
				case Key_NUM0:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='0';
						key_count++;
					}
				break;
				case Key_DOT:
					
					if(key_count<NUM_LENTH&&key_count>0)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					}
//					else 
//					{
//						if(Disp_buff[key_count]==0)
//							Disp_buff[key_count]='-';
//						else if(Disp_buff[key_count]=='-')
//							Disp_buff[key_count]='+';
//						else
//							Disp_buff[key_count]='-';
//						key_count++;
//							
//					
//					
//					
//					
//					}
				break;
				case Key_BACK:
					if(key_count>0)
					{	key_count--;
						Disp_buff[key_count]=' ';
						if(dot_num1==key_count)
						{
							dot_num=0;
							dot_num1=0;
							
						}
					
					}
					else
					{
						if(Disp_buff[key_count]==0)
								Disp_buff[key_count]='-';
							else if(Disp_buff[key_count]=='-')
								Disp_buff[key_count]='+';
							else
								Disp_buff[key_count]='-';
							key_count++;
					}
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
		if(dispflag)
		{
			dispflag=0;
			LCD_DrawRect( Coordinates->xpos, Coordinates->ypos,Coordinates->xpos+Coordinates->lenth , Coordinates->ypos+16 , Red );
			Colour.Fword=White;
			Colour.black=Red;
			WriteString_16(Coordinates->xpos, Coordinates->ypos, Disp_buff,  0);
			//dispflag=0;
		}
	
	}
	for(i=key_count;i<NUM_LENTH;i++)
		Disp_buff[i]='0';
	for(i=0;i<NUM_LENTH;i++)
	{
		if(Disp_buff[0]>='0'&&(Disp_buff[0]<='9'))
		{
			if(Disp_buff[i]>='0'&&(Disp_buff[i]<='9'))
			{
			
				if(dot_num1>i)
				{
					Sort_set.Num*=10;
					Sort_set.Num+=Disp_buff[i]-'0';
				
				}
				else
				{
					Sort_set.Num*=10;
					Sort_set.Num+=Disp_buff[i]-'0';
				
				
				}
			}
			
			
			//Sort_set.Num+=(Disp_buff[key_count-1]-'0');
		
		
		}
	
	
	
	}
	Sort_set.Dot=dot_num1;
//	if(Disp_buff[0]>='0'&&(Disp_buff[0]<'9'))
//		{
//			if(Disp_buff[key_count-1]!='.')		
//			{
//				Sort_set.Num*=Num[key_count-dot_num-1];
//				Sort_set.Num+=(Disp_buff[key_count-1]-'0');//*Num[key_count-dot_num-1];
//				
//			}				
//			//*(Disp_buff[key_count-1]-'0'))+=Num[key_count-dot_num-1];
//			else
//			{
//			
//			}
//		
//		
//		}
//			
//		else 
//			;//(Disp_buff[key_count-1]-'0')*Sort_set.Num+=Num[key_count-dot_num-2];
	return Sort_set;

}
//电阻设置
Sort_TypeDef Disp_Set_Num(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num,Sort_num1;
	Disp_button_Num_time();
	Sort_num=Disp_NumKeyboard_Set(Coordinates);
	Sort_num1=Time_Set_Cov(&Sort_num);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num1;

}
//电压设置
Sort_TypeDef Disp_Set_CompNum(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num,Sort_num1;
	Disp_button_Num_Freq();
	Sort_num=Disp_NumKeyboard_Set(Coordinates);
	Sort_num1=Input_Set_Cov(&Sort_num);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num1;

}
Sort_TypeDef Disp_Set_InputNum(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num1;
	Disp_button_Num_Input(0);
	Sort_num1=Input_compvalue(Coordinates);
	//if(SaveData.Limit_Tab.Mode==0)
	Sort_num1=Input_Set_Cov(&Sort_num1);
	//else
	//Sort_num1=Input_Set_CovPre(&Sort_num1);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num1;
	
	

}
Sort_TypeDef Disp_Set_InputpreNum(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num1;
	Disp_button_Num_Input(0);
	Sort_num1=Input_compvalue(Coordinates);//Input_Set_Cov
	if(SaveData.Limit_Tab.Mode==0)
		Sort_num1=Input_Set_Cov(&Sort_num1);
	else
		Sort_num1=Input_Set_CovPre(&Sort_num1);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num1;
	
	

}


vu16 Freq_Set_Num(Disp_Coordinates_Typedef *Coordinates)//频率设置
{
	Sort_TypeDef Sort_num;
//	vu8 i;
	vu16 num;
	Disp_button_Num_Freq();
	Sort_num=Disp_NumKeyboard_Set(Coordinates);
	num= Debug_Value(&Sort_num);
	//Sort_num1=Freq_Set_Cov(&Sort_num);
	
	
		
	return num;
	
}
vu8 Avg_Set_Num(Disp_Coordinates_Typedef *Coordinates)//平均数设置
{
	Sort_TypeDef Sort_num,Sort_num1;
	vu8 i;
	vu8 num;
	Disp_button_Num_Avg();
	Sort_num=Disp_NumKeyboard_Set(Coordinates);
	Sort_num1=Freq_Set_Cov(&Sort_num);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
	if(Sort_num1.Dot==0)
	{
		if(Sort_num1.Num>32)
			Sort_num1.Num=32;
	
	} else
	if(Sort_num1.Dot==1)
	{
		for(i=0;i<5;i++)
		{
			if(Sort_num1.Num>0&&Sort_num1.Num<=9)
				break;
			else
				Sort_num1.Num/=10;
		
		
		}
	
	}else if(Sort_num1.Dot==2)
	{
		for(i=0;i<5;i++)
		{
			if(Sort_num1.Num>10&&Sort_num1.Num<=99)
				break;
			else
				Sort_num1.Num/=10;
		
		
		}
		if(Sort_num1.Num>32)
			Sort_num1.Num=32;
	
	
	
	}else
	{
		Sort_num1.Num=32;
	
	
	}
	num=Sort_num1.Num;
	if(num==0)
		num=1;
		
	return num;
	
}
void Set_daot(vu8 *buff,vu8 dot)
{
	vu8 i;
	for(i=0;i<dot;i++)
	{
		buff[5-i]=buff[5-i-1];
	
	
	}
	if(dot==0)
		buff[5]=' ';
	else
	buff[5-dot]='.';
	buff[6]=0;


}
int8_t V_Test_Comp(int32_t value)
{
	vu8 data;
	
	if(value>Save_Res.Set_Data.V_high.mid_data)
		data=VH_FAIL;
	else if(value<Save_Res.Set_Data.V_low.mid_data || negvalm == 1)
		data=VL_FAIL;
	else
		data=ALL_PASS;
	return data;
		


}
int8_t R_Test_Comp(int32_t value)
{
	vu8 data;
	
	if(value>Save_Res.Set_Data.High_Res.mid_data)
		data=RH_FAIL;
	else if(value<Save_Res.Set_Data.Res_low.mid_data)
		data=RL_FAIL;
	else
		data=ALL_PASS;
	return data;

}
void Comp_prompt(int8_t value)
{	
	if(value==ALL_PASS)
	{
		Pass_Led();	
	}
	else
	{
		if(Save_Res.Set_Data.openbeep==1 || nodisp_v_flag == 0)
		{
			Fail_led();	
		}else{
			No_Comp();
		}
	}
	
	switch(Save_Res.Set_Data.beep)
	{
		case 0://蜂鸣器关闭
			Beep_Off();
			break;
		case 1://合格讯响
			if(value==ALL_PASS)
				Beep_on();
			else
				Beep_Off();
			break;
		case 2://不合格讯响
			if(Save_Res.Set_Data.openbeep==1)
			{
				if(value==ALL_PASS)
					Beep_Off();
				else
					Beep_on();
			}else{
				if(nodisp_v_flag == 1)
				{
					Beep_Off();
				}else{
					if(value==ALL_PASS)
						Beep_Off();
					else
						Beep_on();
				}
			}
				
			
			break;
		default:
			Beep_Off();
			break;
	
	}



}
void Test_Comp(All_Compvalue_Typedef *pt)
{
	float value;
	float data;
	vu8 i;
	for(i=0;i<5;i++)
	{
		data=pt->all[0].buff[i];
		value+=(pow(10,4-i)*data);//从下位机接收来的数据
	}
	value=value*pow(1000,pt->all[0].Unit);
	value/=(pow(10,pt->all[0].Dot));
	
	if(SaveData.Limit_Tab.Mode)//百分比比较
	{
		for(i=0;i<9;i++)
		{
			if(SaveData.Limit_Tab.Comp_Value[i].low.Num!=0)
			{
				if(value>Comp_Testvalue.comp_highvalue[i])//大于上限
					Count_buff[10]++;
				else if(value<Comp_Testvalue.comp_lowvalue[i])//小于下限 加合格标志位
									//计数
					Count_buff[10]++;
				else Count_buff[i]++;
			
			
			}
		
		
		}
	
	
	
	}
//	else
//	{
//	
//	
//	}//ABS比较

}//Comp_Testvalue  Comp_Testvalue
float Inttofloat(Sort_TypeDef *pt)//int转换为float  INT包含小数点和单位
{
	float value;
//	vu8 i;
	value=pt->Num;
	value=value*pow(1000,pt->Unit);
	value/=pow(10,pt->Dot);
	return value;
}
void Set_Compbcd_float(void)//把设置比较数据转换为float数据  把这个数据与标称值进行运算，得到
	//可以直接比较的值  这个分为2种，一种是ABS一种是%
{
	vu8 i;
	float value;
	for(i=0;i<9;i++)
	{
		Comp_Testvalue.comp_highvalue[i]=Inttofloat( &SaveData.Limit_Tab.Comp_Value[i].high);
		Comp_Testvalue.comp_lowvalue[i]=Inttofloat( &SaveData.Limit_Tab.Comp_Value[i].low);
	
	
	}
	Comp_Testvalue.comp_highvalue[9]=Inttofloat( &SaveData.Limit_Tab.Comp_Value_2nd.high);
	Comp_Testvalue.comp_lowvalue[9]=Inttofloat( &SaveData.Limit_Tab.Comp_Value_2nd.low);
	value=Inttofloat(&SaveData.Limit_Tab.Nom);
	if(SaveData.Limit_Tab.Mode)//百分比比较
	{
		for(i=0;i<9;i++)
		{
			Comp_Testvalue.comp_highvalue[i]=(100000+Comp_Testvalue.comp_highvalue[i])*value;
			Comp_Testvalue.comp_highvalue[i]/=100000;
			Comp_Testvalue.comp_lowvalue[i]=(100000-Comp_Testvalue.comp_lowvalue[i])*value;
			Comp_Testvalue.comp_lowvalue[i]/=100000;
			
		}
	
	
	
	}
	else
	{
		
		for(i=0;i<9;i++)
		{
			Comp_Testvalue.comp_highvalue[i]+=value;
			Comp_Testvalue.comp_lowvalue[i]-=value;
			
		}
		//Comp_Testvalue.comp_highvalue[9]=
		
	
	
	}//ABS比较
	if(Save_Res.Set_Data.dispvr > 2)
		Save_Res.Set_Data.dispvr=0;



}
int32_t Input_int(int8_t *pt)
{
	u32 value=0;
	u8 i,j=0;
//	u8 dot=0;
	for(i=0;i<5;i++)
	{
		if(*(pt+i+1)>='0')
		{
			value*=10;
			value+=*(pt+i+1)-'0';
			j++;
			
		}
//		else
//			dot=4-i;
	
	}
	return value;


}
//电阻BCD转换为INT
int32_t BCDtoInt(int8_t *pt)
{
	u32 value=0;
	u8 i,j=0;
	u8 dot=0;
	if(*(pt+2)=='-')
	{
		value=0xfffffff;
		return value;
	}
	for(i=0;i<5;i++)
	{
		if(*(pt+1+i)>='0')
		{
			value*=10;
			value+=*(pt+1+i)-'0';
			j++;
			
		}
		else
			dot=4-i;
	
	}
	if(Test_Dispvalue.Unit[0])
		value*=1e6;
	else
		value*=1e3;
	value/=pow(10,dot);
	
	return value;
}
//电压BCD转换为INT
int32_t VBCDtoInt(int8_t *pt)
{
	u32 value=0;
	
	u8 i,j=0;
	u8 dot=0;
	for(i=0;i<5;i++)
	{
		if(*(pt+1+i)>='0')
		{
			value*=10;
			value+=*(pt+1+i)-'0';
			j++;
			
		}
		else
			dot=4-i;
		
	
	}
	Test_Unit.V_dot=dot;
	value*=1000;
	value/=pow(10,dot);
	if(*pt=='-')
		Test_Unit.V_Neg=0;
	else
		Test_Unit.V_Neg=1;
	
	return value;
}
void V_BCD_Int(int32_t data)
{
	u8 i;
	vu16 word;
	if(data>60e3)
	{
		for(i=0;i<5;i++)
		DispBuf[i]='-';
	}
	else 
		if(data>6e3)
		{
			word=data/10;
			Hex_Format((word),2,4,TRUE);
		
		
		}
		else
			Hex_Format((data),3,4,TRUE);
			


}
void BCD_Int(int32_t data)
{
	u8 i;
	vu16 word;
	if(data>30e6)
	{
		Test_Unit.Res_dot=1;
		for(i=0;i<5;i++)
		DispBuf[i]='-';
	
	}else
		if(data>=10e6)//xx.xx
		{
			Test_Unit.Res_dot=1;
			word=data/10e3;
			Hex_Format((word),2,4,TRUE);
				
		}else
			if(data>=10e5)//x.xxx
			{
				Test_Unit.Res_dot=1;
				word=data/10e2;
				Hex_Format((word),3,4,TRUE);
			}
			else
				if(data>=10e4)//xxx.xm
				{
					Test_Unit.Res_dot=0;
					word=data/10e1;
					Hex_Format((word),1,4,TRUE);
				}else
				if(data>=10e3)
				{
					Test_Unit.Res_dot=0;
					word=data/10;
					Hex_Format((word),2,4,TRUE);
				}
				else
				{
					Test_Unit.Res_dot=0;
					word=data/10;
					Hex_Format((word),2,4,TRUE);
				
				
				}


}
float Debug_Res(float a,float b,float c)
{
	//float d;
	return a*b/c;
}
void  Write_Usbdata ( uint8_t  *buffer,uint32_t num)
{
    int32_t  fdw;
//    int32_t  fdr;
//    uint32_t  tot_bytes_written;
    uint32_t  bytes_written;
    char filename[30];
//    strcpy(filename,(char*)"JK2520B");
 //       filename=(char*)"JK2520B";
        strcpy(filename,(char*)Save_Res.Sys_Setvalue.textname); 
        strcat(filename,(char*)".TXT");
 //       
        fdw = FILE_Open((uint8_t *)filename, RDWR);
        if (fdw > 0) 
		{
			usb_oenflag=1;

			bytes_written = FILE_Write(fdw, buffer, num);//MAX_BUFFER_SIZE);

			FILE_Close(fdw);
            
        } 
		else
			usb_oenflag=0;

    } 
void input_password(Disp_Coordinates_Typedef *Coordinates )
{
  	char While_flag=1;
	char Disp_buff[10];
	u8 key,i;
	char dispflag=1;
	char dot_num=0,dot_num1=0;
    char password_flag=0;
//	vu8 page=0;
	uint32_t keynum=0;
	char  key_count=0;
	for(i=0;i<9;i++)
	Disp_buff[i]=0;
	//Disp_buff[8]=0;
	Disp_Button_value1(2);
	while(While_flag)
	{
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
            //Key_Beep()
            password_flag=0;
            Key_Beep();
			dispflag=1;
			switch(key)
			{
				case Key_F1://退出
					 While_flag=0;
                       SetSystemStatus(SYS_STATUS_TEST);

					
				break;
				case Key_F3://取消
                    
					
				break;
				case Key_F2://确认
                   if(strcmp(PASSWORD,Disp_buff))//比较函数  当相等时  返回0
                   {//密码错误
                       key_count=0;
                       for(i=0;i<8;i++)
                        Disp_buff[i]=' ';
                       password_flag=1;
                       
                   
                   
                   }
                   else //密码正确
                   {
                        While_flag=0;
                       SetSystemStatus(SYS_STATUS_USERDEBUG);
                   
                   }
                       
					
				break;
				case Key_F4:
					
				break;
				case Key_F5:
					
				break;
				case Key_Disp:
//					SetSystemStatus(SYS_STATUS_TEST);
//					While_flag=0;
//					Sort_set.Updata_flag=0;
				break;
				case Key_SETUP:
//					While_flag=0;
//					Sort_set.Updata_flag=0;
//				SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
				break;
				case Key_DOWN:
				break;
				case Key_NUM1:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='1';
						
						
							
						
						key_count++;
							
					}
				break;
				case Key_NUM2:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='2';
						key_count++;
					}
				break;
				case Key_NUM3:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='3';
						key_count++;
					}
				break;
				case Key_NUM4:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='4';
						key_count++;
					}
					
				break;
				case Key_NUM5:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='5';
						key_count++;
					}
				break;
				case Key_NUM6:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='6';
						key_count++;
					}
				break;
				case Key_NUM7:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='7';
						key_count++;
					}
				break;
				case Key_NUM8:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='8';
						key_count++;
					}
				break;
				case Key_NUM9:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='9';
						key_count++;
					}
				break;
				case Key_NUM0:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='0';
						key_count++;
					}
				break;
				case Key_DOT:
					
					if(key_count<PASSWORD_LENTH&&key_count>0)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					}
//					else 
//					{
//						if(Disp_buff[key_count]==0)
//							Disp_buff[key_count]='-';
//						else if(Disp_buff[key_count]=='-')
//							Disp_buff[key_count]='+';
//						else
//							Disp_buff[key_count]='-';
//						key_count++;
//							
//					
//					
//					
//					
//					}
				break;
				case Key_BACK:
					if(key_count>0)
					{	key_count--;
						Disp_buff[key_count]=' ';
						if(dot_num1==key_count)
						{
							dot_num=0;
							dot_num1=0;
							
						}
					
					}
					else
					{
						if(Disp_buff[key_count]==0)
								Disp_buff[key_count]='-';
							else if(Disp_buff[key_count]=='-')
								Disp_buff[key_count]='+';
							else
								Disp_buff[key_count]='-';
							key_count++;
					}
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
		if(dispflag)
		{
			dispflag=0;
            LCD_DrawLine( Coordinates->xpos+13, Coordinates->ypos-2, Coordinates->xpos+13+80, Coordinates->ypos-2 , Red );
            LCD_DrawLine( Coordinates->xpos+13, Coordinates->ypos+18, Coordinates->xpos+13+80, Coordinates->ypos+18 , Red );
            for(i=0;i<9;i++)
            LCD_DrawLine( Coordinates->xpos+13+i*10, Coordinates->ypos-2, Coordinates->xpos+13+i*10, Coordinates->ypos+18 , Red );
            
			//LCD_DrawRect( Coordinates->xpos, Coordinates->ypos,Coordinates->xpos+Coordinates->lenth , Coordinates->ypos+16 , Red );
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BACK;
				if(Save_Res.Sys_Setvalue.lanage )
						WriteString_16(Coordinates->xpos-38, Coordinates->ypos, "PassWord:",  0);
				else
            WriteString_16(Coordinates->xpos-38, Coordinates->ypos, "密码:",  0);
				WriteString_16(Coordinates->xpos+15, Coordinates->ypos, ( uint8_t *)Disp_buff,  0);
            if(password_flag)
								if(Save_Res.Sys_Setvalue.lanage )
									WriteString_16(Coordinates->xpos, Coordinates->ypos+20, "PassWord Error!",  0);
								else
									WriteString_16(Coordinates->xpos, Coordinates->ypos+20, "密码错误",  0);
            else
               LCD_DrawRect( Coordinates->xpos, Coordinates->ypos+20,Coordinates->xpos+100 , Coordinates->ypos+40, LCD_COLOR_TEST_BACK );
               // WriteString_16(Coordinates->xpos, Coordinates->ypos, "         ",  0);
							if(Save_Res.Sys_Setvalue.lanage )
								WriteString_16(Coordinates->xpos, Coordinates->ypos-20, "Please input 8-bits PassWord",  0);
							else
								WriteString_16(Coordinates->xpos, Coordinates->ypos-20, "请输入8位密码",  0);
			//dispflag=0;
		}
	
	}

}
void input_num(Disp_Coordinates_Typedef *Coordinates )
{
  	char While_flag=1;
	uint8_t Disp_buff[12];
	uint8_t  key,i;
	char dispflag=1;
//	char dot_num=0,dot_num1=0;
//    char password_flag=0;
	char keynum=0;
	char key_count=0;
//	Sort_TypeDef   Sort_set;
	
	for(i=0;i<8;i++)
	Disp_buff[i]=' ';
	Disp_buff[8]=0;
	
	while(While_flag)
	{
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
 //           password_flag=0;
            Key_Beep();
			dispflag=1;
			switch(key)
			{
				case Key_F1://退出
					 
                       //SetSystemStatus(SYS_STATUS_USERDEBUG);

					
				break;
				case Key_F2://取消
                    
					
				break;
				case Key_F3://确认保存机号和日期
//                   if(strcmp(PASSWORD,Disp_buff))//比较函数  当相等时  返回0
//                   {//密码错误
//                       key_count=0;
//                       for(i=0;i<8;i++)
//                        Disp_buff[i]=' ';
//                       password_flag=1;
//                       
//                   
//                   
//                   }
//                   else //密码正确
//                   {
//                        While_flag=0;
//                       SetSystemStatus(SYS_STATUS_USERDEBUG);
//                   
//                   }//复制到内存
                       
					
				break;
				case Key_F4:
					
				break;
				case Key_F5:
                   //return  &Disp_buff[0];
					
				break;
				case Key_Disp:
//					SetSystemStatus(SYS_STATUS_TEST);
//					While_flag=0;
//					Sort_set.Updata_flag=0;
				break;
				case Key_SETUP:
//					While_flag=0;
//					Sort_set.Updata_flag=0;
//				SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
                    While_flag=0;//保存
                    dispflag=0;
                    for(i=0;i<8;i++)
                    {
                        Save_Res.fac_num[i]=Disp_buff[i];
                    
                    }
                    Savetoeeprom();
                     LCD_DrawRect( 0, Coordinates->ypos-20,Coordinates->xpos+200 , Coordinates->ypos+20, LCD_COLOR_TEST_BACK );
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
				break;
				case Key_DOWN:
				break;
				case Key_NUM1:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='1';
						
						
							
						
						key_count++;
							
					}
				break;
				case Key_NUM2:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='2';
						key_count++;
					}
				break;
				case Key_NUM3:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='3';
						key_count++;
					}
				break;
				case Key_NUM4:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='4';
						key_count++;
					}
					
				break;
				case Key_NUM5:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='5';
						key_count++;
					}
				break;
				case Key_NUM6:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='6';
						key_count++;
					}
				break;
				case Key_NUM7:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='7';
						key_count++;
					}
				break;
				case Key_NUM8:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='8';
						key_count++;
					}
				break;
				case Key_NUM9:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='9';
						key_count++;
					}
				break;
				case Key_NUM0:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='0';
						key_count++;
					}
				break;
				case Key_DOT:
                    if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='/';
						key_count++;
					}
					
//					if(key_count<NUM_LENTH&&key_count>0)
//					{
//						if(dot_num==0)
//						{
//							if(key_count>0)
//							{
//								Disp_buff[key_count]='.';
//								dot_num1=key_count;
//								key_count++;
//							
//							
//							}
//							dot_num++;
//						}
//					}
//					else 
//					{
//						if(Disp_buff[key_count]==0)
//							Disp_buff[key_count]='-';
//						else if(Disp_buff[key_count]=='-')
//							Disp_buff[key_count]='+';
//						else
//							Disp_buff[key_count]='-';
//						key_count++;
//							
//					
//					
//					
//					
//					}
				break;
				case Key_BACK:
					if(key_count)
					{	key_count--;
						Disp_buff[key_count]=' ';
//						if(dot_num1==key_count)
//						{
//							dot_num=0;
//							dot_num1=0;
//							
//						}
					
					}
//					else
//					{
//						if(Disp_buff[key_count]==0)
//								Disp_buff[key_count]='-';
//							else if(Disp_buff[key_count]=='-')
//								Disp_buff[key_count]='+';
//							else
//								Disp_buff[key_count]='-';
//							key_count++;
//					}
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
		if(dispflag)
		{
			dispflag=0;
            LCD_DrawLine( Coordinates->xpos+13, Coordinates->ypos-2, Coordinates->xpos+13+80, Coordinates->ypos-2 , Red );
            LCD_DrawLine( Coordinates->xpos+13, Coordinates->ypos+18, Coordinates->xpos+13+80, Coordinates->ypos+18 , Red );
            for(i=0;i<9;i++)
            LCD_DrawLine( Coordinates->xpos+13+i*10, Coordinates->ypos-2, Coordinates->xpos+13+i*10, Coordinates->ypos+18 , Red );
            
			//LCD_DrawRect( Coordinates->xpos, Coordinates->ypos,Coordinates->xpos+Coordinates->lenth , Coordinates->ypos+16 , Red );
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BACK;
					if(Save_Res.Sys_Setvalue.lanage )
						 WriteString_16(Coordinates->xpos-70, Coordinates->ypos, "Factory Num:",  0);
					else
            WriteString_16(Coordinates->xpos-70, Coordinates->ypos, "出厂编号:",  0);
					WriteString_16(Coordinates->xpos+15, Coordinates->ypos, ( uint8_t *)Disp_buff,  0);
//            if(password_flag)
//                WriteString_16(Coordinates->xpos, Coordinates->ypos+20, "密码错误",  0);
//            else
//               LCD_DrawRect( Coordinates->xpos, Coordinates->ypos+20,Coordinates->xpos+100 , Coordinates->ypos+40, LCD_COLOR_TEST_BACK );
               // WriteString_16(Coordinates->xpos, Coordinates->ypos, "         ",  0);
					if(Save_Res.Sys_Setvalue.lanage )
						WriteString_16(Coordinates->xpos, Coordinates->ypos-20, "Please input 8-bits Factory Num",  0);
					else
            WriteString_16(Coordinates->xpos, Coordinates->ypos-20, "请输入8位出厂编号",  0);
		}
	
	}

}

