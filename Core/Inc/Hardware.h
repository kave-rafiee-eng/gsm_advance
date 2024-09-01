#include "main.h"


#define HW_LCD_PWR(x)						(x ? (GPIOE ->BSRR =0x00000002) : (GPIOE ->BSRR =0x00020000) ) //PE1
#define	HW_BKL(x)           		(x ? (GPIOE ->BSRR =0x00000004) : (GPIOE ->BSRR =0x00040000) ) //PE2
#define	HW_BUZZ(x)           		(x ? (GPIOA ->BSRR =0x00001000) : (GPIOA ->BSRR =0x10000000) ) //PA12
#define	HW_PO1(x)           		(x ? (GPIOD ->BSRR =0x00000001) : (GPIOD ->BSRR =0x00010000) ) //PD0
#define	HW_PO2(x)           		(x ? (GPIOC ->BSRR =0x00000800) : (GPIOC ->BSRR =0x08000000) ) //PC11
#define	HW_PO3(x)           		(x ? (GPIOC ->BSRR =0x00000400) : (GPIOC ->BSRR =0x04000000) ) //PC10
#define	HW_PO4(x)           		(x ? (GPIOA ->BSRR =0x00008000) : (GPIOA ->BSRR =0x80000000) ) //PA15
#define	HW_7191RLY(x)          	(x ? (GPIOA ->BSRR =0x00000800) : (GPIOA ->BSRR =0x08000000) ) //PA11
#define HW_OUTBJT(x)						(x ? (GPIOD ->BSRR =0x00000002) : (GPIOD ->BSRR =0x00020000) ) //PD1

#define	HW_LED_LINK_DBLX(x)   	(x ? (GPIOB ->BSRR =0x00000008) : (GPIOB ->BSRR =0x00080000) ) //PB3
#define	HW_DIR_DBLX(x)     			(x ? (GPIOD ->BSRR =0x00000080) : (GPIOD ->BSRR =0x00800000) ) //PD7
#define	HW_LED_LINK_DRV(x)   		(x ? (GPIOD ->BSRR =0x00000010) : (GPIOD ->BSRR =0x00100000) ) //PD4
#define	HW_DIR_DRV(x)     			(x ? (GPIOD ->BSRR =0x00000008) : (GPIOD ->BSRR =0x00080000) ) //PD3
#define	HW_LED_LINK_HALL(x)  		(x ? (GPIOA ->BSRR =0x00000004) : (GPIOA ->BSRR =0x00040000) ) //PA2
#define	HW_DIR_HALL(x)     			(x ? (GPIOA ->BSRR =0x00000008) : (GPIOA ->BSRR =0x00080000) ) //PA3

#define	HW_SLAVE_CLK(x)     		(x ? (GPIOA ->BSRR =0x00000020) : (GPIOA ->BSRR =0x00200000) ) //PA5
#define	HW_SLAVE_MOSI(x)     		(x ? (GPIOA ->BSRR =0x00000040) : (GPIOA ->BSRR =0x00400000) ) //PA6
#define HW_SLAVE_MISO    	  		((GPIOA->IDR & 0x0080) && 1) //PA7

#define KEY_UP     	  					((GPIOB->IDR & 0x1000) && 1) //PB12
#define KEY_ENT    	  					((GPIOE->IDR & 0x8000) && 1) //PE15
#define KEY_DN     	  					((GPIOB->IDR & 0x4000) && 1) //PB14
#define KEY_EXT    	  					((GPIOB->IDR & 0x2000) && 1) //PB13

#define HW_IN_CF3     	  			((GPIOC->IDR & 0x0040) && 1) //PC6
#define HW_IN_1CF     	  			((GPIOD->IDR & 0x8000) && 1) //PD15
#define HW_IN_CAN     	  			((GPIOD->IDR & 0x4000) && 1) //PD14
#define HW_IN_CA1    	  				((GPIOD->IDR & 0x2000) && 1) //PD13
#define HW_IN_1     	  				((GPIOD->IDR & 0x1000) && 1) //PD12
#define HW_IN_2     	  				((GPIOD->IDR & 0x0800) && 1) //PD11
#define HW_IN_3     	  				((GPIOD->IDR & 0x0400) && 1) //PD10
#define HW_IN_4     	  				((GPIOD->IDR & 0x0200) && 1) //PD9
#define HW_IN_5     	  				((GPIOD->IDR & 0x0100) && 1) //PD8
#define HW_IN_6     	  				((GPIOB->IDR & 0x8000) && 1) //PB15

#define HW_SAFETY_90     	  		((GPIOC->IDR & 0x0100) && 1) //PC8
#define HW_SAFETY_71     	  		((GPIOC->IDR & 0x0200) && 1) //PC9
#define HW_SAFETY_66     	  		((GPIOA->IDR & 0x0100) && 1) //PA8
#define HW_SAFETY_69     	  		((GPIOA->IDR & 0x0200) && 1) //PA9
#define HW_SAFETY_68     	  		((GPIOA->IDR & 0x0400) && 1) //PA10

#define HW_SHORT_24TO110     	  ((GPIOC->IDR & 0x0080) && 1) //PC7

#define MAX_PARALLEL_PB					12
#define MAX_PARALLEL_NUMRATOR		16
#define MAX_HW_INPUTS						100
#define MAX_HW_OUTPUTS					100

/*each sample take 58.6us * 341 = 20ms for 50Hz, 
	To get the exact value of this number, you can use the TestADCTime variable, 
	which counts the number of ADC conversions in every 20 ms
*/
#define PT_RMS_SAMPLE_COUNT			341 
#define PHASES_LOGIC_COMP_ADC		200 //Should be > 50

#define WDT_RESET 	IWDG->KR = 0x0000AAAA


void EncoderSetValue(int32_t Value);
void EncoderGetValue(void);

void PT_RMS_Calculate(void);

uint8_t	SevenBitToByte(_Bool Bit7,_Bool Bit6,_Bool Bit5,_Bool Bit4,_Bool Bit3,_Bool Bit2,_Bool Bit1,_Bool Bit0);
void AddressableInputsAndOutputs(void);


void TIM2_Init(void);		//100us Interrupter
void TIM4_Init(void);		//1ms Interrupter
void TIM5_Init(void);		//50us Interrupter

void Clear_UART_Errors(void);

void USART1_Init(void);	//For Print
void DMA2_Stream7_Init(void);	//For TX UART1
void PR_SendData(int length);
void DMA2_Stream5_Init(void); //For RX UART1
	
void USART2_Init(void);	//For Doublex
void DMA1_Stream6_Init(void);	//For TX UART2
void DBLX_SendData(int length);
void DMA1_Stream5_Init(void);	//For RX UART2

void UART5_Init(void);	//For Drive
void DMA1_Stream7_Init(void);	//For TX UART5
void DRV_SendData(int length);
void DMA1_Stream0_Init(void);	//For RX UART5

void USART3_Init(void);	//For Cabin
void DMA1_Stream3_Init(void);	//For TX UART3
void CAB_SendData(int length);
void DMA1_Stream1_Init(void);	//For RX UART3

void UART4_Init(void);	//For Hall
void DMA1_Stream4_Init(void);	//For TX UART4
void HALL_SendData(int length);
void DMA1_Stream2_Init(void);	//For RX UART4

//void TIM15_Init(void); 		//For Time measuring
