/*********************************************************************************************************
*
* File                : TouchPanel.h
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

#include "lpc177x_8x_ssp.h"
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_gpio.h"
#include "debug_frmwrk.h"


/* Private typedef -----------------------------------------------------------*/
typedef	struct POINT 
{
   uint16_t x;
   uint16_t y;
}Coordinate;

typedef struct Matrix 
{						
long double An,  
            Bn,     
            Cn,   
            Dn,    
            En,    
            Fn,     
            Divider ;
} Matrix ;



/* Private define ------------------------------------------------------------*/
#define THRESHOLD 2


/* Private define ------------------------------------------------------------*/

#define	CHX 	0x90
#define	CHY 	0xD0


/*SSP0 CS(P1.8)*/
#define TouchPanel_Enable 	 TouchPanel_nCS_L
#define TouchPanel_Disable 	 TouchPanel_nCS_H

#define TouchPanel_nCS_L      GPIO_SetDir (1,(1<<8),1);   \
                              GPIO_ClearValue(1,(1<<8));

#define TouchPanel_nCS_H      GPIO_SetDir (1,(1<<8),1);   \
                              GPIO_SetValue(1,(1<<8));
//
//#define TouchPanel_nCS_L      GPIO_ClearValue(1,(1<<8));
//                              
//
//#define TouchPanel_nCS_H      GPIO_SetValue(1,(1<<8));
                              

/*PERIQ -> P2_15*/
#define TP_INT_IN           (GPIO_ReadValue(2) & (1<<15))

void delay_ms(uint32_t ms);

void SSPI1_Init(void);
Coordinate *TP_GetAdXY(uint32_t *x,uint32_t *y);
void touch_init(void);
Coordinate * Read_XTP2046(void);

void TouchPanel_Calibrate(void);
void DrawCross(uint16_t Xpos,uint16_t Ypos);
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos);
FunctionalState setCalibrationMatrix( Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr);
FunctionalState getDisplayPoint(Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr );
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos);



#endif  /*_TOUCHPANEL_H_*/

