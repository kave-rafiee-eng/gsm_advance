#include "main.h"

#define READ_BIT(REG, BIT)    ((REG) & (BIT))
		
		
int stdout_putchar (int ch) {
 
	UART4->DR = ch;
			
	while(READ_BIT(UART4->SR, USART_SR_TXE) == (USART_SR_TXE) == 0){}
		//HAL_Delay(1);	
  return 1;
}
