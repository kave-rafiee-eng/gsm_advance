#ifndef __LCD
#define __LCD

#include "main.h"
#include "math.h"
#include "font.h"

/////LCD Pins
#define  HW_RESET_PIN(x)    (x ? (GPIOE ->BSRR =0x00000040) : (GPIOE ->BSRR =0x00400000) ) //PE6
#define  HW_CS_PIN(x)       (x ? (GPIOE ->BSRR =0x00000020) : (GPIOE ->BSRR =0x00200000) ) //PE5
#define  HW_MOSI_PIN(x)     (x ? (GPIOE ->BSRR =0x00000010) : (GPIOE ->BSRR =0x00100000) ) //PE4
#define  HW_SCK_PIN(x)      (x ? (GPIOE ->BSRR =0x00000008) : (GPIOE ->BSRR =0x00080000) ) //PE3

#define LCD_X        96
#define LCD_Y        68
#define LCD_String    9
#define LCD_D         1		// Data
#define LCD_C         0   // Command
#define SetYAddr   0xB0
#define SetXAddr4  0x00
#define SetXAddr3  0x10

#define swap(a, b) {uint8_t t = a; a = b; b = t; }

void LCD_SendByte(uint8_t mode, uint8_t c);
void LCD_Clear(void);
void LCD_Update(void);
void LCD_Init(void);
void LCD_DrawPixel (int x, int y, uint8_t color);
void LCD_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
void LCD_DrawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t color);
void LCD_DrawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint8_t color);
void LCD_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void LCD_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void LCD_FillScreen(uint8_t color);
//void LCD_DrawChar(uint8_t x, uint8_t y, uint8_t color, unsigned char c);
//void LCD_print(uint8_t x, uint8_t y, uint8_t color, char *str);
//void LCD_write(uint8_t x, uint8_t y, uint8_t color, float num);
void LCD_DrawBitmap(uint8_t x, uint8_t y, const char *bitmap, uint8_t w, uint8_t h, uint8_t color);
void LCD_write_str(uint8_t x,uint8_t y,char* str);
void showchar2(int16_t x,int16_t y,int c);
void LCD_WriteStatus(uint8_t x,uint8_t y,char* str,uint8_t Status);
void LCD_WriteMenuStatus(uint8_t x,uint8_t y,const char* str,uint8_t Status);
void LCD_write_str_middle(uint8_t y,char* str);
#endif
