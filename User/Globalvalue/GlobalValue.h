//==========================================================
//�ļ����ƣ�GlobalValue.h
//�ļ�������ȫ�ֱ���ͷ�ļ�
//�ļ��汾��Ver1.0
//�������ڣ�2015.10.26 
//�޸����ڣ�2015.10.28 15:38
//�ļ����ߣ��ƺ���
//��ע˵������
//ע�������
//==========================================================
#ifndef __GlobalValue_h__
#define __GlobalValue_h__
#include "stdint.h"
#include "lpc177x_8x_rtc.h"
//#include "TypeDefine.h"
//#define OVER_NO_ALARM   1//�����Ժ󣬵���ʾ-----��ʱ�� ���ж�
#define FIT_INUM    12
//==========================================================
//����汾��10
#define SOFTWARE_VERSION		(0x10)

//==========================================================
//Ĭ�ϳ������(0)
#define DEFAULT_MACHINE_ID 		(0)

//Ĭ�ϳ�������(0)
#define DEFAULT_PASSWORD 		(0)

//Ĭ�Ͽ�������(0)
#define SET_BOOT_DEFAULT		(0)

//==========================================================
//��������
#define _DEBUG_CONFIG_FALSE		(0)
#define _DEBUG_CONFIG_TRUE		(1)

//==========================================================
//�������
//#define DEBUG_SUPPORT			(_DEBUG_CONFIG_TRUE)
#define DEBUG_SUPPORT			(_DEBUG_CONFIG_FALSE)

//==========================================================
//ISP�������ã����������
#if DEBUG_SUPPORT
	#define HW_ISP_SUPPORT		(_DEBUG_CONFIG_FALSE)
#else
	#define HW_ISP_SUPPORT		(_DEBUG_CONFIG_TRUE)
#endif

//==========================================================
//Uart��������
#define HW_UART_SUPPORT			(_DEBUG_CONFIG_TRUE)

//==========================================================
//��������������
#define HW_BEEP_SUPPORT			(_DEBUG_CONFIG_TRUE)

//==========================================================
//ADֵ�˲����ã����������ȶ���
#define AD_FILTER_SUPPORT		(_DEBUG_CONFIG_TRUE)

//==========================================================
#define SETUP_MENU_MAX  	(2)//������ò˵�

#define DEBUG_MENU_MAX  	(6)//���У׼�˵�

#define PARAMETER_MENU_MAX	(3)//�������˵�

#define SYSTEM_MENU_MAX		(4)//���ϵͳ�˵�

#define CONFIG_MENU_MAX		(3)//���������(0-2)

#define PW_LENGTH 			(4)//�������λ��

#define RANGE_MAX 			(4-1)//�������

#define AD_BUF_LENGTH		(8)//A/Dֵ����������

#define MAX_R_RANGE			(5)//�����������

#define BUTTOM_X_VALUE		(84)//��ʾ������ʾ��ʾ�ĵ�һ��Xλ��

#define BUTTOM_MID_VALUE	(80)//��ʾ������ʾ��ʾ���м���λ��

#define BUTTOM_Y_VALUE		(271-30)//��ʾ������ʾ��ʾ�ĵ�һ��Yλ��

#define KEY_NUM				(3)	//����������

#define NUM_LENTH 			(5)//��ʾ����
#define NUM_FREQ 			(33+4)//��ʾ����
#define DEBUG_RANGE			(4+2)

#define SYS_MAX				(12)
//==========================================================
#define OHM 				(0xF4)	//CGROM���Ц��ַ�������Ϊ0xF4

//==========================================================
//��ʾ������X����
#define MSG_ADDR_X 			(1)
#define AUTO_ADDR_X 		(12)
#define RANGE_ADDR_X 		(17)

#define RES_ADDR_X 			(1)
#define VOL_ADDR_X 			(12)

#define DISP_UNIT_XPOS	380
#define DISP_UNIT_YPOS	92

#define PASSWORD_LENTH  8

//#define PASSWORD "12345789"
//==========================================================
//���ⳤ��
#define TITLE_LEN_MAX		(8)

//==========================================================
#define S_RX_BUF_SIZE		30
#define S_TX_BUF_SIZE		128

#define RSP_OK				0		/* ?? */
#define RSP_ERR_CMD			0x01	/* ??????? */
#define RSP_ERR_REG_ADDR	0x02	/* ??????? */
#define RSP_ERR_VALUE		0x03	/* ?????? */
#define RSP_ERR_WRITE		0x04	/* ��??? */

#define SLAVE_REG_P00		0x0000       //R_VOLU
#define SLAVE_REG_P01		0x0001      //Load_Voltage
#define SLAVE_REG_P02		0x0002      //Load_Current
#define SLAVE_REG_P03		0x0003		//Change_Voltage
#define SLAVE_REG_P04		0x0004		//Change_Current
#define SLAVE_REG_P05		0x0005		//Load_OCP
#define SLAVE_REG_P06		0x0006		//Change_OCP
#define SLAVE_REG_P07		0x0007		//Short_Time
#define SLAVE_REG_P08		0x0008		//Leak_Current
#define SLAVE_REG_P09		0x0009		//R1_Volu
#define SLAVE_REG_P10		0x000A		//R2_Volu
#define SLAVE_REG_P11		0x000B		//Temper
#define SLAVE_REG_P12		0x000C		//DHQ_Voltage
#define SLAVE_REG_P13		0x000D		//MODE
#define SLAVE_REG_P14		0x000E		//Load_Mode
#define SLAVE_REG_P15		0x000F		//Load_SET_Voltage
#define SLAVE_REG_P16		0x0010		//Load_SET_Current
#define SLAVE_REG_P17		0x0011		//Change_SET_Voltage
#define SLAVE_REG_P18		0x0012		//Change_SET_Current


struct MODS_T
{
	uint8_t RxBuf[S_RX_BUF_SIZE];
	uint8_t TxBuf[S_TX_BUF_SIZE];
	uint8_t RxCount;
	uint8_t RxStatus;
	uint8_t RxNewFlag;
	uint8_t RspCode;
	
	uint8_t TxCount;
};


extern uint8_t Send_buff2[30];
extern uint8_t Rtc_intflag;

typedef  unsigned long  vu32;
typedef  unsigned short vu16;
typedef  unsigned char  vu8;
//�����������ݶ���
//��С����͵�λ����
typedef struct
{
	vu16 standard;//��׼��ʾֵ
	vu16 ad_value;

}Debug_Value_Typedef;
typedef struct
{
	vu8 Res_dot;
	vu8 V_dot;
	vu8 V_Neg;
}Test_UnitTypedef;
extern Test_UnitTypedef Test_Unit;

typedef struct 
{
	vu8 sign;
	vu32 Num;
	vu32 Dot;
	vu32 Unit;
	vu32 Updata_flag;
	vu32 Order;
	vu32 mid_data;
}Sort_TypeDef;
typedef struct
{
	vu8 trip;//����
	vu8 speed;//�����ٶ�
	vu8 Res_comp;
	Sort_TypeDef Res_low;//��������
	vu8 V_comp;
	Sort_TypeDef V_low;
	vu8 Range;
	vu8 beep;
	Sort_TypeDef Nominal_Res;
	Sort_TypeDef High_Res;
	Sort_TypeDef Nominal_V;
	Sort_TypeDef	V_high;
	vu8 openbeep;
	vu8 dispvr;

}Set_Data_Typedef;
typedef struct
{
	vu8 Year;
	vu8 Mon;
	vu8 data;

}Data_Value_Typedef;
typedef struct
{
	vu8 Hour;
	vu8 Min;
	vu8 Sec;

}Timer_Value_Typedef;
typedef struct
{
	vu8 uart;
	vu8 buard;
    vu8 u_flag;
    vu8 plc;
	vu8 lanage;
    vu8 textname[20];
    
	
//	Timer_Value_Typedef Time;
//	Data_Value_Typedef Data;
	//vu8 
	

}Sys_Setvalue_Typedef;
typedef struct
{
	Set_Data_Typedef Set_Data;
	Debug_Value_Typedef Debug_Value[DEBUG_RANGE];
	Sys_Setvalue_Typedef Sys_Setvalue;
	float clear;
	float V_Clear;
	vu8 softswitch;
    vu8 fac_num[10];
	vu8 open;

}Save_Res_Typedef;

extern Save_Res_Typedef Save_Res;



//


//��ֵ�����Զ���
//typedef struct
//{
//	vu8 *Title;//����
//	vu16 Num;//��ֵ
//	vu8  Dot;//С����(0-3: xxxx xxx.x xx.xx x.xxx)
//	vu8  Unit;//��λ(0-1: mOhm��Ohm)
//	vu16 Max;//���ֵ
//	vu16 Min;//��Сֵ
//}NumBox_TypeDef;
typedef struct
{
	float first_value;
	float second_value;
	float third_value;
	float fourth_value;
	float comp_highvalue[10];
	float comp_lowvalue[10];
	


}Comp_Testvalue_Typedef;
extern Comp_Testvalue_Typedef  Comp_Testvalue;
//typedef struct
//{
//	vu8  buff[5];
//	vu8 dot;
//	vu8 unit;


//}
typedef struct
{
	vu8 Main_Funbuff[8];
	vu8 Main_Secondbuff[8];
	vu8 Main_Vmbuff[8];
	vu8 Main_Imbuff[8];


}Test_DispBCD_Typedef;
typedef struct
{
	vu8 Main_flag;
	vu8 Second_falg;

}Main_Second_TypeDed;

typedef struct
{
	vu8 buff[5];
	vu32 Num;
	vu32 Dot;
	vu32 Unit;


}Range_Set_Typedef;
typedef struct
{
	Range_Set_Typedef all[4];

}All_Compvalue_Typedef;
extern All_Compvalue_Typedef Comp_Change;
typedef struct
{
	vu32 test;
	vu32 page;

}Param_Typedef;
//==========================================================
typedef struct
{
	vu32 Auto;
	vu32 Range;

}AotoRange_Typedef;

//������(20��)��������
typedef struct 
{
	Param_Typedef Param;//����(���б�)
	vu32 Freq;//Ƶ��(���б�)
	vu32 Level;//��ƽ(1V  0.3V)
	vu32 Trig;//�ⲿ��������(0-3���ڲ� �ֶ� �ⲿ ����)
	vu32 Alc;//���ƽ(0-1��Ĭ�Ϲ�0)
	vu32 Rsou;//����(0-1��Ĭ�Ϲ�0)
    Sort_TypeDef Trig_time;
    Sort_TypeDef Temp_time;
    vu32 Dev_a;
    vu32 Dev_b;
    AotoRange_Typedef Range;
    vu32 Bias;
    vu32 Speed;
    vu32 Avg;
    vu32 V_i;
    vu32 Dcr;
    AotoRange_Typedef DC_Range;
    vu32 DC_Level;
    Sort_TypeDef Ref_A; 
    Sort_TypeDef Ref_B;
	vu32 beep;
	vu32 buad;
//	vu8  Count;
}Main_Func_TypeDef;
typedef struct
{
	vu8 param;
	vu8 freq;
	vu8 level;
	vu8 trig;
	vu8 rsou;
	
	vu8 range;
	vu8 bisa;
	vu8 speed;
	vu8 avg;
	vu8 v_i;
	vu8 beep;
	vu8 buad;
	Sort_TypeDef Trig_time;
    Sort_TypeDef Temp_time;
	

}Save_Main_Funce_Typedef;
typedef struct
{
	Sort_TypeDef Mainvalue;
	Sort_TypeDef Secondvalue;
	Sort_TypeDef Vmvalue;
	Sort_TypeDef Imvalue;
	vu8 Rangedisp;
	vu8 Main_valuebuff[10];
	vu8 Secondvaluebuff[10];
	vu8 Vmvaluebuff[10];
	vu8 Imvaluebuff[10];
	vu8 Dot[4];
	vu8 Unit[4];
	vu32 Test_V;
	vu32 Test_R;
	


}Test_Dispvalue_TypeDef;
extern Test_Dispvalue_TypeDef Test_Dispvalue; 
typedef struct
{
    vu32 Open;//��·
    vu32 Short;//��·
    vu32 Load;//����
    vu32 Spot;//У����
    vu32 Freq;//Ƶ��
    Sort_TypeDef Ref_A;
    Sort_TypeDef Open_A;
    Sort_TypeDef Short_A;
    Sort_TypeDef Load_A;
    vu32 Cable;//���³���
    vu32 Mode;//��ʽ
    vu32 Param;//����
    Sort_TypeDef Ref_B;
    Sort_TypeDef Open_B;
    Sort_TypeDef Short_B;
    Sort_TypeDef Load_B;

}User_Correction_Typedef;
typedef struct
{
    Sort_TypeDef low;
    Sort_TypeDef high;

}Comp_Value_Typedef;
//�����б�����
typedef struct
{
    vu32 Param;//����
    Sort_TypeDef Nom;//���
    vu32 Mode;//��ʽ
    vu32 Aux;//����
    vu32 Comp;//�ȽϿ���
    Comp_Value_Typedef Comp_Value[9];
    Comp_Value_Typedef Comp_Value_2nd;
	vu32 count;
}Limit_Tab_Typedef;

typedef struct
{
    vu32 Value;
    vu32 Unit; 
} Sweep_Value_Typedef;
typedef struct
{
    vu32 Freq;
    vu32 Level;
    vu32 Lmt;
    Sweep_Value_Typedef Low;
    Sweep_Value_Typedef High;
    Sweep_Value_Typedef Delay_Time;

}List_Value_Typedef;
//�б�ɨ������
typedef struct
{
    vu32 mode;
    List_Value_Typedef  List_Value[200];


} List_Sweep_Typedef;

//ϵͳ����
typedef struct
{
    vu32 Main_Func;
    vu32 Pass_Beep;
    vu32 Fail_Beep;
    vu32 Language;
    vu32 Password;
    vu32 Menu_Disp;
	Data_Value_Typedef Data_Value;
	Timer_Value_Typedef Timer_Value;
    vu32 Baue_Rate;
	
    vu32 Bias;
    vu32 Bus_Mode;
    vu32 GP_Addr;
    vu32 Talk_Only;
}Sys_Setup_Typedef;

typedef struct
{
	vu8 fun;
	vu8 num;
	vu8 freq[210];
	vu8 ac_leave[210];
	Range_Set_Typedef Range_Set_main[210];
	Range_Set_Typedef Range_Sed_Second[210];
	vu8 comp[210];
	Sort_TypeDef time[210];
	vu8 xpos;
	vu8 ypos;

}Limit_ScanValue_Typedef;
typedef struct
{
	
	User_Correction_Typedef		User_Correction;
	Limit_Tab_Typedef	Limit_Tab;
	List_Value_Typedef	List_Value;
	Sys_Setup_Typedef	Sys_Setup;
	Main_Func_TypeDef	Main_Func;
	Limit_ScanValue_Typedef Limit_ScanValue;
}SaveData_Typedef;
extern SaveData_Typedef SaveData;

typedef struct
{
	Save_Main_Funce_Typedef	Save_Main_Funce;
	Limit_Tab_Typedef	Limit_Tab;

}Saveeeprom_Typedef;
extern Saveeeprom_Typedef	Saveeeprom;

typedef struct
{
	vu8 index;
	vu8 page;
	vu8 third;
	vu8  force;

}Button_Page_Typedef;
typedef struct
{
	vu16 xpos;
	vu16 ypos;
	vu16 lenth;


}Disp_Coordinates_Typedef;
typedef struct
{
	vu8 Ordel;
	vu8 name;

} Send_Ord_Typedef;
typedef struct
{
	Send_Ord_Typedef first;
	Send_Ord_Typedef second;
	Send_Ord_Typedef third;

}Send_Mainord_Typedef;
extern Send_Mainord_Typedef Send_Mainord;
//extern Disp_Coordinates_Typedef Disp_Coordinates;
//==========================================================
//ϵͳ��(2��)
//typedef struct 
//{
//	Sort_TypeDef Res_Hi;//����(xxxx m��)
//	Sort_TypeDef Res_Lo;//����(xxxx m��)
//	Sort_TypeDef Vol_Hi;//����(xxxx V)
//	Sort_TypeDef Vol_Lo;//����(xxxx V)
//}System_TypeDef;

//==========================================================
//������(3��)
//typedef struct 
//{
//	vu16 Password;//��������(0-9999��Ĭ��0)
//	vu16 BootNum;//��������(0-9999��Ĭ��0)
//	vu16 ProductId;//�������(0-9999��Ĭ��0)
//}Config_TypeDef;

//==========================================================
//У��ֵ
//typedef struct 
//{
//	vu16 Num;  //��ʾֵ
//	vu16 NumAd;//A/Dֵ
//}Cal_TypeDef;

////У����
//typedef struct 
//{
//	Cal_TypeDef IrCur[4];//��Ե�������ֵ-4��
//	Cal_TypeDef IrVol[2];//��Ե�����ѹֵ-2��
//	
//}Calibrate_TypeDef;
//typedef struct 
//{
//	vu16 Real;  //ʵ��
//	vu16 Imaginary;//����
//	vu16 DC_voltage;
//	vu8	 Dot;
//	vu8	 Negative;
//}Res_TypeDef;
//==========================================================
//������Ϣ
//typedef struct 
//{
//	Parameter_TypeDef Parameter;//������
//	System_TypeDef System;//ϵͳ��
//	Config_TypeDef Config;//������
//	Calibrate_TypeDef Calibrate;//У׼��
//	vu16 Crc16;//У����
//}Save_TypeDef;
//
////==========================================================
////Ĭ��У׼ֵ������
//extern const u16 CalibrateLimit[][2];
//extern const s16 BASE_NUM[6];
//extern const u8 DOT_POS[6];

//==========================================================
//ȫ�ֱ���
//extern  Save_TypeDef SaveData;//����ֵ
//extern  Cal_TypeDef Cal[6];//У׼ֵ
//extern  Res_TypeDef	Res;
//extern  vu8 Range;//��ǰ����
//extern  bool F_Password;//������Ч��־
//extern  bool F_100ms;//100ms��ʱ��־
//extern  bool F_Fail;//����ʧ�ܱ�־
//extern  vu8 MenuIndex;//�����˵���
//extern  vu8 MenuSelect;//�˵�ѡ����
#define  FILENAME_R  "update.bin"
#define  FILENAME_W  "JK2520B.TXT"
#define  FILENAME_S  "UP407.bin"
extern  unsigned long SystemStatus;//ϵͳ״̬
extern unsigned long SystemMessage;//ϵͳ��Ϣ
extern vu8 DispBuf[12];
extern unsigned long Count_buff[12];
extern vu8 Uart_Send_Flag;
extern uint32_t Usb_Open_flag;
extern uint32_t softswitch;
extern RTC_TIME_Type RTC_TIME_DISP;
extern  uint8_t usb_oenflag;
extern uint8_t upflag;
//extern  vu16 Voltage;//���Ե�ѹ
//extern  vu16 Current;//���Ե���
//extern  vu16 Resistance,xxxx;//���Ե���
//extern  NumBox_TypeDef NumBox;//��ֵ��
//extern vu16 i_buff[FIT_INUM];		   //�˲�ֵ
//extern vu8 ffit_data1;
//extern	vu8	count_ffit;
//extern 	u16 	zreo_num[6];
//extern u8 fuhao;
//extern bool clear_flag;
//extern bool vol_flag;
//extern u32 MenuIndex;//�����˵���
//extern u32 MenuSelect;//�˵�ѡ����
//
//extern u32 SystemStatus;//ϵͳ״̬
//extern u32 SystemMessage;//ϵͳ��Ϣ

//==========================================================
//ϵͳ״̬
enum SysStatusEnum
{	
	SYS_STATUS_ERROR ,			//�쳣
	SYS_STATUS_RESET ,			//��λ
	SYS_STATUS_POWER ,			//����
//	SYS_STATUS_POWER ,
	SYS_STATUS_DATASTORE ,		//���ݱ���
	SYS_STATUS_FILE ,			//�ļ�����
	SYS_STATUS_SETUPTEST ,			//��������
	SYS_STATUS_TEST ,			//����
	SYS_STATUS_RANGE ,		//������ʾ
	SYS_STATUS_RANGECOUNT,	//��������ʾ
	SYS_STATUS_ITEM ,			//�б�ɨ����ʾ
	SYS_STATUS_USERDEBUG,//�û�У�� 
	SYS_STATUS_FACRDEBUG, //����У��
	SYS_STATUS_SYSSET,     //ϵͳ����
    SYS_STATUS_LIMITSET,     //��������
    SYS_STATUS_ITEMSET,//�б�ɨ������
	SYS_STATUS_TOOL,//����
	SYS_STATUS_SYS,
};
//==========================================================
//��ѡ״̬
enum SlectStatusEnum
{
	RH_FAIL=0,
	RL_FAIL,
	VH_FAIL,
	VL_FAIL,
	ALL_FAIL,
	ALL_PASS,
//	NO_COMP
	
};
//==========================================================
//ϵͳ��Ϣ
enum SysMessageEnum
{
	MSG_CLEAR , 
	MSG_IDLE , 
	MSG_TEST , 
	MSG_ABORT , 
	MSG_PASS ,
//	MSG_FAIL ,
	MSG_HIGH , 
	MSG_LOW , 
};

//==========================================================
//���ADֵ
#define AD_MAX (3000) //12bit=4096-1
////����ѹADֵ
//#define VOL_AD_MAX (AD_MAX-100)
////������ADֵ
//#define CUR_AD_MAX (AD_MAX-100)

//����������
#define RANGR_LIMIT_HIGH (3000)
#define RANGR_LIMIT_LOW  (190)
extern uint32_t clear_flag;
//==========================================================
//�����б�
void InitGlobalValue(void);//ȫ�ֱ�����ʼ��
void LoadDefaultSet(void);//����Ĭ������ֵ
void LoadDefaultParameter(void);//����Ĭ�ϲ���ֵ
void LoadDefaultSystem(void);//����Ĭ��ϵͳֵ
void LoadDefaultConfig(void);//����Ĭ������ֵ
void LoadDefaultCalibrate(void);//����Ĭ��У׼ֵ
void ReadSaveData(void);//��ȡ�������
void  Bin_Read(void);
void jumpboot(void);
//void WriteSaveData(void);//д�뱣�����

#define SetSystemStatus(status) (SystemStatus=status)
#define GetSystemStatus() (SystemStatus)
#define GetSystemMessage() (SystemMessage)
#define SetSystemMessage(msg) (SystemMessage=msg)

void Check_Calibrate_Limit(void);//У׼ֵ���
void Hex_Format(vu32 dat , vu8 Dot , vu8 len , vu8 dispzero);


#endif
