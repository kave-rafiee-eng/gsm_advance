/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Defines.h"
#include "n1202.h"
#include "Hardware.h"
#include "Serial.h"
#include "Menu.h"
#include "Variable.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////    TIM4 1ms    ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void TIM4_IRQHandler(void)
{
	
	timer_basic_manager();
	
  TIM4->SR =0;
	
	Pr++;
	Task5MsTimer++;
	Task10MsTimer++;
	Task20MsTimer++;
	Task100MsTimer++;
	Task500MsTimer++;
	Task1SecTimer++; 
	
	DayCounterTemporaryTimer++;
	if(DayCounterTemporaryTimer > 9999) {DayCounterTemporaryTimer = 0;}
	
	if(ReInitLCDTimer < 60000) {ReInitLCDTimer++;}
	
	if(DriveRestTime < 5000) DriveRestTime++;
	
	
	if(CAB_TX_DMA_CNT != CAB_LastTxDMACNT) 			{CAB_TX_AfterLastByteTimer=0;		CAB_LastTxDMACNT = CAB_TX_DMA_CNT;}
	else if(CAB_TX_AfterLastByteTimer < 100) 		{CAB_TX_AfterLastByteTimer++;}

	if(DRV_TX_DMA_CNT != DRV_LastTxDMACNT)			{DRV_TX_AfterLastByteTimer = 0;	DRV_LastTxDMACNT = DRV_TX_DMA_CNT;}
	else if(DRV_TX_AfterLastByteTimer < 100) 		{DRV_TX_AfterLastByteTimer++;}
	if(DRV_RX_DMA_CNT != DRV_LastRxDMACNT)			{DRV_RxModeF=1;									DRV_LastRxDMACNT = DRV_RX_DMA_CNT;}
	else 																				{DRV_RxModeF=0;}
	
//	if(HALL_TX_DMA_CNT != HALL_LastTxDMACNT)		{HALL_TX_AfterLastByteTimer=0;	HALL_LastTxDMACNT = HALL_TX_DMA_CNT;}
//	else if(HALL_TX_AfterLastByteTimer < 100) 	{HALL_TX_AfterLastByteTimer++;}
//	if(HALL_RX_DMA_CNT != HALL_LastRxDMACNT)		{Hall_ReceivingF=1;							HALL_LastRxDMACNT = HALL_RX_DMA_CNT;}
//	else 																				{Hall_ReceivingF=0;}
	
	PR_ByteReciveTimer++;
	if(PR_ByteReciveTimer > 99) 
	{
		PR_ByteReciveTimer = 0;
		PR_SyncStatus = 0;
	}
	
	
	if(DRV_GetDataTimer < 5000) 
	{
		DRV_GetDataTimer++;
		
		if(DRV_GetDataTimer > 1000) DRV_LinkLED_Status = 0;
	}
	
	
	///////////////////
	/////  5 MS  //////
	///////////////////
	Ms5++;
	if(Ms5 > 4)
	{
		Ms5 = 0;
	  Keys4Menu();
	}
	///////////////////
	/////  50 MS  /////
	///////////////////
	Ms50++;
	if(Ms50 > 49)
	{
		Ms50 = 0;
	}
	
	
	//////////////////////////////////////////
	////  Safety Circuit & Short 110To24 /////
	//////////////////////////////////////////
	{
		if(HW_SAFETY_90) 	{if(Safety_Timer[0] < 100) 	{Safety_Timer[0]+=20;} 	else {Safety_90 = 1;} }
		else        			{if(Safety_Timer[0] > 0)    {Safety_Timer[0]--;}  	if(Safety_Timer[0] < 40) {Safety_90 = 0;} }
		if(HW_SAFETY_71) 	{if(Safety_Timer[1] < 100) 	{Safety_Timer[1]+=20;} 	else {Safety_71 = 1;} }
		else        			{if(Safety_Timer[1] > 0)    {Safety_Timer[1]--;}  	if(Safety_Timer[1] < 40) {Safety_71 = 0;} }
		if(HW_SAFETY_66) 	{if(Safety_Timer[2] < 100) 	{Safety_Timer[2]+=20;} 	else {Safety_66 = 1;} }
		else        			{if(Safety_Timer[2] > 0)    {Safety_Timer[2]--;}  	if(Safety_Timer[2] < 40) {Safety_66 = 0;} }
		if(HW_SAFETY_69) 	{if(Safety_Timer[3] < 100) 	{Safety_Timer[3]+=20;} 	else {Safety_69 = 1;} }
		else        			{if(Safety_Timer[3] > 0)    {Safety_Timer[3]--;}  	if(Safety_Timer[3] < 40) {Safety_69 = 0;} }
		if(HW_SAFETY_68) 	{if(Safety_Timer[4] < 100) 	{Safety_Timer[4]+=20;} 	else {Safety_68 = 1;} }
		else        			{if(Safety_Timer[4] > 0)    {Safety_Timer[4]--;}  	if(Safety_Timer[4] < 40) {Safety_68 = 0;} }
		if(HW_SHORT_24TO110) 	{if(Safety_Timer[5] < 100) 	{Safety_Timer[5]+=20;} 	else {Short110To24 = 1;} }
		else        					{if(Safety_Timer[5] > 0)    {Safety_Timer[5]--;}  	if(Safety_Timer[5] < 40) {Short110To24 = 0;} }
	}
	
	
	
}


////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////  EVERY 50us ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
void TIM5_IRQHandler(void)
{
  TIM5->SR =0;
	
	if(Task950usTimer< 20) Task950usTimer ++;
	
	//////// RS485 DIR pin & Tx flag control
	{
		if(DBLX_TxModeF)
		{
			if(DBLX_TX_DMA_CNT == 0 && (USART2->SR & USART_SR_TC) ) 
			{
				HW_DIR_DBLX(0);
				DBLX_TxModeF = 0;
			}
		}
		if(DRV_TxModeF)
		{
			if(DRV_TX_DMA_CNT == 0 && (UART5->SR & USART_SR_TC) ) 
			{
				HW_DIR_DRV(0);
				DRV_TxModeF = 0;
			}
		}
		if(HALL_TxModeF)
		{
			if(HALL_TX_DMA_CNT == 0 && (UART4->SR & USART_SR_TC) ) 
			{
				HW_DIR_HALL(0);
				HALL_TxModeF = 0;
			}
		}
		if(CAB_TxModeF)
		{
			if(CAB_TX_DMA_CNT == 0 && (USART3->SR & USART_SR_TC) ) 
			{
				CAB_TxModeF = 0;
			}
		}
		if(PR_TxModeF)
		{
			if(PR_TX_DMA_CNT == 0 && (USART1->SR & USART_SR_TC) ) 
			{
				PR_TxModeF = 0;
			}
		}
	}
	
	///////////////////////////////////
	//////  Slave Communication  //////
	///////////////////////////////////
	if(SlaveByteCounter < SlaveDataLength)
	{
		// Synching: Before CLK low 250us for sync bit and 500us for sync byte
		if(	(SlaveByteCounter > 0 && SlaveSyncTimer < 5) || (SlaveByteCounter == 0 && SlaveSyncTimer < 10) ) 
		{
			SlaveCLK_Level = 0;
			
			SlaveSyncTimer ++;
			if( (SlaveByteCounter > 0 && SlaveSyncTimer == 5) || (SlaveByteCounter == 0 && SlaveSyncTimer == 10) )
			{
				SlaveBitCounter = 0;
				SlaveTxData = SlaveSendBuffer[SlaveByteCounter];
				SlaveRxData = 0x00;
				
				SlaveMOSI_Level = SlaveTxData & 0x80; //Send bits (First MSB)
			}
		}
		// After Synching: Send & Receive 8 Bit
		else
		{
			if(SlaveCLK_Level) //When CLK should be LOW
			{
				//Receive bits
				if(HW_SLAVE_MISO) {SlaveRxData |= 0x01;} 
				
				SlaveBitCounter ++;
				if(SlaveBitCounter < 8)
				{	
					SlaveRxData <<= 1; //Receive bits (First MSB)
					
					SlaveTxData <<= 1; //Send bits (First MSB)
					SlaveMOSI_Level = SlaveTxData & 0x80; 
				}
				else //Completed byte
				{
					if(SlaveByteCounter == 0)
					{
						SlaveSendBuffer[0] = SlaveRxData;
					}
					SlaveReceiveBuffer[SlaveByteCounter] = SlaveRxData;
					
					SlaveSyncTimer = 0;
					SlaveByteCounter ++;
					SlaveMOSI_Level = 0;
					SlaveCLK_Level = 1;
				}
			}
			
			SlaveCLK_Level = !SlaveCLK_Level;
		}
	}
	else
	{
		SlaveByteCounter = 0;
		SlaveDataLength = 0;
		
		SlaveSyncTimer = 0;
		SlaveCLK_Level = 0;
		SlaveMOSI_Level = 0;
	}
	
	HW_SLAVE_CLK(SlaveCLK_Level);
	HW_SLAVE_MOSI(SlaveMOSI_Level);
}

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////  EVERY 100us ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
void TIM2_IRQHandler(void)
{
  TIM2->SR =0;
	
	
	if(LCD_InitF)
	{
		LCD_InitTimer++;

		if(LCD_InitState == 0)
		{
			HW_LCD_PWR(1);
			HW_RESET_PIN(0); // Activate the reset
			if(LCD_InitTimer > 28) // 2.9 ms
			{
				HW_LCD_PWR(0);   //Power LCD On
				
				LCD_InitTimer = 0;
				LCD_InitState++;
			}
		}
		else if(LCD_InitState == 1)
		{
			if(LCD_InitTimer > 2) // 0.3ms
			{
				HW_RESET_PIN(1);  // deactivate the reset
				HW_SCK_PIN(0);    // Clock output low state
				HW_MOSI_PIN(0);   // Outputting data low
				HW_CS_PIN(1);		 // Choosing a display

				LCD_InitTimer = 0;
				LCD_InitState++;
			}
		}
		else if(LCD_InitState == 2)
		{
		   if(LCD_InitTimer > 2) // 0.3ms
			{
				LCD_SendByte(LCD_C, 0xE2);  // Chip reset

				LCD_InitTimer = 0;
				LCD_InitState++;
			}
		}
		else if(LCD_InitState == 3)
		{
			uint8_t Delay = 40; //40 -> 4ms
			// Setting the segment charge energy
			if(LCD_InitTimer == Delay)
			{
        LCD_SendByte (LCD_C, 0x3D); // Charge energy multiplier
			}
			else if(LCD_InitTimer == Delay+1)
			{
				LCD_SendByte (LCD_C, 0x01); // Unclear value of the multiplier
			}
			// Command followed by contrast data
			else if(LCD_InitTimer == Delay+2)
			{
				LCD_SendByte (LCD_C, 0xE1); // Additional VOP for contrast increase
			}
			else if(LCD_InitTimer == Delay+3)
			{
				LCD_SendByte (LCD_C, 0x90); // from -127 to +127
			}
			// Set the mode to Normal
			else if(LCD_InitTimer == Delay+4)
			{
				LCD_SendByte (LCD_C, 0xA4); // Power saver off
			}
			else if(LCD_InitTimer == Delay+5)
			{
				LCD_SendByte(LCD_C, 0x2F);  // Booster ON Voltage regulator ON Voltage follover ON
			}
			else if(LCD_InitTimer == Delay+6)
			{
				LCD_SendByte(LCD_C, 0xA0);  // Segment driver direction select: Normal
			}
			else if(LCD_InitTimer == Delay+7)
			{
				LCD_SendByte(LCD_C, 0xAF);  // Turn on the display
			}
			else if(LCD_InitTimer > Delay+17) // 1ms
			{
				LCD_InitF = 0;
				//if(LCD_UpdateF == 0) NVIC_ClearPendingIRQ(TIM17_IRQn);
			}
		}
	}
	else if(LCD_UpdateF)
	{
		if(LCD_Cmd_Counter < 3)
		{
			if(LCD_Cmd_Counter == 0)       LCD_SendByte(LCD_C,SetYAddr | LCD_Cmd_Row);
			else if(LCD_Cmd_Counter == 1)  LCD_SendByte(LCD_C,SetXAddr4);
			else                           LCD_SendByte(LCD_C,SetXAddr3);
			LCD_Cmd_Counter++;
		}
		else
		{
			LCD_SendByte(LCD_D,_LCD_RAM[LCD_SendAdd]);
			//DataOut = _LCD_RAM[LCD_SendAdd];
			_LCD_RAM[LCD_SendAdd] = 0; //Clear for nex update
			LCD_SendAdd++;

			LCD_Data_Counter ++;
			if(LCD_Data_Counter >= LCD_X)
			{
				LCD_Cmd_Counter = 0;
				LCD_Data_Counter = 0;
				LCD_Cmd_Row++;

				if(LCD_SendAdd >= 864)
				{
					LCD_UpdateF = 0;
					//NVIC_ClearPendingIRQ(TIM17_IRQn);
				}
			}
		}
	}
	
	
	
	
	/////////////////////////////////////////////////////////
	/////////////////////  Three Phase  /////////////////////
	/////////////////////////////////////////////////////////		
	if(Ph_RtoS_Timer < 1000) {Ph_RtoS_Timer ++;}
	if(Ph_StoT_Timer < 1000) {Ph_StoT_Timer ++;}
	if(Ph_TtoR_Timer < 1000) {Ph_TtoR_Timer ++;}
	if(Ph_R_Logic) 	
	{
		if(Ph_PreR == 0)
		{
			Ph_TtoR_Time = Ph_TtoR_Timer;
			Ph_RtoS_Timer = 0;
		}
		Ph_PreR = 1;
		PhaseTimerR = 999;
	}
	else
	{
		if(PhaseTimerR > 0)	PhaseTimerR--;
		Ph_PreR = 0;
	}
	if(Ph_S_Logic) 	
	{
		if(Ph_PreS == 0)
		{
			Ph_RtoS_Time = Ph_RtoS_Timer;
			Ph_StoT_Timer = 0;
		}
		Ph_PreS = 1;
		PhaseTimerS = 999;
	}
	else
	{
		if(PhaseTimerS > 0) PhaseTimerS--;
		Ph_PreS = 0;
	}
	
	if(Ph_T_Logic) 	
	{
		if(Ph_PreT == 0)
		{
			Ph_StoT_Time = Ph_StoT_Timer;
			Ph_TtoR_Timer = 0;
		}
		Ph_PreT = 1;
		PhaseTimerT = 999;
	}
	else
	{
		if(PhaseTimerT > 0) PhaseTimerT--;
		Ph_PreT = 0;
	}
}


void TIM3_IRQHandler(void)
{
  TIM3->SR =0;
	
	uint16_t	TimerCNT = TIM3->CNT;
	if(TimerCNT > 32000) 	EncoderINT -= 65536;
	else 									EncoderINT += 65536;
}

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA2 stream0 global interrupt.
  */
void DMA2_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */
	
	TestADCTimer++;
	
	PT_R_ADC = adcv[1] - adcv[0];
	PT_S_ADC = adcv[2] - adcv[0];
	PT_T_ADC = adcv[3] - adcv[0];
	
	if(PT_R_ADC > PHASES_LOGIC_COMP_ADC) 							Ph_R_Logic = 1;
	else if(PT_R_ADC < (PHASES_LOGIC_COMP_ADC - 50)) 	Ph_R_Logic = 0;
	if(PT_S_ADC > PHASES_LOGIC_COMP_ADC) 							Ph_S_Logic = 1;
	else if(PT_S_ADC < (PHASES_LOGIC_COMP_ADC - 50)) 	Ph_S_Logic = 0;
	if(PT_T_ADC > PHASES_LOGIC_COMP_ADC) 							Ph_T_Logic = 1;
	else if(PT_T_ADC < (PHASES_LOGIC_COMP_ADC - 50)) 	Ph_T_Logic = 0;
	
	if(PT_RMS_Counter < PT_RMS_SAMPLE_COUNT)
	{
		PT_R_RMS_Sum += PT_R_ADC * PT_R_ADC;
		PT_S_RMS_Sum += PT_S_ADC * PT_S_ADC;
		PT_T_RMS_Sum += PT_T_ADC * PT_T_ADC;
		PT_RMS_Counter ++;
		
		VRefIntSum += adcv[4];
	}
  /* USER CODE END DMA2_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */

  /* USER CODE END DMA2_Stream0_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
