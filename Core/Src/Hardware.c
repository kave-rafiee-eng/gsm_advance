#include "main.h"
#include "Hardware.h"
#include "Menu.h"
#include "Defines.h"
#include "Serial.h"
#include "Variable.h"


void EncoderSetValue(int32_t Value)
{
	EncoderINT = 0;
	while(Value > 65535)
	{
		EncoderINT += 65535;
		Value -= 65535;
	}
	while(Value < 0)
	{
		EncoderINT -= 65535;
		Value += 65535;
	}
	
	//uint32_t	 Cnt = Value;
	TIM3->CNT = (uint16_t) Value;
}

void EncoderGetValue(void)
{
	Encoder = EncoderINT + TIM3->CNT;
}


void PT_RMS_Calculate(void)
{
	if(PT_RMS_Counter >= PT_RMS_SAMPLE_COUNT)
	{
		float VRefIntAvg = (float) VRefIntSum / PT_RMS_Counter;
		ADCToVoltageFactor = 1.21 / VRefIntAvg;
		float PT_R_RMS = sqrt(PT_R_RMS_Sum / PT_RMS_Counter);
		float	PT_S_RMS = sqrt(PT_S_RMS_Sum / PT_RMS_Counter);
		float	PT_T_RMS = sqrt(PT_T_RMS_Sum / PT_RMS_Counter);
		
		Ph_R_RMS = (PT_R_RMS * ADCToVoltageFactor) * 470.0;
		Ph_S_RMS = (PT_S_RMS * ADCToVoltageFactor) * 470.0;
		Ph_T_RMS = (PT_T_RMS * ADCToVoltageFactor) * 470.0;
		
		PT_R_RMS_Sum = 0;
		PT_S_RMS_Sum = 0;
		PT_T_RMS_Sum = 0;
		PT_RMS_Counter = 0;
		VRefIntSum = 0;
	}
}


uint8_t	SevenBitToByte(_Bool Bit7,_Bool Bit6,_Bool Bit5,_Bool Bit4,_Bool Bit3,_Bool Bit2,_Bool Bit1,_Bool Bit0)
{
	_Bool BitArray[8] = {Bit0,Bit1,Bit2,Bit3,Bit4,Bit5,Bit6,Bit7};
	
	uint8_t Resault=0;
	
	for(uint8_t i=7;i<8;i--)
	{
		Resault |= ((uint8_t)BitArray[i] << i);
	}
	
	return Resault;
}

void AddressableInputsAndOutputs(void)
{
	/////	Putting program values into the addressable hardware output array
	HwOut[Stng[MAIN_OUT_HWADD +  1] ]	=	MOut.MUP			^ Stng[MAIN_OUT_TYPE_ADD +  1];
	HwOut[Stng[MAIN_OUT_HWADD +  2] ]	=	MOut.MDN			^ Stng[MAIN_OUT_TYPE_ADD +  2];
	HwOut[Stng[MAIN_OUT_HWADD +  3] ]	=	MOut.V0				^ Stng[MAIN_OUT_TYPE_ADD +  3];
	HwOut[Stng[MAIN_OUT_HWADD +  4] ]	=	MOut.V1				^ Stng[MAIN_OUT_TYPE_ADD +  4];
	HwOut[Stng[MAIN_OUT_HWADD +  5] ]	=	MOut.V2				^ Stng[MAIN_OUT_TYPE_ADD +  5];
	HwOut[Stng[MAIN_OUT_HWADD +  6] ]	=	MOut.TC				^ Stng[MAIN_OUT_TYPE_ADD +  6];
	HwOut[Stng[MAIN_OUT_HWADD +  7] ]	=	MOut.STDL			^ Stng[MAIN_OUT_TYPE_ADD +  7];
	HwOut[Stng[MAIN_OUT_HWADD +  8] ]	=	MOut.URA			^ Stng[MAIN_OUT_TYPE_ADD +  8];
	HwOut[Stng[MAIN_OUT_HWADD +  9] ]	=	MOut.Fan			^ Stng[MAIN_OUT_TYPE_ADD +  9];
	HwOut[Stng[MAIN_OUT_HWADD + 10] ]	=	MOut.EN_Out		^ Stng[MAIN_OUT_TYPE_ADD + 10];
	HwOut[Stng[MAIN_OUT_HWADD + 11] ]	=	MOut.Relay		^ Stng[MAIN_OUT_TYPE_ADD + 11];
	HwOut[Stng[MAIN_OUT_HWADD + 12] ]	=	MOut.UPSRelay	^ Stng[MAIN_OUT_TYPE_ADD + 12];
	HwOut[Stng[MAIN_OUT_HWADD + 13] ]	=	MOut.UPSRef		^ Stng[MAIN_OUT_TYPE_ADD + 13];
	HwOut[Stng[MAIN_OUT_HWADD + 14] ]	=	MOut.ADO			^ Stng[MAIN_OUT_TYPE_ADD + 14];
	HwOut[Stng[MAIN_OUT_HWADD + 15] ]	=	MOut.Out1CF		^ Stng[MAIN_OUT_TYPE_ADD + 15];
	HwOut[Stng[MAIN_OUT_HWADD + 16] ]	=	MOut.DRV_RST	^ Stng[MAIN_OUT_TYPE_ADD + 16];
	HwOut[Stng[MAIN_OUT_HWADD + 17] ]	=	MOut.ERS			^ Stng[MAIN_OUT_TYPE_ADD + 17];
	HwOut[Stng[MAIN_OUT_HWADD + 18] ]	=	MOut.ENR			^ Stng[MAIN_OUT_TYPE_ADD + 18];
	HwOut[Stng[MAIN_OUT_HWADD + 19] ]	=	MOut.RES4			^ Stng[MAIN_OUT_TYPE_ADD + 19];
	HwOut[Stng[MAIN_OUT_HWADD + 20] ]	=	MOut.RES5			^ Stng[MAIN_OUT_TYPE_ADD + 20];
	HwOut[Stng[MAIN_OUT_HWADD + 21] ]	=	MOut.Close1		^ Stng[MAIN_OUT_TYPE_ADD + 21];
	HwOut[Stng[MAIN_OUT_HWADD + 22] ]	=	MOut.Close2		^ Stng[MAIN_OUT_TYPE_ADD + 22];
	HwOut[Stng[MAIN_OUT_HWADD + 23] ]	=	MOut.Close3		^ Stng[MAIN_OUT_TYPE_ADD + 23];
	HwOut[Stng[MAIN_OUT_HWADD + 24] ]	=	MOut.Close4		^ Stng[MAIN_OUT_TYPE_ADD + 24];
	HwOut[Stng[MAIN_OUT_HWADD + 25] ]	=	MOut.Close5		^ Stng[MAIN_OUT_TYPE_ADD + 25];
	
	/////	Putting addressable HwOut array values into the board outputs
	Hw_OUTPN 	= HwOut[1];
	Hw_PO1		= HwOut[2];
	Hw_PO2		= HwOut[3];
	Hw_PO3		= HwOut[4];
	Hw_PO4		= HwOut[5];
	Hw_OC1		= HwOut[6];
	Hw_OC2		= HwOut[7];
	Hw_OC3		= HwOut[8];
	Hw_OC4		= HwOut[9];
	Hw_OC5		= HwOut[10];
	Hw_OC6		= HwOut[11];
	Hw_OC7		= HwOut[12];
	
	HW_OUTBJT(Hw_OUTPN);
	HW_PO1(Hw_PO1);
	HW_PO2(Hw_PO2);
	HW_PO3(Hw_PO3);
	HW_PO4(Hw_PO4);
	HW_7191RLY(Hw_RLY_7191);
	HW_BUZZ(Hw_BUZZ);
	/////	The other outputs will be sent to the slave micro
	
	
	/////	Putting board inputs to addressable hardware inputs array
	HwIn[1]	= HW_IN_CF3;
	HwIn[2]	= HW_IN_1CF;
	HwIn[3]	= HW_IN_CAN;
	HwIn[4]	= HW_IN_CA1;
	HwIn[5]	= HW_IN_1;
	HwIn[6]	= HW_IN_2;
	HwIn[7]	= HW_IN_3;
	HwIn[8]	= HW_IN_4;
	HwIn[9]	= HW_IN_5;
	HwIn[10]= HW_IN_6;
	/////	The other inputs will read from slave micro
	
	/////	Putting addressable HwIn array values into the program inputs
	MIn.CF3 			= (HwIn[Stng[MAIN_IN_HWADD +  1]] ^ Stng[MAIN_IN_TYPE_ADD +  1]);
	MIn._1CF 			= (HwIn[Stng[MAIN_IN_HWADD +  2]] ^ Stng[MAIN_IN_TYPE_ADD +  2]);
	MIn.CAN 			= (HwIn[Stng[MAIN_IN_HWADD +  3]] ^ Stng[MAIN_IN_TYPE_ADD +  3]);
	MIn.CA1 			= (HwIn[Stng[MAIN_IN_HWADD +  4]] ^ Stng[MAIN_IN_TYPE_ADD +  4]);
	MIn.IFN 			= (HwIn[Stng[MAIN_IN_HWADD +  5]] ^ Stng[MAIN_IN_TYPE_ADD +  5]);
	MIn.IF1 			= (HwIn[Stng[MAIN_IN_HWADD +  6]] ^ Stng[MAIN_IN_TYPE_ADD +  6]);
	MIn.FLTDR			= (HwIn[Stng[MAIN_IN_HWADD +  7]] ^ Stng[MAIN_IN_TYPE_ADD +  7]);
	MIn.RLS				= (HwIn[Stng[MAIN_IN_HWADD +  8]] ^ Stng[MAIN_IN_TYPE_ADD +  8]);
	MIn.DRC				=	(HwIn[Stng[MAIN_IN_HWADD +  9]] ^ Stng[MAIN_IN_TYPE_ADD +  9]);
	MIn.Fire			= (HwIn[Stng[MAIN_IN_HWADD + 10]] ^ Stng[MAIN_IN_TYPE_ADD + 10]);
	MIn.Overload	=	(HwIn[Stng[MAIN_IN_HWADD + 11]] ^ Stng[MAIN_IN_TYPE_ADD + 11]);
	MIn.FullLoad	=	(HwIn[Stng[MAIN_IN_HWADD + 12]] ^ Stng[MAIN_IN_TYPE_ADD + 12]);
	MIn.RelvUp		=	(HwIn[Stng[MAIN_IN_HWADD + 13]] ^ Stng[MAIN_IN_TYPE_ADD + 13]);
	MIn.RelvDn		=	(HwIn[Stng[MAIN_IN_HWADD + 14]] ^ Stng[MAIN_IN_TYPE_ADD + 14]);
	MIn.ZADO 			= (HwIn[Stng[MAIN_IN_HWADD + 15]] ^ Stng[MAIN_IN_TYPE_ADD + 15]);
	MIn.Ready			= (HwIn[Stng[MAIN_IN_HWADD + 16]] ^ Stng[MAIN_IN_TYPE_ADD + 16]);
	MIn.RES2			= (HwIn[Stng[MAIN_IN_HWADD + 17]] ^ Stng[MAIN_IN_TYPE_ADD + 17]);
	MIn.RES3			= (HwIn[Stng[MAIN_IN_HWADD + 18]] ^ Stng[MAIN_IN_TYPE_ADD + 18]);
	MIn.RES4			= (HwIn[Stng[MAIN_IN_HWADD + 19]] ^ Stng[MAIN_IN_TYPE_ADD + 19]);
	MIn.RES5			= (HwIn[Stng[MAIN_IN_HWADD + 20]] ^ Stng[MAIN_IN_TYPE_ADD + 20]);
}



void TIM2_Init(void) 
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;			/* enable clock for TIM2   */        
	
	TIM2->CR1  = 0;
  TIM2->PSC  = 83;                        // 83+1 = 84 MHz APB1 Timers Clock Frequency
  TIM2->ARR   = ( 99 );               		/* set auto-reload = 100us    */

	TIM2->DIER = TIM_DIER_UIE;            	/* Update Interrupt enable  */
  NVIC_EnableIRQ(TIM2_IRQn);             	/* TIM2   Interrupt enable */

  TIM2->CR1  |= TIM_CR1_CEN;            	/* timer enable             */
}

void TIM4_Init(void) 
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;			/* enable clock for TIM2   */        
	
	TIM4->CR1  = 0;
  TIM4->PSC  = 83;                        // 83+1 = 84 MHz APB1 Timers Clock Frequency
  TIM4->ARR   = ( 999 );               		/* set auto-reload = 1ms    */

	TIM4->DIER = TIM_DIER_UIE;            	/* Update Interrupt enable  */
  NVIC_EnableIRQ(TIM4_IRQn);             	/* TIM2   Interrupt enable */

  TIM4->CR1  |= TIM_CR1_CEN;            	/* timer enable             */
}
void TIM5_Init(void) 
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;			/* enable clock for TIM2   */        
	
	TIM5->CR1  = 0;
  TIM5->PSC  = 83;                        // 83+1 = 84 MHz APB1 Timers Clock Frequency
  TIM5->ARR   = ( 49 );               		/* set auto-reload = 1ms    */

	TIM5->DIER = TIM_DIER_UIE;            	/* Update Interrupt enable  */
  NVIC_EnableIRQ(TIM5_IRQn);             	/* TIM2   Interrupt enable */

  TIM5->CR1  |= TIM_CR1_CEN;            	/* timer enable             */
}

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////  Clear UARTs Errors  ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
void Clear_UART_Errors(void)
{
	if(USART1->SR & 0x0000000A) //Error Check
	{
		USART1->DR;
		USART1->SR &= ~(0x000003E0);
		USART1_Init();	//For Print
		DMA2_Stream7_Init();	//For TX UART1 (PR)
		DMA2_Stream5_Init();	//For RX UART1 (PR)
	}
	if(USART2->SR & 0x0000000A) //Error Check
	{
		USART2->DR;
		USART2->SR &= ~(0x000003E0);
		USART2_Init();	//For Print
		DMA1_Stream6_Init();	//For TX UART2 (DBLX)
		DMA1_Stream5_Init();	//For RX UART2 (DBLX)
	}
	if(USART3->SR & 0x0000000A) //Error Check
	{
		USART3->DR;
		USART3->SR &= ~(0x000003E0);
		USART3_Init();	//For Print
		DMA1_Stream3_Init();	//For TX UART2 (DBLX)
		DMA1_Stream1_Init();	//For RX UART2 (DBLX)
	}
	if(UART4->SR & 0x0000000A) //Error Check
	{
		UART4->DR;
		UART4->SR &= ~(0x000003E0);
		UART4_Init();	//For Drive
		DMA1_Stream4_Init();	//For TX UART2 (DRV)
		DMA1_Stream2_Init();	//For RX UART2 (DRV)
	}
	if(UART5->SR & 0x0000000A) //Error Check
	{
		UART5->DR;
		UART5->SR &= ~(0x000003E0);
		UART5_Init();	//For Drive
		DMA1_Stream7_Init();	//For TX UART2 (DRV)
		DMA1_Stream0_Init();	//For RX UART2 (DRV)
	}
	
	
}


void USART1_Init(void) //For Print
{
	int i;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER &= (uint32_t)~( ((uint32_t)0x03 << 12) | ((uint32_t)0x03 << 14) );  //PB6,7 Clear
	GPIOB->MODER |= (uint32_t) ( ((uint32_t)0x02 << 12) | ((uint32_t)0x02 << 14) );  //PB6,7 Alternate function mode
	
	GPIOB->AFR[0] &= ~( 0xFF000000 ); //PB6,7 AF Clear
	GPIOB->AFR[0] |=  ( 0x77000000 ); //PB6,7 AF Set 7 for USART1
	
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;    // enable USART1 clock

	USART1->BRR   = 0x02D9;                  // 115200 baud @ APB2_CLK == 84MHz
	USART1->CR1   = 0;                       // Clear Control Register 1
	USART1->CR1   = USART_CR1_TE  | USART_CR1_RE;     // enable TX & RX

	
	USART1->CR3  |= USART_CR3_DMAT | USART_CR3_DMAR;        // Enable TX DMA Bit & RX DMA Bit
	for (i = 0; i < 0x1000; i++) __NOP();    // avoid unwanted output

	USART1->CR1  |= USART_CR1_UE;// | USART_CR1_RXNEIE_RXFNEIE;   // Enable USART3 & RX Interrupts
	//NVIC_EnableIRQ(USART1_IRQn);                   /* USART2  RX Interrupt enable */
}


void DMA2_Stream7_Init(void)	//For TX UART1
{
	DMA2_Stream7->CR  = 0; 	/* DMA Disable  */
	//
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN; 	/* enable peripheral clock for DMA2 */
	
	DMA2_Stream7->M0AR  = (uint32_t) PR_TX_Buffer;  	/* set chn1 memory address    */
	DMA2_Stream7->PAR  	= (uint32_t) &USART1->DR;  		/* set chn1 peripheral address*/
	DMA2_Stream7->NDTR 	= 4;  												/* transmit size word         */
	
	DMA2->HIFCR |= 0x0F400000;
	
	/* configure DMA Stream      */
	PR_TxDMA_CR_Val =  		((uint32_t) 4<<25) |   		//CHSEL[2:0]:	Stream channel selection
												((uint32_t)	0<<16) |  		//PL[1:0]:		Channel priority level 0:Low 1:Medium 2:High 3:Very high
												((uint32_t)	0<<13) |  		//MSIZE[1:0]:	Memory size 0:8bit 1:16bit 2:32bit
												((uint32_t)	0<<11) |   		//PSIZE[1:0]:	Peripheral size 0:8bit 1:16bit 2:32bit
												((uint32_t)	1<<10) |   		//MINC: Memory increment mode Disable
												((uint32_t)	0<< 9) |   		//PINC: Peripheral increment mode Enable
												((uint32_t)	0<< 8) |   		//CIRC: Circular mode Disable
												((uint32_t)	1<< 6) |   		//DIR: Data transfer direction  0: Peripheral-to-memory, 1: Memory-to-peripheral, 2: Memory-to-Memory
												((uint32_t)	0<< 5) |			//PFCTRL: Peripheral flow controller 0: The DMA is the flow controller, 1: The peripheral is the flow controller
												((uint32_t)	0<< 4) |   		//TCIE: Transfer complete interrupt enable
												((uint32_t)	0<< 3) |   		//HTIE: Half transfer interrupt enable
												((uint32_t)	0<< 2) | 			//TEIE: Transfer error interrupt enable
												((uint32_t)	0<< 1) |   		//DMEIE: Direct mode error interrupt enable
												((uint32_t)	0<< 0) ;   		//Stream enable / flag stream ready when read low
	DMA2_Stream7->CR	= PR_TxDMA_CR_Val;
	
	
	DMA2_Stream7->CR  |= DMA_SxCR_EN;     /* DMA Enable  */
}

void PR_SendData(int length)
{
	DMA2_Stream7->CR  = 0;
	DMA2_Stream7->NDTR = length;
	DMA2->HIFCR |= 0x0F400000;						// Clear Flags
	DMA2_Stream7->CR	=	PR_TxDMA_CR_Val;
	DMA2_Stream7->CR	|= DMA_SxCR_EN;     /* DMA Enable  */
}

void DMA2_Stream5_Init(void)	//For RX UART1
{
	DMA2_Stream5->CR  = 0; 	/* DMA 2 Stream 5 Disable  */
	//
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN; 	/* enable peripheral clock for DMA2 */
	
	DMA2_Stream5->M0AR  = (uint32_t) PR_RX_Buffer;  	/* set chn1 memory address    */
	DMA2_Stream5->PAR  	= (uint32_t) &USART1->DR;  		/* set chn1 peripheral address*/
	DMA2_Stream5->NDTR 	= PR_BUFFER_SIZE;//sizeof(PR_TX_Buffer)-1;  		/* transmit size word         */
	
	/* configure DMA Stream      */
	DMA2_Stream5->CR	= 	((uint32_t) 4<<25) |   		//CHSEL[2:0]:	Stream channel selection
												((uint32_t)	0<<16) |  		//PL[1:0]:		Channel priority level 0:Low 1:Medium 2:High 3:Very high
												((uint32_t)	0<<13) |  		//MSIZE[1:0]:	Memory size 0:8bit 1:16bit 2:32bit
												((uint32_t)	0<<11) |   		//PSIZE[1:0]:	Peripheral size 0:8bit 1:16bit 2:32bit
												((uint32_t)	1<<10) |   		//MINC: Memory increment mode Disable
												((uint32_t)	0<< 9) |   		//PINC: Peripheral increment mode Enable
												((uint32_t)	1<< 8) |   		//CIRC: Circular mode Disable
												((uint32_t)	0<< 6) |   		//DIR: Data transfer direction  0: Peripheral-to-memory, 1: Memory-to-peripheral, 2: Memory-to-Memory
												((uint32_t)	0<< 5) |			//PFCTRL: Peripheral flow controller 0: The DMA is the flow controller, 1: The peripheral is the flow controller
												((uint32_t)	0<< 4) |   		//TCIE: Transfer complete interrupt enable
												((uint32_t)	0<< 3) |   		//HTIE: Half transfer interrupt enable
												((uint32_t)	0<< 2) | 			//TEIE: Transfer error interrupt enable
												((uint32_t)	0<< 1) |   		//DMEIE: Direct mode error interrupt enable
												((uint32_t)	0<< 0) ;   		//Stream enable / flag stream ready when read low
	
	DMA2->HIFCR |= 0x00000F40;
	
	DMA2_Stream5->CR  |= DMA_SxCR_EN;     /* DMA Enable  */
}





void USART2_Init(void) //For Doublex
{
	int i;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER &= (uint32_t)~( ((uint32_t)0x03 << 10) | ((uint32_t)0x03 << 12) );  //PD5,6 Clear
	GPIOD->MODER |= (uint32_t) ( ((uint32_t)0x02 << 10) | ((uint32_t)0x02 << 12) );  //PD5,6 Alternate function mode
	
	GPIOD->AFR[0] &= ~( 0x0FF00000 ); //PD5,6 AF Clear
	GPIOD->AFR[0] |=  ( 0x07700000 ); //PD5,6 AF Set 7 for USART2
	
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;    // enable USART2 clock

	USART2->BRR   = 0x1117;                  // 9600 baud @ APB1_CLK == 42MHz
	USART2->CR1   = 0;                       // Clear Control Register 1
	USART2->CR1   = USART_CR1_TE  | USART_CR1_RE;     // enable TX & RX

	
	USART2->CR3  |= USART_CR3_DMAT | USART_CR3_DMAR;        // Enable TX DMA Bit & RX DMA Bit
	for (i = 0; i < 0x1000; i++) __NOP();    // avoid unwanted output

	USART2->CR1  |= USART_CR1_UE;// | USART_CR1_RXNEIE_RXFNEIE;   // Enable USART3 & RX Interrupts
}


void DMA1_Stream6_Init(void)	//For TX UART2
{
	DMA1_Stream6->CR  = 0; 	/* DMA Disable  */
	//
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; 	/* enable peripheral clock for DMA1 */
	
	DMA1_Stream6->M0AR  = (uint32_t) DBLX_TX_Buffer;	/* set chn1 memory address    */
	DMA1_Stream6->PAR  	= (uint32_t) &USART2->DR;  		/* set chn1 peripheral address*/
	DMA1_Stream6->NDTR 	= 4;  												/* transmit size word         */
	
	DMA1->HIFCR |= 0x003D0000;
	
	/* configure DMA Stream      */
	DBLX_TxDMA_CR_Val =		((uint32_t) 4<<25) |   		//CHSEL[2:0]:	Stream channel selection
												((uint32_t)	0<<16) |  		//PL[1:0]:		Channel priority level 0:Low 1:Medium 2:High 3:Very high
												((uint32_t)	0<<13) |  		//MSIZE[1:0]:	Memory size 0:8bit 1:16bit 2:32bit
												((uint32_t)	0<<11) |   		//PSIZE[1:0]:	Peripheral size 0:8bit 1:16bit 2:32bit
												((uint32_t)	1<<10) |   		//MINC: Memory increment mode Disable
												((uint32_t)	0<< 9) |   		//PINC: Peripheral increment mode Enable
												((uint32_t)	0<< 8) |   		//CIRC: Circular mode Disable
												((uint32_t)	1<< 6) |   		//DIR: Data transfer direction  0: Peripheral-to-memory, 1: Memory-to-peripheral, 2: Memory-to-Memory
												((uint32_t)	0<< 5) |			//PFCTRL: Peripheral flow controller 0: The DMA is the flow controller, 1: The peripheral is the flow controller
												((uint32_t)	0<< 4) |   		//TCIE: Transfer complete interrupt enable
												((uint32_t)	0<< 3) |   		//HTIE: Half transfer interrupt enable
												((uint32_t)	0<< 2) | 			//TEIE: Transfer error interrupt enable
												((uint32_t)	0<< 1) |   		//DMEIE: Direct mode error interrupt enable
												((uint32_t)	0<< 0) ;   		//Stream enable / flag stream ready when read low
	DMA1_Stream6->CR	= DBLX_TxDMA_CR_Val;
	
	DMA1_Stream6->CR  |= DMA_SxCR_EN;     /* DMA Enable  */
}

void DBLX_SendData(int length)
{
	HW_DIR_DBLX(1);
	DBLX_TxModeF = 1;
	
	DMA1_Stream6->CR  = 0;
	DMA1_Stream6->NDTR = length;
	DMA1->HIFCR |= 0x003D0000;						// Clear Flags
	DMA1_Stream6->CR	=	DBLX_TxDMA_CR_Val;
	DMA1_Stream6->CR  |= DMA_SxCR_EN;     /* DMA Enable  */
}

void DMA1_Stream5_Init(void)	//For RX UART2
{
	DMA1_Stream5->CR  = 0; 	/* DMA Disable  */
	//
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; 	/* enable peripheral clock for DMA1 */
	
	DMA1_Stream5->M0AR  = (uint32_t) DBLX_RX_Buffer;  	/* set chn1 memory address    */
	DMA1_Stream5->PAR  	= (uint32_t) &USART2->DR;  			/* set chn1 peripheral address*/
	DMA1_Stream5->NDTR 	= UART_BUFFER_SIZE;//sizeof(PR_TX_Buffer)-1;  		/* transmit size word         */
	
	/* configure DMA Stream      */
	DMA1_Stream5->CR	= 	((uint32_t) 4<<25) |   		//CHSEL[2:0]:	Stream channel selection
												((uint32_t)	0<<16) |  		//PL[1:0]:		Channel priority level 0:Low 1:Medium 2:High 3:Very high
												((uint32_t)	0<<13) |  		//MSIZE[1:0]:	Memory size 0:8bit 1:16bit 2:32bit
												((uint32_t)	0<<11) |   		//PSIZE[1:0]:	Peripheral size 0:8bit 1:16bit 2:32bit
												((uint32_t)	1<<10) |   		//MINC: Memory increment mode Disable
												((uint32_t)	0<< 9) |   		//PINC: Peripheral increment mode Enable
												((uint32_t)	1<< 8) |   		//CIRC: Circular mode Disable
												((uint32_t)	0<< 6) |   		//DIR: Data transfer direction  0: Peripheral-to-memory, 1: Memory-to-peripheral, 2: Memory-to-Memory
												((uint32_t)	0<< 5) |			//PFCTRL: Peripheral flow controller 0: The DMA is the flow controller, 1: The peripheral is the flow controller
												((uint32_t)	0<< 4) |   		//TCIE: Transfer complete interrupt enable
												((uint32_t)	0<< 3) |   		//HTIE: Half transfer interrupt enable
												((uint32_t)	0<< 2) | 			//TEIE: Transfer error interrupt enable
												((uint32_t)	0<< 1) |   		//DMEIE: Direct mode error interrupt enable
												((uint32_t)	0<< 0) ;   		//Stream enable / flag stream ready when read low
	
	DMA1->HIFCR |= 0x00000F40;
	
	DMA1_Stream5->CR  |= DMA_SxCR_EN;     /* DMA Enable  */
}





void UART5_Init(void)	//For Drive
{
	int i;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOD->MODER &= (uint32_t)~( ((uint32_t)0x03 << 4) );  	//PD2 Clear
	GPIOD->MODER |= (uint32_t) ( ((uint32_t)0x02 << 4) );  	//PD2 Alternate function mode
	GPIOC->MODER &= (uint32_t)~( ((uint32_t)0x03 << 24) );  //PC12 Clear
	GPIOC->MODER |= (uint32_t) ( ((uint32_t)0x02 << 24) );  //PC12 Alternate function mode
	
	GPIOD->AFR[0] &= ~( 0x00000F00 ); //PD2 AF Clear
	GPIOD->AFR[0] |=  ( 0x00000800 ); //PD2 AF Set 8 for USART5
	GPIOC->AFR[1] &= ~( 0x000F0000 ); //PC12 AF Clear
	GPIOC->AFR[1] |=  ( 0x00080000 ); //PC12 AF Set 8 for USART5
	
	RCC->APB1ENR |= RCC_APB1ENR_UART5EN;    // enable USART5 clock

	UART5->BRR   = 0x1117;                  // 9600 baud @ APB1_CLK == 42MHz
	UART5->CR1   = 0;                       // Clear Control Register 1
	UART5->CR1   = USART_CR1_TE  | USART_CR1_RE;     // enable TX & RX

	
	UART5->CR3  |= USART_CR3_DMAT | USART_CR3_DMAR;        // Enable TX DMA Bit & RX DMA Bit
	for (i = 0; i < 0x1000; i++) __NOP();    // avoid unwanted output

	UART5->CR1  |= USART_CR1_UE;// | USART_CR1_RXNEIE_RXFNEIE;   // Enable USART3 & RX Interrupts
}


void DMA1_Stream7_Init(void)	//For TX UART5
{
	DMA1_Stream7->CR  = 0; 	/* DMA Disable  */
	//
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; 	/* enable peripheral clock for DMA1 */
	
	DMA1_Stream7->M0AR  = (uint32_t) DRV_TX_Buffer;	/* set chn1 memory address    */
	DMA1_Stream7->PAR  	= (uint32_t) &UART5->DR;  		/* set chn1 peripheral address*/
	DMA1_Stream7->NDTR 	= 4;  												/* transmit size word         */
	
	DMA1->HIFCR |= 0x0F400000;
	
	/* configure DMA Stream      */
	DRV_TxDMA_CR_Val =		((uint32_t) 4<<25) |   		//CHSEL[2:0]:	Stream channel selection
												((uint32_t)	0<<16) |  		//PL[1:0]:		Channel priority level 0:Low 1:Medium 2:High 3:Very high
												((uint32_t)	0<<13) |  		//MSIZE[1:0]:	Memory size 0:8bit 1:16bit 2:32bit
												((uint32_t)	0<<11) |   		//PSIZE[1:0]:	Peripheral size 0:8bit 1:16bit 2:32bit
												((uint32_t)	1<<10) |   		//MINC: Memory increment mode Disable
												((uint32_t)	0<< 9) |   		//PINC: Peripheral increment mode Enable
												((uint32_t)	0<< 8) |   		//CIRC: Circular mode Disable
												((uint32_t)	1<< 6) |   		//DIR: Data transfer direction  0: Peripheral-to-memory, 1: Memory-to-peripheral, 2: Memory-to-Memory
												((uint32_t)	0<< 5) |			//PFCTRL: Peripheral flow controller 0: The DMA is the flow controller, 1: The peripheral is the flow controller
												((uint32_t)	0<< 4) |   		//TCIE: Transfer complete interrupt enable
												((uint32_t)	0<< 3) |   		//HTIE: Half transfer interrupt enable
												((uint32_t)	0<< 2) | 			//TEIE: Transfer error interrupt enable
												((uint32_t)	0<< 1) |   		//DMEIE: Direct mode error interrupt enable
												((uint32_t)	0<< 0) ;   		//Stream enable / flag stream ready when read low
	DMA1_Stream7->CR	= DRV_TxDMA_CR_Val;
	
	DMA1_Stream7->CR  |= DMA_SxCR_EN;     /* DMA Enable  */
}

void DRV_SendData(int length)
{
	HW_DIR_DRV(1);
	DRV_TxModeF = 1;
	
	DMA1_Stream7->CR  = 0;
	DMA1_Stream7->NDTR = length;
	DMA1->HIFCR |= 0x0F400000;						// Clear Flags
	DMA1_Stream7->CR	=	DRV_TxDMA_CR_Val;
	DMA1_Stream7->CR  |= DMA_SxCR_EN;     /* DMA Enable  */
}

void DMA1_Stream0_Init(void)	//For RX UART5
{
	DMA1_Stream0->CR  = 0; 	/* DMA Disable  */
	//
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; 	/* enable peripheral clock for DMA1 */
	
	DMA1_Stream0->M0AR  = (uint32_t) DRV_RX_Buffer;  	/* set chn1 memory address    */
	DMA1_Stream0->PAR  	= (uint32_t) &UART5->DR;  			/* set chn1 peripheral address*/
	DMA1_Stream0->NDTR 	= UART_BUFFER_SIZE;//sizeof(PR_TX_Buffer)-1;  		/* transmit size word         */
	
	/* configure DMA Stream      */
	DMA1_Stream0->CR	= 	((uint32_t) 4<<25) |   		//CHSEL[2:0]:	Stream channel selection
												((uint32_t)	0<<16) |  		//PL[1:0]:		Channel priority level 0:Low 1:Medium 2:High 3:Very high
												((uint32_t)	0<<13) |  		//MSIZE[1:0]:	Memory size 0:8bit 1:16bit 2:32bit
												((uint32_t)	0<<11) |   		//PSIZE[1:0]:	Peripheral size 0:8bit 1:16bit 2:32bit
												((uint32_t)	1<<10) |   		//MINC: Memory increment mode Disable
												((uint32_t)	0<< 9) |   		//PINC: Peripheral increment mode Enable
												((uint32_t)	1<< 8) |   		//CIRC: Circular mode Disable
												((uint32_t)	0<< 6) |   		//DIR: Data transfer direction  0: Peripheral-to-memory, 1: Memory-to-peripheral, 2: Memory-to-Memory
												((uint32_t)	0<< 5) |			//PFCTRL: Peripheral flow controller 0: The DMA is the flow controller, 1: The peripheral is the flow controller
												((uint32_t)	0<< 4) |   		//TCIE: Transfer complete interrupt enable
												((uint32_t)	0<< 3) |   		//HTIE: Half transfer interrupt enable
												((uint32_t)	0<< 2) | 			//TEIE: Transfer error interrupt enable
												((uint32_t)	0<< 1) |   		//DMEIE: Direct mode error interrupt enable
												((uint32_t)	0<< 0) ;   		//Stream enable / flag stream ready when read low
	
	DMA1->LIFCR |= 0x0000003D;
	
	DMA1_Stream0->CR  |= DMA_SxCR_EN;     /* DMA Enable  */
}



void USART3_Init(void) //For Cabin
{
	int i;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER &= (uint32_t)~( ((uint32_t)0x03 << 20) | ((uint32_t)0x03 << 22) );  //PB10,11 Clear,
	GPIOB->MODER |= (uint32_t) ( ((uint32_t)0x02 << 20) | ((uint32_t)0x02 << 22) );  //PB10,11 Alternate function mode
	
	GPIOB->AFR[1] &= ~( 0x0000FF00 ); //PB10,11 AF Clear
	GPIOB->AFR[1] |=  ( 0x00007700 ); //PB10,11 AF Set 7 for USART3
	
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;    // enable USART3 clock

	USART3->BRR   = 0x1117;                  // 9600 baud @ APB1_CLK == 42MHz
	USART3->CR1   = 0;                       // Clear Control Register 1
	USART3->CR1   = USART_CR1_TE  | USART_CR1_RE;     // enable TX & RX

	
	USART3->CR3  |= USART_CR3_DMAT | USART_CR3_DMAR;        // Enable TX DMA Bit & RX DMA Bit
	for (i = 0; i < 0x1000; i++) __NOP();    // avoid unwanted output

	USART3->CR1  |= USART_CR1_UE;// | USART_CR1_RXNEIE_RXFNEIE;   // Enable USART3 & RX Interrupts
}


void DMA1_Stream3_Init(void)	//For TX UART3
{
	DMA1_Stream3->CR  = 0; 	/* DMA Disable  */
	//
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; 	/* enable peripheral clock for DMA1 */
	
	DMA1_Stream3->M0AR  = (uint32_t) CAB_TX_Buffer;	/* set chn1 memory address    */
	DMA1_Stream3->PAR  	= (uint32_t) &USART3->DR;  		/* set chn1 peripheral address*/
	DMA1_Stream3->NDTR 	= 4;  												/* transmit size word         */
	
	DMA1->LIFCR |= 0x0F400000;
	
	/* configure DMA Stream      */
	CAB_TxDMA_CR_Val =		((uint32_t) 4<<25) |   		//CHSEL[2:0]:	Stream channel selection
												((uint32_t)	0<<16) |  		//PL[1:0]:		Channel priority level 0:Low 1:Medium 2:High 3:Very high
												((uint32_t)	0<<13) |  		//MSIZE[1:0]:	Memory size 0:8bit 1:16bit 2:32bit
												((uint32_t)	0<<11) |   		//PSIZE[1:0]:	Peripheral size 0:8bit 1:16bit 2:32bit
												((uint32_t)	1<<10) |   		//MINC: Memory increment mode Disable
												((uint32_t)	0<< 9) |   		//PINC: Peripheral increment mode Enable
												((uint32_t)	0<< 8) |   		//CIRC: Circular mode Disable
												((uint32_t)	1<< 6) |   		//DIR: Data transfer direction  0: Peripheral-to-memory, 1: Memory-to-peripheral, 2: Memory-to-Memory
												((uint32_t)	0<< 5) |			//PFCTRL: Peripheral flow controller 0: The DMA is the flow controller, 1: The peripheral is the flow controller
												((uint32_t)	0<< 4) |   		//TCIE: Transfer complete interrupt enable
												((uint32_t)	0<< 3) |   		//HTIE: Half transfer interrupt enable
												((uint32_t)	0<< 2) | 			//TEIE: Transfer error interrupt enable
												((uint32_t)	0<< 1) |   		//DMEIE: Direct mode error interrupt enable
												((uint32_t)	0<< 0) ;   		//Stream enable / flag stream ready when read low
	DMA1_Stream3->CR	= CAB_TxDMA_CR_Val;
	
	DMA1_Stream3->CR  |= DMA_SxCR_EN;     /* DMA Enable  */
}

void CAB_SendData(int length)
{
	CAB_TxModeF = 1;
	
	DMA1_Stream3->CR  = 0;
	DMA1_Stream3->NDTR = length;
	DMA1->LIFCR |= 0x0F400000;					// Clear Flags
	DMA1_Stream3->CR	=	CAB_TxDMA_CR_Val;
	DMA1_Stream3->CR  |= DMA_SxCR_EN;     /* DMA Enable  */
}

void DMA1_Stream1_Init(void)	//For RX UART3
{
	DMA1_Stream1->CR  = 0; 	/* DMA Disable  */
	//
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; 	/* enable peripheral clock for DMA1 */
	
	DMA1_Stream1->M0AR  = (uint32_t) CAB_RX_Buffer;  	/* set chn1 memory address    */
	DMA1_Stream1->PAR  	= (uint32_t) &USART3->DR;  			/* set chn1 peripheral address*/
	DMA1_Stream1->NDTR 	= UART_BUFFER_SIZE;//sizeof(PR_TX_Buffer)-1;  		/* transmit size word         */
	
	/* configure DMA Stream      */
	DMA1_Stream1->CR	= 	((uint32_t) 4<<25) |   		//CHSEL[2:0]:	Stream channel selection
												((uint32_t)	0<<16) |  		//PL[1:0]:		Channel priority level 0:Low 1:Medium 2:High 3:Very high
												((uint32_t)	0<<13) |  		//MSIZE[1:0]:	Memory size 0:8bit 1:16bit 2:32bit
												((uint32_t)	0<<11) |   		//PSIZE[1:0]:	Peripheral size 0:8bit 1:16bit 2:32bit
												((uint32_t)	1<<10) |   		//MINC: Memory increment mode Disable
												((uint32_t)	0<< 9) |   		//PINC: Peripheral increment mode Enable
												((uint32_t)	1<< 8) |   		//CIRC: Circular mode Disable
												((uint32_t)	0<< 6) |   		//DIR: Data transfer direction  0: Peripheral-to-memory, 1: Memory-to-peripheral, 2: Memory-to-Memory
												((uint32_t)	0<< 5) |			//PFCTRL: Peripheral flow controller 0: The DMA is the flow controller, 1: The peripheral is the flow controller
												((uint32_t)	0<< 4) |   		//TCIE: Transfer complete interrupt enable
												((uint32_t)	0<< 3) |   		//HTIE: Half transfer interrupt enable
												((uint32_t)	0<< 2) | 			//TEIE: Transfer error interrupt enable
												((uint32_t)	0<< 1) |   		//DMEIE: Direct mode error interrupt enable
												((uint32_t)	0<< 0) ;   		//Stream enable / flag stream ready when read low
	
	DMA1->LIFCR |= 0x00000F40;
	
	DMA1_Stream1->CR  |= DMA_SxCR_EN;     /* DMA Enable  */
}





void UART4_Init(void)	//For Hall
{
	int i;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER &= (uint32_t)~( ((uint32_t)0x03) | ((uint32_t)0x03 << 2) );  //PA0,1 Clear
	GPIOA->MODER |= (uint32_t) ( ((uint32_t)0x02) | ((uint32_t)0x02 << 2) );  //PA0,1 Alternate function mode
	
	GPIOA->AFR[0] &= ~( 0x000000FF ); //PA0,1 AF Clear
	GPIOA->AFR[0] |=  ( 0x00000088 ); //PA0,1 AF Set 8 for USART4
	
	RCC->APB1ENR |= RCC_APB1ENR_UART4EN;    // enable USART4 clock

	//UART4->BRR   = 0x016C;                  // 115200 baud @ APB1_CLK == 42MHz
	UART4->BRR   = 0x1117;                  // 9600 baud @ APB1_CLK == 42MHz
	UART4->CR1   = 0;                       // Clear Control Register 1
	UART4->CR1   = USART_CR1_TE  | USART_CR1_RE;     // enable TX & RX

	
	UART4->CR3  |= USART_CR3_DMAT | USART_CR3_DMAR;        // Enable TX DMA Bit & RX DMA Bit
	for (i = 0; i < 0x1000; i++) __NOP();    // avoid unwanted output

	UART4->CR1  |= USART_CR1_UE;// | USART_CR1_RXNEIE_RXFNEIE;   // Enable USART3 & RX Interrupts
}


void DMA1_Stream4_Init(void)	//For TX UART4
{
	DMA1_Stream4->CR  = 0; 	/* DMA Disable  */
	//
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; 	/* enable peripheral clock for DMA1 */
	
	DMA1_Stream4->M0AR  = (uint32_t) HALL_TX_Buffer;	/* set chn1 memory address    */
	DMA1_Stream4->PAR  	= (uint32_t) &UART4->DR;  		/* set chn1 peripheral address*/
	DMA1_Stream4->NDTR 	= 4;  												/* transmit size word         */
	
	DMA1->HIFCR |= 0x0000003D;
	
	/* configure DMA Stream      */
	HALL_TxDMA_CR_Val =		((uint32_t) 4<<25) |   		//CHSEL[2:0]:	Stream channel selection
												((uint32_t)	0<<16) |  		//PL[1:0]:		Channel priority level 0:Low 1:Medium 2:High 3:Very high
												((uint32_t)	0<<13) |  		//MSIZE[1:0]:	Memory size 0:8bit 1:16bit 2:32bit
												((uint32_t)	0<<11) |   		//PSIZE[1:0]:	Peripheral size 0:8bit 1:16bit 2:32bit
												((uint32_t)	1<<10) |   		//MINC: Memory increment mode Disable
												((uint32_t)	0<< 9) |   		//PINC: Peripheral increment mode Enable
												((uint32_t)	0<< 8) |   		//CIRC: Circular mode Disable
												((uint32_t)	1<< 6) |   		//DIR: Data transfer direction  0: Peripheral-to-memory, 1: Memory-to-peripheral, 2: Memory-to-Memory
												((uint32_t)	0<< 5) |			//PFCTRL: Peripheral flow controller 0: The DMA is the flow controller, 1: The peripheral is the flow controller
												((uint32_t)	0<< 4) |   		//TCIE: Transfer complete interrupt enable
												((uint32_t)	0<< 3) |   		//HTIE: Half transfer interrupt enable
												((uint32_t)	0<< 2) | 			//TEIE: Transfer error interrupt enable
												((uint32_t)	0<< 1) |   		//DMEIE: Direct mode error interrupt enable
												((uint32_t)	0<< 0) ;   		//Stream enable / flag stream ready when read low
	DMA1_Stream4->CR	= HALL_TxDMA_CR_Val;
	
	DMA1_Stream4->CR  |= DMA_SxCR_EN;     /* DMA Enable  */
}

void HALL_SendData(int length)
{
	HW_DIR_HALL(1);
	HALL_TxModeF = 1;
	
	DMA1_Stream4->CR  = 0;
	DMA1_Stream4->NDTR = length;
	DMA1->HIFCR |= 0x0000003D;						// Clear Flags
	DMA1_Stream4->CR	=	HALL_TxDMA_CR_Val;
	DMA1_Stream4->CR  |= DMA_SxCR_EN;     /* DMA Enable  */
}

void DMA1_Stream2_Init(void)	//For RX UART4
{
	DMA1_Stream2->CR  = 0; 	/* DMA Disable  */
	//
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; 	/* enable peripheral clock for DMA1 */
	
	DMA1_Stream2->M0AR  = (uint32_t) HALL_RX_Buffer;  	/* set chn1 memory address    */
	DMA1_Stream2->PAR  	= (uint32_t) &UART4->DR;  			/* set chn1 peripheral address*/
	DMA1_Stream2->NDTR 	= UART_BUFFER_SIZE;//sizeof(PR_TX_Buffer)-1;  		/* transmit size word         */
	
	/* configure DMA Stream      */
	DMA1_Stream2->CR	= 	((uint32_t) 4<<25) |   		//CHSEL[2:0]:	Stream channel selection
												((uint32_t)	0<<16) |  		//PL[1:0]:		Channel priority level 0:Low 1:Medium 2:High 3:Very high
												((uint32_t)	0<<13) |  		//MSIZE[1:0]:	Memory size 0:8bit 1:16bit 2:32bit
												((uint32_t)	0<<11) |   		//PSIZE[1:0]:	Peripheral size 0:8bit 1:16bit 2:32bit
												((uint32_t)	1<<10) |   		//MINC: Memory increment mode Disable
												((uint32_t)	0<< 9) |   		//PINC: Peripheral increment mode Enable
												((uint32_t)	1<< 8) |   		//CIRC: Circular mode Disable
												((uint32_t)	0<< 6) |   		//DIR: Data transfer direction  0: Peripheral-to-memory, 1: Memory-to-peripheral, 2: Memory-to-Memory
												((uint32_t)	0<< 5) |			//PFCTRL: Peripheral flow controller 0: The DMA is the flow controller, 1: The peripheral is the flow controller
												((uint32_t)	0<< 4) |   		//TCIE: Transfer complete interrupt enable
												((uint32_t)	0<< 3) |   		//HTIE: Half transfer interrupt enable
												((uint32_t)	0<< 2) | 			//TEIE: Transfer error interrupt enable
												((uint32_t)	0<< 1) |   		//DMEIE: Direct mode error interrupt enable
												((uint32_t)	0<< 0) ;   		//Stream enable / flag stream ready when read low
	
	DMA1->LIFCR |= 0x003D0000;
	
	DMA1_Stream2->CR  |= DMA_SxCR_EN;     /* DMA Enable  */
}




