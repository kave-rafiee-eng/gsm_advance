#include "main.h"
#include "n1202.h"
#include "Hardware.h"
#include "Defines.h"
#include "Serial.h"
#include "Menu.h"
#include "AT24Cxx.h"
#include "EEPROM.h"
#include "PB_Num.h"

void	Set_FirstValueInArrays(void);
void 	InitializeVariables(void);
	
///////// LCD
extern	volatile  uint8_t			_LCD_RAM[LCD_X*LCD_String],xlcd;
extern	volatile  uint8_t   	LCD_Cmd_Counter,LCD_Data_Counter,LCD_InitF,LCD_InitState,LCD_InitTimer;
extern	volatile  uint8_t   	LCD_UpdateF,LCD_Cmd_Row,lcdt,LCD_RefreshTimer;
extern	volatile  uint16_t  	LCD_SendAdd;
extern	char                	LcdStr[100];

///////// Menu UI
extern	volatile  uint8_t   	K_Up,K_Dn,K_Ent,K_Ext,KeyLast[4],EEPROMSaveFlag;
extern	volatile	uint8_t   	Selector[20],FirstToMenu,Selected_address,Xmenu,ExitFromMenuTimer;
extern	volatile  int8_t    	MenuState,MenuBar[10],EnterWithTimerF,ExitWithTimerF;
extern	volatile  uint16_t  	KeyTime[4],KeyHoldTime[2],Ymenu;
extern	_Bool									FirstInDayCounterMenu;

/////////	Slave MCU
extern	volatile	_Bool    		SlaveCLK_Level,SlaveMOSI_Level;
extern	volatile	uint8_t			SlaveSyncTimer,SlaveByteCounter,SlaveBitCounter,SlaveTxData,SlaveRxData;
extern	volatile	uint8_t			SlaveSendBuffer[30],SlaveReceiveBuffer[30],SlaveDataLength;

///////// Serial PR
extern	char                 	PR_TX_Buffer[PR_BUFFER_SIZE];
extern	volatile 	char 				PR_RX_Buffer[PR_BUFFER_SIZE];
extern	volatile	uint16_t		PR_LastRxDMACNT,PR_LastTxDMACNT;
extern	volatile	_Bool				PR_TxModeF,PR_RxModeF;
extern	volatile	uint32_t		PR_TxDMA_CR_Val;

extern	uint8_t								PR_ReceivedByte[3500];
extern	volatile	uint16_t		PR_ByteReciveTimer;
extern	volatile	uint16_t		PR_RXDataCNT,PR_DataLen,PR_ByteCounter,PR_SyncStatus;

///////// Serial Doublex
extern	char                 	DBLX_TX_Buffer[UART_BUFFER_SIZE];
extern	volatile 	char 				DBLX_RX_Buffer[UART_BUFFER_SIZE];
extern	volatile	uint16_t		DBLX_LastRxDMACNT,DBLX_LastTxDMACNT;
extern	volatile	_Bool				DBLX_TxModeF,DBLX_RxModeF;
extern	volatile	uint32_t		DBLX_TxDMA_CR_Val;

///////// Serial Cabin
extern	char                 	CAB_TX_Buffer[UART_BUFFER_SIZE];
extern	volatile 	char 				CAB_RX_Buffer[UART_BUFFER_SIZE];
extern	volatile	uint16_t		CAB_LastRxDMACNT,CAB_LastTxDMACNT;
extern	volatile	_Bool				CAB_TxModeF,CAB_RxModeF;
extern	volatile	uint32_t		CAB_TxDMA_CR_Val;

extern	volatile	_Bool				CAB_NewPacketF,CAB_ClearDataInDataLostF;
extern	volatile	uint8_t			CAB_ReceivedByte[50];
extern	volatile	uint16_t		CAB_RXDataCNT,CAB_PacketCounter,CAB_PacketError,CAB_PacketLen,CAB_SendDataCounter,CAB_GetDataCounter,CAB_SyncStatus;
extern	volatile	uint16_t		CAB_GetDataTimer,HALL_TX_AfterLastByteTimer,TransferHWAddCounter;
extern	volatile  int16_t			CAB_SendDataCount,CAB_GetDataCount,CAB_DataLost;


///////// Serial Hall
extern	char 									HALL_TX_Buffer[UART_BUFFER_SIZE];
extern	volatile	char				HALL_RX_Buffer[UART_BUFFER_SIZE];
extern	volatile	uint16_t		HALL_LastRxDMACNT,HALL_LastTxDMACNT,HALL_RXDataCNT;
extern	volatile	_Bool				HALL_TxModeF,HALL_RxModeF;
extern	volatile	uint32_t		HALL_TxDMA_CR_Val;

extern	volatile	_Bool				HALL_NewPacketF;
extern	volatile	uint8_t			HALL_ReceivedByte[50],HALL_LinkLED_Status;
extern	volatile	uint16_t		HALL_PacketCounter,HALL_PacketError,HALL_PacketLen,HALL_SendDataCounter,HALL_GetDataCounter,HALL_SyncStatus;
extern	volatile	uint16_t		HALL_GetDataTimer,CAB_TX_AfterLastByteTimer;

///////// Serial Drive
extern	char 									DRV_TX_Buffer[UART_BUFFER_SIZE];
extern	volatile	char				DRV_RX_Buffer[UART_BUFFER_SIZE];
extern	volatile	uint16_t		DRV_LastRxDMACNT,DRV_LastTxDMACNT,DRV_RXDataCNT;
extern	volatile	_Bool				DRV_TxModeF,DRV_RxModeF;
extern	volatile	uint32_t		DRV_TxDMA_CR_Val;

extern	volatile	_Bool				DRV_NewPacketF;
extern	volatile	uint8_t			DRV_ReceivedByte[50],DRV_LinkLED_Status;
extern	volatile	uint16_t		DRV_PacketError,DRV_PacketLen,DRV_SendDataCounter,DRV_GetDataCounter,DRV_SyncStatus;
extern	volatile	uint16_t		DRV_GetDataTimer,DRV_TX_AfterLastByteTimer;



/////////	Setting
extern	volatile	uint8_t			Stng[SETTING_ARRAY_SIZE];
extern	uint8_t 							EEPROM_Buffer[EE_BUFFER_SIZE],CAB_TransferHWAdd[100];
extern	_Bool									Stng_DriveParallelV0[10],Stng_DriveParallelV1[10],Stng_DriveParallelV2[10];
extern	uint8_t								Pass_Num[5],DayCounter_Status,DayCounter_Day,DayCounter_Hour,DayCounter_Minutes,DayAccuracy;
extern	volatile	uint32_t		BoardFixedID,BoardUID,DayCounterPassword50Day,DayCounterPassword30Day,DayCounterPasswordPermanent,SavedPermanentPass;																		
extern	volatile	uint32_t 		HwPass[3],TemporaryID;

///////// Hardware
extern	volatile	_Bool    		SetOutputsShiftF,ShiftCLK_Level,ShiftDATA1_Level,ShiftDATA2_Level,ShiftSTCP_Level;
extern	volatile	uint8_t  		OutputsShiftStatus,FTO_Adc,CT1_RMS,CT2_RMS,FTO_MotorTemp;
extern	volatile	_Bool				Hw_OUTPN,Hw_BUZZ,Hw_PO1,Hw_PO2,Hw_PO3,Hw_PO4;
extern	volatile	_Bool				Hw_OC1,Hw_OC2,Hw_OC3,Hw_OC4,Hw_OC5,Hw_OC6,Hw_OC7,Hw_RLY_7191;
extern	volatile	int16_t			adcv[10];
extern	volatile	int					VRefIntSum,TestADCTimer,TestADCTime;
extern	volatile  float   		BatteryVoltage,PowerVoltageMainBoard,PowerVoltageCabin,ADCToVoltageFactor;

///Inputs
extern	_Bool									HwIn[MAX_HW_INPUTS];
extern	struct	MainInputList{
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
}MIn;


///Outputs
extern	_Bool		HwOut[MAX_HW_OUTPUTS];
extern	struct	MainOutputList{
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
}MOut;

///Push Buttons
extern	_Bool									In_PB_Par[12],Out_PB_Par[12];
extern	struct	PushButton{
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

///Numrator						
extern	_Bool									Numrator_Par[MAX_PARALLEL_NUMRATOR],SegUp,SegDn;
extern	volatile 	uint8_t			NumR,NumL;

extern	struct SerialExtentionInputList{
	
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
extern	struct CarcodecInputList{
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
}CIn;

//Car	Out
extern	volatile	_Bool				CMD_URA;//,COut_FAN,COut_OVL,COut_GANG,COut_AVA;
extern	struct CarcodecOutputList{
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
}COut;

/////////	Safty Circuit
extern	volatile  _Bool				Safety_90,Safety_71,Safety_66,Safety_69,Safety_68,PreSafety_66,Short110To24;
extern	volatile 	uint8_t			Safety_Timer[6],SafetySum,PreSafetySum;

///////// Phase Control 
extern	volatile  _Bool				Ph_R_Logic,Ph_S_Logic,Ph_T_Logic,Ph_PreR,Ph_PreS,Ph_PreT;
extern	volatile  uint16_t		PhaseTimerR,PhaseTimerS,PhaseTimerT,Ph_RtoS_Timer,Ph_StoT_Timer,Ph_TtoR_Timer;
extern	volatile	uint16_t		Ph_RtoS_Time,Ph_StoT_Time,Ph_TtoR_Time;
extern	volatile	int16_t			PT_R_ADC,PT_S_ADC,PT_T_ADC,PT_RMS_Counter;
extern	volatile	uint64_t		PT_R_RMS_Sum,PT_S_RMS_Sum,PT_T_RMS_Sum;
extern	volatile  float   		Ph_R_RMS,Ph_S_RMS,Ph_T_RMS;

///////// Timer
extern	volatile  uint8_t   	Pr,Task950usTimer,Ms5,Ms50,Task5MsTimer,Task10MsTimer,Task20MsTimer;
extern	volatile	uint8_t			SaveDayCounterTimer,ShowSerialResetTimer,ResetDriveTimer;
extern	volatile	uint8_t     Deb69_OnTimer,Deb69_OffTimer,Deb68_OnTimer,Deb68_OffTimer,Deb1CFTimer,DebCF3Timer,DoorParkTimer;
extern	volatile	uint8_t			DebCA1_OnTimer,DebCA1_OffTimer,DebCAN_OnTimer,DebCAN_OffTimer,DebFireTimer,DebOVLTimer;
extern	volatile	uint8_t			WaitingFloorTimer,URA_Set_Time,PB_BlinkTimer,HYD_RelevelTimer,CF3DelayTimer,T501cf,Timeout_1CF;
extern	volatile	uint8_t			ReplayExtraCapacityTimer,GoToParkFloorTimer,gtpt60,StandbyTimer,ToStartEmergencyTimer;
extern	volatile	uint8_t			URA_TurnOffTimer,TurnOnBoardTimer,ShowErrorTimer,FLTDRVCheckTimer,ShortCheckTimer;
extern	volatile  uint16_t  	Task100MsTimer,Task500MsTimer,Task1SecTimer,Task1MinuteTimer,ResetPlayDisturbDoorTimer,BackLightOffTimer,DoorOpenTimer;
extern	volatile	uint16_t		ReInitLCDTimer,DayCounterTemporaryTimer,BuzzerTimer;
extern	volatile	uint16_t		DisableERSENR_Timer,Hall_RxTimer;
extern	volatile	int16_t			DriveRestTime,RLS_Timer,DRC_Timer,OverlapTimer,TimeToOpenDoor,ToClose66Timer,DoorSetCloseTimer;
extern	volatile	int16_t			LevelingTimer,StopTimer,MoveTimer;
extern	float									Drive_Timing,Door_Timing,RelayOut_Timer;

/////////	Encoder
extern	volatile	int32_t			Encoder,EncoderINT;

///////// Day Counter
extern	volatile 	uint8_t			ErrorList[ERROR_LIST_SIZE],LastOKInErrorList,ErrorListCounter;

/////////	Error
extern	volatile	_Bool				Error_RLS,Error_RLSCut,Error_DRC,Error_DRCCut,Error_RLS_RelevelF,Error_FTO,Error_TravelTimeout;
extern	volatile	_Bool				Error_DoorCloseTimeout,Error_URA,MainErrorF,Error_CutSerial,Error_DayCounter;
extern	volatile	_Bool				Error_Safety90,Error_SafetyCircuitCut,Error_Cut66,Error_Cut68,Error_Cut69,Error_LevelingTimeout;
extern	volatile	_Bool				Error_FLT_UNB,Error_PHR,Error_PHL,Error_OverCurrent,Error_LowCurrent,Error_CA1CAN,Error_1CF,Error_CF3;
extern	volatile	_Bool				Error_EndDoorTime,Error_FLT_DRV,Error_Cut_1CF_CF3,Error_RLS_S,Error_RLSCut_S;
extern	volatile 	uint8_t			NumOfErrors,PreNumOfErrors,ShowError,PreShowError;
extern	volatile	uint8_t			ErrorList[ERROR_LIST_SIZE],Clear68CutError,InfoSelector;
extern	char 									ErrorAlarmStr[20];

/////////	Alarm
extern	volatile	_Bool				Alarm_MRVCRV,Alarm_Fire,Alarm_Overload,Alarm_GoToPark,Alarm_PHC,Alarm_InputSTP;
extern	volatile	_Bool				Alarm_LowVoltge24,Alarm_DOActive,Alarm_DCActive,Alarm_MRV,Alarm_CRV,Alarm_ShortCircuit;
extern	volatile 	uint8_t			NumOfAlarms,PreNumOfAlarms;


/////////	Program
extern	volatile	_Bool				InCA1,InCAN,In1CF,InCF3,PreCA1,InFire,Debounced_69,Debounced_68;
extern	volatile	_Bool				RunOrStop,PreRunOrStop,StandbyF,LandingF,PreLandingF,LedFTO,BuzzerState;
extern	volatile	_Bool				CMD_Door,PreCMD_Door,DCMD,DontOpenDoor,LowestFloorFlag,HighestFloorFlag;
extern	volatile	_Bool				CMD_Dir,PreDir,OverlapF,ForceDoorOpen,DoorParkF,CloseComplete,EmergencyModeF,HYD_Relevel_AfterRev;
extern	volatile	_Bool				Pre1CF,PreR1CF,PreCF3,ForceLanding,RelevelingF,TravelTimerEnable,HYD_RelevelTimerEnable,DoorParkTimerEnable;
extern	volatile	_Bool				WFT_Enable,ClosingDoorF,GoToAim,InLimitF,StartCF3Delay,Timeout1CF_Enable,GoToParkFloorTimerEnable;
extern	volatile	_Bool				StandbyTimerEnable,SetAllFloorForFTO,MotorFan,PassCorrect,OutUPSRelay,OutEmergencyDriveRef;
extern	volatile	_Bool				InRevF,CF3Delay_Enable,DoNotOpenDoorAfterRelevel,AllowToRelevel,OutRelayOneTimeF;
extern	volatile	_Bool				PHC_temporary;
extern	volatile	uint8_t			TypeElevator,CMD_MP3File,CMD_Speed,PreSpeed,OpeningDoorState,ClosingDoorState,IncompleteCloseCounter;
extern	volatile	uint8_t			AimFloor,PreAimFloor,Mode,PreMode,Calibration,TravelTimer,TravelShowTime;//Open door announce floor number
extern	volatile	uint8_t			AimCF3,Aim1CF,CalAimCF3,CalAim1CF,Delay1CF,CF3ErrorChecker,Move;
extern	volatile	uint8_t			PhaseLostChecker,PhaseSameChecker,PhaseReverseChecker,Activity,TurnOffTCTimer,NumOfMainPB,NumOfCarPB;
extern	volatile	int8_t			E_Level,Counter1CF,MaxCounter1CF;
extern	volatile	int16_t			FloorDif,Checker,CounterCF3;
extern	volatile 	uint16_t		DOOR_OPEN_TIME_REF,ODAFN;
extern	volatile	uint32_t		HwPass[3],U_ID[3];
extern	volatile	float 			CMD_FOut;




extern	struct	FloorProperty{
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



int32_t abs32(int32_t In);



