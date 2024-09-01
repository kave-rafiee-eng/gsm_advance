#include "main.h"
#include "PB_Num.h"
#include "Defines.h"
#include "Hardware.h"
#include "Menu.h"
#include "stdio.h"
#include "Serial.h"



/////////	Setting
extern volatile		uint8_t			Stng[3000];

///Inputs
extern	_Bool									HwIn[MAX_HW_INPUTS];
///Outputs
extern	_Bool									HwOut[MAX_HW_OUTPUTS];
///HW Push Buttons
extern	_Bool									In_PB_Par[12],Out_PB_Par[12];
///Numrator						
extern	_Bool									Numrator_Par[MAX_PARALLEL_NUMRATOR],SegUp,SegDn;

///////// PB & Num
extern 	struct	PushButton{
	volatile	_Bool			Hall[MAX_PB];
  volatile	_Bool			Hall_Cancel[MAX_PB];
  volatile	_Bool			Cabin[MAX_PB];
	volatile	_Bool			Cabin_Cancel[MAX_PB];
  volatile	_Bool			FirstCabin[MAX_PB];
  volatile	_Bool			ClearAll;
  volatile	_Bool			BlinkLedStatus;
	volatile	uint8_t		ResetPBTimer[MAX_PB];
	volatile	uint8_t		HallBlinker[6];
	volatile	uint8_t		BlinkTimer;
	volatile	uint8_t		HallSet;
	volatile	uint8_t		HallClear;
	volatile	uint8_t		CabinSet;
	volatile	uint8_t		CabinClear;
	volatile	uint8_t		CabinBlinker[3];
	
	//////Parallel PB
	volatile	uint8_t 	Time[MAX_PB];
	volatile	_Bool			Last[MAX_PARALLEL_PB];
	volatile	_Bool			ForCancelStatus[MAX_PARALLEL_PB];
}PB;
//volatile 	_Bool				PB.Hall[MAX_PB],PB.Hall_Cancel[MAX_PB];
//volatile	_Bool				PB.Cabin[MAX_PB],PB.Cabin_Cancel[MAX_PB],PB.FirstCabin[MAX_PB];
//volatile 	uint8_t 		PB_Time[MAX_PB];
//volatile	_Bool				PB.ClearAll,PB.BlinkLedStatus;
//volatile	uint8_t			ResetPBTimer[MAX_PB],PB.HallBlinker[6],PB.BlinkTimer,PB.HallSet,PB.HallClear,PB.CabinSet,PB.CabinClear,PB.Cabin_Blinker[3];
extern	volatile 	uint8_t			NumR,NumL;
/// Parallel PB
//extern 	volatile	_Bool				L_HPB[MAX_PARALLEL_PB],ForCancelStatus[MAX_PARALLEL_PB];
//extern	volatile 	uint8_t 		PB_Time[MAX_PARALLEL_PB];


/////////	Error
extern	volatile	_Bool				Error_RLS,Error_RLSCut,Error_DRC,Error_DRCCut,Error_RLS_RelevelF,Error_FTO,Error_TravelTimeout;
extern	volatile	_Bool				Error_DoorCloseTimeout,Error_URA,MainErrorF,Error_CutSerial,Error_DayCounter;
extern	volatile	_Bool				Error_SafetyCircuitCut,Error_Cut66,Error_Cut68,Error_Cut69,Error_LevelingTimeout;
extern	volatile	_Bool				Error_FLT_UNB,Error_PHR,Error_PHL,Error_OverCurrent,Error_LowCurrent,Error_CA1CAN,Error_1CF,Error_CF3;
extern	volatile 	uint8_t			NumOfErrors;

/////////	Alarm
extern	volatile	_Bool				Alarm_MRVCRV,Alarm_Fire;


/////////	Timer

/////////	Program
extern	volatile	_Bool				CIn_IN,CIn_PHC,CIn_DO,CIn_OVL,CIn_REV,CIn_UP,CIn_DN,CIn_STP,ODAFN;//Open door announce floor number
extern	volatile	_Bool				RunOrStop,StandbyF,LandingF,CMD_Door,CarOut;
extern	volatile	_Bool				CMD_Dir,PreDir,OverlapF,ForceDoorOpen,DoorParkF,CloseComplete,EmergencyModeF,HYD_Relevel_AfterRev;
extern	volatile	_Bool				Pre1CF,PreCF3,SetAllFloorForFTO,BlinkNumState,Hall_TxModeF;
extern	volatile	uint8_t			E_Level,CMD_MP3File,CMD_Speed,PreSpeed,OpeningDoorState,ClosingDoorState,IncompleteCloseCounter;
extern	volatile	uint8_t			AimFloor,PreAimFloor,Mode,PreMode,BlinkNumTimer,NumOfMainPB,NumOfCarPB;
extern	volatile	int8_t			Counter1CF,CounterCF3,MaxCounter1CF;


extern struct	FloorProperty{
	uint8_t		Delay_1CF_Up;
	uint8_t		Delay_1CF_Down;
	uint8_t		Door1Select;	//Value:0-1-2
	uint8_t		Door2Select;	//Value:0-1-2
	uint8_t		Door3Select;	//Value:0-1-2
	uint8_t		SegL;
	uint8_t		SegR;
	uint8_t		Talk;
	uint8_t		Jump1Speed;
	int8_t		Jump1Flag;
	int8_t		JumpNFlag;
	uint8_t		LevelDistance;
	uint32_t	PB;
	uint8_t		MainPBNum;
	uint8_t		CarPBNum;
	uint32_t	StartMonitor;
}Floor[MAX_NOS];

const char SegmentMap[] = 
{
	0x00, // 0: NULL
	0x3f, // 1: 0 
	0x06, // 2: 1
	0x5b, // 3: 2
	0x4f, // 4: 3 
	0x66, // 5: 4
	0x6d, // 6: 5
	0x7d, // 7: 6
	0x07, // 8: 7
	0x7f, // 9: 8 
	0x6f, // 10:9
	0x3D, // 11:G
	0x73, // 12:P
	0x7C, // 13:B
	0x50, // 14:r
	0x01, // 15:a
	0x02, // 16:b
	0x04, // 17:c
	0x08, // 18:d
	0x10, // 19:e
	0x20, // 20:f
	0x40, // 21:g
	0x80, // 22:h
	0x49, // 23:Day Counter Mark
	0x54, // 24:n for CF3 & 1CF Error
	0x1B, // 25:Start Mode Mark
	0x79,	// 26:E for Errors
	0x71,	// 27:F Fire&FTO
	0x76,	// 28:H	Emergency
	0x39,	// 29:C Calibration
};



////Each floor and total PB number calculator:
void CalculatePBNumber(void)
{
	NumOfMainPB = Stng[NUM_OF_STOP];
	NumOfCarPB = Stng[NUM_OF_STOP];
	
	for(uint8_t i=0;i<Stng[NUM_OF_STOP];i++)
	{
		Floor[i].MainPBNum = 1;
		Floor[i].CarPBNum = 1;
	}
}


void 	Correspondence_And_Control_Of_PB(void)
{
	
}

void	Parallel_PushButton_Operation(void)
{
	/////	Putting addressable (HwIn array values into the program "PB Parallel In")
	for(uint8_t i=0; i<MAX_PARALLEL_PB; i++)
	{
		if(Stng[MAIN_PB_ADD + i + 1] == 0) 
		{
			In_PB_Par[i] = 0;
		}
		else 
		{
			In_PB_Par[i] = HwIn[Stng[MAIN_PB_ADD + i + 1] + 10];
		}
	}
	
	
	_Bool StillIsOn=0;
	
	for(uint8_t iw=0;iw<NumOfMainPB;iw++)
	{
		/////////////////////
		//// All PB Type ////
		/////////////////////
		//////////////////////////Start
		///For All Type
		if(SetAllFloorForFTO)
		{
			PB.ClearAll = 0;
			PB.HallClear = NO_AIM;
			PB.CabinClear = NO_AIM;
			if(PB.HallSet == NO_AIM) 	PB.HallSet = iw;
			if(PB.CabinSet == NO_AIM) PB.CabinSet = iw;
		}
		///For All Type
		if(PB.ClearAll)
		{
			if(PB.Hall[iw]) 		{StillIsOn = 1; PB.Hall[iw] = 0;}
			if(PB.Cabin[iw])		{StillIsOn = 1; PB.CabinClear = iw;}
		}
		
		///For parallel type.For other types, it should be written like the carcodec part
		else
		{
			if(PB.HallClear == iw) 										{PB.Hall[iw] = 0; PB.HallClear = NO_AIM;}
			else if(PB.HallSet == iw)									{PB.Hall[iw] = 1; PB.Hall_Cancel[iw] = 0; PB.ForCancelStatus[iw] = 0; PB.HallSet = NO_AIM;}
			///carcodec part
			if(PB.CabinClear == iw && PB.Cabin[iw] == 0)  		{PB.CabinClear = NO_AIM;}
			else if(PB.CabinSet == iw && PB.Cabin[iw] == 1)		{PB.CabinSet = NO_AIM;}
		}
		
		if(PB.Cabin[iw] == 1 && PB.Hall[iw] == 0 && PB.HallClear != iw && PB.CabinClear != iw)
		{
			PB.FirstCabin[iw] = 1;
			PB.Hall[iw] = 1;
			PB.Hall_Cancel[iw] = 0; 
		}
		
		if(PB.Hall[iw] == 0) 	
		{
			if(PB.Cabin[iw] == 0) {PB.FirstCabin[iw] = 0;}
		}
		/////////////////////////End
		
		
		//////////////////////////
		//// Parallel PB Type ////
		//////////////////////////
		if(PB.Hall[iw] == 0) 	
		{
			PB.Hall_Cancel[iw] = 0; 
			PB.ForCancelStatus[iw] = 0; 
		}
		
		if(In_PB_Par[iw])
		{
			 if(PB.Time[iw] < 60) PB.Time[iw]++;
			 
			 if(PB.Hall[iw]==0 && PB.Last[iw]==0) 
			 {
					if(PB.Time[iw] > 10)
					{
						 PB.Hall[iw] = 1;
						 PB.Last[iw] = 1;
					}
			 }
			 else if(PB.Hall_Cancel[iw]==0 && PB.Hall[iw] && PB.ForCancelStatus[iw])  //&& L_PB.Hall[i]
			 {
					if(PB.Time[iw] > CANCEL_PUSHB_TIME) PB.Hall_Cancel[iw] = 1;
			 }
		}
		//else if(PB.Hall_Cancel[iw])  {PB.Hall[iw]=0; PB.Hall_Cancel[iw]=0;}
		else
		{
			 if(PB.Time[iw]>1) PB.Time[iw]-=2;
			 
			 else
			 {
					if(PB.Hall[iw]) {PB.ForCancelStatus[iw] = 1;}
					
					PB.Last[iw] = PB.Hall[iw];
			 }
		}
		
		_Bool v = PB.Hall[iw];
		if(PB.HallBlinker[0] == iw || PB.HallBlinker[1] == iw || PB.HallBlinker[2] == iw || PB.HallBlinker[3] == iw || PB.HallBlinker[4] == iw || PB.HallBlinker[5] == iw)
		{
			 v = v && PB.BlinkLedStatus;
		}
		Out_PB_Par[iw] = v;
	}
	
	
	if(PB.ClearAll && StillIsOn == 0) PB.ClearAll = 0; 
	
	/////	Putting addressable (program "PB Parallel Out" into the HwOut)
	for(uint8_t i=0; i<MAX_PARALLEL_PB; i++)
	{
		HwOut[Stng[MAIN_PB_ADD + i + 1] + 23] = Out_PB_Par[i];
	}
}


void 	SerialExt_PushButton_Operation(void)
{
	_Bool StillIsOn=0;
	
	for(uint8_t iw=0;iw<NumOfMainPB;iw++)
	{
		/////////////////////
		//// All PB Type ////
		/////////////////////
		//////////////////////////Start
		///For All Type
		if(SetAllFloorForFTO)
		{
			PB.ClearAll = 0;
			PB.HallClear = NO_AIM;
			PB.CabinClear = NO_AIM;
			if(PB.HallSet == NO_AIM) 	PB.HallSet = iw;
			if(PB.CabinSet == NO_AIM) PB.CabinSet = iw;
		}
		///For All Type
		if(PB.ClearAll)
		{
			if(PB.Hall[iw]) 		{StillIsOn = 1; PB.HallClear = iw;}
			if(PB.Cabin[iw])		{StillIsOn = 1; PB.CabinClear = iw;}
		}
		
		///For parallel type.For other types, it should be written like the carcodec part
		else
		{
			if(PB.HallClear == iw && PB.Hall[iw] == 0)  			{PB.HallClear = NO_AIM;}
			else if(PB.HallSet == iw && PB.Hall[iw] == 1)			{PB.HallSet = NO_AIM;}
			
			///carcodec part
			if(PB.CabinClear == iw && PB.Cabin[iw] == 0)  		{PB.CabinClear = NO_AIM;}
			else if(PB.CabinSet == iw && PB.Cabin[iw] == 1)		{PB.CabinSet = NO_AIM;}
		}
		
		
		///For All Type
		if(PB.Cabin[iw] == 1 && PB.Hall[iw] == 0 && PB.HallClear != iw && PB.CabinClear != iw)
		{
			PB.FirstCabin[iw] = 1;
			if(PB.HallSet == NO_AIM) PB.HallSet = iw;
//			PB.Hall[iw] = 1;
//			PB.Hall_Cancel[iw] = 0; 
		}
		
		
		if(PB.Hall[iw] == 0 && PB.Cabin[iw] == 0) 	
		{
			PB.FirstCabin[iw] = 0;
		}
		
		/////////////////////////End
	}
	
	if(PB.ClearAll && StillIsOn == 0) PB.ClearAll = 0; 
	
}



void NumratorUpdate(void)
{
	////////////////
	/// Numrator ///
	////////////////
	
	if(Mode == MODE_START)
	{
		NumR = SegmentMap[25]; //Start Mode Mark
	  NumL = 0;
	}
	else if(Error_FTO || Alarm_Fire)
	{
		NumR = SegmentMap[27]; //F
		NumL = 0;
		
	}
	else if(Error_LevelingTimeout || Error_TravelTimeout || Mode==MODE_ERROR)
	{
		NumR = SegmentMap[26]; //E
		NumL = 0;
	}                  
	else if(Mode == MODE_REVESION)
	{
		NumR = SegmentMap[14]; //r
		NumL = 0;
	}
	else if(EmergencyModeF)
	{
		NumR = SegmentMap[28]; //H
		NumL = 0;
	}
	else if(Error_DayCounter)
	{
		NumR = SegmentMap[23]; //Day Counter Mark
		NumL = 0; 
	}
	else if(Mode == MODE_CALIBRATION)
	{
		NumR = SegmentMap[29]; //C
		NumL = 0; 
	}
	else if(Error_CF3)
	{
		NumR = SegmentMap[24]; //n
		NumL = 0; 
	}         
	else if(Mode == MODE_NORMAL)
	{
		//if(StandbyF == 0)
		//{
			NumR 	= SegmentMap[Floor[E_Level].SegR];
			NumL	= SegmentMap[Floor[E_Level].SegL];
		//}

	}
	
	
	// UP & DOWN 
	if(RunOrStop == RUN)
	{
		if(CMD_Dir == UP)           {SegUp = 1;   SegDn = 0;}
		if(CMD_Dir == DOWN)         {SegUp = 0;   SegDn = 1;}
	}
	else if(Mode == MODE_NORMAL && AimFloor < NO_AIM)
	{  
		if(AimFloor > E_Level)       {SegUp = 1;   SegDn = 0;}
		else if(AimFloor < E_Level)  {SegUp = 0;   SegDn = 0;}
	}         
	else 
	{
		SegUp = 0;
		SegDn = 0;
	}
	
	
	//// Blink Up&Down&PB
	if(RunOrStop == STOP) 
	{
		PB.BlinkLedStatus = 1;
		PB.BlinkTimer = 0;
	}
	else
	{
		if(LandingF == 1 && PB.BlinkTimer < 2) 			PB.BlinkTimer++;
		else if(LandingF == 0 && PB.BlinkTimer < 4)	PB.BlinkTimer++;
		else
		{
			PB.BlinkTimer = 0;
			PB.BlinkLedStatus = !PB.BlinkLedStatus;
		}
	}
	_Bool	su = SegUp & PB.BlinkLedStatus;
	_Bool	sd = SegDn & PB.BlinkLedStatus;
	
	
	/////	Putting addressable program "Numrator_Par" into the HwOut)
	if(Stng[MAIN_NUM_TYPE] == NUM_TYPE_PARALLEL)
	{
		Numrator_Par[0] = (NumR & 0x01)&&1;
		Numrator_Par[1] = (NumR & 0x02)&&1;
		Numrator_Par[2] = (NumR & 0x04)&&1;
		Numrator_Par[3] = (NumR & 0x08)&&1;
		Numrator_Par[4] = (NumR & 0x10)&&1;
		Numrator_Par[5] = (NumR & 0x20)&&1;
		Numrator_Par[6] = (NumR & 0x40)&&1;
		Numrator_Par[7] =	su;
		Numrator_Par[8] =	sd;
		Numrator_Par[9] =  (NumL & 0x01)&&1;
		Numrator_Par[10] = (NumL & 0x02)&&1;
		Numrator_Par[11] = (NumL & 0x04)&&1;
		Numrator_Par[12] = (NumL & 0x08)&&1;
		Numrator_Par[13] = (NumL & 0x10)&&1;
		Numrator_Par[14] = (NumL & 0x20)&&1;
		Numrator_Par[15] = (NumL & 0x40)&&1;
		
		for(uint8_t i=0; i<MAX_PARALLEL_PB; i++)
		{
			HwOut[Stng[MAIN_NUM_ADD + i + 1]] = Numrator_Par[i];
		}
	}
}



