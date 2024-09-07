/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "math.h"
#include "n1202.h"
#include "Hardware.h"
#include "Defines.h"
#include "Serial.h"
#include "Menu.h"
#include "AT24Cxx.h"
#include "EEPROM.h"
#include "PB_Num.h"
#include "Variable.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;

IWDG_HandleTypeDef hiwdg;

TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */

const	char SpeedList[][10]={
	"STOP",
	"SLOW",
	"REV",
	"MEDIUM",
	"FAST",
	"FORCE"
};

const char List_SegmentChoice[]={
  /// Right Segment
		'?',
		'0',
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',
		'G',
		'P',
		'r',
		'b',
		'H',
		'E',
		'F',
		'd',
		'L',
		'A',
		'C',
		'y',
		'-'
};

static const char ActivityList[][15]= //Length = 14
{
   "STANDBY DO",    //0
   "STANDBY DC",    //1
   "STOP DO",       //2
   "STOP DC",       //3
   "SLOW UP",       //4
   "SLOW DOWN",     //5
   "FAST UP",       //6
   "FAST DOWN",     //7
   "REV UP",        //8
   "REV DOWN",      //9
   "MEDIUM UP",     //10
   "MEDIUM DOWN",   //11/////
   "OPENING DOOR",  //12
   "CLOSING DOOR",  //13
   "Waiting Floor", //14
   "LEARNING",      //15/////
   "Emergency MB",  //16
   "Emergency CB",  //17
   "CALIBRATION",   //18
   "RELEVEL UP",    //19
   "RELEVEL DOWN",  //20
   "UPS HELP MODE"  //21
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM3_Init(void);
static void MX_ADC1_Init(void);
static void MX_IWDG_Init(void);
/* USER CODE BEGIN PFP */

void DayCounterCheck(void)
{
	if(Error_DayCounter == 0 && DayCounter_Status != DAY_COUNTER_STATUS_PERMANENT)
	{
		if(DayCounter_Minutes > 0)		{DayCounter_Minutes --;}
		else
		{
			if(DayCounter_Day > 0 || DayCounter_Hour > 0)	{DayCounter_Minutes = 59;}
			if(DayCounter_Hour > 0) {DayCounter_Hour--;}
			else
			{
				if(DayCounter_Day > 0)
				{
					DayCounter_Hour = 23;
					DayCounter_Day --;
				}
			}
		}
		
		SaveDayCounterTimer++;
	}
		
	if(DayCounter_Day == 0 && DayCounter_Hour == 0 && DayCounter_Minutes == 0 && DayCounter_Status != DAY_COUNTER_STATUS_PERMANENT) 
	{
		if(Error_DayCounter == 0)
		{
			SaveDayCounterValues();
		}
		Error_DayCounter = 1;
	}
	else Error_DayCounter = 0;
}

void Debouncer_CA1CAN(void)
{
	///// CA1 Debouncer
	if(MIn.CA1 == 1) 	
	{
		if(DebCA1_OnTimer < 100) 	{DebCA1_OnTimer++;	}	
		if(DebCA1_OnTimer >= 15 || DebCA1_OffTimer >= 10) {InCA1 = 1;} //150ms
		DebCA1_OffTimer = 0;
	}
	else 
	{
		if(DebCA1_OffTimer < 100) {DebCA1_OffTimer++;}
		if(DebCA1_OffTimer >= 15 || DebCA1_OnTimer >= 10) {InCA1 = 0;}
		DebCA1_OnTimer = 0;
	}
	///// CAN Debouncer
	if(MIn.CAN == 1) 	
	{
		if(DebCAN_OnTimer < 100) 	{DebCAN_OnTimer++;	}	
		if(DebCAN_OnTimer >= 15 || DebCAN_OffTimer >= 10) {InCAN = 1;}
		DebCAN_OffTimer = 0;
	}
	else 
	{
		if(DebCAN_OffTimer < 100) {DebCAN_OffTimer++;}
		if(DebCAN_OffTimer >= 15 || DebCAN_OnTimer >= 10) {InCAN = 0;}
		DebCAN_OnTimer = 0;
	}
}

void Debouncer_68_69(void)
{
	///// 69 Debouncer
	if(Safety_69 == 1) 	
	{
		if(Deb69_OnTimer < 30) 	{Deb69_OnTimer++;	}	
		if(Deb69_OnTimer >= Stng[DEBOUNCER69] || Deb69_OffTimer >= Stng[DEBOUNCER69]) {Debounced_69 = 1;}
		Deb69_OffTimer = 0;
	}
	else 
	{
		if(Deb69_OffTimer < 30) {Deb69_OffTimer++;}
		if(Deb69_OnTimer >= Stng[DEBOUNCER69] || Deb69_OffTimer >= Stng[DEBOUNCER69]) {Debounced_69 = 0;}
		Deb69_OnTimer = 0;
	}
	
	///// 68 Debouncer
	if(Safety_68 == 1) 	
	{
		if(Deb68_OnTimer < 30) 	{Deb68_OnTimer++;	}	
		if(Deb68_OnTimer >= Stng[DEBOUNCER68] || Deb68_OffTimer >= Stng[DEBOUNCER68]) {Debounced_68 = 1;}
		Deb68_OffTimer = 0;
	}
	else 
	{
		if(Deb68_OffTimer < 30) {Deb68_OffTimer++;}
		if(Deb68_OnTimer >= Stng[DEBOUNCER68] || Deb68_OffTimer >= Stng[DEBOUNCER68]) {Debounced_68 = 0;}
		Deb68_OnTimer = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////    3VF    //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Drive_3VF_Parallel_Control(void)
{
	_Bool OUTACTF = MOut.MUP | MOut.MDN | MOut.V0 | MOut.V1 | MOut.V2 | MOut.TC;
	_Bool 	DriveDir = CMD_Dir;
	uint8_t DriveSpeed = CMD_Speed;
	
//	if(ErrorCut71 || ErrorCut66 || ErrorCut68 || ErrorCut69)  DriveRestTime = 0;
	
//	if(CMD_Speed > STOP && ((Debounced_68==0 || Debounced_69==0) && URA_Set_Time > Time_URA_Delay) )
//	{
//		DriveSpeed = FORCE_STOP;
//		if(CMD_URA) DriveRestTime = 0;
//	}
	
	///Motor/Valve rest for change direction
	if(PreDir != DriveDir && DriveSpeed < FORCE_STOP)
	{
		if(OUTACTF==0) PreDir = DriveDir;
		else 
		{
			DriveSpeed = STOP;
		}
	}
   
	if(DriveSpeed == FORCE_STOP || CMD_Speed  == FORCE_STOP || DriveSpeed==10)//|| MInV[_FLTDR]==0
	{
		MOut.MUP = 0;
		MOut.MDN = 0;
		MOut.V0 = 0;
		MOut.V1 = 0;
		MOut.V2 = 0;
		MOut.TC = 0;
		
		RunOrStop = STOP;
		LandingF=0;
	}
	
	
	/// STOP
	else if(DriveSpeed == STOP || DriveRestTime<2000)
	{
		if(Drive_Timing < 200.0) Drive_Timing += 0.1;
		
		if(PreSpeed != STOP)
		{
			 Drive_Timing = 0;               
		
			 if(PreDir == DriveDir)
			 {
					MOut.MUP = (DriveDir == UP);
					MOut.MDN = (DriveDir == DOWN);
			 }
			 MOut.V0 = 0;
			 MOut.V1 = 0;
			 MOut.V2 = 0;
		}
		else if(OUTACTF)
		{
			 if(Drive_Timing >= Stng[UD_RELEASE_DELAY]) 
			 {
					MOut.MUP = 0;
					MOut.MDN = 0;
			 }
			 if(Drive_Timing >= (Stng[TC_RELEASE_DELAY] + Stng[UD_RELEASE_DELAY])) 
			 {
					MOut.TC  = 0;
					DriveRestTime = 0;            
			 }
		}
	}
	
	/// RUN
	else if(DriveSpeed > STOP && PreDir == DriveDir && DriveRestTime>2000) // SLOW , REV , FAST
	{
		RunOrStop = RUN;
		
		Error_RLS =0;
		Error_RLSCut =0;
		
		MOut.MUP = (DriveDir == UP);
		MOut.MDN = (DriveDir == DOWN);
		
		MOut.TC  = 1;
		
		if(Stng[ENABLE_1CF_ERROR] == DISABLE && DriveSpeed == FAST) {DriveSpeed = SLOW;}
		
		MOut.V0 = Stng_DriveParallelV0[DriveSpeed];
		MOut.V1 = Stng_DriveParallelV1[DriveSpeed];
		MOut.V2 = Stng_DriveParallelV2[DriveSpeed]; 
	}
	
	///// RLS Error Detection    
	if( MOut.TC != MIn.RLS )
	{
		if(Error_RLS == 0 && Error_RLSCut == 0)
		{
			if(RLS_Timer < 10000) RLS_Timer += 10;
			uint16_t  CheckTime = 2000; //2000 ms
			if(MOut.TC == 0)  CheckTime = 3000; //3s
			if(RLS_Timer > CheckTime)
			{
				if(RunOrStop)  	{Error_RLSCut = 1; Error_RLSCut_S = 1;}
				else           	{Error_RLS = 1; Error_RLS_S = 1;}
				Error_RLS_RelevelF = 1;
				CMD_Speed = FORCE_STOP;
				DriveRestTime = 0;
				MOut.TC = 0;
			}
		}
	}
	else if(RLS_Timer > 20)   RLS_Timer -= 20;
	else
	{
		RLS_Timer = 0;
		Error_RLS = 0;
		Error_RLSCut = 0;
	}

	if(MOut.TC == 0)
	{
		MOut.MUP = 0;
		MOut.MDN = 0;
		MOut.V0 = 0;
		MOut.V1 = 0;
		MOut.V2 = 0;
		
		RunOrStop = STOP;
	}
   
   
	///////////////////////////////
	/////   [Relay Output]   //////
	///////////////////////////////
	if(RunOrStop)
	{
		if(PreRunOrStop == 0)
		{
			RelayOut_Timer = 0;
			PreRunOrStop = 1;
		}
		else 
		{
			if(MOut.Relay == 0) 
			{
				if(RelayOut_Timer < Stng[RELAY_ON_DELAY])
				{
					 RelayOut_Timer += 0.1;
				}
				else if(OutRelayOneTimeF == 0)
				{
					 MOut.Relay = 1;
					 RelayOut_Timer = 0;               
				}
			}
			else
			{
				if(RelayOut_Timer < Stng[RELAY_ON_TIME])
				{
					 RelayOut_Timer += 0.1;
				}
				else 
				{
					 MOut.Relay = 0; 
					 OutRelayOneTimeF = 1;
				}
			}
		}
	}
	else
	{
		PreRunOrStop = 0;

		OutRelayOneTimeF = 0;
		RelayOut_Timer = 0;
		MOut.Relay = 0;
	}
   
   PreSpeed = DriveSpeed;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////   Hydraulic HEVOS   /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Hydraulic_HEVOS_ValveControl()
{
	_Bool OUTACTF = MOut.MUP | MOut.MDN | MOut.V0 | MOut.V1 | MOut.V2 | MOut.TC | MOut.STDL;
	_Bool 	DriveDir = CMD_Dir;
	uint8_t DriveSpeed = CMD_Speed;
	
   ///Motor/Valve rest for change direction
	if(PreDir != DriveDir && DriveSpeed < FORCE_STOP)
	{
		if(OUTACTF==0) PreDir = DriveDir;
		else 
		{
			DriveSpeed = STOP;
		}
	}
   
	if(DriveSpeed == FORCE_STOP)//
	{
		MOut.MUP = 0;
		MOut.MDN = 0;
		MOut.V0 = 0;
		MOut.V1 = 0;
		MOut.V2 = 0;
		MOut.TC = 0;
		MOut.STDL = 0;
		
		RunOrStop = STOP;
		LandingF=0;
	}
   
   
	//////Stop
	else if(DriveSpeed == STOP || DriveRestTime<2000 || !MIn.Ready) 
	{
		if(PreSpeed > STOP && PreSpeed < FORCE_STOP) 	{Drive_Timing = 0;}
		else if(Drive_Timing < 200.0) 								{Drive_Timing += 0.1;}
		
		
		if(Stng[MOTOR_STOP_DELAY] == 100)
		{
			if(OUTACTF)
			{
				MOut.MUP = 0;
				MOut.MDN = 0;
				MOut.V1 = 0;
				MOut.V2 = 0;
				MOut.TC = 0;
				MOut.STDL = 0;
				MOut.EN_Out = 0;
				
				if((DriveSpeed == STOP || DriveSpeed == FORCE_STOP) && RunOrStop) {DriveRestTime = 0; }
				RunOrStop = STOP;
			}
		}
		else if(Stng[MOTOR_STOP_DELAY] < 100) 
		{
			MOut.TC = 0;
			if((Drive_Timing > ((int)100 - Stng[MOTOR_STOP_DELAY])) && OUTACTF ) 
			{
				MOut.MUP = 0;
				MOut.MDN = 0;
				MOut.V1 = 0;
				MOut.V2 = 0;
				MOut.STDL = 0;
				MOut.EN_Out = 0;
				
				if((DriveSpeed == STOP || DriveSpeed == FORCE_STOP) && RunOrStop) {DriveRestTime = 0; }
				RunOrStop = STOP;

			}
		}
		else if(Stng[MOTOR_STOP_DELAY] > 100) 
		{
			MOut.MUP = 0;
			MOut.MDN = 0;
			MOut.V1 = 0;
			MOut.V2 = 0;
			MOut.STDL = 0;
			if(Drive_Timing > ((int)Stng[MOTOR_STOP_DELAY] - 100) && MOut.TC==1)
			{
				MOut.TC = 0;
				MOut.EN_Out = 0;
				
				if((DriveSpeed == STOP || DriveSpeed == FORCE_STOP) && RunOrStop) {DriveRestTime = 0; }
				
				RunOrStop = STOP;
			}
			else if(MOut.TC==0)
			{
				MOut.EN_Out = 0;
				
				if((DriveSpeed == STOP || DriveSpeed == FORCE_STOP) && RunOrStop) {DriveRestTime = 0; }
				
				RunOrStop = STOP;
			}
		}
	}
   
	///Run
	else if(DriveSpeed > STOP && DriveSpeed < FORCE_STOP && DriveRestTime>=2000 && MIn.Ready) // SLOW , FAST , REV , MEDIUM  //&& DriveRestTime>2000
	{
		// && PreDir == DriveDir
		RunOrStop = RUN;
		
		MOut.EN_Out = 1;
		
		Error_RLS =0;
		
		if(PreSpeed == STOP || PreSpeed >= FORCE_STOP) 	{Drive_Timing = 0;}
		else if(Drive_Timing < 200.0)  									{Drive_Timing += 0.1;}
		
		if(Drive_Timing >= Stng[START_SLOW_DELAY] && MOut.MUP==0 && MOut.MDN==0)
		{
			MOut.MUP = (DriveDir == UP);
			MOut.MDN = (DriveDir == DOWN);
		}
		
		if(Drive_Timing >= Stng[START_FAST_DELAY])
		{
			if(DriveSpeed == SLOW)  
			{
				MOut.V1 = 0;   
				MOut.V2 = 0;
			}
			else if(DriveSpeed == REV || DriveSpeed == MEDIUM) 
			{
				MOut.V1 = 1; 
				MOut.V2 = 0;
			}
			else if(DriveSpeed == FAST)                        
			{
				MOut.V1 = 0; 
				MOut.V2 = 1;
			}
		}

		if(DriveDir == UP) //The motor will only turn on in up direction
		{
			if(Drive_Timing >= Stng[MOTOR_START_DELAY] && MOut.TC == 0)
			{
				MOut.TC = 1;
				MOut.STDL = 1;
			}
			if( Drive_Timing >= (Stng[MOTOR_START_DELAY] + Stng[START_TO_DELTA]) )
			{
				MOut.STDL = 0;
			}
		}
		else //The motor will turn off in down direction
		{
			MOut.TC = 0;
			MOut.STDL = 0;
		}
	}

   ///// RLS Error detection just in UP direction 
	if( (MOut.TC != MIn.RLS) && DriveDir == UP)
	{
		if(Error_RLS == 0 && Error_RLSCut == 0)
		{
			 if(RLS_Timer < 10000) RLS_Timer += 10;
			 uint16_t  CheckTime = 3000; //3000 ms
			 if(MOut.TC == 0)  CheckTime = 3000; //3s
			 if(RLS_Timer > CheckTime)
			 {
					if(RunOrStop)  	{Error_RLSCut = 1;	Error_RLSCut_S = 1;}
					else           	{Error_RLS = 1;	Error_RLS_S = 1;}

					CMD_Speed = FORCE_STOP;
					DriveRestTime = 0;
					MOut.TC = 0;
			 }
		}
	}
	else if(RLS_Timer > 50)   RLS_Timer -= 50;
	else
	{
		RLS_Timer = 0;
		Error_RLS = 0;
		Error_RLSCut = 0;
	}
	
	

   
	///////////////////////////////
	/////  ERS & ENR Output  //////
	///////////////////////////////
	if(MOut.MUP == 1 || MOut.MDN == 1 )
	{
		if(MOut.MUP == 1)      
		{	
			MOut.ERS = 1;
			MOut.ENR = 0; 
			//DisableERSENR_Timer = 0;
		}
		else if(MOut.MDN == 1)
		{
			MOut.ERS = 1;
			MOut.ENR = 1;
			
		}
		DisableERSENR_Timer = 200;
		
	}
	else if(MIn.RLS == 0)
	{
		if(DisableERSENR_Timer > 0) {DisableERSENR_Timer --;}
		else
		{
			MOut.ERS = 0;
			MOut.ENR = 0;
		}
	}
   
	///////////////////////////////
	/////   [Relay Output]   //////
	///////////////////////////////
	if(RunOrStop)
	{
		if(PreRunOrStop == 0)
		{
			RelayOut_Timer = 0;
			PreRunOrStop = 1;
		}
		else 
		{
			if(MOut.Relay == 0) 
			{
				if(RelayOut_Timer < Stng[RELAY_ON_DELAY])
				{
					 RelayOut_Timer += 0.1;
				}
				else if(OutRelayOneTimeF == 0)
				{
					 MOut.Relay = 1;
					 RelayOut_Timer = 0;               
				}
			}
			else
			{
				if(RelayOut_Timer < Stng[RELAY_ON_TIME])
				{
					 RelayOut_Timer += 0.1;
				}
				else 
				{
					 MOut.Relay = 0; 
					 OutRelayOneTimeF = 1;
				}
			}
		}
	}
	else
	{
		PreRunOrStop = 0;

		OutRelayOneTimeF = 0;
		RelayOut_Timer = 0;
		MOut.Relay = 0;
	}
   
   
   PreSpeed = DriveSpeed;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////   DOOR CONTROL   //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DoorControl(void)
{
	if(RunOrStop == STOP)
	{
		if(CMD_Door == CLOSE)
		{
			if(DoorSetCloseTimer < 1000) DoorSetCloseTimer++;
		}
		else {DoorSetCloseTimer = 0;}
		
		//// Time to open door check
		if(TimeToOpenDoor < DOOR_OPEN_TIME_REF)
		{
			TimeToOpenDoor++;
			CMD_Door = OPEN;
		}
		
		//// 66 check
		if(DCMD == CLOSE && Safety_66 == 0 && LEVEL)
		{
			TimeToOpenDoor = 0;
			ToClose66Timer = TO_CLOSE_66_TIME+1;
		}
		else if(CMD_Door == CLOSE && Safety_66 == 0)
		{
			if(ToClose66Timer <= TO_CLOSE_66_TIME) ToClose66Timer++; 
		}
		else {ToClose66Timer = 0;}
		
		//// PreCMD Check
		if(CMD_Door != PreCMD_Door)
		{
			PreCMD_Door = CMD_Door;
			OpeningDoorState = 0;
			ClosingDoorState = 0;
			Door_Timing = 0;
		}
		
		//// Opening Operation
		if(CMD_Door == OPEN && LEVEL) // && RunOrStop == STOP
		{
			if(ForceDoorOpen) {Door_Timing = Stng[DOOR_OPEN_DELAY];} //If DO Pressed, do not calculate the DoorOpenDelay (FORCE DOOR OPEN)            
			
			if(Door_Timing < Stng[DOOR_OPEN_DELAY])  {Door_Timing += 0.1;}
			else 
			{
				if(DCMD == CLOSE) {TimeToOpenDoor=0;}
				CMD_URA = 0; 
				DCMD = OPEN;
				OpeningDoorState = 1;
				ClosingDoorState = 0;
				ForceDoorOpen = 0;
				DoorSetCloseTimer = 0;
			}
		}
		//// Closing Operation
		else if(CMD_Door == CLOSE && Safety_66 && (PHC_temporary == 0 || Safety_69)) // && RunOrStop == STOP
		{
			if(EmergencyModeF == 0) {CMD_MP3File = PLAYER_STOP;}
			
			if(ClosingDoorState == 0 && Debounced_69 == 1)
			{
				ClosingDoorState = 1;
				Door_Timing = 0.0;
			}
			else if(ClosingDoorState >= 2 && Debounced_69 == 0)
			{
				ClosingDoorState = 1;
				Door_Timing = 0.0;
				CMD_URA = 0;
			}
			else if(ClosingDoorState == 3 && ( Debounced_68 == 0 || CMD_URA == 0 || DoorParkF ) )
			{
				ClosingDoorState = 2;
				Door_Timing = 0.0;
			}
			
			
			if(ClosingDoorState == 0)  //Door Close Delay Calc
			{
				CMD_URA = 0; 
				if(Door_Timing < Stng[DOOR_CLOSE_DELAY])  Door_Timing += 0.1;
				else
				{
					DCMD = CLOSE;
					ClosingDoorState = 1;
					Door_Timing = 0;
				}
			}
			else if(ClosingDoorState == 1) //Door Close Timeout Calc (To come 69)
			{
				if(Debounced_69)
				{
					ClosingDoorState = 2;
					Door_Timing = 0;
					Error_URA=0;
				}
				else
				{
					Door_Timing += 0.01; 
					if(Door_Timing >= Stng[DOOR_CLOSE_TIMEOUT])
					{
						Error_DoorCloseTimeout = 1;
						if(LEVEL)
						{
							CMD_Door = OPEN;
							ForceDoorOpen = 1;
							TimeToOpenDoor = 0;
							IncompleteCloseCounter++;
						}
					}
				}
				 
			}
			else if(ClosingDoorState == 2) //URA Delay
			{
				if(DoorParkF == 0) 
				{
					CMD_URA = 1;
					
					if(Debounced_68)
					{
						ClosingDoorState = 3;
						Door_Timing = 0;
						IncompleteCloseCounter = 0;
					}
					else if(URA_Set_Time > Stng[URA_DELAY])
					{
						Error_URA = 1;  
						if(LEVEL)
						{
							CMD_Door = OPEN;
							ForceDoorOpen = 1;
							TimeToOpenDoor = 0;
							IncompleteCloseCounter++;
						}
					}
				}
				else
				{
					CMD_URA = 0;
					IncompleteCloseCounter = 0;
				}
			}
		}//End Close Operation
		
		///66 Play dont diturb door
		else if(	CMD_Door == CLOSE && (Safety_66 == 0 && Error_Cut66 ==0) && LEVEL ) // && RunOrStop == STOP
		{
			if(ToClose66Timer >= TO_CLOSE_66_TIME && AimFloor < NO_AIM)
			{
				CMD_MP3File = PLAY_DISTURB_DOOR;
				ToClose66Timer = 0;
			}
			
			DoorSetCloseTimer = 0;
		}
		
		//// PHC Play dont diturb door & open door if is in closing
		else if(CMD_Door == CLOSE && PHC_temporary && LEVEL && StopTimer > 9)
		{
			Alarm_PHC = 1;
			// After 3 seconds of people standing still between the doors. or the person jumping when the door is closing
			if(DoorSetCloseTimer > 350 || ClosingDoorState == 1)
			{
				if(ResetPlayDisturbDoorTimer == 0) CMD_MP3File = PLAY_DISTURB_DOOR;
				if(ClosingDoorState == 1)
				{
					CMD_Door = OPEN;
					ForceDoorOpen = 1;
					TimeToOpenDoor = 0;
					//ClosingDoorState = 0;
				}
			}
		}
		
		if(CMD_MP3File == PLAY_DISTURB_DOOR) 
		{
			if(ResetPlayDisturbDoorTimer < 100) ResetPlayDisturbDoorTimer ++;
			else
			{
				CMD_MP3File = 0;
				DoorSetCloseTimer = 0;
				ToClose66Timer = 0;
			}
		}
		else {ResetPlayDisturbDoorTimer = 0;}
		
		if(Safety_69 == 0 || CMD_URA == 0)
		{
			CMD_URA = 0;
			URA_Set_Time = 0;
		}
		
	}
	
	//// Door Close Complete Check
	if(ClosingDoorState == 3 && (Safety_66 == 0 || Safety_69 == 0 || Safety_68 == 0)) {ClosingDoorState = 0;}
	CloseComplete = (ClosingDoorState == 3);
	
  //// Loaded Control Type
	if(Stng[DOOR_CONTROL_TYPE] == DOOR_CONTROL_TYPE_LOADED)
	{
		if(RunOrStop == RUN)																			
		{
			COut.Door1Close = CLOSE; 
			COut.Door2Close = CLOSE;
			IncompleteCloseCounter = 0;
		}
		///else if(Emergency mode && Stng_NumOfDoor == 1) -> 2 Door open
		else 
		{
			/////	2Door Open In Emergency 
			if(Calibration && Stng[NUM_OF_DOOR] && Error_PHL && LEVEL)
			{
				COut.Door1Close = DCMD;
				COut.Door2Close = DCMD;
			}
			///// Normal Mode
			else 
			{
				if(Floor[E_Level].Door1Select)				{COut.Door1Close = DCMD;}
				else 																	{COut.Door1Close = CLOSE;}
				if(Stng[NUM_OF_DOOR] == 0 || Floor[E_Level].Door2Select == 0)	{COut.Door2Close = CLOSE;}
				else if(Floor[E_Level].Door2Select) 		{COut.Door2Close = DCMD;}
			}
		}
	}
	////68 or 69 Control Type
	else 
	{
		if(RunOrStop == RUN)																			
		{
			COut.Door1Close = 0;
			COut.Door1Open = 0;
			IncompleteCloseCounter = 0;
			DoorOpenTimer = 0;
		}
		else if(DCMD == OPEN)
		{
			if(DoorOpenTimer < DOOR_OPEN_TIME_REF) 
			{
				DoorOpenTimer ++;
				COut.Door1Open = 1; 	//OPEN
				COut.Door1Close = 0; 	//CLOSE
			}
			else 
			{
				COut.Door1Open = 0; 
				COut.Door1Close = 0;
			}
		}
		else 
		{
			DoorOpenTimer = 0;
			if(Stng[DOOR_CONTROL_TYPE] == DOOR_CONTROL_TYPE_68 && Debounced_68 == 0)
			{
				COut.Door1Open = 0; 	//OPEN
				COut.Door1Close = 1; 	//CLOSE
			}
			else if(Stng[DOOR_CONTROL_TYPE] == DOOR_CONTROL_TYPE_69 && Debounced_69 == 0)
			{
				COut.Door1Open = 0;		//OPEN
				COut.Door1Close = 1; 	//CLOSE
			}
			else 
			{
				COut.Door1Open = 0; 
				COut.Door1Close = 0;
			}
		}
	}
	
	if(IncompleteCloseCounter >= Stng[DOOR_END_TIME])
	{
		IncompleteCloseCounter = 0;
		Error_EndDoorTime = 1;
		PB.ClearAll = 1;
		AimFloor = NO_AIM;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////  RELEVELING  /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RelevelOperation(void)
{
	AllowToRelevel = 0;

	
	if(TypeElevator == TYPE_HYD || TypeElevator == TYPE_HEVOS)
	{
		if(Debounced_69 == 0)
		{
			DoNotOpenDoorAfterRelevel = 0; //Faghat bar asas khali kardan jac
		}
		else if(Debounced_69 && PreR1CF == 0 && In1CF && RunOrStop == STOP)
		{
			DoNotOpenDoorAfterRelevel = 1;//Faghat bar asas khali kardan jac
		}
		PreR1CF = In1CF;
	}
	else 
	{
		DoNotOpenDoorAfterRelevel = 0;
	}
	
	if(In1CF == 1 && RunOrStop == STOP)// && (DriveSpeed == STOP || DriveSpeed >= FORCE_STOP) 
	{
		WaitingFloorTimer = Stng[WAITING_FLOOR];
		if(StopTimer > 5) //0.6 Sec
		{
			if(TypeElevator == TYPE_HYD || TypeElevator == TYPE_HEVOS)
			{
				HYD_RelevelTimerEnable = 1; 
				if(HYD_RelevelTimer >= STOPPING_TIME_BEFORE_RELEVEL_IN_HYD)
				{
					 AllowToRelevel = 1;
					 DontOpenDoor = DONT_OPEN_DOOR_AFTER_RELEVER_IN_HYD; //bad az relevel dar tamam halat ha mesl REV
				}
			}
			else 
			{
				AllowToRelevel = 1;
				DontOpenDoor = 0;
			}
		}
	}
	else 
	{
		HYD_RelevelTimerEnable = 0;
		HYD_RelevelTimer = 0;
		if(LEVEL) 
		{
			HYD_Relevel_AfterRev = 0; // faghat yek bar bad az revesion hata agar dar CA1 bood baraye Relevel bere Down
		} 
	}
	
	
	if(AllowToRelevel)  // && AimFloor>=NO_AIM 
	{
		//Close door and check
		DoorParkF = 0;
		CMD_Door = CLOSE;
		if(CloseComplete)
		{
			ForceLanding = 1;
			RelevelingF = 1;

			if(TypeElevator == TYPE_HYD || TypeElevator == TYPE_HEVOS)
			{
				if(HYD_Relevel_AfterRev && InCA1 == 0)	{CMD_Dir = DOWN;}
				else {CMD_Dir = UP;}
			}
			else if(InCA1 == 0 || InCAN == 0)
			{
				if(InCA1 == 0)       {CMD_Dir = DOWN;}
				else if(InCAN == 0)  {CMD_Dir = UP;}
			}
			else if(Error_RLS_RelevelF) //Change Direction 
			{
				if(CMD_Dir == UP)        	{CMD_Dir = DOWN;}
				else if(CMD_Dir == DOWN)	{CMD_Dir = UP;}
			}
			else {Error_RLS_RelevelF = 0;}
		}
	}
	else if(RunOrStop == 0 && LEVEL) 
	{
		if(RelevelingF && DontOpenDoor == 0)
		{
			if(DoNotOpenDoorAfterRelevel == 0) {ODAFN = 1000;}
		}
		RelevelingF = 0;
		Error_RLS_RelevelF = 0;
		HYD_Relevel_AfterRev = 0;
	}
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////  CALCULATE LANDING FLAGS  //////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CalculateLandingFlags(int16_t OriginF,int16_t AimF)
{
	int16_t Floor_Dif = (int16_t) AimF - OriginF;
	
	//j1s = Stng_Jump1Speed[AimF];
	
	int8_t 	jmp1=Floor[AimF].Jump1Flag;
	int8_t 	jmpn=Floor[AimF].JumpNFlag - 2;
	
	CalAimCF3 = AimF * 2;
	CalAim1CF = CalAimCF3;
	
	int8_t	mv = 0;
	//// JumpN Flag Selection
	if(Floor_Dif > 1 || Floor_Dif < -1)
	{
		CF3Delay_Enable = 0;
		if(jmpn == 0)
		{
			CalAimCF3 = NO_AIM;
		}
		else
		{
			if(jmpn == -2)			mv =-3;
			else if(jmpn == -1)	mv =-2;
			else if(jmpn == 1)	mv =-1;
			else if(jmpn == 2)	mv = 0;
		}
	}
	//// Jump1 Flag Selection
	else
	{
		if(Floor[AimF].Jump1Speed == FAST-1) //if Speed is FAST (in menu FAST is FAST) 
		{
			CF3Delay_Enable = 1;
		}
		else {CF3Delay_Enable = 0;}
		
		if(jmp1 == 0)
		{
			CalAimCF3 = NO_AIM;
		}
		else
		{
			if(jmp1 == 2)				mv = 0;
			else if(jmp1 == 1)	mv =-1;
		}
	}
	//// Calculate 1CF & CF3 Aim
	if(CalAimCF3 < NO_AIM)
	{
		if(Floor_Dif < 0) {mv = -mv;}
		
		CalAimCF3 += mv;
	}
	
	if(Floor_Dif > 0) 	{CalAim1CF -= 1;}
	else             		{CalAim1CF += 1;} 
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// Collective Down & Full Collective ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CollectiveService(_Bool CollectiveDown)
{
	
	int8_t 	iw=0;
	_Bool		Cpb[MAX_NOS],Hpb[MAX_NOS];
	
	//nhpb = 0;
	//ncpb = 0;
	
	for(iw=0;iw<MAX_NOS;iw++)
	{
		Cpb[iw] = PB.Cabin[iw];
		Hpb[iw] = PB.Hall[iw];
		//if(PB.HallClear == iw || PB.CabinClear == iw || PB.ClearAll) {Cpb[iw] = 0; Hpb[iw] = 0;}
		
		//if(Cpb[iw]) ncpb++;
		//if(Hpb[iw]) nhpb++;
	}
	
	
	if(Hpb[E_Level] && RunOrStop == STOP && StopTimer > 9)
	{
		/* 	Aval faghat line 1 bood va dar ghesmat Normal dastoor baz shodan dar ersal mishod (reopen)
				ama chon motoghayer AimFloor taghir mikard moshkel zir etefagh miyoftad:
				agar masalan cabin level 2 bashe	va shasi level 0 bokhore, agar shasi level 5 ham badesh bokhore
				va ghabl az baste shodan kamel dar shasi level 2 bokhore va reopen beshe bejaye inke level 2 ro 
				javab bede mire level 5 */
		
		//AimFloor = E_Level; //Line 1
		
		
		if(GoToAim == 0 && RunOrStop == STOP && LEVEL && StopTimer > 10)
		{
			ODAFN = 1000;
			ForceDoorOpen = 1;
			PB.CabinClear = E_Level;
			PB.HallClear = E_Level;
		}
	}
	
	else if(Move == MOVE_RESET && AimFloor == NO_AIM)
	{
		for(iw=Stng[NUM_OF_STOP]-1 ;iw>=0 ;iw--) 
		{
			if(Hpb[iw]) 
			{
				AimFloor = iw;
				break;
			}		
		}
	}
	//// UP Collective Down
	else if(Move == UP && CollectiveDown == 1)
	{
		if(E_Level < (Stng[NUM_OF_STOP]-1) && (E_Level +1 != AimFloor || Cpb[E_Level+1] == 0))
		{
			_Bool UpperHpbF = 0;
			
			for(iw=Stng[NUM_OF_STOP] - 1 ;iw>E_Level-1 ;iw--) 
			{
				if(UpperHpbF == 0 && Hpb[iw])
				{
					UpperHpbF = 1;
					if(iw != AimFloor)
					{
						CalculateLandingFlags(E_Level,iw);
						if(Counter1CF < CalAim1CF && CounterCF3 < CalAimCF3) 
						{
							AimFloor = iw;
						}
					}
				}
				else if(Cpb[iw] && iw != AimFloor)
				{
					CalculateLandingFlags(E_Level,iw);
					if(Counter1CF < CalAim1CF && CounterCF3 < CalAimCF3) 
					{
						AimFloor = iw;
					}
				}
			}
		}
	}
	//// UP Full Collective
	else if(Move == UP && CollectiveDown == 0)
	{
		if(E_Level < (Stng[NUM_OF_STOP]-1) && (E_Level +1 != AimFloor) )
		{
			for(iw=E_Level + 1 ;iw<Stng[NUM_OF_STOP] ;iw++) 
			{
				if(Hpb[iw])// && iw != AimFloor)
				{
					CalculateLandingFlags(E_Level,iw);
					if(Counter1CF < CalAim1CF && CounterCF3 < CalAimCF3) 
					{
						AimFloor = iw;
						break;
					}
				}
			}
		}
	}
	////Down
	else if(Move == DOWN)
	{
		if(E_Level > 0 && (E_Level -1 != AimFloor))
		{
			for(iw=E_Level - 1 ;iw>=0 ;iw--)
			{
				if(Hpb[iw])
				{
					if(iw != AimFloor)
					{
						CalculateLandingFlags(E_Level,iw);
						if(Counter1CF > CalAim1CF && CounterCF3 > CalAimCF3) 
						{
							AimFloor = iw;
							break;
						}
					}
					else break;
				}
			}
		}
	}	
	
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////  ERRORS  ///////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void All_Errors_Check(void)
{
	///////////////////////////////////////////////////// 90
	if(Safety_90 == 0) 									{Error_Safety90 = 1;}
	else if((TypeElevator != TYPE_HYD && TypeElevator != TYPE_HEVOS) || Error_PHL)		{Error_Safety90 = 0;}

	
	///////////////////////////////////////////////////// 71
	if(Safety_71 == 0) 	{Error_SafetyCircuitCut = 1; MainErrorF = 1;}
	else								{Error_SafetyCircuitCut = 0;}
	
	if(Safety_71)
	{
		///////////////////////////////////////////////////// 66
		if(Safety_66 == 0) 		
		{
			if(RunOrStop) {Error_Cut66 = 1; MainErrorF = 1;} 
		}
		else 	{Error_Cut66 = 0;}

		if(Safety_66)
		{
			///////////////////////////////////////////////////// 69
			if(Debounced_69 == 0)	
			{
				if(RunOrStop) {Error_Cut69 = 1; MainErrorF = 1;} 
			}
			else {Error_Cut69 = 0;}
			
			if(Debounced_69)
			{
				///////////////////////////////////////////////////// 68      
				if(Debounced_68 == 0) 
				{
					if(RunOrStop) {Error_Cut68 = 1; MainErrorF = 1;} 
				}
				else {Error_Cut68 = 0;}
			}
			else {Error_Cut68 = 0;}
		}
		else 
		{
			Error_Cut69 = 0;
			Error_Cut68 = 0;
		}
	}
	else 
	{
		Error_Cut66 = 0;
		Error_Cut69 = 0;
		Error_Cut68 = 0;
	}
	///////////////////////////////////////////////////// Feedbacks
	if(Error_RLS || Error_RLSCut || Error_DRC || Error_DRCCut)  					{MainErrorF = 1;}
	if((Error_FLT_UNB || Error_PHR || Error_PHL) && EmergencyModeF == 0) 	{MainErrorF = 1;}
	if(Error_OverCurrent || Error_LowCurrent)  														{MainErrorF = 1;}
	if(Error_OverCurrent && MIn.Rev==0) {Error_OverCurrent = 0;}
	if(Error_FLT_DRV)		{MainErrorF = 1;}
	
	///////////////////////////////////////////////////// CA1,CAN,CF3 Sensors
	if(InCA1 == 0 && InCAN == 0)  {Error_CA1CAN = 1; 	MainErrorF = 1;}
	else {Error_CA1CAN = 0;}
	
	///////////////////////////////////////////////////// CF3
	if(LEVEL && CounterCF3 != Counter1CF && RunOrStop == RUN && Calibration == 0)
	{
		if(CF3ErrorChecker < 2)  CF3ErrorChecker++;
		else
		{
			Error_CF3 = 1;
			Calibration = 1;
		}
	}
	else 
	{
		CF3ErrorChecker=0;
		if(Calibration == 0) {Error_CF3 = 0;}
	}
	
	///////////////////////////////////////////////////// 1CF Timeout
	if(Timeout_1CF > Stng[ERROR_1CF_TIMEOUT])
	{
		Error_1CF = 0;
		MainErrorF = 1;
	}
	
	///////////////////////////////////////////////////// Cut Serial
	if(CAB_DataLost == 100) 
	{
		Error_CutSerial = 1;
		PB.ClearAll = 1;
		AimFloor = NO_AIM;
		if(LEVEL) {ODAFN = 1000;}
	}
	else {Error_CutSerial = 0;}
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////  Show Error & Alarm  /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const char ErrorNameList[][16]= //Max Length = 15
{
	"NO ERROR",       //0 
	"Day Counter!",   //1
	"Error CA1/CAN ", //2
	"Cut 66",         //3
	"Cut 69",         //4
	"Cut 68",         //5
	"Cut Serial",     //6
	"Close Timeout", 	//7
	"Error DRC",      //8
	"Cut DRC",        //9
	"Error RLS",      //10
	"Cut RLS",        //11
	"FLT UNB",        //12
	"Phase Lost",			//13
	"Phase Reverse",	//14
	"Over Current", 	//15
	"Low Current",		//16
	"Safety Cir Cut", //17
	"Error URA",      //18
	"Error FTO",      //19
	"Err Travel T",   //20
	"Error 1CF",      //21
	"Error CF3",      //22
	"LongTimeLevel",  //23
	"FLT DRV",        //25
	"End Door Time",  //24
	"Err Safety 90",  //25
	"Cut 1CF-CF3"			//26
};

static const char AlarmNameList[][16]= //Length = 15
{
	"NO ALARM",    	//0
	"MRV/CRV",     	//1
	"Fire Active", 	//2
	"Go To Park",  	//3
	"Overload",			//4
	"PHC",         	//5
	"LowVoltage24",	//6
	"STP Pressed", 	//7
	"DO Active",   	//8
	"DC Active",   	//9
	"MRV",					//10
	"CRV",					//11
	"24 SHORT 110"	//12
};

void ShowLastErr()
{
	//// Sum of errors
	NumOfErrors = (uint8_t) Error_DayCounter + Error_CA1CAN + Error_Cut66 + Error_Cut68 + Error_Cut69 + Error_CutSerial + Error_DoorCloseTimeout +
													Error_DRC + Error_DRCCut + Error_RLS_S + Error_RLSCut_S + Error_FLT_UNB + Error_PHL + Error_PHR + Error_OverCurrent + 
													Error_LowCurrent + Error_SafetyCircuitCut + Error_URA + Error_FTO + Error_TravelTimeout + Error_1CF + Error_CF3 +
													Error_LevelingTimeout + Error_FLT_DRV + Error_EndDoorTime + Error_Safety90 + Error_Cut_1CF_CF3;
	
	NumOfAlarms = (uint8_t) Alarm_Fire + Alarm_GoToPark + Alarm_MRVCRV + Alarm_Overload + Alarm_PHC + Alarm_InputSTP + 
													Alarm_LowVoltge24 + Alarm_DOActive + Alarm_DCActive + Alarm_MRV + Alarm_CRV + Alarm_ShortCircuit;

	if(NumOfErrors == 0)  {LastOKInErrorList = ErrorListCounter;}

	if( (NumOfErrors || NumOfAlarms) && ShowErrorTimer > 20 && TurnOnBoardTimer > TURN_ON_BOARD_TIME)
	{
		if(NumOfErrors)
		{
			if(TypeElevator != TYPE_2SP)
			{
				Error_DRC = 0;
				Error_DRCCut = 0;
			}

			if(Error_DayCounter            	&& ShowError < 1 )  	{ShowError = 1;}
			else if(Error_CA1CAN         		&& ShowError < 2 )  	{ShowError = 2;}
			else if(Error_Cut66          		&& ShowError < 3 )  	{ShowError = 3;	Error_Cut66 = 0;}
			else if(Error_Cut69             && ShowError < 4 ) 		{ShowError = 4;	Error_Cut69 = 0;}
			else if(Error_Cut68          		&& ShowError < 5 )  	{ShowError = 5;	if(Clear68CutError < 5) {Clear68CutError++;} else {Error_Cut68=0;} } //Clear after 10 sec
			else if(Error_CutSerial        	&& ShowError < 6 )  	{ShowError = 6;}
			else if(Error_DoorCloseTimeout 	&& ShowError < 7 )  	{ShowError = 7;	Error_DoorCloseTimeout = 0;}
			else if(Error_DRC             	&& ShowError < 8 )  	{ShowError = 8;}
			else if(Error_DRCCut           	&& ShowError < 9 ) 		{ShowError = 9;}
			else if(Error_RLS_S             && ShowError < 10) 		{ShowError =10; Error_RLS_S = 0;}
			else if(Error_RLSCut_S        	&& ShowError < 11)		{ShowError =11;	Error_RLSCut_S = 0;}
			else if(Error_FLT_UNB         	&& ShowError < 12)		{ShowError =12;}
			else if(Error_PHL             	&& ShowError < 13)		{ShowError =13;}
			else if(Error_PHR             	&& ShowError < 14)		{ShowError =14;}
			else if(Error_OverCurrent    		&& ShowError < 15)		{ShowError =15;}
			else if(Error_LowCurrent       	&& ShowError < 16)		{ShowError =16;	Error_LowCurrent = 0;}
			else if(Error_SafetyCircuitCut 	&& ShowError < 17)		{ShowError =17;}
			else if(Error_URA             	&& ShowError < 18)		{ShowError =18; Error_URA = 0;}
			else if(Error_FTO             	&& ShowError < 19)		{ShowError =19;}
			else if(Error_TravelTimeout    	&& ShowError < 20)		{ShowError =20;}
			else if(Error_1CF             	&& ShowError < 21)		{ShowError =21; Error_1CF = 0;}
			else if(Error_CF3             	&& ShowError < 22)		{ShowError =22;}
			else if(Error_LevelingTimeout		&& ShowError < 23)		{ShowError =23;	Error_LevelingTimeout = 0;}
			else if(Error_FLT_DRV          	&& ShowError < 24)		{ShowError =24;}
			else if(Error_EndDoorTime     	&& ShowError < 25)		{ShowError =25;	Error_EndDoorTime = 0;}
			else if(Error_Safety90     			&& ShowError < 26)		{ShowError =26;}
			else if(Error_Cut_1CF_CF3				&& ShowError < 27) 		{ShowError =27;}
			sprintf(ErrorAlarmStr,"%s",ErrorNameList[ShowError]); 
			
			
			///Error List
			_Bool check = 1;
			for(uint16_t i = LastOKInErrorList; i <= ErrorListCounter; i++)
			{
				if(ErrorList[i] == ShowError) {check = 0; break;}
			}
			if(check && ShowError>0) //&& ShowError!=3 && ShowError!=5 && ShowError!=7
			{
				if(ErrorListCounter >= (ERROR_LIST_SIZE - 1))
				{
					ErrorListCounter = (ERROR_LIST_SIZE - 1);

					for(uint8_t iw = 0; iw < (ERROR_LIST_SIZE - 1) ;iw++)
					{
						ErrorList[iw] = ErrorList[iw+1];
					}
				}
				else if(ErrorListCounter > 0 || ErrorList[0] > 0)  ErrorListCounter ++;
				
				ErrorList[ErrorListCounter] = ShowError;
				
				SaveErrorList();
			}
		}
		else
		{
			if(Alarm_MRVCRV                 && ShowError < 1 )    {ShowError = 1;}
			else if(Alarm_Fire         			&& ShowError < 2 )    {ShowError = 2;}
			else if(Alarm_GoToPark         	&& ShowError < 3 )    {ShowError = 3;}
			else if(Alarm_Overload       	  && ShowError < 4 )    {ShowError = 4;}
			else if(Alarm_PHC								&& ShowError < 5 )    {ShowError = 5;	Alarm_PHC = 0;}
			else if(Alarm_LowVoltge24    		&& ShowError < 6 )    {ShowError = 6;}
			else if(Alarm_InputSTP        	&& ShowError < 7 )    {ShowError = 7;}
			else if(Alarm_DOActive        	&& ShowError < 8 )    {ShowError = 8; Alarm_DOActive = 0;}
			else if(Alarm_DCActive         	&& ShowError < 9 )    {ShowError = 9; Alarm_DCActive = 0;}
			else if(Alarm_MRV        				&& ShowError < 10)    {ShowError = 10;}// Alarm_MRV = 0;}
			else if(Alarm_CRV         			&& ShowError < 11)    {ShowError = 11;}// Alarm_CRV = 0;}
			else if(Alarm_ShortCircuit			&& ShowError < 12)    {ShowError = 12;}
			
			sprintf(ErrorAlarmStr,"%s",AlarmNameList[ShowError]);
		}
		
		if(Error_Cut68 == 0) Clear68CutError = 0;
		
		BackLightOffTimer = 0;
		
		if(PreShowError != ShowError)  
		{
			 ShowErrorTimer = 0;
			 PreShowError = ShowError;
		}
		else
		{
			ShowError = 0;
			PreShowError = 0;
		}
	}
	else if(NumOfErrors == 0 && NumOfAlarms == 0 && ShowErrorTimer > 20) 
	{
		sprintf(ErrorAlarmStr,"OK");
		ShowError = 0;
		PreShowError = 0;
	}
	
	if(TurnOnBoardTimer > TURN_ON_BOARD_TIME) 
	{
		LCD_write_str(1,17,ErrorAlarmStr);
	}
}

void SelectorChange(uint8_t Max,_Bool Dir)
{
	if(KeyTime[0] > 2 || KeyTime[2] > 2)
	{
		if(KEY_UP && KEY_DN)
		{
			InfoSelector = 0;
			K_Up = 0;
			K_Dn = 0;
			KeyTime[0] = 0;
			KeyTime[2] = 0;

			if(MenuState == -1) //Clear Error List
			{
				for(uint8_t i=0; i < (ERROR_LIST_SIZE - 1); i++) {ErrorList[i] = 0;}
				
				ErrorListCounter = 0;
				LastOKInErrorList = 0;
				Error_OverCurrent = 0;
				Error_LowCurrent = 0;
				SaveErrorList();
			}
		}
		else if(Dir)
		{
			if(K_Dn) 
			{
				K_Dn=0;
				if(InfoSelector < Max) InfoSelector++;
				else InfoSelector =Max;
			}
			else if(K_Up) 
			{
				K_Up=0;
				if(InfoSelector > 0)   InfoSelector--;      
			}
		}
		else 
		{
			if(K_Dn) 
			{
				K_Dn=0;
				if(InfoSelector > 0)   InfoSelector--;
			}
			else if(K_Up) 
			{
				K_Up=0;         
				if(InfoSelector < Max) InfoSelector++;
				else InfoSelector=Max;
			}
		}
	}
}



void DayCounter(void)
{
	if(Error_DayCounter == 0 && DayCounter_Status != DAY_COUNTER_STATUS_PERMANENT)
	{
		if(DayCounter_Minutes > 0)		{DayCounter_Minutes --;}
		else
		{
			if(DayCounter_Day > 0 || DayCounter_Hour > 0)	{DayCounter_Minutes = 59;}
			if(DayCounter_Hour > 0) {DayCounter_Hour--;}
			else
			{
				if(DayCounter_Day > 0)
				{
					DayCounter_Hour = 23;
					DayCounter_Day --;
				}
			}
		}
		
		SaveDayCounterTimer++;
	}
		
	if(DayCounter_Day == 0 && DayCounter_Hour == 0 && DayCounter_Minutes == 0 && DayCounter_Status != DAY_COUNTER_STATUS_PERMANENT) 
	{
		if(Error_DayCounter == 0)
		{
			SaveDayCounterValues();
		}
		Error_DayCounter = 1;
	}
	else Error_DayCounter = 0;
}





//==========================================================================================================
//==========================================================================================================
//===============================================[  MAIN  ]=================================================
//==========================================================================================================
//==========================================================================================================

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();
  //MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
	
	TIM5_Init();		//50us Interrupter
	TIM2_Init();		//100us Interrupter
	TIM4_Init();		//1ms Interrupter
	USART1_Init();	//For Print
	DMA2_Stream7_Init();	//For TX UART1 (PR)
	DMA2_Stream5_Init();	//For RX UART1 (PR)
	USART2_Init();	//For Doublex
	DMA1_Stream6_Init();	//For TX UART2 (DBLX)
	DMA1_Stream5_Init();	//For RX UART2 (DBLX)
	UART5_Init();		//For Drive
	DMA1_Stream7_Init();	//For TX UART5
	DMA1_Stream0_Init();	//For RX UART5
	USART3_Init();	//For Cabin
	DMA1_Stream3_Init();	//For TX UART3
	DMA1_Stream1_Init();	//For RX UART3
	UART4_Init();		//For Hall
	DMA1_Stream4_Init();	//For TX UART4
	DMA1_Stream2_Init();	//For RX UART4
	
	//ADC for PT 3 Phase
	
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)adcv,5);
	//Encoder Timer3
	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_1);
	TIM3->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM3_IRQn);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	/////////////// Reset LCD PWR
	SetOutputsShiftF = 1;
	HW_LCD_PWR(1);
	HW_BKL(0);
	HW_RESET_PIN(0);
	HW_CS_PIN(0);
	HW_MOSI_PIN(0);
	HW_SCK_PIN(0);
	HW_BKL(1);
	HAL_Delay(10);
	
	HW_LCD_PWR(0);
	//Hw_BUZZ = 1;
	HAL_Delay(50);
	/////////////// Show RAVIS logo
	if(LCD_InitF == 0) LCD_Init();
	LCD_DrawBitmap(5,3,LogoRavis,86,64,1);
	LCD_Update();
	for(uint8_t	Del=0;Del<10;Del++)
	{
		WDT_RESET;
		HAL_Delay(10);
	}
	
	PassCorrect = 1;
	ReadDataFromEEPROM();
	CalculatePBNumber();
	ReadBoardFixedID();
	CheckLoadAndSaveDefault();
	ReadDayCounterValues();
	ReadErrorList();
	
	//// Controlling the stored information related to the day counter
	//// to ensure that no one has stored fake information
	if(	(DayAccuracy != DayCounter_Day) || (DayCounter_Day > 51 && DayCounter_Status != DAY_COUNTER_STATUS_PERMANENT) )
	{
		DayCounter_Day = 0;
		DayCounter_Hour = 0;
		DayCounter_Minutes = 0;
		DayCounter_Status = DAY_COUNTER_STATUS_30_DAY;
		Error_DayCounter = 0;
		SavedPermanentPass = 0;
		SaveDayCounterValues();
		Error_DayCounter = 1;
	}
	
	InitializeVariables();
	SetCarcodecTransferHWAddressArray();
	
	TypeElevator = Stng[TYPE_ELEVATOR];
	Mode = MODE_START;	
	
	
	Set_FirstValueInArrays();
	InitializeVariables();
	EncoderSetValue(0);
	
	gsm_init();
	/**
	while (1)
  {
		//WDT_RESET;
		//test();
		
	}**/
	
	
	while (1)
  {
		

		//test();
		
		test_modbus();
		
		WDT_RESET;
		Clear_UART_Errors();
		PT_RMS_Calculate();
		EncoderGetValue();
		
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////  Call Task 950 us  /////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		if(Task950usTimer > 18)
		{
			Task950usTimer = 0;
			
			AddressableInputsAndOutputs(); //It takes maximum 24 us
		}
			
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////  Call Task 20 Ms   /////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		if(Task20MsTimer > 19) 
		{
			Task20MsTimer -= 20;
			
			
			TestADCTime = TestADCTimer;
			TestADCTimer = 0;
			
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////  Call Task 100 Ms  /////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		if(Task100MsTimer > 99) 
		{
			Task100MsTimer -= 100;
			
			HW_BKL(BackLightOffTimer < LCD_BACKLIGHT_TIME);
			
			if(TurnOnBoardTimer < 100) {TurnOnBoardTimer++;}
			
			if(ShowErrorTimer < 100) {ShowErrorTimer++;}
			
			
			///// FTO
			FTO_MotorTemp = FTO_COOL;
			Error_FTO = 0;
			
			
			if(FTO_Adc > 150)												{FTO_MotorTemp = FTO_HOT;		Error_FTO = 1; 	MotorFan = 1;	LedFTO = 1;}
			else if(FTO_Adc < 70)										{FTO_MotorTemp = FTO_COOL;	Error_FTO = 0;	MotorFan = 0; LedFTO = 0;}
			else if(FTO_Adc > 80 && FTO_Adc < 140)	{FTO_MotorTemp = FTO_WARM;	MotorFan = 1;}
		}
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////  Call Task 500 Ms  /////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		if(Task500MsTimer > 499) 
		{
			Task500MsTimer -= 500;
			
			
			/////////////////////////////////
			///// Low Voltage 24V Alarm	/////
			/////////////////////////////////
			//PowerVoltage = adcv[2]/58.6;
			if(PowerVoltageMainBoard < 18.0 || PowerVoltageCabin < 18.0) {Alarm_LowVoltge24 = 1;}
			else {Alarm_LowVoltge24 = 0;}
			
			/////////////////////////////////
			///// Data Lost Calculation	/////
			/////////////////////////////////
			CAB_GetDataCount = CAB_GetDataCounter;
			CAB_GetDataCounter = 0;
			CAB_SendDataCount = CAB_SendDataCounter;
			CAB_SendDataCounter = 0;
			CAB_DataLost = 100 - ((float)CAB_GetDataCount / (CAB_SendDataCount -1)) * 100;
			if(CAB_DataLost < 0) CAB_DataLost = 0;
			if(CAB_DataLost > 100) CAB_DataLost = 100;
			
			
		}
		
		
		
		
		
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////    Serial Data Transmission    ///////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		CAB_SerialDataTransmition();			//In Serial.c
		Module_SerialDataTransmition();		//In Serial.c
		if(	Stng[DRIVE_CONNECTION] == DRIVE_CONNECTION_PARALLEL ||
				Stng[DRIVE_CONNECTION] == DRIVE_CONNECTION_ANALOG )			ParallelAnalog_OD_Module_SerialDataTransmition(); //In Serial.c
		
		//if(Stng[MAIN_PB_TYPE] == PB_TYPE_SERIAL_EXT) 				{HALL_SerialExtentionDataTransmition();}	//In Serial.c
		
		
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////  Prepare Values Associated With The Slave  /////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		if(SlaveDataLength == 0)
		{
			SlaveByteCounter = 0;
			
			SlaveSendBuffer[1] = 	((uint8_t)HwOut[20] << 7) | ((uint8_t)HwOut[19] << 6) | ((uint8_t)HwOut[18] << 5) | ((uint8_t)HwOut[17] << 4) | 
														((uint8_t)HwOut[16] << 3) | ((uint8_t)HwOut[15] << 2) | ((uint8_t)HwOut[14] << 1) | ((uint8_t)HwOut[13]);
			SlaveSendBuffer[2] = 	((uint8_t)LedFTO << 3) 		| ((uint8_t)HwOut[23] << 2) | ((uint8_t)HwOut[22] << 1) | ((uint8_t)HwOut[21]);
			
			SlaveSendBuffer[3] = 	((uint8_t)HwOut[31] << 7) | ((uint8_t)HwOut[30] << 6) | ((uint8_t)HwOut[29] << 5) | ((uint8_t)HwOut[28] << 4) | 
														((uint8_t)HwOut[27] << 3) | ((uint8_t)HwOut[26] << 2) | ((uint8_t)HwOut[25] << 1) | ((uint8_t)HwOut[24]);
			SlaveSendBuffer[4] = 	((uint8_t)HwOut[35] << 3) | ((uint8_t)HwOut[34] << 2) | ((uint8_t)HwOut[33] << 1) | ((uint8_t)HwOut[32]);
			
			SlaveSendBuffer[5] = Stng[MAIN_NUM_ACTIVE_LIGHT]; //0-20 High Light
			if(StandbyF)	{SlaveSendBuffer[6] = Stng[MAIN_NUM_SLEEP_LIGHT];}  //0-20 Low Light
			else 					{SlaveSendBuffer[6] = SlaveSendBuffer[5];}
			
			uint8_t	Data = SlaveReceiveBuffer[1];
			if( ((Data & 0x04) && 1) == ((Data & 0x20) && 1) )	MIn.Rev = (Data & 0x04) && 1;
			if( ((Data & 0x02) && 1) == ((Data & 0x10) && 1) )	MIn.RUP = (Data & 0x02) && 1;
			if( ((Data & 0x01) && 1) == ((Data & 0x08) && 1) )	MIn.RDN = (Data & 0x01) && 1;
			
			FTO_Adc = SlaveReceiveBuffer[2];
			PowerVoltageMainBoard = SlaveReceiveBuffer[3] / 5.0;
			
			CT1_RMS = (SlaveReceiveBuffer[4] / 2.0);	//  /2.0 for ZEMCT303 and /1.6 for ZMCT183
			CT2_RMS = (SlaveReceiveBuffer[5] / 2.0);	//  /2.0 for ZEMCT303 and /1.6 for ZMCT183
			
			Data = SlaveReceiveBuffer[6];
			HwIn[11] = Data & 0x01; //H1
			HwIn[12] = Data & 0x02; //H1
			HwIn[13] = Data & 0x04; //H3
			HwIn[14] = Data & 0x08; //H4
			HwIn[15] = Data & 0x10; //H5
			HwIn[16] = Data & 0x20; //H6
			HwIn[17] = Data & 0x40; //H7
			HwIn[18] = Data & 0x80; //H8
			Data = SlaveReceiveBuffer[7];
			HwIn[19] = Data & 0x01; //H9
			HwIn[20] = Data & 0x02; //H10
			HwIn[21] = Data & 0x04; //H11
			HwIn[22] = Data & 0x08; //H12
			
			//SlaveSendBuffer[3] +=10;
			SlaveDataLength = 8;
		}
		
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////    MENU    /////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		if(TurnOnBoardTimer > TURN_ON_BOARD_TIME) 
		{
			Menu();
			
			if(MenuState < -3) MenuState = -3;
			
			if(EEPROMSaveFlag)
			{
				Show_PleaseWait();
				
				if(Stng[WAITING_FLOOR] == 0) Stng[WAITING_FLOOR] = 1;
				
				SaveDataToEEPROM();
				
				Task5MsTimer = 0;
				Task10MsTimer = 0;
				Task20MsTimer = 0;
				Task100MsTimer = 0;
				Task500MsTimer = 0;
				Task1SecTimer = 1000;
				Task1MinuteTimer = 0;
				MaxCounter1CF = (Stng[NUM_OF_STOP] - 1) *2;
				DOOR_OPEN_TIME_REF = (uint16_t) Stng[DOOR_OPEN_TIME] * 100;
				TimeToOpenDoor = DOOR_OPEN_TIME_REF;
				EEPROMSaveFlag = 0;
				CalculatePBNumber();
				SetCarcodecTransferHWAddressArray();
				TypeElevator = Stng[TYPE_ELEVATOR];
			}
		}
		
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////    LCD Display    /////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		if(LCD_UpdateF == 0)
		{
			if(ReInitLCDTimer > 15000)//After 15 Second
			{
				ReInitLCDTimer = 0;
				LCD_Init();
				//HPB_Set = 5;
			}
			//// Show Ravis Logo
			else if(TurnOnBoardTimer < 2)
			{
				
			}
			//// Show Compile Date
			else if(TurnOnBoardTimer < TURN_ON_BOARD_TIME)
			{
				HYD_Relevel_AfterRev = 0;
				sprintf(LcdStr,"Compile:");
				LCD_write_str_middle(10,LcdStr);
				sprintf(LcdStr,"%u/%02u/%02u",COMPILE_YEAR,COMPILE_MONTH,COMPILE_DAY);
				LCD_write_str_middle(29,LcdStr);
				
				//LCD_write_str_middle(51,LcdStr);
				LCD_Update();
			}
			else if(MenuState == 0) //Main Page
			{
				//// Line 1
				sprintf(LcdStr,"%u",Stng[TYPE_ELEVATOR]);
				LCD_write_str(1,1,LcdStr);
				
				//// Line 3
				sprintf(LcdStr,"[%u,%u],%u,%u",StopTimer,MoveTimer,LandingF,Move);
				LCD_write_str(1,34,LcdStr);
				
				//// Line 4
				if(ShowSerialResetTimer == 0)	sprintf(LcdStr,"Dl:%u,%u,FTO:%u",CAB_DataLost,Mode,FTO_Adc);
				LCD_write_str(1,51,LcdStr);
				
				LCD_Update();
			}
			//////Error display 
			else if(MenuState == -1) //Second Page
			{
				if(FirstToMenu)
				{
					InfoSelector = ErrorListCounter;
					FirstToMenu = 0;
					K_Up = 0;
					K_Dn = 0;
					
					if(ErrorList[0] >= ERROR_LIST_SIZE)
					{
						for(uint8_t iw=0; iw<ERROR_LIST_SIZE; iw++) {ErrorList[iw] = 0;}
						SaveErrorList();
					}
				}

				sprintf(LcdStr,"ERROR [%u/%u]:",InfoSelector,ErrorListCounter);
				LCD_write_str(1,1,LcdStr);
				sprintf(LcdStr,"%s",ErrorNameList[ErrorList[InfoSelector]]);
				LCD_write_str(1,17,LcdStr);
				
				SelectorChange(ErrorListCounter,0);
				LCD_Update();
			}
			
			//// Hardware Inputs Display
			else if(MenuState == -2)
			{
				char LCD_LINE1[20],LCD_LINE2[20],LCD_LINE3[20],LCD_LINE4[20];
				
				if(FirstToMenu)
				{
					InfoSelector = 0;
					FirstToMenu = 0;
					K_Up = 0;
					K_Dn = 0;
				}

				if(InfoSelector == 0)
				{
					sprintf(LCD_LINE1,"DC:%.1fV,%.1fV",PowerVoltageMainBoard,PowerVoltageCabin);
					sprintf(LCD_LINE2,"FTO : %u",FTO_Adc);
					if(TypeElevator != TYPE_3VF && TypeElevator != TYPE_HEVOS)
					{
						 char v = 0;
						 if(CMD_Speed == FAST)       {v = 4;}
						 else if(CMD_Speed == SLOW)  {v = 1;}
						 
						 sprintf(LCD_LINE3,"I1 : %u A  %s",CT1_RMS,SpeedList[v]);
						 sprintf(LCD_LINE4,"I2 : %u A  %s",CT2_RMS,SpeedList[v]);
					}
					else 
					{
						LCD_LINE3[0] = 0;
						LCD_LINE4[0] = 0;
					}
				}
				else if(InfoSelector == 1)
				{
					sprintf(LCD_LINE1,"Phase R: %u V",(int)Ph_R_RMS);
					sprintf(LCD_LINE2,"Phase S: %u V",(int)Ph_S_RMS);
					sprintf(LCD_LINE3,"Phase T: %u V",(int)Ph_T_RMS);
					LCD_LINE4[0] = 0;
				}
				else if(InfoSelector == 2) 
				{
					sprintf(LCD_LINE1,"Safety:");
					sprintf(LCD_LINE2,"90: %u ,71: %u",Safety_90,Safety_71);
					sprintf(LCD_LINE3,"66: %u ,69: %u",Safety_66,Safety_69);
					sprintf(LCD_LINE4,"68: %u",Safety_68);                       
				}
				else if(InfoSelector == 3) 
				{
					if(CounterCF3 > 1000) CounterCF3 = 0;
					sprintf(LCD_LINE1,"CF3: %u  ,1CF: %u",CounterCF3,In1CF);
					sprintf(LCD_LINE2,"CA1: %u  ,CAN: %u",InCA1,InCAN);
					sprintf(LCD_LINE3,"RLS: %u  ,DRC: %u",MIn.RLS,MIn.DRC);    
					sprintf(LCD_LINE4,"FIRE: %u",InFire);          
				}
				else if(InfoSelector == 4) 
				{
					sprintf(LCD_LINE1,"IF1: %u  ,IFN: %u",MIn.IF1,MIn.IFN);
					sprintf(LCD_LINE2," ");
					sprintf(LCD_LINE3," ");    
					sprintf(LCD_LINE4," ");          
				}
				else if(InfoSelector == 5)
				{
					sprintf(LCD_LINE1,"HP1: %u ,HP2: %u",PB.Hall[0],PB.Hall[1]);
					sprintf(LCD_LINE2,"HP3: %u ,HP4: %u",PB.Hall[2],PB.Hall[3]);
					sprintf(LCD_LINE3,"HP5: %u ,HP6: %u",PB.Hall[4],PB.Hall[5]);
					sprintf(LCD_LINE4,"HP7: %u ,HP8: %u",PB.Hall[6],PB.Hall[7]);					
				}
				else if(InfoSelector == 6)
				{
					sprintf(LCD_LINE1,"HP9: %u ,HP10: %u",PB.Hall[8],PB.Hall[9]);
					sprintf(LCD_LINE2,"HP11: %u ,HP12: %u",PB.Hall[10],PB.Hall[11]);
					sprintf(LCD_LINE3,"HP13: %u ,HP14: %u",PB.Hall[12],PB.Hall[13]);
					sprintf(LCD_LINE4,"HP15: %u ,HP16: %u",PB.Hall[14],PB.Hall[15]);					
				}
				else if(InfoSelector == 7)
				{
					sprintf(LCD_LINE1,"CP1: %u ,CP2: %u",PB.Cabin[0],PB.Cabin[1]);
					sprintf(LCD_LINE2,"CP3: %u ,CP4: %u",PB.Cabin[2],PB.Cabin[3]);
					sprintf(LCD_LINE3,"CP5: %u ,CP6: %u",PB.Cabin[4],PB.Cabin[5]);
					sprintf(LCD_LINE4,"CP7: %u ,CP8: %u",PB.Cabin[6],PB.Cabin[7]);
				}
				else if(InfoSelector == 8)
				{
					sprintf(LCD_LINE1,"CP9: %u ,CP10: %u",PB.Cabin[8],PB.Cabin[9]);
					sprintf(LCD_LINE2,"CP11: %u ,CP12: %u",PB.Cabin[10],PB.Cabin[11]);
					sprintf(LCD_LINE3,"CP13: %u ,CP14: %u",PB.Cabin[12],PB.Cabin[13]);
					sprintf(LCD_LINE4,"CP15: %u ,CP16: %u",PB.Cabin[14],PB.Cabin[15]);
				}
				SelectorChange(8,1);
				
				LCD_write_str(1,1,LCD_LINE1);
				LCD_write_str(1,17,LCD_LINE2);
				LCD_write_str(1,34,LCD_LINE3);
				LCD_write_str(1,51,LCD_LINE4);
				LCD_Update();
			}
			
			
			//// Set PB in menu for calling floor
			else if(MenuState == -3)
			{
				char LCD_LINE1[16],LCD_LINE3[16];
					
				if(FirstToMenu)
				{
					InfoSelector = 0;
					FirstToMenu = 0;
					K_Up = 0;
					K_Dn = 0;
					KeyLast[1]=0;
					K_Ent = 0;            
				}
				
				char Ls = List_SegmentChoice[Floor[InfoSelector].SegL];
				if(Ls == '?') Ls = ' ';
				
				//sprintf(LCD_LINE1," Floor PB: %u",InfoSelector);
				if(Floor[InfoSelector].SegL == 0 && Floor[InfoSelector].SegR == 0)  {sprintf(LCD_LINE1," Floor PB: 0");}
				else {sprintf(LCD_LINE1," Floor PB: %c%c",Ls,List_SegmentChoice[Floor[InfoSelector].SegR]);}
				
				if(E_Level == InfoSelector) {sprintf(LCD_LINE3,"Level");}
				else                        {sprintf(LCD_LINE3,"ENT To Set");}

				if(K_Ext)
				{ 
					MenuState = 0;
					K_Ext = 0;
				}
				if(K_Ent)
				{
					PB.CabinSet = InfoSelector;
					K_Ent = 0;
				}
					
				SelectorChange(Stng[NUM_OF_STOP]-1,0);
				
				LCD_write_str(1,1,LCD_LINE1);
				LCD_write_str_middle(34,LCD_LINE3);
				LCD_Update();
			}
			
			else if(lcdt == 100) 
			{
				lcdt = 0;
				LCD_Update();
			}
		}
		
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////    Serial Print    /////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		if(Pr > 100)
		{
			Pr = 0;
			
			uint8_t Len = sprintf(PR_TX_Buffer,"90:%u,71:%u,66:%u,69:%u,68:%u,110To24:%u\r\n",Safety_90,Safety_71,Safety_66,Safety_69,Safety_68,Short110To24);
			PR_SendData(Len);
			
			//Len = sprintf(DBLX_TX_Buffer,"Test:%u\r\n",DBLX_RX_DMA_CNT);
			//DBLX_SendData(Len);
			//HW_LED_LINK_DBLX(1);
			
			//Len = sprintf(DRV_TX_Buffer,"Test:%u\r\n",DRV_RX_DMA_CNT);
			//DRV_SendData(Len);
			//HW_LED_LINK_DRV(1);
			
			//Len = sprintf(HALL_TX_Buffer,"Test:%u\r\n",HALL_RX_DMA_CNT);
			//HALL_SendData(Len);
			//HW_LED_LINK_HALL(1);
			
			//Len = sprintf(CAB_TX_Buffer,"Test:%u\r\n",CAB_RX_DMA_CNT);
			//CAB_SendData(Len);
			//HW_LED_LINK_HALL(1);
		}
		
		
		
		PreMode = Mode;
  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 5;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_VREFINT;
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
  hiwdg.Init.Reload = 999;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pins : PE2 PE3 PE4 PE5
                           PE6 PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA2 PA3 PA5 PA6
                           PA11 PA12 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA7 PA8 PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PE15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB11 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD8 PD9 PD10 PD11
                           PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PC6 PC7 PC8 PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC10 PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 PD3 PD4
                           PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD5 PD6 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
