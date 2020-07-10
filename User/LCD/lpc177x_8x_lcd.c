#include "LCD/lpc177x_8x_lcd.h"
#include"EX_SDRAM/EX_SDRAM.H"
#include "LCD/AsciiLib.h"
#include  "Globalvalue/globalvalue.h"
#include "hzk.h"

#define C_GLCD_CLK_PER_LINE     (C_GLCD_H_SIZE + C_GLCD_H_PULSE + C_GLCD_H_FRONT_PORCH + C_GLCD_H_BACK_PORCH)
#define C_GLCD_LINES_PER_FRAME  (C_GLCD_V_SIZE + C_GLCD_V_PULSE + C_GLCD_V_FRONT_PORCH + C_GLCD_V_BACK_PORCH)
#define C_GLCD_PIX_CLK          (C_GLCD_CLK_PER_LINE * C_GLCD_LINES_PER_FRAME * C_GLCD_REFRESH_FREQ)
Typedf_Disp_Colour Colour; 

/*************************************************************************
 * Function Name: GLCD_Ctrl
 * Parameters: Bool bEna
 *
 * Return: none
 *
 * Description: GLCD enable disabe sequence
 *
 *************************************************************************/
void GLCD_Ctrl (Bool bEna)
{
	volatile uint32_t i;
  if (bEna)
  {
    LPC_LCD->CTRL |= (1<<0);
    for(i = C_GLCD_PWR_ENA_DIS_DLY; i; i--)
    LPC_LCD->CTRL |= (1<<11);
  }
  else
  {
    LPC_LCD->CTRL &= ~(1<<11);
    for(i = C_GLCD_PWR_ENA_DIS_DLY; i; i--);
    LPC_LCD->CTRL &= ~(1<<0);
  }
}


/*************************************************************************
 * Function Name: GLCD_Init
 * Parameters: const unsigned long *pPain, const unsigned long * pPallete
 *
 * Return: none
 *
 * Description: GLCD controller init
 *
 *************************************************************************/

void GLCD_Init (const unsigned long *pPain, const unsigned long * pPallete)
{
	unsigned long i;
	unsigned long *pDst = (unsigned long *)LCD_VRAM_BASE_ADDR;
	unsigned long *pDst16;
	const unsigned long *pBmp16 = pBmp16;
  /* R */
  LPC_IOCON->P0_4  = 0x27;	 // LCD_VD_0
  LPC_IOCON->P0_5   = 0x27;  // LCD_VD_1
  LPC_IOCON->P4_28   = 0x27;  // LCD_VD_2

  LPC_IOCON->P4_29  = 0x27;	 // LCD_VD_3
  LPC_IOCON->P2_6   = 0x27;  // LCD_VD_4
  LPC_IOCON->P2_7   = 0x27;  // LCD_VD_5
  LPC_IOCON->P2_8   = 0x27;  // LCD_VD_6
  LPC_IOCON->P2_9  = 0x27;  // LCD_VD_7 

  /* G */     
  LPC_IOCON->P0_6  = 0x27;  // LCD_VD_8
  LPC_IOCON->P0_7  = 0x27;  // LCD_VD_9


  LPC_IOCON->P1_20  = 0x27;  // LCD_VD_10
  LPC_IOCON->P1_21  = 0x27;  // LCD_VD_11
  LPC_IOCON->P1_22  = 0x27;  // LCD_VD_12
  LPC_IOCON->P1_23  = 0x27;  // LCD_VD_13
  LPC_IOCON->P1_24  = 0x27;  // LCD_VD_14
  LPC_IOCON->P1_25  = 0x27;  // LCD_VD_15

	/* B */
  LPC_IOCON->P0_8  = 0x27;  // LCD_VD_16
  LPC_IOCON->P0_9  = 0x27;  // LCD_VD_17
  LPC_IOCON->P2_12  = 0x27;  // LCD_VD_18

  LPC_IOCON->P2_13  = 0x27;	 // LCD_VD_19
  LPC_IOCON->P1_26  = 0x27;  // LCD_VD_20
  LPC_IOCON->P1_27  = 0x27;  // LCD_VD_21
  LPC_IOCON->P1_28  = 0x27;  // LCD_VD_22
  LPC_IOCON->P1_29  = 0x27;  // LCD_VD_23

  LPC_IOCON->P2_2   = 0x27;  // LCD_DCLK
  LPC_IOCON->P2_5   = 0x27;  // LCD_LP -- HSYNC
  LPC_IOCON->P2_3   = 0x27;  // LCD_FP -- VSYNC
  LPC_IOCON->P2_4   = 0x27;  // LCD_ENAB_M -- LCDDEN
  LPC_IOCON->P2_0   = 0x27;  // LCD_PWR

	// <<< debug <<<
	
	/*Back light enable*/
	LPC_GPIO2->DIR |= (1<<1);
	LPC_GPIO2->SET|= (5<<1);

	//Turn on LCD clock
	LPC_SC->PCONP |= 1<<0;
	
	// Disable cursor
	LPC_LCD->CRSR_CTRL &=~(1<<0);
	
	// disable GLCD controller	
	LPC_LCD->CTRL = 0;
	
	LPC_LCD->CTRL &= ~(0x07 <<1);
//	#ifndef __RAM__
	// RGB565
//	LPC_LCD->CTRL |= (6<<1);
//	#else
	// RGB888
	LPC_LCD->CTRL |= (5<<1);
//	#endif
	
	// TFT panel
	LPC_LCD->CTRL |= (1<<5);
	// single panel
	LPC_LCD->CTRL &= ~(1<<7);
	// notmal output
	LPC_LCD->CTRL &= ~(1<<8);
	// little endian byte order
	LPC_LCD->CTRL &= ~(1<<9);
	// little endian pix order
	LPC_LCD->CTRL &= ~(1<<10);
	// disable power
	LPC_LCD->CTRL &= ~(1<<11);
	// init pixel clock
	LPC_SC->LCD_CFG = PeripheralClock / ((unsigned long) C_GLCD_PIX_CLK);
	// bypass inrenal clk divider
	LPC_LCD->POL |=(1<<26);
	// clock source for the LCD block is HCLK
	LPC_LCD->POL &= ~(1<<5);
	// LCDFP pin is active LOW and inactive HIGH
	LPC_LCD->POL |= (1<<11);
	// LCDLP pin is active LOW and inactive HIGH
	LPC_LCD->POL |= (1<<12);
	// data is driven out into the LCD on the falling edge
	LPC_LCD->POL &= ~(1<<13);
	// active high
	LPC_LCD->POL &= ~(1<<14);
	LPC_LCD->POL &= ~(0x3FF <<16);
	LPC_LCD->POL |= (C_GLCD_H_SIZE-1)<<16;
	
	// init Horizontal Timing
	LPC_LCD->TIMH = 0; //reset TIMH before set value
	LPC_LCD->TIMH |= (C_GLCD_H_BACK_PORCH - 1)<<24;
	LPC_LCD->TIMH |= (C_GLCD_H_FRONT_PORCH - 1)<<16;
	LPC_LCD->TIMH |= (C_GLCD_H_PULSE - 1)<<8;
	LPC_LCD->TIMH |= ((C_GLCD_H_SIZE/16) - 1)<<2;
	
	// init Vertical Timing
	LPC_LCD->TIMV = 0;  //reset TIMV value before setting
	LPC_LCD->TIMV |= (C_GLCD_V_BACK_PORCH)<<24;
	LPC_LCD->TIMV |= (C_GLCD_V_FRONT_PORCH)<<16;
	LPC_LCD->TIMV |= (C_GLCD_V_PULSE - 1)<<10;
	LPC_LCD->TIMV |= C_GLCD_V_SIZE - 1;
	// Frame Base Address doubleword aligned
	LPC_LCD->UPBASE = LCD_VRAM_BASE_ADDR & ~7UL ;
	LPC_LCD->LPBASE = LCD_VRAM_BASE_ADDR & ~7UL ;
        // init colour pallet

	if(NULL != pPallete)
	{
		GLCD_SetPallet(pPallete);
	}

    pDst16 = (unsigned long *) LCD_VRAM_BASE_ADDR;

	for( i = 0; (C_GLCD_H_SIZE * C_GLCD_V_SIZE) > i; i++)
	{
  		*pDst16++ = Blue;
	}

  	for(i = C_GLCD_ENA_DIS_DLY; i; i--);
}
/*************************************************************************
 * Function Name: lcd_Clear
 * Parameters: uint32_t color
 *
 * Return: none
 *
 * Description: LCD screen clearing
 *
 *************************************************************************/
void lcd_Clear(uint32_t color)
{
    unsigned long i;
	unsigned long *pDst = (unsigned long *)LCD_VRAM_BASE_ADDR;
  
    for( i = 0; (C_GLCD_H_SIZE * C_GLCD_V_SIZE) > i; i++)
    {
    	*pDst++ = color;        
    }
}
void LCD_DrawRect( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint32_t color )
{
	unsigned long i,j;
	unsigned long *pDst ;//= (unsigned long *)LCD_VRAM_BASE_ADDR + (y0)*480+x0;
	pDst = (unsigned long *)LCD_VRAM_BASE_ADDR + (C_GLCD_V_SIZE-y0-1)*480+(C_GLCD_H_SIZE-x0-1);

	for(j=0;j<(y1-y0);j++)
	{
		for( i = 0; ((x1-x0) ) > i; i++)
		{
			*pDst-- = color;        
		}
		pDst-=(480-(x1-x0));
	}


}
/*************************************************************************
 * Function Name: GLCD_SetPallet
 * Parameters: const unsigned long * pPallete
 *
 * Return: none
 *
 * Description: GLCD init colour pallete
 *
 *************************************************************************/
void GLCD_SetPallet (const unsigned long * pPallete)
{
	unsigned long i;
	unsigned long * pDst = (unsigned long *)LPC_LCD->PAL;
	// assert(pPallete);
	for (i = 0; i < 128; i++)
	{
	*pDst++ = *pPallete++;
	}
}


/******************************************************************************
* Function Name  : LCD_SetPoint
* Description    : 
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint32_t color)
{
	unsigned long *pDst = (unsigned long *)LCD_VRAM_BASE_ADDR;
    unsigned long value;
	
	if( Xpos >= C_GLCD_H_SIZE || Ypos >= C_GLCD_V_SIZE )
	{
		return;
	}

//    value=Ypos*480+Xpos;
//	index = (uint32_t)((LCD_PIXEL_HEIGHT - _usY - 1) * 
//	LCD_PIXEL_WIDTH + (LCD_PIXEL_WIDTH - _usX - 1))*3;
	value=(C_GLCD_V_SIZE-Ypos-1)*480+(C_GLCD_H_SIZE-Xpos-1);
    pDst = pDst +value ;
    *pDst = color; 
}



/******************************************************************************
* Function Name  : LCD_DrawLine
* Description    : Bresenham's line algorithm
* Input          : - x0:
*                  - y0:
*       				   - x1:
*       		       - y1:
*                  - color:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint32_t color )
{
  short dx,dy;
  short temp;

  if( x0 > x1 )
  {
    temp = x1;
    x1 = x0;
    x0 = temp;   
  }
  if( y0 > y1 )
  {
    temp = y1;
    y1 = y0;
    y0 = temp;   
  }

  dx = x1-x0;
  dy = y1-y0;

  if( dx == 0 )
  {
    do
    { 
      LCD_SetPoint(x0, y0, color);
      y0++;
    }while( y1 >= y0 ); 
    return; 
  }
  if( dy == 0 )
  {
    do
    {
      LCD_SetPoint(x0, y0, color);
      x0++;
    }
    while( x1 >= x0 ); 
		return;
  }

	/* Bresenham's line algorithm  */
  if( dx > dy )
  {
    temp = 2 * dy - dx;
    while( x0 != x1 )
    {
	    LCD_SetPoint(x0,y0,color);
	    x0++;
	    if( temp > 0 )
	    {
	      y0++;
	      temp += 2 * dy - 2 * dx; 
	 	  }
      else         
      {
			  temp += 2 * dy;
			}       
    }
    LCD_SetPoint(x0,y0,color);
  }  
  else
  {
    temp = 2 * dx - dy;
    while( y0 != y1 )
    {
	 	  LCD_SetPoint(x0,y0,color);     
      y0++;                 
      if( temp > 0 )           
      {
        x0++;               
        temp+=2*dy-2*dx; 
      }
      else
			{
        temp += 2 * dy;
			}
    } 
    LCD_SetPoint(x0,y0,color);
	}
} 


/******************************************************************************
* Function Name  : PutChar
* Description    : 
* Input          : - Xpos:
*                  - Ypos:
*				           - ASCI:
*				           - charColor:
*				           - bkColor:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint32_t charColor, uint32_t bkColor )
{
	uint32_t i, j;
    uint8_t buffer[16], tmp_char;
    GetASCIICode(buffer,ASCI);
    for( i=0; i<16; i++ )
    {
        tmp_char = buffer[i];
        for( j=0; j<8; j++ )
        {
            if( (tmp_char >> 7 - j) & 0x01 == 0x01 )
            {
                LCD_SetPoint( Xpos + j, Ypos + i, charColor );
            }
            else
            {
                LCD_SetPoint( Xpos + j, Ypos + i, bkColor );
            }
        }
    }
}

void PutChar_12( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint32_t charColor, uint32_t bkColor )
{
	uint32_t i, j;
    uint8_t buffer[12], tmp_char;
    GetASCIICode_12(buffer,ASCI);
    for( i=0; i<12; i++ )
    {
        tmp_char = buffer[i];
        for( j=0; j<8; j++ )
        {
            if( (tmp_char >> 7 - j) & 0x01 == 0x01 )
            {
                LCD_SetPoint( Xpos + j, Ypos + i, charColor );
            }
            else
            {
//                LCD_SetPoint( Xpos + j, Ypos + i, bkColor );
            }
        }
    }
}
/******************************************************************************
* Function Name  : GUI_Text
* Description    : 
* Input          : - Xpos: 
*                  - Ypos: 
*				           - str:
*				           - charColor:
*				           - bkColor:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint32_t Color, uint32_t bkColor)
{
    uint8_t TempChar;
    do
    {
        TempChar = *str++;  
        PutChar( Xpos, Ypos, TempChar, Color, bkColor );    
        if( Xpos < C_GLCD_H_SIZE - 8 )
        {
            Xpos += 8;
        } 
        else if ( Ypos < C_GLCD_V_SIZE - 16 )
        {
            Xpos = 0;
            Ypos += 16;
        }   
        else
        {
            Xpos = 0;
            Ypos = 0;
        }    
    }
    while ( *str != 0 );
}

/******************************************************************************
* Function Name  : lcd_display_coord
* Description    : 
* Input          : - Xpos: 
*                  - Ypos: 
* uint16_t color     color
* uint16_t color1    color
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

void lcd_display_coord(uint16_t x,uint16_t y,uint32_t color,uint32_t color1)
{
    uint16_t dx,dy;
    uint16_t dx1,dy1;
    uint16_t i;
    dx=x;dy=y;
    dx1=x;dy1=y;
    for(i=0;i<40;i++)
    {
     LCD_SetPoint(dx-20,dy,color);
     dx++;
     LCD_SetPoint(dx1,dy1-20,color);
     dy1++;  
    }
    dx=x;dy=y;
    dx1=x;dy1=y;
    for(i=0;i<10;i++)
    {
     LCD_SetPoint(dx-20,dy-20,color1);
     LCD_SetPoint(dx-20,dy+20,color1); 
     dx++; 
     LCD_SetPoint(dx1-20,dy1-20,color1);
     LCD_SetPoint(dx1+20,dy1-20,color1);
     dy1++; 
    }
    dx=x;dy=y;
    dx1=x;dy1=y;
    for(i=0;i<10;i++)
    {
     LCD_SetPoint(dx+10,dy-20,color1);
     LCD_SetPoint(dx+10,dy+20,color1);
      
     dx++; 
     LCD_SetPoint(dx1-20,dy1+10,color1);
     LCD_SetPoint(dx1+20,dy1+10,color1);
     dy1++; 
    }

}
/*****************************************************************************
** 函数名称: findHzIndex
** 函数功能: 查找汉字在自定义字库的索引
                                hz 指向(机内码)的指针
** ?   ?: Dream
** ?   ?: 2010?12?06?
*****************************************************************************/
unsigned int findHzIndex(const unsigned char *hz)                                                                       
{
    uint16_t i,j;
  //NT_GB16 *ptGb16 = (FNT_GB16 *)GBHZ_16;
    for(j=0;j<sizeof(GB_21)/sizeof(GB_21[0]);j++)
	{
        //if ((*hz == ptGb16->Index[0]) && (*(hz+1) == ptGb16->Index[1])) FNT_GB16//&&(*(hz+1) == GB_21[j].Index[1])
		if((GB_21[j].Index[0]==*hz)&&(*(hz+1)==GB_21[j].Index[1]))
        {
            return j;
        }
    }
    return 0;
}
unsigned int findHzIndex_16(const unsigned char *hz)                                                                       
{
    uint16_t j;
  //NT_GB16 *ptGb16 = (FNT_GB16 *)GBHZ_16;
    for(j=0;j<sizeof(GB_16)/sizeof(GB_16[0]);j++)
	{
        //if ((*hz == ptGb16->Index[0]) && (*(hz+1) == ptGb16->Index[1])) FNT_GB16//&&(*(hz+1) == GB_21[j].Index[1])
		if((GB_16[j].Index[0]==*hz)&&(*(hz+1)==GB_16[j].Index[1]))
        {
            return j;
        }
    }
    return 0;
}
void LCD_ShowFontCN_40_55(uint16_t x,uint16_t y,uint16_t xsize,uint16_t ysize,uint8_t * image)
{                                  
	uint8_t temp,t1;
	uint16_t x0=x,t;
	temp=0;  	       
	for(t=0;t<xsize*ysize/8;t++)
	{   
		//temp=GB_16[fno].Msk[t];                //gb16Dot[fno][t];    //调用16*08??   
		temp=*(image+t);
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)
				LCD_SetPoint(x,y,Colour.Fword); 
			else 
				LCD_SetPoint(x,y,Colour.black); 
			   
			temp<<=1; 
			x++;
			if(x-x0==xsize)
			{
				x=x0;
				y++;                        
				break;
			}                        
		} 
	}			
}
void LCD_ShowFontCN_16(uint16_t x,uint16_t y,uint16_t fno,uint8_t mode)
{                                  
	uint8_t temp,t,t1;
	uint16_t x0=x;
	temp=0;  
	if(!mode) //正常显示
	{        
		for(t=0;t<16*2;t++)
		{   
			temp=GB_16[fno].Msk[t];                //gb16Dot[fno][t];    //调用16*08??   

			for(t1=0;t1<8;t1++)
			{
				if(temp&0x80)
					LCD_SetPoint(x,y,Colour.Fword); 
				else 
					LCD_SetPoint(x,y,Colour.black); 
				   
				temp<<=1; 
				x++;
				if(x-x0==16)
				{
					x=x0;
					y++;                        
					break;
				}                        
			} 
		}
	}
	else
	{
		for(t=0;t<16*2;t++)
		{   
			temp=GB_16[fno].Msk[t];                //gb16Dot[fno][t];    //调用16*08汉字                                 
			for(t1=0;t1<8;t1++)
			{
				if(temp&0x80)
					LCD_SetPoint(x,y,Colour.black);                                      
				else 
					LCD_SetPoint(x,y,Colour.Fword); 
				     
				temp<<=1; 
				x++;
				if(x-x0==16)
				{
					x=x0;
					y++;                        
					break;
				}                        
			} 
		}                
	}
	
}
void LCD_ShowFontCN(uint16_t x,uint16_t y,uint16_t fno,uint8_t mode)
{                                  
	uint8_t temp,t,t1;
	uint16_t x0=x;
	temp=0;  
	if(!mode) //正常显示
	{        
		for(t=0;t<21*3;t++)
		{   
			temp=GB_21[fno].Msk[t];                //gb16Dot[fno][t];    //调用16*08??   

			for(t1=0;t1<8;t1++)
			{
				if(temp&0x80)
					LCD_SetPoint(x,y,Colour.Fword); 
				else 
					LCD_SetPoint(x,y,Colour.black); 
				   
				temp<<=1; 
				x++;
				if(x-x0==24)
				{
					x=x0;
					y++;                        
					break;
				}                        
			} 
		}
	}
	else
	{
		for(t=0;t<21*3;t++)
		{   
			temp=GB_21[fno].Msk[t];                //gb16Dot[fno][t];    //调用16*08汉字                                 
			for(t1=0;t1<8;t1++)
			{
				if(temp&0x80)
					LCD_SetPoint(x,y,Colour.black);                                      
				else 
					LCD_SetPoint(x,y,Colour.Fword); 
				     
				temp<<=1; 
				x++;
				if(x-x0==24)
				{
					x=x0;
					y++;                        
					break;
				}                        
			} 
		}                
	}
	
}

uint16_t WriteOneHzChar(uint16_t fno,uint16_t x0,uint16_t y0,uint8_t mode)
{
	LCD_ShowFontCN(x0,y0,fno,mode);
	return (24);                                          /* 返回16位列宽               */
}
uint16_t WriteOneHzChar_16(uint16_t fno,uint16_t x0,uint16_t y0,uint8_t mode)
{
	LCD_ShowFontCN_16(x0,y0,fno,mode);
	return (18);                                          /* 返回16位列宽               */
}
/*****************************************************************************
** 名称: WriteString
* 功能: 在指定的位置显示一个字符串或者汉字支持自动换行

** ?   ?: Dream
** ?   ?: 2010?12?06? showstring
*****************************************************************************/                              
void WriteString(uint16_t x0, uint16_t y0, const uint8_t *pcStr, uint8_t mode)
{
	uint16_t usIndex=0;
	uint16_t usWidth = 0; 
	while(1)
	{
		if(*pcStr == 0) 
		{
			break;                                     /*字符串结束         */
		}      
		x0 = x0 + (usWidth);                           /* 调节字符串的松紧度         */
		if(*pcStr > 0x80)                              /* 判断为汉字                  */
		{
			if((x0 + 24) > C_GLCD_H_SIZE)                      /* 检查剩余空间是否充足        */
			{
				x0 = 0;
				y0 = y0 + 22;                          /* 改变显示坐标                */
				if(y0 > C_GLCD_V_SIZE)                         /* 纵坐标超出                  */
				{
					y0 = 0;
				}
			}
			usIndex = findHzIndex(pcStr);
			usWidth = WriteOneHzChar(usIndex, x0, y0, mode);
			/* ????                     */
			pcStr += 2;
		}
		else 
		{                                               /* 判断为非汉字                */
			if (*pcStr == '\r')                         /* 换行                        */
			{ 
				y0 = y0 + 22;                           /* 改变显示坐标                */
				if(y0 > C_GLCD_V_SIZE)                          /* 纵坐标超出                   */
				{
					y0 = 0;
				}
				pcStr++;
				usWidth = 0;
				continue;
			} 
			else if (*pcStr == '\n')                    /* 对其到起点                  */
			{
				x0 = 0;
				pcStr++;
				usWidth = 0;
				continue;
			} 
			else 
			{
				if((x0 + 12) > C_GLCD_H_SIZE)                     /* 检查剩余空间是否充足       */
				{
					x0 = 0;
					y0 = y0 + 12;                        /* 改变显示坐标                  */
					if(y0 > C_GLCD_V_SIZE)                       /* 纵坐标超出                   */
					{ 
						y0 = 0;
					}
				}
				//usWidth = WriteOneASCII(pcStr, x0, y0, mode);
				PutChar( x0, y0, *pcStr, Colour.Fword, Colour.black );   
				/* ASCII码表21H的值对应区位码3区*/
				pcStr += 1;
			}
		}
	}                                                                                                            
}
void WriteString_Big(uint16_t x0, uint16_t y0, const uint8_t *pcStr)
{
	uint16_t usIndex=0;
	uint16_t i,usWidth = 0; 
	while(1)
	{
		if(*pcStr == 0) 
		{
			break;                                     /*字符串结束         */
		}      		
//		if((x0 + 8) > C_GLCD_H_SIZE)                     /* 检查剩余空间是否充足       */
//		{
//			x0 = 0;
//			y0 = y0 + 8;                        /* 改变显示坐标                  */
//			if(y0 > C_GLCD_V_SIZE)                       /* 纵坐标超出                   */
//			{ 
//				y0 = 0;
//			}
//		}
		//usWidth = WriteOneASCII(pcStr, x0, y0, mode);
//		PutChar( x0, y0, *pcStr, Colour.Fword, Colour.black );  
		if(*pcStr>='0'&&*pcStr<='9')
		{
			i=*pcStr-'0';
		
		}
		else if(*pcStr=='.')
			i=10;
		else if(*pcStr=='-')
			i=11;
		else
			i=12;
		
		LCD_ShowFontCN_40_55(x0+usWidth,y0,32,55,(uint8_t*)(_num)+i*32/8*55);		
		/* ASCII码表21H的值对应区位码3区*/
		pcStr += 1;
		usWidth+=32;
		
		
	}                                                                                                            
}
void WriteString_16(uint16_t x0, uint16_t y0, const uint8_t *pcStr, uint8_t mode)
{
	uint16_t usIndex=0;
	uint16_t usWidth = 0; 
	while(1)
	{
		if(*pcStr == 0) 
		{
			break;                                     /*字符串结束         */
		}      
		x0 = x0 + (usWidth);                           /* 调节字符串的松紧度         */
		if(*pcStr > 0x80)                              /* 判断为汉字                  */
		{
			if((x0 + 16) > C_GLCD_H_SIZE)                      /* 检查剩余空间是否充足        */
			{
				x0 = 0;
				y0 = y0 + 16;                          /* 改变显示坐标                */
				if(y0 > C_GLCD_V_SIZE)                         /* 纵坐标超出                  */
				{
					y0 = 0;
				}
			}
			usIndex = findHzIndex_16(pcStr);
			usWidth = WriteOneHzChar_16(usIndex, x0, y0, mode);
			/* ????                     */
			pcStr += 2;
		}
		else 
		{                                               /* 判断为非汉字                */
			if (*pcStr == '\r')                         /* 换行                        */
			{ 
				y0 = y0 + 16;                           /* 改变显示坐标                */
				if(y0 > C_GLCD_V_SIZE)                          /* 纵坐标超出                   */
				{
					y0 = 0;
				}
				pcStr++;
				usWidth = 0;
				continue;
			} 
			else if (*pcStr == '\n')                    /* 对其到起点                  */
			{
				x0 = 0;
				pcStr++;
				usWidth = 0;
				continue;
			} 
			else 
			{
				if((x0 + 8) > C_GLCD_H_SIZE)                     /* 检查剩余空间是否充足       */
				{
					x0 = 0;
					y0 = y0 + 8;                        /* 改变显示坐标                  */
					if(y0 > C_GLCD_V_SIZE)                       /* 纵坐标超出                   */
					{ 
						y0 = 0;
					}
				}
				//usWidth = WriteOneASCII(pcStr, x0, y0, mode);
				
				PutChar( x0, y0, *pcStr, Colour.Fword, Colour.black );   
				/* ASCII码表21H的值对应区位码3区*/
				pcStr += 1;
				usWidth=10;
			}
		}
	}                                                                                                            
}
void WriteString_12(uint16_t x0, uint16_t y0, const uint8_t *pcStr, uint8_t mode)
{
	uint16_t usIndex=0;
	uint16_t usWidth = 0; 
	while(1)
	{
		if(*pcStr == 0) 
		{
			break;                                     /*字符串结束         */
		}      
		x0 = x0 + (usWidth);                           /* 调节字符串的松紧度         */
		if(*pcStr > 0x80)                              /* 判断为汉字                  */
		{
			if((x0 + 16) > C_GLCD_H_SIZE)                      /* 检查剩余空间是否充足        */
			{
				x0 = 0;
				y0 = y0 + 16;                          /* 改变显示坐标                */
				if(y0 > C_GLCD_V_SIZE)                         /* 纵坐标超出                  */
				{
					y0 = 0;
				}
			}
			usIndex = findHzIndex_16(pcStr);
			usWidth = WriteOneHzChar_16(usIndex, x0, y0, mode);
			/* ????                     */
			pcStr += 2;
		}
		else 
		{                                               /* 判断为非汉字                */
			if (*pcStr == '\r')                         /* 换行                        */
			{ 
				y0 = y0 + 16;                           /* 改变显示坐标                */
				if(y0 > C_GLCD_V_SIZE)                          /* 纵坐标超出                   */
				{
					y0 = 0;
				}
				pcStr++;
				usWidth = 0;
				continue;
			} 
			else if (*pcStr == '\n')                    /* 对其到起点                  */
			{
				x0 = 0;
				pcStr++;
				usWidth = 0;
				continue;
			} 
			else 
			{
				if((x0 + 8) > C_GLCD_H_SIZE)                     /* 检查剩余空间是否充足       */
				{
					x0 = 0;
					y0 = y0 + 8;                        /* 改变显示坐标                  */
					if(y0 > C_GLCD_V_SIZE)                       /* 纵坐标超出                   */
					{ 
						y0 = 0;
					}
				}
				//usWidth = WriteOneASCII(pcStr, x0, y0, mode);
				PutChar_12( x0, y0, *pcStr, Colour.Fword, Colour.black );   
				/* ASCII码表21H的值对应区位码3区*/
				pcStr += 1;
				usWidth=8;
			}
		}
	}                                                                                                            
}
//==========================================================
//函数名称：Hex_Format
//函数功能：格式化整数值到DispBuf
//入口参数：整数，小数点，数值位数，显示零
//出口参数：无
//创建日期：2014.12.15
//修改日期：2015.01.14 13:12
//备注说明：dot=0-XXXXX XXXX.X XXX.XX XX.XXX X.XXXX-Dot=4
//注意事项：最多5位数
//==========================================================
void Hex_Format(vu32 dat , vu8 Dot , vu8 len , vu8 dispzero)
{
	vu8 i,j;

	if(len>6)len=10;//最大长度5
	if(len==0)len=1;
	if(Dot>5)Dot=5;//最大小数点4
	if(Dot>len)Dot=len;

	for(i=0;i<sizeof(DispBuf);i++)	//清空缓冲区
		DispBuf[i]=' ';

	for(i=len;i>0;i--)	//数制转换6  len
	{
		DispBuf[i-1] = dat%10 + '0';//转为ASCII
		dat/=10;
	}
	
	if(Dot)//有小数点
	{
		j=len;
		i=Dot;//取小数点位置
		while(i--)
		{
			DispBuf[j]=DispBuf[j-1];
			j--;
		}
		DispBuf[j]='.';//加入小数点
	}
	else
	DispBuf[len]=' ';	

	if(dispzero==FALSE)//不显示无效零
	{
		for(i=0;i<(len-1);i++)
		{
			if((DispBuf[i]=='0')&&(DispBuf[i+1]!='.'))
				DispBuf[i]=' ';
			else
				break;
		}
	}
	//if(Dot)
	len++;
	for(i=len;i<sizeof(DispBuf);i++)//末尾补结束符'\0'
	{
		DispBuf[i]=0;
	}			
}
void lcd_image(uint8_t *pt)
{
    unsigned long i;
	uint32_t color;
	unsigned long *pDst = (unsigned long *)LCD_VRAM_BASE_ADDR;
  
    for( i = 0; (C_GLCD_H_SIZE * C_GLCD_V_SIZE) > i; i++)
    {
		color=(uint32_t)*pt<<16|(uint32_t)*(pt+1)<<8|*(pt+2);
    	*pDst++ = color; 
		*pt++;
		*pt++;
		*pt++;
    }
}

void lcd_image2(uint8_t *pt)
{
    unsigned long i,j;
	uint32_t color;
	unsigned long *pDst = (unsigned long *)LCD_VRAM_BASE_ADDR;
  
    for( i = 0; (272) > i; i++)
    {
		for(j=0;j<362;j++)
		{
			
//			for(j=0;j<(y1-y0);j++)
//	{
//		for( i = 0; ((x1-x0) ) > i; i++)
//		{
//			*pDst-- = color;        
//		}
//		pDst-=(480-(x1-x0));
//	}
	
			color=(uint32_t)*pt<<16|(uint32_t)*(pt+1)<<8|*(pt+2);
			*pDst++ = color; 
			*pt++;
			*pt++;
			*pt++;
		}
		pDst+=480-362;
    }
}

