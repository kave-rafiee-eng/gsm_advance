#include "main.h"
#include "n1202.h"
#include "stdio.h"

extern volatile uint8_t  _LCD_RAM[LCD_X*LCD_String]; // Память нашего LCD
extern volatile uint8_t   LCD_Cmd_Counter,LCD_Data_Counter,LCD_InitF,LCD_InitState,LCD_InitTimer;
extern volatile uint8_t  LCD_UpdateF;
extern volatile uint16_t LCD_SendAdd;
extern volatile uint8_t  LCD_Cmd_Row;
extern uint8_t xlcd;


void F4Delay()
{
	__NOP();__NOP();__NOP();__NOP();//__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
}
int abs1(int in)
{
	if(in>=0) return(in);
	else      return(-in);
}
//Sending a byte of data to the display
void LCD_SendByte(uint8_t mode, uint8_t c)
{
  // Lowering the CS leg for the display
	HW_CS_PIN(0);
	F4Delay();
  //We form the first transmitted bit - select memory-command
	if (mode)  HW_MOSI_PIN(1);
	else   	 	 HW_MOSI_PIN(0);

	// Pushing with a clock pulse
	F4Delay();
	HW_SCK_PIN(1);
	F4Delay();
	HW_SCK_PIN(0);
	F4Delay();
	// In the loop, transfer the remaining bits

  HW_MOSI_PIN(c & 0x80);
	F4Delay();
	HW_SCK_PIN(1);
	F4Delay();
	HW_SCK_PIN(0);
	F4Delay();
	
	HW_MOSI_PIN(c & 0x40);
	F4Delay();
	HW_SCK_PIN(1);
	F4Delay();
	HW_SCK_PIN(0);
	F4Delay();
	
	HW_MOSI_PIN(c & 0x20);
	F4Delay();
	HW_SCK_PIN(1);
	F4Delay();
	HW_SCK_PIN(0);
	F4Delay();
	
	HW_MOSI_PIN(c & 0x10);
	F4Delay();
	HW_SCK_PIN(1);
	F4Delay();
	HW_SCK_PIN(0);
	F4Delay();
	
	HW_MOSI_PIN(c & 0x08);
	F4Delay();
	HW_SCK_PIN(1);
	F4Delay();
	HW_SCK_PIN(0);
	F4Delay();
	
	HW_MOSI_PIN(c & 0x04);
	F4Delay();
	HW_SCK_PIN(1);
	F4Delay();
	HW_SCK_PIN(0);
	F4Delay();
	
	HW_MOSI_PIN(c & 0x02);
	F4Delay();
	HW_SCK_PIN(1);
	F4Delay();
	HW_SCK_PIN(0);
	F4Delay();
	
	HW_MOSI_PIN(c & 0x01);
	F4Delay();
	HW_SCK_PIN(1);
	F4Delay();
	HW_SCK_PIN(0);
	//F4Delay();
}

// Очистка памяти дисплея
void LCD_Clear(void) {
  for (int index = 0; index < sizeof(_LCD_RAM) ; index++){
    _LCD_RAM[index] = (0x00);
  }
}

// Обновляем данные на экране
//void LCD_Update(void) 
//{
//  for(uint8_t p = 0; p < 9; p++) 
//	{
//    LCD_SendByte(LCD_C, SetYAddr | p); 
//    LCD_SendByte(LCD_C, SetXAddr4);
//    LCD_SendByte(LCD_C, SetXAddr3);
//    for(uint8_t col=0; col < LCD_X; col++){
//      LCD_SendByte(LCD_D, _LCD_RAM[(LCD_X * p) + col]);
//    }
//		HAL_Delay(200);
//  }
//}
//void LCD_Update(void) 
//{
//	uint16_t add = 0;
//  for(uint8_t p = 0; p < 9; p++) 
//	{
//		_LCD_RAM_SEND[add] = SetYAddr | p;
//		add++;
//		_LCD_RAM_SEND[add] = SetXAddr4;
//		add++;
//		_LCD_RAM_SEND[add] = SetXAddr3;
//    add++;
//    for(uint8_t col=0; col < LCD_X; col++)
//		{
//			_LCD_RAM_SEND[add] = _LCD_RAM[(LCD_X * p) + col];
//      add++;
//    }
//  }
//	LCD_UpdateState = 0;
//	LCD_UpdateF = 1;
//	NVIC_EnableIRQ(TIM17_IRQn);
//}
void LCD_Update(void) 
{
	TIM2->SR =0;
	LCD_Cmd_Counter = 0;
  LCD_Data_Counter = 0;
	LCD_Cmd_Row = 0;
	LCD_SendAdd = 0;
	LCD_UpdateF = 1;
}

// Рисование пикселя по координатам и цвету
void LCD_DrawPixel (int x, int y, uint8_t color) {
  if ((x < 0) || (x >= LCD_X) || (y < 0) || (y >= LCD_Y)) return;

  if (color) _LCD_RAM[x+ (y/8)*LCD_X] |= 1<<(y%8);
  else       _LCD_RAM[x+ (y/8)*LCD_X] &= ~(1<<(y%8)); 
}

// Рисование линии
void LCD_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color) {
  int steep = abs1(y1 - y0) > abs1(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }
  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }
  int dx, dy;
  dx = x1 - x0;
  dy = abs1(y1 - y0);
  int err = dx / 2;
  int ystep;
  if (y0 < y1) {ystep = 1;}
  else {ystep = -1;};
  for ( ; x0 <= x1; x0++) {
    if (steep) {LCD_DrawPixel(y0, x0, color);}
    else {LCD_DrawPixel(x0, y0, color);};
		err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// Рисование вертикальной линии
void LCD_DrawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t color) {
  LCD_DrawLine(x, y, x, y+h-1, color);
}

// Рисование горизонтальной линии
void LCD_DrawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint8_t color) {
  LCD_DrawLine(x, y, x+w-1, y, color);
}

// Рисование прямоугольника
void LCD_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
  LCD_DrawFastHLine(x, y, w, color);
  LCD_DrawFastHLine(x, y+h-1, w, color);
  LCD_DrawFastVLine(x, y, h, color);
  LCD_DrawFastVLine(x+w-1, y, h, color);
}

// Рисование залитый прямоугольник
void LCD_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
  for (int16_t i=x; i<x+w; i++) {
    LCD_DrawFastVLine(i, y, h, color);
  }
}

// Заливка экрана
void LCD_FillScreen(uint8_t color) {
  LCD_FillRect(0, 0, LCD_X, LCD_Y, color);
}


// Image output
void LCD_DrawBitmap(uint8_t x, uint8_t y, const char *bitmap, uint8_t w, uint8_t h, uint8_t color) {
  for (int16_t j=0; j<h; j++) {
    for (int16_t i=0; i<w; i++ ) {
      if (bitmap[i + (j/8)*w] & 1<<(j%8)) { LCD_DrawPixel(x+i, y+j, color); }
    }
  }
}



void bit_set(volatile uint8_t *DataIn,uint8_t Bit)
{
	     if(Bit ==  0) *DataIn |= 0x01;
	else if(Bit ==  1) *DataIn |= 0x02;
	else if(Bit ==  2) *DataIn |= 0x04;
	else if(Bit ==  3) *DataIn |= 0x08;
	else if(Bit ==  4) *DataIn |= 0x10;
	else if(Bit ==  5) *DataIn |= 0x20;
	else if(Bit ==  6) *DataIn |= 0x40;
	else if(Bit ==  7) *DataIn |= 0x80;
}
void showchar2(int16_t x,int16_t y,int c)
{
  c -=32;
  uint16_t character = ((uint16_t) c * 25 );
  
  xlcd += font12x15[character] +1;
  
  character++;

  for (uint8_t i=0; i<12; i++ )
  {
     uint16_t chara=character+(i*2);
     uint8_t  v1=font12x15[chara];
     uint8_t  v2=font12x15[chara+1];
     uint16_t v3=(uint16_t)((uint16_t)v2 << 8)| v1;

     for (uint8_t j=0; j<15; j++ )
     {
			 int address = (x+i)+ ((y+j)/8)*LCD_X;
			 
			 if(address < sizeof(_LCD_RAM))
			 {
					if(v3 & 0x01) bit_set(&_LCD_RAM[address] , (y+j)%8);
       }
			 v3>>=1;
     }
  }
}

void LCD_write_str(uint8_t x,uint8_t y,char* str)
{
	xlcd = x;
	while(*str)
	{
		if(*str == ' ') xlcd+=font12x15[0];   
		else   showchar2(xlcd,y,*str);    

		str++;
	}
}

void LCD_write_str_middle(uint8_t y,char* str)
{
	uint8_t  XMid = 0;
	uint8_t  LenStr = 0;

	char *strcpy = str;
	while(*strcpy)
	{
		char ch = *strcpy;
		if(ch == ' ') LenStr+=font12x15[0];   
		else
		{
			ch -=32;
			uint16_t character = ((uint16_t) ch * 25 );
			
			LenStr += font12x15[character] +1;
		}			
		
		strcpy++;
	}
	
	XMid = (96 - LenStr)/ 2;
	LCD_write_str(XMid,y,str);
}

void LCD_WriteStatus(uint8_t x,uint8_t y,char* str,uint8_t Status)
{
   xlcd=x+2;
   while(*str)
   {
      showchar2(xlcd,y+2,*str);
      str++;
   }
   
   if(Status) LCD_DrawRect(x,y,(xlcd - x)+1,16,1);
}

void LCD_WriteMenuStatus(uint8_t x,uint8_t y,const char* str,uint8_t Status)
{
   xlcd=x+2;
   while(*str)
   {
      showchar2(xlcd,y+2,*str);
      str++;
   }
   
   if(Status) LCD_DrawRect(x,y,(xlcd - x)+1,16,1);
}

//// Initializing the display
//void LCD_Init(void) {
//  // Display initialization
//  HW_RESET_PIN(0);  // Activate the reset
//	HAL_Delay(5);
//	HW_RESET_PIN(1);  // deactivate the reset
//  HW_SCK_PIN(0);    // Clock output low state
//	HW_MOSI_PIN(0);   // Outputting data low
//	HW_CS_PIN(1);		 // Choosing a display
//	// Delay
//  HAL_Delay(5);
//	//HW_CS_PIN(1);	  //Choosing a display
//	
//	LCD_SendByte(LCD_C, 0xE2);  // Chip reset
//  HAL_Delay(5);
//  // Setting the segment charge energy
//	LCD_SendByte (LCD_C, 0x3D); // Charge energy multiplier
//  LCD_SendByte (LCD_C, 0x01); // Unclear value of the multiplier
//  // Command followed by contrast data
//  LCD_SendByte (LCD_C, 0xE1); // Additional VOP for contrast increase
//  LCD_SendByte (LCD_C, 0x90); // from -127 to +127
//  // Set the mode to Normal
//  LCD_SendByte (LCD_C, 0xA4); // Power saver off
//	
//	LCD_SendByte(LCD_C, 0x2F);  // Booster ON Voltage regulator ON Voltage follover ON
//	LCD_SendByte(LCD_C, 0xA0);  // Segment driver direction select: Normal
//  LCD_SendByte(LCD_C, 0xAF);  // Turn on the display
//  HAL_Delay (10);
//  // Clean up, update
//  //LCD_Clear ();
//  //LCD_Update ();
//}

// Initializing the display
void LCD_Init()
{
	LCD_InitF = 1;
	LCD_InitState = 0;
	LCD_InitTimer = 0;
	
	//NVIC_EnableIRQ(TIM2_IRQn);
}

