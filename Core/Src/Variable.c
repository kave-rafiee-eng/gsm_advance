#include "main.h"
#include "n1202.h"
#include "Hardware.h"
#include "Defines.h"
#include "Serial.h"
#include "Menu.h"
#include "AT24Cxx.h"
#include "EEPROM.h"
#include "PB_Num.h"

///////// LCD
volatile  uint8_t			_LCD_RAM[LCD_X*LCD_String],xlcd=0;
volatile  uint8_t   	LCD_Cmd_Counter=0,LCD_Data_Counter=0,LCD_InitF=0,LCD_InitState=0,LCD_InitTimer=0;
volatile  uint8_t   	LCD_UpdateF=0,LCD_Cmd_Row=0,lcdt=0,LCD_RefreshTimer=0;
volatile  uint16_t  	LCD_SendAdd=0;
char                	LcdStr[100];

///////// Menu UI
volatile  uint8_t   	K_Up=0,K_Dn=0,K_Ent=0,K_Ext=0,KeyLast[4]={0,0,0,0},EEPROMSaveFlag=0;
volatile	uint8_t   	Selector[20],FirstToMenu=0,Selected_address=0,Xmenu=0,ExitFromMenuTimer;
volatile  int8_t    	MenuState=0,MenuBar[10],EnterWithTimerF=0,ExitWithTimerF=0;
volatile  uint16_t  	KeyTime[4]={0,0,0,0},KeyHoldTime[2],Ymenu=0;
_Bool									FirstInDayCounterMenu=1;

/////////	Slave MCU
volatile	_Bool    		SlaveCLK_Level=1,SlaveMOSI_Level=0;
volatile	uint8_t			SlaveSyncTimer=0,SlaveByteCounter=0,SlaveBitCounter=0,SlaveTxData=0,SlaveRxData=0;
volatile	uint8_t			SlaveSendBuffer[30],SlaveReceiveBuffer[30],SlaveDataLength=0;

///////// Serial PR
char                 	PR_TX_Buffer[PR_BUFFER_SIZE];
volatile 	char 				PR_RX_Buffer[PR_BUFFER_SIZE];
volatile	uint16_t		PR_LastRxDMACNT=0,PR_LastTxDMACNT=0;
volatile	_Bool				PR_TxModeF=0,PR_RxModeF=0;
volatile	uint32_t		PR_TxDMA_CR_Val=0;

uint8_t								PR_ReceivedByte[3500];
volatile	uint16_t		PR_ByteReciveTimer=0;
volatile	uint16_t		PR_RXDataCNT=0,PR_DataLen=0,PR_ByteCounter=0,PR_SyncStatus=0;

///////// Serial Doublex
char                 	DBLX_TX_Buffer[UART_BUFFER_SIZE];
volatile 	char 				DBLX_RX_Buffer[UART_BUFFER_SIZE];
volatile	uint16_t		DBLX_LastRxDMACNT=0,DBLX_LastTxDMACNT=0;
volatile	_Bool				DBLX_TxModeF=0,DBLX_RxModeF=0;
volatile	uint32_t		DBLX_TxDMA_CR_Val=0;

///////// Serial Cabin
char                 	CAB_TX_Buffer[UART_BUFFER_SIZE];
volatile 	char 				CAB_RX_Buffer[UART_BUFFER_SIZE];
volatile	uint16_t		CAB_LastRxDMACNT=0,CAB_LastTxDMACNT=0;
volatile	_Bool				CAB_TxModeF=0,CAB_RxModeF=0;
volatile	uint32_t		CAB_TxDMA_CR_Val=0;

volatile	_Bool				CAB_NewPacketF=0,CAB_ClearDataInDataLostF=0;
volatile	uint8_t			CAB_ReceivedByte[50];
volatile	uint16_t		CAB_RXDataCNT=0,CAB_PacketCounter=0,CAB_PacketError=0,CAB_PacketLen=0,CAB_SendDataCounter=0,CAB_GetDataCounter=0,CAB_SyncStatus=0;
volatile	uint16_t		CAB_GetDataTimer=0,CAB_TX_AfterLastByteTimer=0,TransferHWAddCounter=0;
volatile  int16_t			CAB_SendDataCount=0,CAB_GetDataCount=0,CAB_DataLost=0;


///////// Serial Hall
char 									HALL_TX_Buffer[UART_BUFFER_SIZE];
volatile	char				HALL_RX_Buffer[UART_BUFFER_SIZE];
volatile	uint16_t		HALL_LastRxDMACNT=0,HALL_LastTxDMACNT=0,HALL_RXDataCNT=0;
volatile	_Bool				HALL_TxModeF=0,HALL_RxModeF=0;
volatile	uint32_t		HALL_TxDMA_CR_Val=0;

volatile	_Bool				HALL_NewPacketF=0;
volatile	uint8_t			HALL_ReceivedByte[50],HALL_LinkLED_Status=0;
volatile	uint16_t		HALL_PacketCounter=0,HALL_PacketError=0,HALL_PacketLen=0,HALL_SendDataCounter=0,HALL_GetDataCounter=0,HALL_SyncStatus=0;
volatile	uint16_t		HALL_GetDataTimer=0,HALL_TX_AfterLastByteTimer=0;

///////// Serial Drive
char 									DRV_TX_Buffer[UART_BUFFER_SIZE];
volatile	char				DRV_RX_Buffer[UART_BUFFER_SIZE];
volatile	uint16_t		DRV_LastRxDMACNT=0,DRV_LastTxDMACNT=0,DRV_RXDataCNT=0;
volatile	_Bool				DRV_TxModeF=0,DRV_RxModeF=0;
volatile	uint32_t		DRV_TxDMA_CR_Val=0;

volatile	_Bool				DRV_NewPacketF=0;
volatile	uint8_t			DRV_ReceivedByte[50],DRV_LinkLED_Status=0;
volatile	uint16_t		DRV_PacketError=0,DRV_PacketLen=0,DRV_SendDataCounter=0,DRV_GetDataCounter=0,DRV_SyncStatus=0;
volatile	uint16_t		DRV_GetDataTimer=0,DRV_TX_AfterLastByteTimer=0;



/////////	Setting
volatile	uint8_t			Stng[SETTING_ARRAY_SIZE];
uint8_t 							EEPROM_Buffer[EE_BUFFER_SIZE],CAB_TransferHWAdd[100];
_Bool									Stng_DriveParallelV0[10],Stng_DriveParallelV1[10],Stng_DriveParallelV2[10];
uint8_t								Pass_Num[5],DayCounter_Status=0,DayCounter_Day=50,DayCounter_Hour=0,DayCounter_Minutes=0,DayAccuracy=0;
volatile	uint32_t		BoardFixedID,BoardUID,DayCounterPassword50Day=0,DayCounterPassword30Day=0,DayCounterPasswordPermanent=0,SavedPermanentPass=0;																		
volatile	uint32_t 		HwPass[3],TemporaryID=0;

///////// Hardware
volatile	_Bool    		SetOutputsShiftF=0,ShiftCLK_Level=0,ShiftDATA1_Level=0,ShiftDATA2_Level=0,ShiftSTCP_Level=0;
volatile	uint8_t  		OutputsShiftStatus=0,FTO_Adc=0,CT1_RMS=0,CT2_RMS=0,FTO_MotorTemp=FTO_COOL;
volatile	_Bool				Hw_OUTPN=0,Hw_BUZZ=0,Hw_PO1=0,Hw_PO2=0,Hw_PO3=0,Hw_PO4=0;
volatile	_Bool				Hw_OC1=0,Hw_OC2=0,Hw_OC3=0,Hw_OC4=0,Hw_OC5=0,Hw_OC6=0,Hw_OC7=0,Hw_RLY_7191=0;
volatile	int16_t			adcv[10];
volatile	int					VRefIntSum=0,TestADCTimer,TestADCTime;
volatile  float   		BatteryVoltage=4.2,PowerVoltageMainBoard=24.0,PowerVoltageCabin=24.0,ADCToVoltageFactor;

///Inputs
_Bool									HwIn[MAX_HW_INPUTS]={0,0};
struct	MainInputList{
	_Bool		CF3;
	_Bool		_1CF;
	_Bool		CAN;
	_Bool		CA1;
	_Bool		IFN;
	_Bool		IF1;
	_Bool		FLTDR;
	_Bool		RLS;
	_Bool		DRC;
	_Bool		Fire;
	_Bool		Overload;
	_Bool		FullLoad;
	_Bool		RelvUp;
	_Bool		RelvDn;
	_Bool		ZADO;
	_Bool		Ready;
	_Bool		RES2;
	_Bool		RES3;
	_Bool		RES4;
	_Bool		RES5;
	_Bool		Rev;
	_Bool		RUP;
	_Bool		RDN;
}MIn={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


///Outputs
_Bool									HwOut[MAX_HW_OUTPUTS]={0,0};
struct	MainOutputList{
	_Bool		MUP;
	_Bool		MDN;
	_Bool		V0;
	_Bool		V1;
	_Bool		V2;
	_Bool		TC;
	_Bool		STDL;
	_Bool		URA;
	_Bool		Fan;
	_Bool		EN_Out;
	_Bool		Relay;
	_Bool		UPSRelay;
	_Bool		UPSRef;
	_Bool		ADO;
	_Bool		Out1CF;
	_Bool		DRV_RST;
	_Bool		ERS;
	_Bool		ENR;
	_Bool		RES4;
	_Bool		RES5;
	_Bool		Close1;
	_Bool		Close2;
	_Bool		Close3;
	_Bool		Close4;
	_Bool		Close5;
}MOut={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

///Push Buttons
_Bool									In_PB_Par[12],Out_PB_Par[12];
struct	PushButton{
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
}
PB;

///Numrator						
_Bool									Numrator_Par[MAX_PARALLEL_NUMRATOR],SegUp=0,SegDn=0;
volatile 	uint8_t			NumR,NumL;

struct SerialExtentionInputList{
	
	_Bool		F1;
	_Bool		F2;
	_Bool		I1;
	_Bool		I2;
	_Bool		I3;
	_Bool		I4;
	_Bool		I5;
	_Bool		S68;
	
}SerialExtInput;

//Car In
struct CarcodecInputList{
	_Bool		Ovl;
	_Bool		Ful;
	_Bool		Fan;
	_Bool		PHC1;
	_Bool		PHC2;
	_Bool		PHC3;
	_Bool		DO1;
	_Bool		DO2;
	_Bool		DO3;
	_Bool		DC;
	_Bool		Rev;
	_Bool		RUP;
	_Bool		RDN;
	_Bool		STP;
	_Bool		RES1;
	_Bool		RES2;
	_Bool		RES3;
	_Bool		RES4;
	_Bool		RES5;
}
CIn={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//Car	Out
volatile	_Bool				CMD_URA=0;//,COut_FAN=0,COut_OVL=0,COut_GANG=0,COut_AVA=0;
struct CarcodecOutputList{
	_Bool		Door1Close;
	_Bool		Door2Close;
	_Bool		Door3Close;
	_Bool		Door1Open;
	_Bool		Door2Open;
	_Bool		Door3Open;
	_Bool		URA;
	_Bool		Fan;
	_Bool		Ovl;
	_Bool		Gang;
	_Bool		Ava;
	_Bool		RES1;
	_Bool		RES2;
	_Bool		RES3;
	_Bool		RES4;
	_Bool		RES5;
}
COut={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/////////	Safty Circuit
volatile  _Bool				Safety_90,Safety_71,Safety_66,Safety_69,Safety_68,PreSafety_66,Short110To24;
volatile 	uint8_t			Safety_Timer[6],SafetySum=0,PreSafetySum=0;

///////// Phase Control 
volatile  _Bool				Ph_R_Logic=0,Ph_S_Logic=0,Ph_T_Logic=0,Ph_PreR=0,Ph_PreS=0,Ph_PreT=0;
volatile  uint16_t		PhaseTimerR,PhaseTimerS,PhaseTimerT,Ph_RtoS_Timer,Ph_StoT_Timer,Ph_TtoR_Timer;
volatile	uint16_t		Ph_RtoS_Time,Ph_StoT_Time,Ph_TtoR_Time;
volatile	int16_t			PT_R_ADC=0,PT_S_ADC=0,PT_T_ADC=0,PT_RMS_Counter=0;
volatile	uint64_t		PT_R_RMS_Sum=0,PT_S_RMS_Sum=0,PT_T_RMS_Sum=0;
volatile  float   		Ph_R_RMS=0,Ph_S_RMS=0,Ph_T_RMS=0;

///////// Timer
volatile  uint8_t   	Pr=0,Task950usTimer=0,Ms5=0,Ms50=0,Task5MsTimer=0,Task10MsTimer=0,Task20MsTimer=0;
volatile	uint8_t			SaveDayCounterTimer=0,ShowSerialResetTimer=0,ResetDriveTimer = 0;
volatile	uint8_t     Deb69_OnTimer=0,Deb69_OffTimer=0,Deb68_OnTimer=0,Deb68_OffTimer=0,Deb1CFTimer=0,DebCF3Timer=0,DoorParkTimer=0;
volatile	uint8_t			DebCA1_OnTimer=0,DebCA1_OffTimer=0,DebCAN_OnTimer=0,DebCAN_OffTimer=0,DebFireTimer=0,DebOVLTimer=0;
volatile	uint8_t			WaitingFloorTimer=0,URA_Set_Time=0,PB_BlinkTimer=0,HYD_RelevelTimer=0,CF3DelayTimer=0,T501cf=0,Timeout_1CF=0;
volatile	uint8_t			ReplayExtraCapacityTimer=0,GoToParkFloorTimer=0,gtpt60=0,StandbyTimer=0,ToStartEmergencyTimer=0;
volatile	uint8_t			URA_TurnOffTimer=0,TurnOnBoardTimer=0,ShowErrorTimer=0,FLTDRVCheckTimer=0,ShortCheckTimer=0;
volatile  uint16_t  	Task100MsTimer=0,Task500MsTimer=0,Task1SecTimer=0,Task1MinuteTimer=0,ResetPlayDisturbDoorTimer=0,BackLightOffTimer=0,DoorOpenTimer=0;
volatile	uint16_t		ReInitLCDTimer=0,DayCounterTemporaryTimer=0,BuzzerTimer=0;
volatile	uint16_t		DisableERSENR_Timer=0,Hall_RxTimer=0;
volatile	int16_t			DriveRestTime=0,RLS_Timer=0,DRC_Timer=0,OverlapTimer=0,TimeToOpenDoor=1000,ToClose66Timer=0,DoorSetCloseTimer=0;
volatile	int16_t			LevelingTimer=0,StopTimer=0,MoveTimer=0;
float									Drive_Timing=0.0,Door_Timing=0.0,RelayOut_Timer=0.0;

/////////	Encoder
volatile	int32_t			Encoder=0,EncoderINT=0;

///////// Day Counter
volatile 	uint8_t			ErrorList[ERROR_LIST_SIZE],LastOKInErrorList=0,ErrorListCounter=0;

/////////	Error
volatile	_Bool				Error_RLS=0,Error_RLSCut=0,Error_DRC=0,Error_DRCCut=0,Error_RLS_RelevelF=0,Error_FTO=0,Error_TravelTimeout=0;
volatile	_Bool				Error_DoorCloseTimeout=0,Error_URA=0,MainErrorF=0,Error_CutSerial=0,Error_DayCounter=0;
volatile	_Bool				Error_Safety90=0,Error_SafetyCircuitCut=0,Error_Cut66=0,Error_Cut68=0,Error_Cut69=0,Error_LevelingTimeout=0;
volatile	_Bool				Error_FLT_UNB=0,Error_PHR=0,Error_PHL=0,Error_OverCurrent=0,Error_LowCurrent=0,Error_CA1CAN=0,Error_1CF=0,Error_CF3=0;
volatile	_Bool				Error_EndDoorTime=0,Error_FLT_DRV=0,Error_Cut_1CF_CF3=0,Error_RLS_S=0,Error_RLSCut_S;
volatile 	uint8_t			NumOfErrors=0,PreNumOfErrors=0,ShowError=0,PreShowError=0;
volatile	uint8_t			ErrorList[ERROR_LIST_SIZE],Clear68CutError=0,InfoSelector=0;
char 									ErrorAlarmStr[20];

/////////	Alarm
volatile	_Bool				Alarm_MRVCRV=0,Alarm_Fire=0,Alarm_Overload=0,Alarm_GoToPark=0,Alarm_PHC=0,Alarm_InputSTP=0;
volatile	_Bool				Alarm_LowVoltge24=0,Alarm_DOActive=0,Alarm_DCActive=0,Alarm_MRV=0,Alarm_CRV=0,Alarm_ShortCircuit=0;
volatile 	uint8_t			NumOfAlarms=0,PreNumOfAlarms=0;


/////////	Program
volatile	_Bool				InCA1=1,InCAN=1,In1CF=0,InCF3=0,PreCA1=0,InFire=0,Debounced_69=0,Debounced_68=0;
volatile	_Bool				RunOrStop=STOP,PreRunOrStop=STOP,StandbyF=0,LandingF=0,PreLandingF=0,LedFTO=0,BuzzerState=0;
volatile	_Bool				CMD_Door=0,PreCMD_Door=0,DCMD=0,DontOpenDoor=0,LowestFloorFlag=1,HighestFloorFlag=1;
volatile	_Bool				CMD_Dir=0,PreDir=0,OverlapF=0,ForceDoorOpen=0,DoorParkF=0,CloseComplete=0,EmergencyModeF=0,HYD_Relevel_AfterRev=0;
volatile	_Bool				Pre1CF=0,PreR1CF=0,PreCF3=0,ForceLanding=0,RelevelingF=0,TravelTimerEnable=0,HYD_RelevelTimerEnable=0,DoorParkTimerEnable=0;
volatile	_Bool				WFT_Enable=0,ClosingDoorF=0,GoToAim=0,InLimitF=0,StartCF3Delay=0,Timeout1CF_Enable=0,GoToParkFloorTimerEnable=0;
volatile	_Bool				StandbyTimerEnable=0,SetAllFloorForFTO=0,MotorFan=0,PassCorrect=0,OutUPSRelay=0,OutEmergencyDriveRef=0;
volatile	_Bool				InRevF=0,CF3Delay_Enable=0,DoNotOpenDoorAfterRelevel=0,AllowToRelevel=0,OutRelayOneTimeF=0;
volatile	_Bool				PHC_temporary=0;
volatile	uint8_t			TypeElevator=0,CMD_MP3File=0,CMD_Speed=0,PreSpeed=0,OpeningDoorState=0,ClosingDoorState=0,IncompleteCloseCounter=0;
volatile	uint8_t			AimFloor=NO_AIM,PreAimFloor=NO_AIM,Mode=0,PreMode=0,Calibration=0,TravelTimer=0,TravelShowTime=0;//Open door announce floor number
volatile	uint8_t			AimCF3=NO_AIM,Aim1CF=NO_AIM,CalAimCF3=NO_AIM,CalAim1CF=NO_AIM,Delay1CF=0,CF3ErrorChecker=0,Move=MOVE_RESET;
volatile	uint8_t			PhaseLostChecker=0,PhaseSameChecker=0,PhaseReverseChecker=0,Activity=0,TurnOffTCTimer=0,NumOfMainPB=0,NumOfCarPB=0;
volatile	int8_t			E_Level=0,Counter1CF=0,MaxCounter1CF=0;
volatile	int16_t			FloorDif=0,Checker=0,CounterCF3=0;
volatile 	uint16_t		DOOR_OPEN_TIME_REF=600,ODAFN=0;
volatile	uint32_t		HwPass[3],U_ID[3];
volatile	float 			CMD_FOut=0;




struct	FloorProperty{
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




void	Set_FirstValueInArrays(void)
{
	uint16_t i=0;
	for(i=0;i<30;i++)
	{
		SlaveSendBuffer[i] = 0;
	}
	SlaveSendBuffer[1] = 1;
	
	for(i=0;i<MAX_HW_INPUTS;i++)
	{
		HwIn[i] = 0;
	}
	for(i=0;i<MAX_HW_OUTPUTS;i++)
	{
		HwOut[i] = 0;
	}
	
	for(i=0;i<MAX_HW_OUTPUTS;i++)
	{
		PB.Hall[i] = 0;
		PB.Hall_Cancel[i] = 0;
		PB.FirstCabin[i] = 0;
		PB.Time[0];
		PB.ResetPBTimer[i] = 0;
		if(i<6) PB.HallBlinker[i] = 0;
		if(i<3) PB.CabinBlinker[i] = 0;
	}
	
}


void InitializeVariables(void)
{
	Set_FirstValueInArrays();
	
	Task950usTimer = 0;
	Task5MsTimer = 0;
	Task10MsTimer = 0;
	Task20MsTimer = 0;
	Task100MsTimer = 0;
	Task500MsTimer = 0;
	Task1SecTimer = 0;
	Task1MinuteTimer = 0;
  //CAB_RXDataCNT = CAB_RX_DMA_CNT;
	//HALL_RXDataCNT = HALL_RX_DMA_CNT;
	Counter1CF = 0;
	CounterCF3 = 0;
	
	//Pre1CF = HwIn_1CF;
	//PreCF3 = HwIn_CF3;
	
	MaxCounter1CF = (Stng[NUM_OF_STOP] - 1) *2;
	DOOR_OPEN_TIME_REF = (uint16_t) Stng[DOOR_OPEN_TIME] * 100;
	TimeToOpenDoor = DOOR_OPEN_TIME_REF;
	RunOrStop = STOP;
	Calibration = 2;
	InLimitF = 0;
	Error_OverCurrent = 0;
	Error_LowCurrent = 0;
	TurnOnBoardTimer=0;
	K_Dn = 0;
	K_Up = 0;
	K_Ent = 0;
	K_Ext = 0;
	ExitFromMenuTimer = 0;
	SetAllFloorForFTO = 0;
	PB.ClearAll = 0;
	PB.HallSet 		= NO_AIM;
	PB.HallClear 	= NO_AIM;
	PB.CabinSet  	= NO_AIM;
	PB.CabinClear	= NO_AIM;
	PB.CabinBlinker[0] = NO_AIM;
	PB.CabinBlinker[1] = NO_AIM;
	PB.CabinBlinker[2] = NO_AIM;
	PB.HallBlinker[0]  = NO_AIM;
	PB.HallBlinker[1]  = NO_AIM;
	PB.HallBlinker[2]  = NO_AIM;
	PB.HallBlinker[3]  = NO_AIM;
	PB.HallBlinker[4]  = NO_AIM;
	PB.HallBlinker[5]  = NO_AIM;
	PreCA1 = InCA1;
	
	PreCA1 = InCA1;
	HYD_Relevel_AfterRev = 0;
	
	CMD_FOut = 0;
}






int32_t abs32(int32_t In)
{
	if(In < 0) return In;
	else       return-In;
}



