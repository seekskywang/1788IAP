#ifndef __LPC177X_8X_LCD_H_
#define __LPC177X_8X_LCD_H_


#include "LPC177x_8x.h"
#include "lpc_types.h"
#include "system_LPC177x_8x.h" 

/**
 * @brief A struct for Bitmap on LCD screen
 */
typedef struct _Bmp_t 
{
  uint32_t  H_Size;
  uint32_t  V_Size;
  uint32_t  BitsPP;
  uint32_t  BytesPP;
  uint32_t *pPalette;
  unsigned long *pPicStream;
  uint8_t *pPicDesc;
} Bmp_t, *pBmp_t;
typedef struct 
{
	uint32_t Fword;
	uint32_t black;

}Typedf_Disp_Colour;
extern Typedf_Disp_Colour Colour; 
//typedef Disp_Colour Colour;
/**
 * @brief A struct for Font Type on LCD screen
 */
 
typedef struct _FontType_t 
{
  uint32_t H_Size;
  uint32_t V_Size;
  uint32_t CharacterOffset;
  uint32_t CharactersNuber;
  uint8_t *pFontStream;
  uint8_t *pFontDesc;
} FontType_t, *pFontType_t;

typedef uint32_t LdcPixel_t, *pLdcPixel_t;




#define LCD_VRAM_BASE_ADDR 	((unsigned long)SDRAM_BASE + 0x00010000)
#define LCD_CURSOR_BASE_ADDR 	((unsigned long)0x20088800)

#define C_GLCD_REFRESH_FREQ     (50)	// Hz
#define C_GLCD_H_SIZE           480
#define C_GLCD_H_PULSE          2	//
#define C_GLCD_H_FRONT_PORCH    5	//
#define C_GLCD_H_BACK_PORCH     40 	//
#define C_GLCD_V_SIZE           272
#define C_GLCD_V_PULSE          2
#define C_GLCD_V_FRONT_PORCH    8
#define C_GLCD_V_BACK_PORCH     8

#define C_GLCD_PWR_ENA_DIS_DLY  10000
#define C_GLCD_ENA_DIS_DLY      10000

#define CRSR_PIX_32     0
#define CRSR_PIX_64     1
#define CRSR_ASYNC      0
#define CRSR_FRAME_SYNC 2

#define TEXT_DEF_TAB_SIZE 5

#define TEXT_BEL1_FUNC()

void GLCD_SetPallet (const unsigned long * pPallete);
void GLCD_Init (const unsigned long *pPain, const unsigned long * pPallete);
void GLCD_Ctrl (Bool bEna);
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint32_t color );
void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint32_t charColor, uint32_t bkColor );
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint32_t Color, uint32_t bkColor);
void lcd_Clear(uint32_t color);
void lcd_display_coord(uint16_t x,uint16_t y,uint32_t color,uint32_t color1);
void LCD_ShowFontCN_40_55(uint16_t x,uint16_t y,uint16_t xsize,uint16_t ysize,uint8_t * image); 
void LCD_DrawRect( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint32_t color );

void WriteString(uint16_t x0, uint16_t y0, const uint8_t *pcStr, uint8_t mode);

void WriteString_16(uint16_t x0, uint16_t y0, const uint8_t *pcStr, uint8_t mode);
void WriteString_12(uint16_t x0, uint16_t y0, const uint8_t *pcStr, uint8_t mode);
void WriteString_Big(uint16_t x0, uint16_t y0, const uint8_t *pcStr);
void lcd_image(uint8_t *pt);
void lcd_image2(uint8_t *pt);
#endif   /*__LPC177X_8X_LCD_H_*/



