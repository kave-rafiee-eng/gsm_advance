#include "main.h"
#include "Menu.h"
#include "n1202.h"
#include "Hardware.h"
#include "stdio.h"
#include "Defines.h"
#include "EEPROM.h"
#include "Variable.h"

char		MenuStr[16];


void Keys4Menu(void)
{
	if(KEY_UP  && !KeyLast[0])    { KeyLast[0]=1;  K_Up=1; ExitFromMenuTimer=0;}
	else if(KEY_UP && KeyLast[0]) 
	{
		KeyTime[0]++;
		if(K_Up==0)
		{
			 if( (KeyHoldTime[0]>10 && KeyTime[0]>8) ||
					 (KeyHoldTime[0]> 3 && KeyTime[0]>20) ||
					 (KeyTime[0]>50) )
					 {K_Up=1; KeyTime[0]=0; KeyHoldTime[0]++;}
		}
	}
	else if(!KEY_UP)    {KeyLast[0]=0;  KeyTime[0]=0; KeyHoldTime[0]=0;}   
	
	
	if(KEY_DN  && !KeyLast[2])    { KeyLast[2]=1;  K_Dn=1; ExitFromMenuTimer=0;}
	else if(KEY_DN && KeyLast[2])
	{
		KeyTime[2]++;
		if(K_Dn==0)
		{
			 if( (KeyHoldTime[1]>10 && KeyTime[2]>8) ||
					 (KeyHoldTime[1]> 3 && KeyTime[2]>20) ||
					 (KeyTime[2]>50) )
					 {K_Dn=1; KeyTime[2]=0; KeyHoldTime[1]++;}
		}
	}
	else if(!KEY_DN)   { KeyLast[2]=0;  KeyTime[2]=0; KeyHoldTime[1]=0;}

	
  if(KEY_ENT && !KeyLast[1])    { KeyLast[1]=1;  K_Ent=1; ExitFromMenuTimer=0;}
	else if(KEY_ENT && KeyLast[1]){ KeyTime[1]++;}
	else if(!KEY_ENT)             { KeyLast[1]=0;  KeyTime[1]=0;}
	
	
	if(KEY_EXT && !KeyLast[3])    { KeyLast[3]=1;  K_Ext=1; ExitFromMenuTimer=0;}
	else if(KEY_EXT && KeyLast[3]){ KeyTime[3]++;}
	else if(!KEY_EXT)             { KeyLast[3]=0;  KeyTime[3]=0;}
	
	if(KEY_ENT || KEY_DN || KEY_EXT || KEY_UP) {BackLightOffTimer = 0;}
}





////////////////////////////////////////////////// Const 
uint8_t HEIGHT_CHAR[]={0,17,34,51};

char List_MainMenu[][15]=
{
	"General",			//0
	"Advance",			//1
	"Information"  	//2
};
char List_GeneralMenu[][15]= 
{
	"Num Of Stop",		//0
	"Service Type",   //1
	"Num And Talk", 	//2
	"Travel Time",   	//3
	"Door", 					//4
	"Sound",      		//5
	"Floor",					//6
	"Motor Safe",			//7
	"HYD Setting",		//8
	"Landing",				//9
	"Calibration",    //10
	"Emergency",   		//11
	"1CF Error",    	//12
};

char List_AdvanceMenu[][15]= 
{
	"Type Elevator",	//0
	"Hardware",				//1
	"Timer Setting",	//2
	"Phase Control", 	//3
	"Doublex"					//4
};

char List_InformationMenu[][14]= 
{
	"Day Counter",	//0
	"Starting Log",	//1
	"Program Date"	//2
};

char List_ServiceType[][17]= 
{
	"Collective DN", 	//0
	"Full Collective",//1
	"Push Button",   	//2
	"Collective U/D", 			//3
};

char List_DoorSetting[][13]= 
{
	"Door Time", 			//0
	"Control Type",		//1
	"Second Door",   	//2
	//"5KT Type"				//3
};

char List_DoorTime[][17]= 
{
	"Open Delay", 		//0
	"Close Delay",		//1
	"End DoorTime", 	//2
	"Close Timeout",	//3
	"Door Park",			//4
	"69 Debouncer",		//5
	"68 Debouncer",		//6
	"DoorOpenTime"		//7
};

char List_DoorControlType[][8]= 
{
	"Loaded",	//0
	"68",			//1
	"69"    	//2
};

char List_NormalOpenClose[][4]= 
{
	"NO",	//0
	"NC" 	//1
};

char List_DisableEnable[][9]= 
{
	"Disable",	//0
	"Enable" 		//1
};

char List_DoorSecond[][13]= 
{
	"Num Of Door",	//0
	"Door Select" 	//1
};

char List_SoundSetting[][15]= 
{
	"Music Volume", 	//0
	"Talk Volume",		//1
	"WelcomeFloor", 	//2
	"Gang Select"			//3
};

char List_FloorSetting[][15]= 
{
	"Park Floor", 	//0
	"Time To Park",	//1
	"Fire Floor", 	//2
	"Waiting Floor"	//3
};

char List_MotorSafe[][15]= 
{
	"Enable", 			//0
	"Fast OverC",		//1
	"Slow OverC", 	//2
	"Time OverC",		//3
	"Min Current"		//4
};

char List_HYDSetting[][15]= 
{
	"StartSlow Del", 	//0
	"StartFast Del",	//1
	"Star To Delta", 	//2
	"MotorStartDel",	//3
	"MotorStopDel" 		//4
};

char List_Landing[][16]= 
{
	"Based On",				//0
	"Jump1 Speed",		//1
	"CF3 Flag",				//2
	"CF3 Delay",			//3
	"Motor Speed",		//4
	"Acceleration",		//5
	"Decceleration",	//6
	"Level Distance",	//7
	"Auto Learning"		//8
};

char List_LandingBased[][12]= 
{
	"CF3 Sensor",			//0
	"Timing",					//1
	"Encoder",				//2
};

char List_Calibration[][11]= 
{
	"Direction",		//0
	"Speed" 				//1
};

char List_SelectDirection[][6]= 
{
	"DOWN",		//0
	"UP"			//1
};

char List_SelectSpeed[][6]= 
{
	"Slow",		//0
	"Rev",		//1
	"Med",		//2
	"Fast" 		//3
};

char List_Speed[][6]=
{
	"Stop",		//0
	"Slow",		//1
	"Rev",		//2
	"Med",		//3
	"Fast" 		//4
};

char List_Emergency[][11]= 
{
	"Enable",			//0
	"Direction" 	//1
};


char List_1CFError[][9]= 
{
	"Enable",		//0
	"Timeout" 	//1
};


char List_TimerSetting[][17]= 
{
	"UD ReleaseDel",	//0
	"TC ReleaseDel",	//1
	"1CF Delay", 			//2
	"Leveling Time",	//3
	"Standby Time",		//4
	"URA Delay",			//5
	"Relay Time" 			//6
};

char List_1CF_Delay[][13]= 
{
	"All Floors",			//0
	"Up/Down Del" 		//1
};

char List_RelayTime[][13]= 
{
	"ON Delay",				//0
	"ON Time",				//1
};

char List_PhaseControl[][15]= 
{
	"Phase Reverse", 	//0
	"Phase Fault",		//1
	"One Phase"   		//2
};

char List_TypeElevator[][7]= 
{
	"3VF", 		//0
	"HYD",		//1
	"2Speed",	//2
	"HEVOS"		//3
};

char List_Hardware[][12]= 
{
	"Main Board",
	"Carcodec"
};

char List_HW_MainBorad[][14]= 
{
	"Inputs",
	"Outputs",
	"Inputs Type",
	"Outputs Type",
	"Push Buttons",
	"Numrator",
	"Drive"
};

char List_HW_Main_PB_Type[][16]= 
{
	"Parallel",
	"Serial Ext",
	"Ravis Serial PB",
	//"DoublexMaster"
};

char List_HW_Main_Num_Type[][16]= 
{
	"Parallel",
	"Ravis Serial PB"
};

char List_HW_Car_PB_Type[][15]= 
{
	"Parallel",
	"Serial Ext",
	"Ravis Panel"
};

char List_HW_Car_Num_Type[][15]= 
{
	"Parallel",
	"Ravis Panel"
};

char List_HW_Carcodec[][14]= 
{
	"Inputs",
	"Outputs",
	"Inputs Type",
	"Outputs Type",
	"Push Button",
	"Numrator"
};

char List_HW_PB[][18]= 
{
	"Type",
	"ParallelSetting",
	"Canceller"
};

char List_HW_Num[][18]= 
{
	"Type",
	"ParallelSetting",
	"Sleep Light",
	"Active Light"
};

char List_Drive[][18]= 
{
	"Connection",
	"ParallelSetting"
};

char List_DriveConnection[][18]=
{
	"Parallel",
	"RS485-DCP3",
	"RS485-QMA"
};

const char List_SegmentChoices[]={
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
		'B',
		'r',
		'a',
		'b',
		'c',
		'd',
		'e',
		'f',
		'g',
		'h',
		'-'
};

static const char List_TalkChoices[][6]={
	"???",
	"L1 ","L2 ","L3 ","L4 ","L5 ","L6 ","L7 ","L8 ","L9 ","L10",
	"L11","L12","L13","L14","L15","L16","L17","L18","L19","L20",
	"L21","L22","L23","L24",
	"Grn",
	"Labi",
	"Roof",
	"Pool",
	"Par ",
	"Par1",
	"Par2",
	"B  ",
	"B1 ",
	"B2 ",
	"L25","L26","L27","L28","L29","L30",
	"L31","L32","L33","L34","L35","L36","L37","L38","L39","L40",
	"L41","L42","L43","L44","L45","L46","L47","L48","L49","L50",
	"Tr1",
	"Tr2",
	"Tr3",
	"Tr4",
	"Tr5",
	"Tr6",
	"Tr7",
	"Tr8",
	"Tr9",
	"Tr10",
	"Tr11",
	"Tr12",
	"Tr13",
	"Tr14",
	"Tr15",
	"Tr16",
	"Tr17",
	"Tr18",
	"Tr19",
	"Tr20",
};

static const char List_ProgMainInputs[][6]={
	"CF3",
	"1CF",
	"CAN",
	"CA1",
	"IFN",
	"IF1",
	"FLTDR",
	"RLS",
	"DRC",
	"FIRE",
	"OVL",
	"FUL",
	"RELUP",
	"RELDN",
	"ZADO",
	"Ready",
	"RES2",
	"RES3",
	"RES4",
	"RES5"
};
static const char List_HwMainInputs[][4]={
	"---",
	"CF3",
	"1CF",
	"CAN",
	"CA1",
	"IN1",
	"IN2",
	"IN3",
	"IN4",
	"IN5",
	"IN6",
	"H1",
	"H2",
	"H3",
	"H4",
	"H5",
	"H6",
	"H7",
	"H8",
	"H9",
	"H10",
	"H11",
	"H12"
};


static const char List_ProgMainOutputs[][8]={ //25
	"MUP",
	"MDN",
	"MV0",
	"MV1",
	"MV2",
	"TC",
	"ST.DL",
	"URA",
	"FAN",
	"ENOUT",
	"RELAY",
	"UPSOUT",
	"UPSREF",
	"ADOOUT",
	"1CFOUT",
	"DRV.RST",
	"ERS",
	"ENR",
	"RES4",
	"RES5",
	"CLOSE1",
	"CLOSE2",
	"CLOSE3",
	"CLOSE4",
	"CLOSE5"
};
static const char List_HwMainOutputs[][4]={ //36
	"---",
	"O+-",
	"PO1",
	"PO2",
	"PO3",
	"PO4",
	"OC1",
	"OC2",
	"OC3",
	"OC4",
	"OC5",
	"OC6",
	"OC7",
	"S1",
	"S2",
	"S3",
	"S4",
	"S5",
	"S6",
	"S7",
	"S8",
	"S9",
	"S10",
	"S11",
	"H1",
	"H2",
	"H3",
	"H4",
	"H5",
	"H6",
	"H7",
	"H8",
	"H9",
	"H10",
	"H11",
	"H12"
};

static const char List_ProgCarInputs[][6]={ //15
	"OVL",
	"FUL",
	"FAN",
	"PHC1",
	"PHC2",
	"PHC3",
	"DO1",
	"DO2",
	"DO3",
	"DC",
	"RES1",
	"RES2",
	"RES3",
	"RES4",
	"RES5",
};

static const char List_HwCarInputs[][4]={ //20
	"---",
	"IN1",
	"IN2",
	"IN3",
	"IN4",
	"IN5",
	"IN6",
	"IN7",
	"H1",
	"H2",
	"H3",
	"H4",
	"H5",
	"H6",
	"H7",
	"H8",
	"H9",
	"H10",
	"H11",
	"H12"
};


static const char List_ProgCarOutputs[][7]={ //15
	"CLOSE1",
	"CLOSE2",
	"CLOSE3",
	"OPEN1",
	"OPEN2",
	"OPEN3",
	"FAN",
	"OVLO",
	"GANG",
	"AVA",
	"RES1",
	"RES2",
	"RES3",
	"RES4",
	"RES5",
};
static const char List_HwCarOutputs[][4]={ //30
	"---",
	"CM1",
	"CM2",
	"PO1",
	"PO2",
	"PO3",
	"PO4",
	"S1",
	"S2",
	"S3",
	"S4",
	"S5",
	"S6",
	"S7",
	"S8",
	"S9",
	"S10",
	"S11",
	"H1",
	"H2",
	"H3",
	"H4",
	"H5",
	"H6",
	"H7",
	"H8",
	"H9",
	"H10",
	"H11",
	"H12"
};

static const char List_HwType[][4]={ //36
	"NO",
	"NC"
};

static const char List_HwPBInputs[][4]={
	"---",
	"H1",
	"H2",
	"H3",
	"H4",
	"H5",
	"H6",
	"H7",
	"H8",
	"H9",
	"H10",
	"H11",
	"H12"
};

static const char List_SegmentsOutputs[][4]={
	"RA",
	"RB",
	"RC",
	"RD",
	"RE",
	"RF",
	"RG",
	"UP",
	"DN",
	"LA",
	"LB",
	"LC",
	"LD",
	"LE",
	"LF",
	"LG"
};

void SetCarcodecTransferHWAddressArray(void)
{
	uint8_t i=0;
	_Bool		TypeIn[16],TypeOut[16];
	///Inputs & Outputs Address
	for(i=0;i<15;i++) // 15 is size of the program outputs & program inputs arrays
	{
		CAB_TransferHWAdd[i]			= Stng[CAR_IN_HWADD + i + 1];
		CAB_TransferHWAdd[i + 15]	= Stng[CAR_OUT_HWADD + i + 1];
		TypeIn[i] 	= (Stng[CAR_IN_TYPE_ADD + i + 1] && 1);
		TypeOut[i] 	= (Stng[CAR_OUT_TYPE_ADD + i + 1] && 1);
	}
	
	///Type of Inputs & Outputs
	CAB_TransferHWAdd[30] = 0;
	CAB_TransferHWAdd[31] = 0;
	CAB_TransferHWAdd[32] = 0;
	CAB_TransferHWAdd[33] = 0;
	for(i=0;i<8;i++)
	{
		CAB_TransferHWAdd[30] |= ((uint8_t) TypeIn[i] 	<< i);
		CAB_TransferHWAdd[31] |= ((uint8_t) TypeIn[i+8]	<< i);
		CAB_TransferHWAdd[32] |= ((uint8_t) TypeOut[i] 	<< i);
		CAB_TransferHWAdd[33] |= ((uint8_t) TypeOut[i+8]	<< i);
	}
	
	///PB I/O Address
	for(i=0;i<12;i++) // 12 is number of PB
	{
		CAB_TransferHWAdd[i + 34]	= Stng[CAR_PB_ADD + i + 1];
	}
	
	///Segments Outputs Address
	for(i=0;i<16;i++) // 16 is number of segments outputs
	{
		CAB_TransferHWAdd[i + 46]	= Stng[CAR_NUM_ADD + i + 1];
	}
	
	CAB_TransferHWAdd[62]	=	Stng[CAR_PB_TYPE];
	CAB_TransferHWAdd[63]	=	Stng[CAR_NUM_TYPE];
	CAB_TransferHWAdd[64]	= Stng[CAR_NUM_SLEEP_LIGHT];
	CAB_TransferHWAdd[65]	=	Stng[CAR_NUM_ACTIVE_LIGHT];
	CAB_TransferHWAdd[66]	=	NumOfCarPB;
}



void SelChg(uint8_t Max,uint8_t Dir)
{
   if(KeyTime[0] > 2 || KeyTime[2] > 2)
   {
      if(KEY_UP && KEY_DN)
      {
				Selector[MenuState] = 0;
				K_Up = 0;
				K_Dn = 0;
				KeyTime[0] = 0;
				KeyTime[2] = 0;
				KeyHoldTime[0] = 0;
				KeyHoldTime[1] = 0;
      }
      else if(Dir)
      {
				if(K_Dn) 
				{
					K_Dn=0;
					if(Selector[MenuState] < Max) Selector[MenuState]++;
					else Selector[MenuState]=Max;
				}
				else if(K_Up) 
				{
					K_Up=0;
					if(Selector[MenuState] > 0)   Selector[MenuState]--;      
				}
      }
      else 
      {
				if(K_Dn) 
				{
					K_Dn=0;
					if(Selector[MenuState] > 0)   Selector[MenuState]--;
				}
				else if(K_Up) 
				{
					K_Up=0;         
					if(Selector[MenuState] < Max) Selector[MenuState]++;
					else Selector[MenuState]=Max;
				}
      }
   }
}

void BarChange(uint8_t ms,uint8_t max)
{
   if( ((int8_t)Selector[ms]-MenuBar[ms] ) > max ) MenuBar[ms]++;
   if( ((int8_t)Selector[ms]-MenuBar[ms] ) < 0)    MenuBar[ms]--;
}



void MakeDayCounterTemporaryID(void)
{
	uint32_t	TempPartID = DayCounterTemporaryTimer;
	
	while(TempPartID < 1000)
	{
		TempPartID *= 10;
		if(TempPartID == 0)
		{
			TempPartID = 1234;
		}
	}
	
	uint32_t	FixedPartID = BoardFixedID % 10000;
	if(FixedPartID == 0)
	{
		FixedPartID = 4321;
	}
	else
	{
		while(FixedPartID < 1000)
		{
			FixedPartID *= 10;
		}
		while(FixedPartID > 9999)
		{
			FixedPartID /= 10;
		}
	}
	
	TemporaryID = (FixedPartID * 10000) + (TempPartID);
}

const uint8_t S_Box[10]={8,2,3,5,7,0,9,6,4,1};

void CalculatePassword(void)
{
	DayCounterPassword50Day = 0;
	DayCounterPassword30Day = 0;
	DayCounterPasswordPermanent = 0;
	
	uint8_t Digits[8] = {0,0,0,0,0,0,0,0},DigC=0;
	uint32_t uid = TemporaryID;

	while(uid > 9) 
	{
		Digits[DigC] = uid % 10;
		uid /= 10;
		DigC ++;
	}
	Digits[DigC] = uid;
	
	uid = 0;
	for(uint8_t iw=7;iw<10;iw--)
	{
		uid += S_Box[Digits[iw]];
		if(iw > 0) uid *= 10;
	}
	
	DayCounterPassword50Day = (uid ^ 0x4143CB0) % 10000;
	DayCounterPassword30Day = (uid ^ 0x1831952) % 10000;
	DayCounterPasswordPermanent  = (uid ^ 0x2DCEBB3) % 10000;
}

void Menu(void)
{
	EnterWithTimerF = (MENU_COND_INFORMATION && Selector[MenuState] == 0);
	if(EnterWithTimerF) {Pass_Num[0] = 0; Pass_Num[1] = 0; Pass_Num[2] = 0; Pass_Num[3] = 0;}
	
	if(MenuState > -3) {MENU_ENT}
	
	if(MenuState > 0)
	{
		uint8_t i=0;
		////////////////////////////////// Wording Menu
		if(	MENU_COND_MAINMENU 						||
				MENU_COND_GENERAL							||
				MENU_COND_ADVANCE							||
				MENU_COND_INFORMATION					||
				MENU_COND_SERVICE_TYPE				||
				MENU_COND_DOOR								||
				MENU_COND_DOOR_TIME 					||
				MENU_COND_DOOR_CONTROL_TYPE		||
				MENU_COND_DOOR_SECOND					||
				MENU_COND_SOUND								||
				MENU_COND_FLOOR								||
				MENU_COND_MOTOR_SAFE					||
				MENU_COND_MOTOR_SAFE_ENABLE		||
				MENU_COND_HYD_SETTING					||
				MENU_COND_LANDING							||
				MENU_COND_LANDING_BASED				||
				MENU_COND_CALIBRATION					||
				MENU_COND_CALIBRATION_DIR			||
				MENU_COND_CALIBRATION_SPEED		||
				MENU_COND_EMERGENCY						||
				MENU_COND_EMERGENCY_ENABLE		||
				MENU_COND_EMERGENCY_DIR				||
				MENU_COND_1CF_ERROR						||
				MENU_COND_1CF_ERROR_ENABLE		||
				MENU_COND_TIMER_SETTING				||
				MENU_COND_1CF_DELAY						||
				MENU_COND_RELAY_TIME					||
				MENU_COND_PHASE_CONTROL				||
				MENU_COND_PHASE_REVERSE				||
				MENU_COND_PHASE_FAULT					||
				MENU_COND_ONE_PHASE						||
				MENU_COND_TYPE_ELEVATOR				||
				MENU_COND_HARDWARE						||
				MENU_COND_HW_MAINBOARD				||
				MENU_COND_HW_CARCODEC					||
				MENU_COND_HW_MAIN_PUSHBUTTON	||
				MENU_COND_HW_CAR_PUSHBUTTON		||
				MENU_COND_HW_MAIN_NUMRATOR		||
				MENU_COND_HW_CAR_NUMRATOR			||
				MENU_COND_HW_MAIN_PB_TYPE			||
				MENU_COND_HW_CAR_PB_TYPE			||
				MENU_COND_HW_MAIN_NUM_TYPE		||
				MENU_COND_HW_CAR_NUM_TYPE			||
				MENU_COND_HW_DRIVE						||
				MENU_COND_HW_DRIVE_CONNCTION	||
				MENU_COND_HW_MAIN_PB_CANCEL_ENABLE	||
				MENU_COND_HW_CAR_PB_CANCEL_ENABLE   )
		{
			Xmenu=16; //lentgh of items
			
			if(FirstToMenu)
			{
				if(MENU_COND_MAINMENU)       					{Ymenu = 3;}
				else if(MENU_COND_GENERAL)						{Ymenu = 13;}
				else if(MENU_COND_ADVANCE)						{Ymenu = 5;}
				else if(MENU_COND_INFORMATION)				{Ymenu = 3;}
				else if(MENU_COND_SERVICE_TYPE) 			{Selector[MenuState] = Stng[SERVICE_TYPE];			Ymenu=4; }
				else if(MENU_COND_DOOR) 							{Ymenu=3; }
				else if(MENU_COND_DOOR_TIME)          {Ymenu=8; }
				else if(MENU_COND_DOOR_CONTROL_TYPE) 	{Selector[MenuState] = Stng[DOOR_CONTROL_TYPE];	Ymenu=3; }
				else if(MENU_COND_DOOR_SECOND)        {Ymenu=2; }
				else if(MENU_COND_SOUND) 							{Ymenu=4; }
				else if(MENU_COND_FLOOR) 							{Ymenu=4; }
				else if(MENU_COND_TIMER_SETTING) 			{Ymenu=7; }
				else if(MENU_COND_1CF_DELAY)        	{Ymenu=2; }
				else if(MENU_COND_RELAY_TIME)					{Ymenu=2; }
				else if(MENU_COND_MOTOR_SAFE) 				{Ymenu=4; }
				else if(MENU_COND_MOTOR_SAFE_ENABLE) 	{Selector[MenuState] = Stng[ENABLE_MOTOR_SAFE];	Ymenu=2; }
				else if(MENU_COND_HYD_SETTING) 				{Ymenu=5; }
				else if(MENU_COND_PHASE_CONTROL)			{Ymenu=3; }
				else if(MENU_COND_PHASE_REVERSE) 			{Selector[MenuState] = Stng[ENABLE_PHASE_REVERSE];	Ymenu=2; }
				else if(MENU_COND_PHASE_FAULT) 				{Selector[MenuState] = Stng[ENABLE_PHASE_FAULT];		Ymenu=2; }
				else if(MENU_COND_ONE_PHASE) 					{Selector[MenuState] = Stng[ENABLE_ONE_PHASE];			Ymenu=2; }
				else if(MENU_COND_LANDING) 						{Ymenu=9; }
				else if(MENU_COND_LANDING_BASED)			{Selector[MenuState] = Stng[LANDING_BASED];					Ymenu=3; }
				else if(MENU_COND_CALIBRATION) 				{Ymenu=2; }
				else if(MENU_COND_CALIBRATION_DIR)   	{Selector[MenuState] = Stng[CALIBRATION_DIR];				Ymenu=2; }
				else if(MENU_COND_CALIBRATION_SPEED) 	{Selector[MenuState] = Stng[CALIBRATION_SPEED];			Ymenu=3; }
				else if(MENU_COND_EMERGENCY) 					{Ymenu=2; }
				else if(MENU_COND_EMERGENCY_ENABLE) 	{Selector[MenuState] = Stng[ENABLE_EMERGENCY];			Ymenu=2; }
				else if(MENU_COND_EMERGENCY_DIR) 			{Selector[MenuState] = Stng[EMERGENCY_DIR];					Ymenu=2; }
				else if(MENU_COND_1CF_ERROR) 					{Ymenu=2; }
				else if(MENU_COND_1CF_ERROR_ENABLE) 	{Selector[MenuState] = Stng[ENABLE_1CF_ERROR];			Ymenu=2; }
				else if(MENU_COND_INFORMATION) 				{Ymenu=1; }
				else if(MENU_COND_TYPE_ELEVATOR)			{Selector[MenuState] = Stng[TYPE_ELEVATOR]; 				Ymenu=4; }
				else if(MENU_COND_HARDWARE) 					{Ymenu=2; }
				else if(MENU_COND_HW_MAINBOARD)				{Ymenu=7; }
				else if(MENU_COND_HW_CARCODEC)				{Ymenu=6; }
				else if(MENU_COND_HW_MAIN_PUSHBUTTON || MENU_COND_HW_CAR_PUSHBUTTON) 	{Ymenu=3; }
				else if(MENU_COND_HW_MAIN_NUMRATOR || MENU_COND_HW_CAR_NUMRATOR)			{Ymenu=4; }
				else if(MENU_COND_HW_MAIN_PB_TYPE)		{Selector[MenuState] = Stng[MAIN_PB_TYPE]; 		Ymenu=3; }
				else if(MENU_COND_HW_CAR_PB_TYPE)			{Selector[MenuState] = Stng[CAR_PB_TYPE]; 		Ymenu=3; }
				else if(MENU_COND_HW_MAIN_NUM_TYPE)		{Selector[MenuState] = Stng[MAIN_NUM_TYPE]; 	Ymenu=2; }
				else if(MENU_COND_HW_CAR_NUM_TYPE)		{Selector[MenuState] = Stng[CAR_NUM_TYPE]; 		Ymenu=2; }
				else if(MENU_COND_HW_DRIVE)						{Ymenu=2; }
				else if(MENU_COND_HW_DRIVE_CONNCTION)	{Selector[MenuState] = Stng[DRIVE_CONNECTION];	Ymenu=3; }
				else if(MENU_COND_HW_MAIN_PB_CANCEL_ENABLE)		{Selector[MenuState] = Stng[MAIN_PB_CANCEL_ENABLE];			Ymenu=2; }
				else if(MENU_COND_HW_CAR_PB_CANCEL_ENABLE)		{Selector[MenuState] = Stng[CAR_PB_CANCEL_ENABLE];			Ymenu=2; }
				FirstToMenu=0;
			}
			
			uint8_t sel = Selector[MenuState];
			uint8_t mb = MenuBar[MenuState];
			
			for(i=0;(i<4 && i<Ymenu);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Status = (sel == mbi);
				uint8_t Ylcd = HEIGHT_CHAR[i];
				if(MENU_COND_MAINMENU)         				{LCD_WriteMenuStatus(0,Ylcd,List_MainMenu[mbi],Status);					}
				else if(MENU_COND_GENERAL)						{LCD_WriteMenuStatus(0,Ylcd,List_GeneralMenu[mbi],Status);			}
				else if(MENU_COND_ADVANCE)						{LCD_WriteMenuStatus(0,Ylcd,List_AdvanceMenu[mbi],Status);			}
				else if(MENU_COND_INFORMATION)				{LCD_WriteMenuStatus(0,Ylcd,List_InformationMenu[mbi],Status);	}
				else if(MENU_COND_SERVICE_TYPE)  			{LCD_WriteMenuStatus(0,Ylcd,List_ServiceType[mbi],Status);			Stng[SERVICE_TYPE] = Selector[MenuState];}
				else if(MENU_COND_DOOR)         			{LCD_WriteMenuStatus(0,Ylcd,List_DoorSetting[mbi],Status);	}
				else if(MENU_COND_DOOR_TIME)         	{LCD_WriteMenuStatus(0,Ylcd,List_DoorTime[mbi],Status);	}
				else if(MENU_COND_DOOR_CONTROL_TYPE) 	{LCD_WriteMenuStatus(0,Ylcd,List_DoorControlType[mbi],Status);	Stng[DOOR_CONTROL_TYPE] = Selector[MenuState];}
				else if(MENU_COND_DOOR_SECOND)        {LCD_WriteMenuStatus(0,Ylcd,List_DoorSecond[mbi],Status);	}
				else if(MENU_COND_SOUND)         			{LCD_WriteMenuStatus(0,Ylcd,List_SoundSetting[mbi],Status);	}
				else if(MENU_COND_FLOOR)         			{LCD_WriteMenuStatus(0,Ylcd,List_FloorSetting[mbi],Status);	}
				else if(MENU_COND_TIMER_SETTING)     	{LCD_WriteMenuStatus(0,Ylcd,List_TimerSetting[mbi],Status);	}
				else if(MENU_COND_1CF_DELAY)     			{LCD_WriteMenuStatus(0,Ylcd,List_1CF_Delay[mbi],Status);	}
				else if(MENU_COND_RELAY_TIME)     		{LCD_WriteMenuStatus(0,Ylcd,List_RelayTime[mbi],Status);	}
				else if(MENU_COND_MOTOR_SAFE)     		{LCD_WriteMenuStatus(0,Ylcd,List_MotorSafe[mbi],Status);	}
				else if(MENU_COND_MOTOR_SAFE_ENABLE) 	{LCD_WriteMenuStatus(0,Ylcd,List_DisableEnable[mbi],Status);		Stng[ENABLE_MOTOR_SAFE] = Selector[MenuState];}
				else if(MENU_COND_HYD_SETTING)     		{LCD_WriteMenuStatus(0,Ylcd,List_HYDSetting[mbi],Status);	}
				else if(MENU_COND_PHASE_CONTROL)     	{LCD_WriteMenuStatus(0,Ylcd,List_PhaseControl[mbi],Status);	}
				else if(MENU_COND_PHASE_REVERSE)			{LCD_WriteMenuStatus(0,Ylcd,List_DisableEnable[mbi],Status);		Stng[ENABLE_PHASE_REVERSE] = Selector[MenuState];}
				else if(MENU_COND_PHASE_FAULT)				{LCD_WriteMenuStatus(0,Ylcd,List_DisableEnable[mbi],Status);		Stng[ENABLE_PHASE_FAULT] = Selector[MenuState];}
				else if(MENU_COND_ONE_PHASE)					{LCD_WriteMenuStatus(0,Ylcd,List_DisableEnable[mbi],Status);		Stng[ENABLE_ONE_PHASE] = Selector[MenuState];}
				else if(MENU_COND_LANDING)     				{LCD_WriteMenuStatus(0,Ylcd,List_Landing[mbi],Status);	}
				else if(MENU_COND_LANDING_BASED)			{LCD_WriteMenuStatus(0,Ylcd,List_LandingBased[mbi],Status);			Stng[LANDING_BASED] = Selector[MenuState];}
				else if(MENU_COND_CALIBRATION)     		{LCD_WriteMenuStatus(0,Ylcd,List_Calibration[mbi],Status);	}
				else if(MENU_COND_CALIBRATION_DIR)   	{LCD_WriteMenuStatus(0,Ylcd,List_SelectDirection[mbi],Status);	Stng[CALIBRATION_DIR] = Selector[MenuState];}
				else if(MENU_COND_CALIBRATION_SPEED) 	{LCD_WriteMenuStatus(0,Ylcd,List_SelectSpeed[mbi],Status);			Stng[CALIBRATION_SPEED] = Selector[MenuState];}			
				else if(MENU_COND_EMERGENCY)     			{LCD_WriteMenuStatus(0,Ylcd,List_Emergency[mbi],Status);	}
				else if(MENU_COND_EMERGENCY_ENABLE) 	{LCD_WriteMenuStatus(0,Ylcd,List_DisableEnable[mbi],Status);		Stng[ENABLE_EMERGENCY] = Selector[MenuState];}
				else if(MENU_COND_EMERGENCY_DIR)   		{LCD_WriteMenuStatus(0,Ylcd,List_SelectDirection[mbi],Status);	Stng[EMERGENCY_DIR] = Selector[MenuState];}
				else if(MENU_COND_1CF_ERROR)     			{LCD_WriteMenuStatus(0,Ylcd,List_1CFError[mbi],Status);	}
				else if(MENU_COND_1CF_ERROR_ENABLE) 	{LCD_WriteMenuStatus(0,Ylcd,List_DisableEnable[mbi],Status);		Stng[ENABLE_1CF_ERROR] = Selector[MenuState];}
				else if(MENU_COND_TYPE_ELEVATOR)			{LCD_WriteMenuStatus(0,Ylcd,List_TypeElevator[mbi],Status);			Stng[TYPE_ELEVATOR] = Selector[MenuState];}
				else if(MENU_COND_HARDWARE) 					{LCD_WriteMenuStatus(0,Ylcd,List_Hardware[mbi],Status);}
				else if(MENU_COND_HW_MAINBOARD)				{LCD_WriteMenuStatus(0,Ylcd,List_HW_MainBorad[mbi],Status);}
				else if(MENU_COND_HW_CARCODEC)				{LCD_WriteMenuStatus(0,Ylcd,List_HW_Carcodec[mbi],Status);}
				else if(MENU_COND_HW_MAIN_PUSHBUTTON || MENU_COND_HW_CAR_PUSHBUTTON)	{LCD_WriteMenuStatus(0,Ylcd,List_HW_PB[mbi],Status);}
				else if(MENU_COND_HW_MAIN_NUMRATOR || MENU_COND_HW_CAR_NUMRATOR)			{LCD_WriteMenuStatus(0,Ylcd,List_HW_Num[mbi],Status);}
				else if(MENU_COND_HW_MAIN_PB_TYPE)		{LCD_WriteMenuStatus(0,Ylcd,List_HW_Main_PB_Type[mbi],Status);		Stng[MAIN_PB_TYPE] 	= Selector[MenuState];}
				else if(MENU_COND_HW_CAR_PB_TYPE)			{LCD_WriteMenuStatus(0,Ylcd,List_HW_Car_PB_Type[mbi],Status);			Stng[CAR_PB_TYPE] 	= Selector[MenuState];}
				else if(MENU_COND_HW_MAIN_NUM_TYPE)		{LCD_WriteMenuStatus(0,Ylcd,List_HW_Main_Num_Type[mbi],Status);		Stng[MAIN_NUM_TYPE] = Selector[MenuState];}
				else if(MENU_COND_HW_CAR_NUM_TYPE)		{LCD_WriteMenuStatus(0,Ylcd,List_HW_Car_Num_Type[mbi],Status);		Stng[CAR_NUM_TYPE] 	= Selector[MenuState];}
				else if(MENU_COND_HW_DRIVE)						{LCD_WriteMenuStatus(0,Ylcd,List_Drive[mbi],Status);}
				else if(MENU_COND_HW_DRIVE_CONNCTION)	{LCD_WriteMenuStatus(0,Ylcd,List_DriveConnection[mbi],Status);		Stng[DRIVE_CONNECTION] = Selector[MenuState];}
				else if(MENU_COND_HW_MAIN_PB_CANCEL_ENABLE)		{LCD_WriteMenuStatus(0,Ylcd,List_DisableEnable[mbi],Status);		Stng[MAIN_PB_CANCEL_ENABLE] = Selector[MenuState];}
				else if(MENU_COND_HW_CAR_PB_CANCEL_ENABLE)		{LCD_WriteMenuStatus(0,Ylcd,List_DisableEnable[mbi],Status);		Stng[CAR_PB_CANCEL_ENABLE] = Selector[MenuState];}
			}

			lcdt = 100;
			SelChg(Ymenu-1,1);
			BarChange(MenuState,3);
		}
		
		
		////////////////////////////////// Numerical Menu
		else if(	MENU_COND_NUM_OF_STOP					||
							MENU_COND_TRAVEL_TIME					||
							MENU_COND_DOOR_OPEN_DELAY 		||
							MENU_COND_DOOR_CLOSE_DELAY		||
							MENU_COND_END_DOOR_TIME				||
							MENU_COND_DOOR_CLOSE_TIMEOUT	||
							MENU_COND_DOOR_PARK_TIME			||
							MENU_COND_69_DEBOUNCER				||
							MENU_COND_68_DEBOUNCER				||
							MENU_COND_DOOR_OPEN_TIME			||
							MENU_COND_NUM_OF_DOOR					||
							MENU_COND_MUSIC_VOLUME				||
							MENU_COND_TALK_VOLUME					||
							MENU_COND_WELCOME_FLOOR				||
							MENU_COND_GANG_SELECT					||
							MENU_COND_PARK_FLOOR					||
							MENU_COND_TIME_TO_PARK				||
							MENU_COND_FIRE_FLOOR					||
							MENU_COND_WAITING_FLOOR				||
							MENU_COND_UD_RELEASE_DELAY		||
							MENU_COND_TC_RELEASE_DELAY		||
							MENU_COND_1CF_ALL_DELAY				||	
							MENU_COND_LEVELING_TIME				||
							MENU_COND_STANDBY_TIME				||
							MENU_COND_URA_DELAY						||
							MENU_COND_RELAY_ON_DELAY			||
							MENU_COND_RELAY_ON_TIME				||
							MENU_COND_FAST_OC							||
							MENU_COND_SLOW_OC							||
							MENU_COND_TIME_OC							||
							MENU_COND_HYD_START_SLOW_DEL	||
							MENU_COND_HYD_START_FAST_DEL	||
							MENU_COND_HYD_STAR_TO_DELTA		||
							MENU_COND_MOTOR_START_DEL			||
							MENU_COND_MOTOR_STOP_DEL			||
							MENU_COND_CF3_DELAY						||
							MENU_COND_MOTOR_SPEED					||
							MENU_COND_ACCELERATION				||
							MENU_COND_DECCELERATION				||
							MENU_COND_1CF_ERROR_TIMEOUT		||
							MENU_COND_MAIN_NUM_SLEEP_LIGHT	||
							MENU_COND_MAIN_NUM_ACTIVE_LIGHT	||
							MENU_COND_CAR_NUM_SLEEP_LIGHT		||
							MENU_COND_CAR_NUM_ACTIVE_LIGHT  
						)
		{
			
			if(FirstToMenu)
			{
				Ymenu = 200;
				if(MENU_COND_NUM_OF_STOP)        				{Selector[MenuState] = Stng[NUM_OF_STOP];      		Ymenu = MAX_NOS;	}	//
				else if(MENU_COND_TRAVEL_TIME)					{Selector[MenuState] = Stng[TRAVEL_TIME];   									}	// 0-200 s
				else if(MENU_COND_DOOR_OPEN_DELAY)			{Selector[MenuState] = Stng[DOOR_OPEN_DELAY];									}	// 0.0-20.0 s (0.1s)
				else if(MENU_COND_DOOR_CLOSE_DELAY)			{Selector[MenuState] = Stng[DOOR_CLOSE_DELAY];								}	// 0.0-20.0 s (0.1s)
				else if(MENU_COND_END_DOOR_TIME)   			{Selector[MenuState] = Stng[DOOR_END_TIME]; 			Ymenu = 10;	}	//
				else if(MENU_COND_DOOR_CLOSE_TIMEOUT)		{Selector[MenuState] = Stng[DOOR_CLOSE_TIMEOUT];							}	// 0-200 s
				else if(MENU_COND_DOOR_PARK_TIME)				{Selector[MenuState] = Stng[DOOR_PARK_TIME];			Ymenu = 250;}	// 0-250 s				
				else if(MENU_COND_69_DEBOUNCER)					{Selector[MenuState] = Stng[DEBOUNCER69];					Ymenu = 30;	}	// 0.0-3.0 s (0.1s)
				else if(MENU_COND_68_DEBOUNCER)					{Selector[MenuState] = Stng[DEBOUNCER68];					Ymenu = 30;	}	// 0.0-3.0 s (0.1s)
				else if(MENU_COND_DOOR_OPEN_TIME)				{Selector[MenuState] = Stng[DOOR_OPEN_TIME];									}	// 0-200 s
				else if(MENU_COND_NUM_OF_DOOR)					{Selector[MenuState] = Stng[NUM_OF_DOOR];					Ymenu = 2;	}	// 
				else if(MENU_COND_MUSIC_VOLUME) 				{Selector[MenuState] = Stng[MUSIC_VOLUME];				Ymenu = 20;	}	// 0-100% (*5)
				else if(MENU_COND_TALK_VOLUME) 					{Selector[MenuState] = Stng[TALK_VOLUME];					Ymenu = 20;	}	// 0-100% (*5)
				else if(MENU_COND_WELCOME_FLOOR)				{Selector[MenuState] = Stng[WELCOME_FLOOR];				Ymenu = Stng[NUM_OF_STOP];	}	//
				else if(MENU_COND_GANG_SELECT)					{Selector[MenuState] = Stng[GANG_SELECT];					Ymenu = 3;	}	// 0:No Gang, 1,2,3
				else if(MENU_COND_PARK_FLOOR)						{Selector[MenuState] = Stng[PARK_FLOOR];					Ymenu = 6;	}	//
				else if(MENU_COND_TIME_TO_PARK)					{Selector[MenuState] = Stng[TIME_TO_PARK];										}	// 0-200 m
				else if(MENU_COND_FIRE_FLOOR)						{Selector[MenuState] = Stng[FIRE_FLOOR];					Ymenu = 6;	}	//
				else if(MENU_COND_WAITING_FLOOR)				{Selector[MenuState] = Stng[WAITING_FLOOR];				Ymenu = 250;}	// 0-250 s
				else if(MENU_COND_UD_RELEASE_DELAY)			{Selector[MenuState] = Stng[UD_RELEASE_DELAY];								}	// 0.0-20.0 s (0.1s)
				else if(MENU_COND_TC_RELEASE_DELAY)			{Selector[MenuState] = Stng[TC_RELEASE_DELAY];								}	// 0.0-20.0 s (0.1s)
				else if(MENU_COND_1CF_ALL_DELAY)				{Selector[MenuState] = Stng[ALL_FLOOR_1CF_DELAY];							}	// 0.00-10.00 s (0.05s)
				else if(MENU_COND_LEVELING_TIME)				{Selector[MenuState] = Stng[LEVELING_TIME];										}	// 0-200 s
				else if(MENU_COND_STANDBY_TIME)					{Selector[MenuState] = Stng[STANDBY_TIME];										}	// 0-1000 s (5 s)
				else if(MENU_COND_URA_DELAY)						{Selector[MenuState] = Stng[URA_DELAY];						Ymenu = 50;	}	// 0.0-5.0 s (0.1s)
				else if(MENU_COND_RELAY_ON_DELAY)				{Selector[MenuState] = Stng[RELAY_ON_DELAY];			Ymenu = 50;	}	// 0.0-5.0 s (0.1s)
				else if(MENU_COND_RELAY_ON_TIME)				{Selector[MenuState] = Stng[RELAY_ON_TIME];				Ymenu = 50;	}	// 0.0-5.0 s (0.1s)
				else if(MENU_COND_FAST_OC)							{Selector[MenuState] = Stng[FAST_OVERCURRENT];								}	// 0-200 A
				else if(MENU_COND_SLOW_OC)							{Selector[MenuState] = Stng[SLOW_OVERCURRENT];		Ymenu =100;	}	// 0-100 A
				else if(MENU_COND_TIME_OC)							{Selector[MenuState] = Stng[TIME_OVERCURRENT];		Ymenu =100;	}	// 0.0-10.0 s (0.1s)
				else if(MENU_COND_HYD_START_SLOW_DEL)		{Selector[MenuState] = Stng[START_SLOW_DELAY];								}	// 0.0-20.0 s (0.1s)
				else if(MENU_COND_HYD_START_FAST_DEL)		{Selector[MenuState] = Stng[START_FAST_DELAY];								}	// 0.0-20.0 s (0.1s)
				else if(MENU_COND_HYD_STAR_TO_DELTA)		{Selector[MenuState] = Stng[START_TO_DELTA];									}	// 0.0-20.0 s (0.1s)
				else if(MENU_COND_MOTOR_START_DEL)			{Selector[MenuState] = Stng[MOTOR_START_DELAY];								}	// 0.0-20.0 s (0.1s)
				else if(MENU_COND_MOTOR_STOP_DEL)				{Selector[MenuState] = Stng[MOTOR_STOP_DELAY];								}	// -10.0~+10.0 s (0.1s)
				else if(MENU_COND_CF3_DELAY)						{Selector[MenuState] = Stng[CF3_DELAY];												}	// 0.0-20.0 s (0.1s)
				else if(MENU_COND_MOTOR_SPEED)					{Selector[MenuState] = Stng[MOTOR_SPEED];					Ymenu =100;	} // 0.0-10.0 m
				else if(MENU_COND_ACCELERATION)					{Selector[MenuState] = Stng[ACCELERATION];				 						} // 0.0-20.0 s
				else if(MENU_COND_DECCELERATION)				{Selector[MenuState] = Stng[DECCELERATION];										} // 0.0-20.0 s
				else if(MENU_COND_1CF_ERROR_TIMEOUT)		{Selector[MenuState] = Stng[ERROR_1CF_TIMEOUT];								}	// 0-200s
				else if(MENU_COND_MAIN_NUM_SLEEP_LIGHT)		{Selector[MenuState] = Stng[MAIN_NUM_SLEEP_LIGHT]; Ymenu = 20;}	// 0-20
				else if(MENU_COND_MAIN_NUM_ACTIVE_LIGHT)	{Selector[MenuState] = Stng[MAIN_NUM_ACTIVE_LIGHT];Ymenu = 20;}	// 0-20
				else if(MENU_COND_CAR_NUM_SLEEP_LIGHT)		{Selector[MenuState] = Stng[CAR_NUM_SLEEP_LIGHT];	 Ymenu = 20;}	// 0-20
				else if(MENU_COND_CAR_NUM_ACTIVE_LIGHT)		{Selector[MenuState] = Stng[CAR_NUM_ACTIVE_LIGHT]; Ymenu = 20;}	// 0-20
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu;
				
				FirstToMenu=0;
			}

			int16_t Number = 0;
			char    Unit = 'S';
			float   Ratio = 1.0;
			char  	*TitleNum;
			
			if(MENU_COND_NUM_OF_STOP)
			{
				Stng[NUM_OF_STOP] = Selector[MenuState];    		Number = Stng[NUM_OF_STOP];      		TitleNum = List_GeneralMenu[0];   								Unit = 0;	
			}
			else if(MENU_COND_TRAVEL_TIME)
			{
				Stng[TRAVEL_TIME] = Selector[MenuState];    		Number = Stng[TRAVEL_TIME];   			TitleNum = List_GeneralMenu[3];
			}
			else if(MENU_COND_DOOR_OPEN_DELAY)
			{
				Stng[DOOR_OPEN_DELAY] = Selector[MenuState];		Number = Stng[DOOR_OPEN_DELAY];			TitleNum = List_DoorTime[0];   		Ratio = 0.1;
			}
			else if(MENU_COND_DOOR_CLOSE_DELAY)
			{
				Stng[DOOR_CLOSE_DELAY] = Selector[MenuState];		Number = Stng[DOOR_CLOSE_DELAY];		TitleNum = List_DoorTime[1];   		Ratio = 0.1;
			}
			else if(MENU_COND_END_DOOR_TIME)
			{
				Stng[DOOR_END_TIME] = Selector[MenuState];			Number = Stng[DOOR_END_TIME];				TitleNum = List_DoorTime[2];   										Unit = 0;
			}
			else if(MENU_COND_DOOR_CLOSE_TIMEOUT)
			{
				Stng[DOOR_CLOSE_TIMEOUT] = Selector[MenuState];	Number = Stng[DOOR_CLOSE_TIMEOUT];	TitleNum = List_DoorTime[3];
			}
			else if(MENU_COND_DOOR_PARK_TIME)
			{
				Stng[DOOR_PARK_TIME] = Selector[MenuState];			Number = Stng[DOOR_PARK_TIME];			TitleNum = List_DoorTime[4];
			}
			else if(MENU_COND_69_DEBOUNCER)
			{
				Stng[DEBOUNCER69] = Selector[MenuState];				Number = Stng[DEBOUNCER69];					TitleNum = List_DoorTime[5];   		Ratio = 0.1;
			}
			else if(MENU_COND_68_DEBOUNCER)
			{
				Stng[DEBOUNCER68] = Selector[MenuState];				Number = Stng[DEBOUNCER68];					TitleNum = List_DoorTime[6];   		Ratio = 0.1;
			}
			else if(MENU_COND_DOOR_OPEN_TIME)
			{
				Stng[DOOR_OPEN_TIME] = Selector[MenuState];			Number = Stng[DOOR_OPEN_TIME];			TitleNum = List_DoorTime[7];
			}
			else if(MENU_COND_NUM_OF_DOOR)
			{
				Stng[NUM_OF_DOOR] = Selector[MenuState];				Number = Stng[NUM_OF_DOOR] + 1;			TitleNum = List_DoorSecond[0];									Unit = 0;
			}
			else if(MENU_COND_MUSIC_VOLUME)
			{
				Stng[MUSIC_VOLUME] = Selector[MenuState];				Number = Stng[MUSIC_VOLUME];				TitleNum = List_SoundSetting[0]; 	Ratio = 5.0;	Unit = '%';
			}
			else if(MENU_COND_TALK_VOLUME)
			{
				Stng[TALK_VOLUME] = Selector[MenuState];				Number = Stng[TALK_VOLUME];					TitleNum = List_SoundSetting[1]; 	Ratio = 5.0;	Unit = '%';
			}
			else if(MENU_COND_WELCOME_FLOOR)
			{
				Stng[WELCOME_FLOOR] = Selector[MenuState];			Number = Stng[WELCOME_FLOOR];				TitleNum = List_SoundSetting[2]; 								Unit = 0;
			}
			else if(MENU_COND_GANG_SELECT)
			{
				Stng[GANG_SELECT] = Selector[MenuState];				Number = Stng[GANG_SELECT];					TitleNum = List_SoundSetting[3]; 								Unit = 0;
			}
			else if(MENU_COND_PARK_FLOOR)
			{
				Stng[PARK_FLOOR] = Selector[MenuState];					Number = Stng[PARK_FLOOR];					TitleNum = List_FloorSetting[0]; 								Unit = 0;
			}	
			else if(MENU_COND_TIME_TO_PARK)
			{
				Stng[TIME_TO_PARK] = Selector[MenuState];				Number = Stng[TIME_TO_PARK];				TitleNum = List_FloorSetting[1]; 								Unit = 'm';
			}
			else if(MENU_COND_FIRE_FLOOR)
			{
				Stng[FIRE_FLOOR] = Selector[MenuState];					Number = Stng[FIRE_FLOOR];					TitleNum = List_FloorSetting[2]; 								Unit = 0;
			}
			else if(MENU_COND_WAITING_FLOOR)
			{
				Stng[WAITING_FLOOR] = Selector[MenuState];			Number = Stng[WAITING_FLOOR];				TitleNum = List_FloorSetting[3];
			}
			else if(MENU_COND_UD_RELEASE_DELAY)
			{
				Stng[UD_RELEASE_DELAY] = Selector[MenuState];		Number = Stng[UD_RELEASE_DELAY];		TitleNum = List_TimerSetting[0]; 	Ratio = 0.1;
			}
			else if(MENU_COND_TC_RELEASE_DELAY)
			{
				Stng[TC_RELEASE_DELAY] = Selector[MenuState];		Number = Stng[TC_RELEASE_DELAY];		TitleNum = List_TimerSetting[1];	Ratio = 0.1;
			}
			else if(MENU_COND_1CF_ALL_DELAY)
			{
				Stng[ALL_FLOOR_1CF_DELAY]=Selector[MenuState];	Number = Stng[ALL_FLOOR_1CF_DELAY];	TitleNum = List_1CF_Delay[0];			Ratio = 0.05;
			}
			else if(MENU_COND_LEVELING_TIME)
			{
				Stng[LEVELING_TIME] = Selector[MenuState];			Number = Stng[LEVELING_TIME];				TitleNum = List_TimerSetting[3];
			}
			else if(MENU_COND_STANDBY_TIME)
			{
				Stng[STANDBY_TIME] = Selector[MenuState];				Number = Stng[STANDBY_TIME];				TitleNum = List_TimerSetting[4]; 	Ratio = 5.0;
			}
			else if(MENU_COND_URA_DELAY)
			{
				Stng[URA_DELAY] = Selector[MenuState];					Number = Stng[URA_DELAY];						TitleNum = List_TimerSetting[5]; 	Ratio = 0.1;
			}
			else if(MENU_COND_RELAY_ON_DELAY)
			{
				Stng[RELAY_ON_DELAY] = Selector[MenuState];			Number = Stng[RELAY_ON_DELAY];			TitleNum = List_RelayTime[0]; 	Ratio = 0.1;
			}
			else if(MENU_COND_RELAY_ON_TIME)
			{
				Stng[RELAY_ON_TIME] = Selector[MenuState];			Number = Stng[RELAY_ON_TIME];				TitleNum = List_RelayTime[1]; 	Ratio = 0.1;
			}
			else if(MENU_COND_FAST_OC)
			{
				Stng[FAST_OVERCURRENT] = Selector[MenuState];		Number = Stng[FAST_OVERCURRENT];		TitleNum = List_MotorSafe[1]; 									Unit = 'A';
			}
			else if(MENU_COND_SLOW_OC)
			{
				Stng[SLOW_OVERCURRENT] = Selector[MenuState];		Number = Stng[SLOW_OVERCURRENT];		TitleNum = List_MotorSafe[2]; 									Unit = 'A';
			}
			else if(MENU_COND_TIME_OC)
			{
				Stng[TIME_OVERCURRENT] = Selector[MenuState];		Number = Stng[TIME_OVERCURRENT];		TitleNum = List_MotorSafe[3]; 		Ratio = 0.1;
			}
			else if(MENU_COND_HYD_START_SLOW_DEL)
			{
				Stng[START_SLOW_DELAY] = Selector[MenuState];		Number = Stng[START_SLOW_DELAY];		TitleNum = List_HYDSetting[0]; 		Ratio = 0.1;
			}
			else if(MENU_COND_HYD_START_FAST_DEL)
			{
				Stng[START_FAST_DELAY] = Selector[MenuState];		Number = Stng[START_FAST_DELAY];		TitleNum = List_HYDSetting[1]; 		Ratio = 0.1;
			}
			else if(MENU_COND_HYD_STAR_TO_DELTA)
			{
				Stng[START_TO_DELTA] = Selector[MenuState];			Number = Stng[START_TO_DELTA];			TitleNum = List_HYDSetting[2]; 		Ratio = 0.1;
			}
			else if(MENU_COND_MOTOR_START_DEL)
			{
				Stng[MOTOR_START_DELAY] = Selector[MenuState];	Number = Stng[MOTOR_START_DELAY];		TitleNum = List_HYDSetting[3]; 		Ratio = 0.1;
			}
			else if(MENU_COND_MOTOR_STOP_DEL)
			{
				Stng[MOTOR_STOP_DELAY] = Selector[MenuState];		Number = Stng[MOTOR_STOP_DELAY]-100;	TitleNum = List_HYDSetting[4]; 		Ratio = 0.1;
			}
			else if(MENU_COND_CF3_DELAY)
			{
				Stng[CF3_DELAY] = Selector[MenuState];					Number = Stng[CF3_DELAY];						TitleNum = List_Landing[3]; 			Ratio = 0.1;
			}
			else if(MENU_COND_MOTOR_SPEED)				
			{
				Stng[MOTOR_SPEED] = Selector[MenuState];				Number = Stng[MOTOR_SPEED];					TitleNum = List_Landing[4];				Ratio = 0.1;	Unit = 'm';
			}
			else if(MENU_COND_ACCELERATION)				
			{
				Stng[ACCELERATION] = Selector[MenuState];				Number = Stng[ACCELERATION];				TitleNum = List_Landing[5];				Ratio = 0.1;
			}
			else if(MENU_COND_DECCELERATION)				
			{
				Stng[DECCELERATION] = Selector[MenuState];			Number = Stng[DECCELERATION];				TitleNum = List_Landing[6];				Ratio = 0.1;
			}
			else if(MENU_COND_1CF_ERROR_TIMEOUT)
			{
				Stng[ERROR_1CF_TIMEOUT] = Selector[MenuState];	Number = Stng[ERROR_1CF_TIMEOUT];		TitleNum = List_1CFError[1];
			}
			else if(MENU_COND_MAIN_NUM_SLEEP_LIGHT)
			{
				Stng[MAIN_NUM_SLEEP_LIGHT] = Selector[MenuState];		Number = Stng[MAIN_NUM_SLEEP_LIGHT];	TitleNum = List_HW_Num[2];			Ratio = 5;		Unit = '%';
			}
			else if(MENU_COND_MAIN_NUM_ACTIVE_LIGHT)
			{
				Stng[MAIN_NUM_ACTIVE_LIGHT] = Selector[MenuState];	Number = Stng[MAIN_NUM_ACTIVE_LIGHT];	TitleNum = List_HW_Num[3];			Ratio = 5;		Unit = '%';
			}
			else if(MENU_COND_CAR_NUM_SLEEP_LIGHT)
			{
				Stng[CAR_NUM_SLEEP_LIGHT] = Selector[MenuState];		Number = Stng[CAR_NUM_SLEEP_LIGHT];		TitleNum = List_HW_Num[2];			Ratio = 5;		Unit = '%';
			}
			else if(MENU_COND_CAR_NUM_ACTIVE_LIGHT)
			{
				Stng[CAR_NUM_ACTIVE_LIGHT] = Selector[MenuState];		Number = Stng[CAR_NUM_ACTIVE_LIGHT];	TitleNum = List_HW_Num[3];			Ratio = 5;		Unit = '%';
			}
			
			LCD_write_str_middle(0,TitleNum); //Title
			
			float Res = Number*Ratio;
			
			if( (MENU_COND_WELCOME_FLOOR && Number == Stng[NUM_OF_STOP]) || (MENU_COND_GANG_SELECT && Number == 0) )// || 
					//(MENU_COND_TIME_TO_PARK && Number == 0)  || (MENU_COND_STANDBY_TIME && Number == 0) )
					//(MENU_COND_MOTOR_FAN_TIME && Number == 0))
			{
				sprintf(MenuStr,"OFF");
			}
			else if(Ratio == 1.0 || Unit == 0 || Unit == '%') 
			{
				sprintf(MenuStr,"%u %c",(uint16_t)Res,Unit);
			}
			else if(Ratio > 0.09)      
			{
				sprintf(MenuStr,"%.1f %c",Res,Unit);
			}
			else    
			{
				sprintf(MenuStr,"%.2f %c",Res,Unit);
			}
			
			LCD_write_str_middle(27,MenuStr);
			
			LCD_DrawRect(21,24,54,17,1);
			
			lcdt = 100;
			SelChg(Ymenu,0);
			BarChange(MenuState,3);
		}
		
		
		
		
		////////////////////////////////// 1CF Up/Down Delay
		else if(MENU_COND_1CF_UP_DN_DELAY)
		{
			uint8_t sel = Selector[5];
			
			if(FirstToMenu)
			{
				if(MenuState == 5)
				{
					Ymenu = Stng[NUM_OF_STOP] - 1;
					
				}
				else if(MenuState == 6)
				{
					Ymenu = 50; 
					Selector[MenuState] = Floor[sel].Delay_1CF_Up;
				}
				else if(MenuState == 7)
				{
					Ymenu = 50;
					Selector[MenuState] = Floor[sel].Delay_1CF_Down;
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu-1;
				
				FirstToMenu=0;
			}
			
			
			else if(MenuState == 6) {Floor[sel].Delay_1CF_Up		= Selector[MenuState];}
			else if(MenuState == 7) {Floor[sel].Delay_1CF_Down	= Selector[MenuState];}
			
			uint8_t mb = MenuBar[5];
			
			sprintf(MenuStr,"FLR  UP    DN");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				if(mbi < Stng[NUM_OF_STOP]) 
				{
					sprintf(MenuStr,"%u",mbi);
					LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 5)) );
					
					sprintf(MenuStr,"%.2f",Floor[mbi].Delay_1CF_Up*0.05);
					LCD_WriteMenuStatus(25,Ylcd,MenuStr, (MenuState == 6) && (sel == mbi) );
					
					sprintf(MenuStr,"%.2f",Floor[mbi].Delay_1CF_Down*0.05);      
					LCD_WriteMenuStatus(62,Ylcd,MenuStr, (MenuState == 7) && (sel == mbi) );
				}
				else break;
				//sprintf(MenuStr,"%u",Stng[]V2[mbi]);
				//LCD_WriteMenuStatus(82,Ylcd,MenuStr, (MenuState == 5) && (sel == mbi) );
			}
			
			
			lcdt = 100;
			SelChg(Ymenu,(MenuState == 5));
			BarChange(MenuState,2);
		}
		
		////////////////////////////////// Door Select
		else if(MENU_COND_DOOR_SELECT)
		{
			uint8_t sel = Selector[5];
			
			if(FirstToMenu)
			{
				if(MenuState == 5)
				{
					Ymenu = Stng[NUM_OF_STOP] - 1;
					
				}
				else if(MenuState == 6)
				{
					Ymenu = 2;
					Selector[MenuState] = Floor[sel].Door1Select;
				}
				else if(MenuState == 7)
				{
					Ymenu = 2;
					Selector[MenuState] = Floor[sel].Door2Select;
				}
				else if(MenuState == 8)
				{
					Ymenu = 2;
					Selector[MenuState] = Floor[sel].Door3Select;
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu-1;
				
				FirstToMenu=0;
			}

			else if(MenuState == 6) {Floor[sel].Door1Select = Selector[MenuState];}
			else if(MenuState == 7) {Floor[sel].Door2Select = Selector[MenuState];}
			else if(MenuState == 8) {Floor[sel].Door3Select = Selector[MenuState];}
			
			if(Stng[NUM_OF_DOOR] == 0)
			{
				for(i=0;i<Stng[NUM_OF_STOP];i++)
				{
					Floor[i].Door1Select = 1;
					Floor[i].Door2Select = 0;
					Floor[i].Door3Select = 0;
				}
			}
			
			uint8_t mb = MenuBar[5];
			
			sprintf(MenuStr,"FLR  D1 D2 D3");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				if(mbi < Stng[NUM_OF_STOP]) 
				{
					sprintf(MenuStr,"%u",mbi);
					LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 5)) );
					
					char Ds = 'N';
					if(Floor[mbi].Door1Select == 1) 			Ds = 'Y';
					else if(Floor[mbi].Door1Select == 2) 	Ds = 'S';
					sprintf(MenuStr,"%c",Ds);
					LCD_WriteMenuStatus(35,Ylcd,MenuStr, (MenuState == 6) && (sel == mbi) );
					
					Ds = 'N';
					if(Floor[mbi].Door2Select == 1) 			Ds = 'Y';
					else if(Floor[mbi].Door2Select == 2) 	Ds = 'S';
					sprintf(MenuStr,"%c",Ds);      
					LCD_WriteMenuStatus(56,Ylcd,MenuStr, (MenuState == 7) && (sel == mbi) );

					Ds = 'N';
					if(Floor[mbi].Door3Select == 1) 			Ds = 'Y';
					else if(Floor[mbi].Door3Select == 2) 	Ds = 'S';
					sprintf(MenuStr,"%c",Ds);      
					LCD_WriteMenuStatus(77,Ylcd,MenuStr, (MenuState == 8) && (sel == mbi) );
				}
				else break;
				//sprintf(MenuStr,"%u",Stng[]V2[mbi]);
				//LCD_WriteMenuStatus(82,Ylcd,MenuStr, (MenuState == 5) && (sel == mbi) );
			}
			
			
			lcdt = 100;
			SelChg(Ymenu,(MenuState == 5));
			BarChange(MenuState,2);
		}


		////////////////////////////////// Numrator & Talk Configuration
		else if(MENU_COND_NUM_TALK)
		{
			uint8_t sel = Selector[3];
			
			if(FirstToMenu)
			{
				if(MenuState == 3)
				{
					Ymenu = Stng[NUM_OF_STOP] - 1;
				}
				else if(MenuState == 4)
				{
					Ymenu = 23; 
					Selector[MenuState] = Floor[sel].SegL;
				}
				else if(MenuState == 5)
				{
					Ymenu = 23;
					Selector[MenuState] = Floor[sel].SegR;
				}
				else if(MenuState == 6)
				{
					Ymenu = 100;
					Selector[MenuState] = Floor[sel].Talk;
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu;
				
				FirstToMenu=0;
			}

			else if(MenuState == 4) {Floor[sel].SegL = Selector[MenuState];}
			else if(MenuState == 5) {Floor[sel].SegR = Selector[MenuState];}
			else if(MenuState == 6) {Floor[sel].Talk = Selector[MenuState];}
			
			uint8_t mb = MenuBar[3];
			
			sprintf(MenuStr,"F   SL SR  Talk");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				if(mbi < Stng[NUM_OF_STOP]) 
				{
					sprintf(MenuStr,"%u",mbi);
					LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 3)) );

					sprintf(MenuStr,"%c",List_SegmentChoices[Floor[mbi].SegL]);
					LCD_WriteMenuStatus(25,Ylcd,MenuStr, (MenuState == 4) && (sel == mbi) );
					
					sprintf(MenuStr,"%c",List_SegmentChoices[Floor[mbi].SegR]);      
					LCD_WriteMenuStatus(45,Ylcd,MenuStr, (MenuState == 5) && (sel == mbi) );

					sprintf(MenuStr,"%s",List_TalkChoices[Floor[mbi].Talk]);
					LCD_WriteMenuStatus(66,Ylcd,MenuStr, (MenuState == 6) && (sel == mbi) );
				}
				else break;
			}

			
			lcdt = 100;
			SelChg(Ymenu,(MenuState == 3));
			BarChange(MenuState,2);
		}
		
		
		////////////////////////////////// Select Speed Jump1
		else if(MENU_COND_JUMP1_SPEED)
		{
			uint8_t sel = Selector[4];
			
			if(FirstToMenu)
			{
				if(MenuState == 4)
				{
					Ymenu = Stng[NUM_OF_STOP] - 1;
					
				}
				else if(MenuState == 5)
				{
					Ymenu = 3; 
					Selector[MenuState] = Floor[sel].Jump1Speed;
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu-1;
				
				FirstToMenu=0;
			}
			
			
			else if(MenuState == 5) {Floor[sel].Jump1Speed 	= Selector[MenuState];}
			
			uint8_t mb = MenuBar[4];
			
			sprintf(MenuStr,"FLR    Jump1");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				if(mbi < Stng[NUM_OF_STOP]) 
				{
					sprintf(MenuStr,"%u",mbi);
					LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 4)) );
					
					sprintf(MenuStr,"%s",List_SelectSpeed[Floor[mbi].Jump1Speed]);
					LCD_WriteMenuStatus(45,Ylcd,MenuStr, (MenuState == 5) && (sel == mbi) );
				}
				else break;
			}
			
			lcdt = 100;
			SelChg(Ymenu,(MenuState == 4));
			BarChange(MenuState,2);
		}
		
		////////////////////////////////// CF3 Flag Jump1 JumpN
		else if(MENU_COND_CF3_FLAG)
		{
			uint8_t sel = Selector[4];
			
			if(FirstToMenu)
			{
				if(MenuState == 4)
				{
					Ymenu = Stng[NUM_OF_STOP] - 1;
					
				}
				else if(MenuState == 5)
				{
					Ymenu = 2; 
					Selector[MenuState] = Floor[sel].Jump1Flag;
				}
				else if(MenuState == 6)
				{
					Ymenu = 4; 
					Selector[MenuState] = Floor[sel].JumpNFlag;
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu-1;
				
				FirstToMenu=0;
			}
			
			
			else if(MenuState == 5) {Floor[sel].Jump1Flag 	= Selector[MenuState];}
			else if(MenuState == 6) {Floor[sel].JumpNFlag 	= Selector[MenuState];}
			
			uint8_t mb = MenuBar[4];
			
			sprintf(MenuStr,"FLR Jmp1 JmpN");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				if(mbi < Stng[NUM_OF_STOP]) 
				{
					sprintf(MenuStr,"%u",mbi);
					LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 4)) );
					
					sprintf(MenuStr,"%d",Floor[mbi].Jump1Flag);
					LCD_WriteMenuStatus(36,Ylcd,MenuStr, (MenuState == 5) && (sel == mbi) );
					
					sprintf(MenuStr,"%d",Floor[mbi].JumpNFlag - 2);
					LCD_WriteMenuStatus(70,Ylcd,MenuStr, (MenuState == 6) && (sel == mbi) );
				}
				else break;
			}
			
			lcdt = 100;
			SelChg(Ymenu,(MenuState == 4));
			BarChange(MenuState,2);
		}
		
		////////////////////////////////// Drive Control Signal V0|V1|V2
		else if(MENU_COND_HW_DRIVE_CONTROL)
		{
			uint8_t sel = Selector[6];
			
			if(FirstToMenu)
			{
				if(MenuState == 6)
				{
					Ymenu = 4;
				}
				else if(MenuState == 7)
				{
					Ymenu = 1; 
					Selector[MenuState] = Stng_DriveParallelV0[sel];
				}
				else if(MenuState == 8)
				{
					Ymenu = 1;
					Selector[MenuState] = Stng_DriveParallelV1[sel];
				}
				else if(MenuState == 9)
				{
					Ymenu = 1;
					Selector[MenuState] = Stng_DriveParallelV2[sel];
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu;
				
				FirstToMenu=0;
			}

			else if(MenuState == 7) {Stng_DriveParallelV0[sel] = Selector[MenuState];}
			else if(MenuState == 8) {Stng_DriveParallelV1[sel] = Selector[MenuState];}
			else if(MenuState == 9) {Stng_DriveParallelV2[sel] = Selector[MenuState];}
			
			uint8_t mb = MenuBar[6];
			
			sprintf(MenuStr,"Sp   V0|V1|V2");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				if(mbi < Stng[NUM_OF_STOP]) 
				{
					sprintf(MenuStr,"%s",List_Speed[mbi]);
					LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 6)) );

					sprintf(MenuStr,"%u",Stng_DriveParallelV0[mbi]);
					LCD_WriteMenuStatus(35,Ylcd,MenuStr, (MenuState == 7) && (sel == mbi) );
					
					sprintf(MenuStr,"%u",Stng_DriveParallelV1[mbi]);      
					LCD_WriteMenuStatus(53,Ylcd,MenuStr, (MenuState == 8) && (sel == mbi) );

					sprintf(MenuStr,"%u",Stng_DriveParallelV2[mbi]);
					LCD_WriteMenuStatus(71,Ylcd,MenuStr, (MenuState == 9) && (sel == mbi) );
				}
				else break;
			}

			
			lcdt = 100;
			SelChg(Ymenu,(MenuState == 6));
			BarChange(MenuState,2);
			
		}
		
		////////////////////////////////// Level Distance
		else if(MENU_COND_LEVEL_DISTANCE)
		{
			uint8_t sel = Selector[4];
			
			if(FirstToMenu)
			{
				if(MenuState == 4)
				{
					Ymenu = Stng[NUM_OF_STOP] - 2;
				}
				else if(MenuState == 5)
				{
					Ymenu = 250; 
					Selector[MenuState] = Floor[sel].LevelDistance;
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu-1;
				
				FirstToMenu=0;
			}
			
			
			else if(MenuState == 5) {Floor[sel].LevelDistance = Selector[MenuState];}
			
			uint8_t mb = MenuBar[4];
			
			sprintf(MenuStr," FLR     Dis(m)");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				if(mbi < Stng[NUM_OF_STOP]) 
				{
					sprintf(MenuStr,"%u->%u",mbi,mbi+1);
					LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 4)) );
					
					sprintf(MenuStr,"%.1f",Floor[mbi].LevelDistance/10.0);
					LCD_WriteMenuStatus(55,Ylcd,MenuStr, (MenuState == 5) && (sel == mbi) );
				}
				else break;
			}
			
			lcdt = 100;
			SelChg(Ymenu,(MenuState == 4));
			BarChange(MenuState,2);
		}
		
		////////////////////////////////// Main Inputs Select
		else if(MENU_COND_HW_MAIN_INPUTS)
		{
			uint8_t sel = Selector[5];
			
			if(FirstToMenu)
			{
				if(MenuState == 5)
				{
					Ymenu = 20; //Program Inputs
				}
				else if(MenuState == 6)
				{
					Ymenu = 22; //Hw Inputs Choice 
					Selector[MenuState] = Stng[MAIN_IN_HWADD + sel + 1];
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu-1;
				
				FirstToMenu=0;
			}
			
			
			else if(MenuState == 6) {Stng[MAIN_IN_HWADD + sel + 1]	= Selector[MenuState];}
			
			uint8_t mb = MenuBar[5];
			
			sprintf(MenuStr,"Prog   Board");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				sprintf(MenuStr,"%s",List_ProgMainInputs[mbi]);
				LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 5)) );
				
				sprintf(MenuStr,"%s",List_HwMainInputs[Stng[MAIN_IN_HWADD + mbi + 1]]);
				LCD_WriteMenuStatus(49,Ylcd,MenuStr, (MenuState == 6) && (sel == mbi) );
			}
			
			lcdt = 100;
			SelChg(Ymenu-1,(MenuState == 5));
			BarChange(MenuState,2);
		}
		
		////////////////////////////////// Main Outputs Select
		else if(MENU_COND_HW_MAIN_OUTPUTS)
		{
			uint8_t sel = Selector[5];
			
			if(FirstToMenu)
			{
				if(MenuState == 5)
				{
					Ymenu = 25; //Program Outputs
				}
				else if(MenuState == 6)
				{
					Ymenu = 36; //Hw Outputs Choice 
					Selector[MenuState] = Stng[MAIN_OUT_HWADD + sel + 1];
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu-1;
				
				FirstToMenu=0;
			}
			
			
			else if(MenuState == 6) {Stng[MAIN_OUT_HWADD + sel + 1]	= Selector[MenuState];}
			
			uint8_t mb = MenuBar[5];
			
			sprintf(MenuStr,"Prog    Board");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				sprintf(MenuStr,"%s",List_ProgMainOutputs[mbi]);
				LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 5)) );
				
				sprintf(MenuStr,"%s",List_HwMainOutputs[Stng[MAIN_OUT_HWADD + mbi + 1]]);
				LCD_WriteMenuStatus(57,Ylcd,MenuStr, (MenuState == 6) && (sel == mbi) );
			}
			
			lcdt = 100;
			SelChg(Ymenu-1,(MenuState == 5));
			BarChange(MenuState,2);
		}
		
		
		////////////////////////////////// Main Inputs Type
		else if(MENU_COND_HW_MAIN_IN_TYPE)
		{
			uint8_t sel = Selector[5];
			
			if(FirstToMenu)
			{
				if(MenuState == 5)
				{
					Ymenu = 20; //Program Inputs
				}
				else if(MenuState == 6)
				{
					Ymenu = 2; //Input Type
					Selector[MenuState] = Stng[MAIN_IN_TYPE_ADD + sel + 1];
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu-1;
				
				FirstToMenu=0;
			}
			
			
			else if(MenuState == 6) {Stng[MAIN_IN_TYPE_ADD + sel + 1]	= Selector[MenuState];}
			
			uint8_t mb = MenuBar[5];
			
			sprintf(MenuStr,"Input   Type");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				sprintf(MenuStr,"%s",List_ProgMainInputs[mbi]);
				LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 5)) );
				
				sprintf(MenuStr,"%s",List_HwType[Stng[MAIN_IN_TYPE_ADD + mbi + 1]]);
				LCD_WriteMenuStatus(49,Ylcd,MenuStr, (MenuState == 6) && (sel == mbi) );
			}
			
			lcdt = 100;
			SelChg(Ymenu-1,(MenuState == 5));
			BarChange(MenuState,2);
		}
		////////////////////////////////// Main Outputs Type
		else if(MENU_COND_HW_MAIN_OUT_TYPE)
		{
			uint8_t sel = Selector[5];
			
			if(FirstToMenu)
			{
				if(MenuState == 5)
				{
					Ymenu = 25; //Program Outputs
				}
				else if(MenuState == 6)
				{
					Ymenu = 2; //Output Type
					Selector[MenuState] = Stng[MAIN_OUT_TYPE_ADD + sel + 1];
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu-1;
				
				FirstToMenu=0;
			}
			
			
			else if(MenuState == 6) {Stng[MAIN_OUT_TYPE_ADD + sel + 1] 	= Selector[MenuState];}
			
			uint8_t mb = MenuBar[5];
			
			sprintf(MenuStr,"Output  Type");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				sprintf(MenuStr,"%s",List_ProgMainOutputs[mbi]);
				LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 5)) );
				
				sprintf(MenuStr,"%s",List_HwType[Stng[MAIN_OUT_TYPE_ADD + mbi + 1]]);
				LCD_WriteMenuStatus(57,Ylcd,MenuStr, (MenuState == 6) && (sel == mbi) );
			}
			
			lcdt = 100;
			SelChg(Ymenu-1,(MenuState == 5));
			BarChange(MenuState,2);
		}
		
		////////////////////////////////// Main Parallel Push Button
		else if(MENU_COND_HW_MAIN_PB_PARALLEL)
		{
			uint8_t sel = Selector[6];
			
			if(FirstToMenu)
			{
				if(MenuState == 6)
				{
					Ymenu = MAX_PARALLEL_PB;
				}
				else if(MenuState == 7)
				{
					Ymenu = 13; //Hw Inputs PB
					Selector[MenuState] = Stng[MAIN_PB_ADD + sel + 1];
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu-1;
				
				FirstToMenu=0;
			}
			
			
			else if(MenuState == 7) {Stng[MAIN_PB_ADD + sel + 1]	= Selector[MenuState];}
			
			uint8_t mb = MenuBar[6];
			
			sprintf(MenuStr,"PB    Terminal");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				sprintf(MenuStr,"PB%u",mbi+1);
				LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 6)) );
				
				sprintf(MenuStr,"%s",List_HwPBInputs[Stng[MAIN_PB_ADD + mbi + 1]]);
				LCD_WriteMenuStatus(49,Ylcd,MenuStr, (MenuState == 7) && (sel == mbi) );
			}
			
			lcdt = 100;
			SelChg(Ymenu-1,(MenuState == 6));
			BarChange(MenuState,2);
		}
		////////////////////////////////// Main Parallel Numrator
		else if(MENU_COND_HW_MAIN_NUM_PARALLEL)
		{
			uint8_t sel = Selector[6];
			
			if(FirstToMenu)
			{
				if(MenuState == 6)
				{
					Ymenu = 16; //Segments Outputs
				}
				else if(MenuState == 7)
				{
					Ymenu = 36; //Hw Outputs
					Selector[MenuState] = Stng[MAIN_NUM_ADD + sel + 1];//PrgNumAdd[sel+1];
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu-1;
				
				FirstToMenu=0;
			}
			
			
			else if(MenuState == 7) {Stng[MAIN_NUM_ADD + sel + 1]	= Selector[MenuState];}
			
			uint8_t mb = MenuBar[6];
			
			sprintf(MenuStr,"Seg    Out");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				sprintf(MenuStr,"%s",List_SegmentsOutputs[mbi]);
				LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 6)) );
				
				sprintf(MenuStr,"%s",List_HwMainOutputs[Stng[MAIN_NUM_ADD + mbi + 1]]);
				LCD_WriteMenuStatus(46,Ylcd,MenuStr, (MenuState == 7) && (sel == mbi) );
			}
			
			lcdt = 100;
			SelChg(Ymenu-1,(MenuState == 6));
			BarChange(MenuState,2);
		}
		
		
		////////////////////////////////// Carcodec Inputs Select
		else if(MENU_COND_HW_CAR_INPUTS)
		{
			uint8_t sel = Selector[5];
			
			if(FirstToMenu)
			{
				if(MenuState == 5)
				{
					Ymenu = 15; //Program Inputs
				}
				else if(MenuState == 6)
				{
					Ymenu = 20; //Hw Inputs Choice 
					Selector[MenuState] = Stng[CAR_IN_HWADD + sel + 1];
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu-1;
				
				FirstToMenu=0;
			}
			
			
			else if(MenuState == 6) {Stng[CAR_IN_HWADD + sel + 1]	= Selector[MenuState];}
			
			uint8_t mb = MenuBar[5];
			
			sprintf(MenuStr,"Prog   Board");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				sprintf(MenuStr,"%s",List_ProgCarInputs[mbi]);
				LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 5)) );
				
				sprintf(MenuStr,"%s",List_HwCarInputs[Stng[CAR_IN_HWADD + mbi + 1]]);
				LCD_WriteMenuStatus(49,Ylcd,MenuStr, (MenuState == 6) && (sel == mbi) );
			}
			
			lcdt = 100;
			SelChg(Ymenu-1,(MenuState == 5));
			BarChange(MenuState,2);
		}
		
		
		////////////////////////////////// Carcodec Outputs Select
		else if(MENU_COND_HW_CAR_OUTPUTS)
		{
			uint8_t sel = Selector[5];
			
			if(FirstToMenu)
			{
				if(MenuState == 5)
				{
					Ymenu = 15; //Program Outputs
				}
				else if(MenuState == 6)
				{
					Ymenu = 30; //Hw Outputs Choice 
					Selector[MenuState] = Stng[CAR_OUT_HWADD + sel + 1];
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu-1;
				
				FirstToMenu=0;
			}
			
			
			else if(MenuState == 6) {Stng[CAR_OUT_HWADD + sel + 1]	= Selector[MenuState];}
			
			uint8_t mb = MenuBar[5];
			
			sprintf(MenuStr,"Prog    Board");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				sprintf(MenuStr,"%s",List_ProgCarOutputs[mbi]);
				LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 5)) );
				
				sprintf(MenuStr,"%s",List_HwCarOutputs[Stng[CAR_OUT_HWADD + mbi + 1]]);
				LCD_WriteMenuStatus(57,Ylcd,MenuStr, (MenuState == 6) && (sel == mbi) );
			}
			
			lcdt = 100;
			SelChg(Ymenu-1,(MenuState == 5));
			BarChange(MenuState,2);
		}
		
		////////////////////////////////// Carcodec Inputs Type
		else if(MENU_COND_HW_CAR_IN_TYPE)
		{
			uint8_t sel = Selector[5];
			
			if(FirstToMenu)
			{
				if(MenuState == 5)
				{
					Ymenu = 15; //Program Inputs
				}
				else if(MenuState == 6)
				{
					Ymenu = 2; //Input Type
					Selector[MenuState] = Stng[CAR_IN_TYPE_ADD + sel + 1];
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu-1;
				
				FirstToMenu=0;
			}
			
			
			else if(MenuState == 6) {Stng[CAR_IN_TYPE_ADD + sel + 1]	= Selector[MenuState];}
			
			uint8_t mb = MenuBar[5];
			
			sprintf(MenuStr,"Input   Type");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				sprintf(MenuStr,"%s",List_ProgCarInputs[mbi]);
				LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 5)) );
				
				sprintf(MenuStr,"%s",List_HwType[Stng[CAR_IN_TYPE_ADD + mbi + 1]]);
				LCD_WriteMenuStatus(49,Ylcd,MenuStr, (MenuState == 6) && (sel == mbi) );
			}
			
			lcdt = 100;
			SelChg(Ymenu-1,(MenuState == 5));
			BarChange(MenuState,2);
		}
		////////////////////////////////// Carcodec Outputs Type
		else if(MENU_COND_HW_CAR_OUT_TYPE)
		{
			uint8_t sel = Selector[5];
			
			if(FirstToMenu)
			{
				if(MenuState == 5)
				{
					Ymenu = 15; //Program Outputs
				}
				else if(MenuState == 6)
				{
					Ymenu = 2; //Output Type
					Selector[MenuState] = Stng[CAR_OUT_TYPE_ADD + sel + 1];
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu-1;
				
				FirstToMenu=0;
			}
			
			
			else if(MenuState == 6) {Stng[CAR_OUT_TYPE_ADD + sel + 1] 	= Selector[MenuState];}
			
			uint8_t mb = MenuBar[5];
			
			sprintf(MenuStr,"Output  Type");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				sprintf(MenuStr,"%s",List_ProgCarOutputs[mbi]);
				LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 5)) );
				
				sprintf(MenuStr,"%s",List_HwType[Stng[CAR_OUT_TYPE_ADD + mbi + 1]]);
				LCD_WriteMenuStatus(57,Ylcd,MenuStr, (MenuState == 6) && (sel == mbi) );
			}
			
			lcdt = 100;
			SelChg(Ymenu-1,(MenuState == 5));
			BarChange(MenuState,2);
		}
		////////////////////////////////// Carcodec Parallel Push Button
		else if(MENU_COND_HW_CAR_PB_PARALLEL)
		{
			uint8_t sel = Selector[6];
			
			if(FirstToMenu)
			{
				if(MenuState == 6)
				{
					Ymenu = MAX_PARALLEL_PB;
				}
				else if(MenuState == 7)
				{
					Ymenu = 13; //Hw Inputs PB
					Selector[MenuState] = Stng[CAR_PB_ADD + sel + 1];
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu-1;
				
				FirstToMenu=0;
			}
			
			
			else if(MenuState == 7) {Stng[CAR_PB_ADD + sel + 1]	= Selector[MenuState];}
			
			uint8_t mb = MenuBar[6];
			
			sprintf(MenuStr,"PB    Terminal");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				sprintf(MenuStr,"PB%u",mbi+1);
				LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 6)) );
				
				sprintf(MenuStr,"%s",List_HwPBInputs[Stng[CAR_PB_ADD + mbi + 1]]);
				LCD_WriteMenuStatus(49,Ylcd,MenuStr, (MenuState == 7) && (sel == mbi) );
			}
			
			lcdt = 100;
			SelChg(Ymenu-1,(MenuState == 6));
			BarChange(MenuState,2);
		}
		////////////////////////////////// Carcodec Parallel Numrator
		else if(MENU_COND_HW_CAR_NUM_PARALLEL)
		{
			uint8_t sel = Selector[6];
			
			if(FirstToMenu)
			{
				if(MenuState == 6)
				{
					Ymenu = 16; //Segments Outputs
				}
				else if(MenuState == 7)
				{
					Ymenu = 36; //Hw Outputs
					Selector[MenuState] = Stng[CAR_NUM_ADD + sel + 1];//PrgNumAdd[sel+1];
				}
				
				if(Selector[MenuState] > Ymenu)  Selector[MenuState] = Ymenu-1;
				
				FirstToMenu=0;
			}
			
			
			else if(MenuState == 7) {Stng[CAR_NUM_ADD + sel + 1]	= Selector[MenuState];}
			
			uint8_t mb = MenuBar[6];
			
			sprintf(MenuStr,"Seg    Out");
			LCD_WriteMenuStatus(0,0,MenuStr,0);
			LCD_DrawFastHLine(0,16,96,1);
			
			
			for(i=0;(i<3);i++)
			{
				uint8_t mbi = mb+i;//Menu Bar i
				uint8_t Ylcd = HEIGHT_CHAR[i+1];
				sprintf(MenuStr,"%s",List_SegmentsOutputs[mbi]);
				LCD_WriteMenuStatus(0,Ylcd,MenuStr, ((sel == mbi) && (MenuState == 6)) );
				
				sprintf(MenuStr,"%s",List_HwCarOutputs[Stng[CAR_NUM_ADD + mbi + 1]]);
				LCD_WriteMenuStatus(46,Ylcd,MenuStr, (MenuState == 7) && (sel == mbi) );
			}
			
			lcdt = 100;
			SelChg(Ymenu-1,(MenuState == 6));
			BarChange(MenuState,2);
		}
		
		////////////////////////////////// Day Counter
		////////////////////////////////// Day Counter
		else if(MENU_COND_DAY_COUNTER)
		{
			uint8_t		NumSelect = MenuState - 3;

			if(MenuState <= 6)
			{
				if(FirstToMenu)
				{
					if(FirstInDayCounterMenu)
					{
						MakeDayCounterTemporaryID();
						CalculatePassword();
						FirstInDayCounterMenu = 0;
					}
					
					KeyTime[1] = 0;
					Selector[MenuState] = Pass_Num[NumSelect];
					FirstToMenu = 0;
				}
				else 
				{
					Pass_Num[NumSelect] = Selector[MenuState];
				}
			}
			
			//Check the entered password
			if(KeyTime[1] > 300)
			{
				uint32_t	EnteredPass = 0;
				for(uint8_t iw=0;iw<4;iw++)
				{
					EnteredPass += Pass_Num[iw];
					if(iw < 3) EnteredPass *= 10;
				}
				
				for(uint8_t	Del=0;Del<5;Del++)
				{
					WDT_RESET;
					HAL_Delay(10);
				}
				LCD_Clear();
				
				if(EnteredPass == DayCounterPassword50Day)
				{
					sprintf(MenuStr,"Activated");
					LCD_write_str_middle(17,MenuStr);
					sprintf(MenuStr,"50 Day");
					LCD_write_str_middle(34,MenuStr);
					
					DayCounter_Day = 50;
					DayCounter_Hour = 23;
					DayCounter_Minutes = 59;
					DayCounter_Status = DAY_COUNTER_STATUS_50_DAY;
					Error_DayCounter = 0;
					SavedPermanentPass = 0;
					
					MenuState = 0;
					SaveDayCounterValues();
					EEPROMSaveFlag = 1;
				}
				else if(EnteredPass == DayCounterPassword30Day)
				{
					sprintf(MenuStr,"Activated");
					LCD_write_str_middle(17,MenuStr);
					sprintf(MenuStr,"30 Day");
					LCD_write_str_middle(34,MenuStr);
					
					DayCounter_Day = 30;
					DayCounter_Hour = 23;
					DayCounter_Minutes = 59;
					DayCounter_Status = DAY_COUNTER_STATUS_30_DAY;
					Error_DayCounter = 0;
					SavedPermanentPass = 0;
					
					MenuState = 0;
					SaveDayCounterValues();
					EEPROMSaveFlag = 1;
				}
				else if(EnteredPass == DayCounterPasswordPermanent)
				{
					sprintf(MenuStr,"Activated");
					LCD_write_str_middle(17,MenuStr);
					sprintf(MenuStr,"Permanent");
					LCD_write_str_middle(34,MenuStr);
					
					DayCounter_Day = 100;
					DayCounter_Hour = 23;
					DayCounter_Minutes = 59;
					DayCounter_Status = DAY_COUNTER_STATUS_PERMANENT;
					Error_DayCounter = 0;
					SavedPermanentPass = DayCounterPasswordPermanent;
					
					MenuState = 0;
					SaveDayCounterValues();
					EEPROMSaveFlag = 1;
				}
				else 
				{
					sprintf(MenuStr,"Password");
					LCD_write_str_middle(17,MenuStr);
					sprintf(MenuStr,"Wrong!");
					LCD_write_str_middle(34,MenuStr);
				}
				
				LCD_Update();
				for(uint8_t	Del=0;Del<200;Del++)
				{
					WDT_RESET;
					HAL_Delay(10);
				}
			}
			//Exit from DayCounter
			if(KeyTime[3] > 300) 
			{
				MenuState = 2;
			}
			
			if(DayCounter_Status == DAY_COUNTER_STATUS_50_DAY) 					sprintf(MenuStr,"Status: %u/50",DayCounter_Day);
			else if(DayCounter_Status == DAY_COUNTER_STATUS_30_DAY) 		sprintf(MenuStr,"Status: %u/30",DayCounter_Day);
			else if(DayCounter_Status == DAY_COUNTER_STATUS_PERMANENT) 	sprintf(MenuStr,"Status: OPEN");
			LCD_write_str(1,1,MenuStr);
			sprintf(MenuStr,"ID: %u",TemporaryID);
			LCD_write_str(1,17,MenuStr);
			sprintf(MenuStr,"Serial: %u",BoardFixedID);
			LCD_write_str(1,34,MenuStr);
			
			sprintf(MenuStr,"Pass:");
			LCD_write_str(1,51,MenuStr);
			sprintf(MenuStr,"%u",Pass_Num[0]);
			LCD_WriteMenuStatus(34,51,MenuStr,NumSelect == 0);
			sprintf(MenuStr,"%u",Pass_Num[1]);
			LCD_WriteMenuStatus(46,51,MenuStr,NumSelect == 1);
			sprintf(MenuStr,"%u",Pass_Num[2]);
			LCD_WriteMenuStatus(58,51,MenuStr,NumSelect == 2);
			sprintf(MenuStr,"%u",Pass_Num[3]);
			LCD_WriteMenuStatus(70,51,MenuStr,NumSelect == 3);
			//sprintf(MenuStr,"--");
			//LCD_write_str(34+(NumSelect*11),57,MenuStr);
			
			lcdt = 100;
			SelChg(9,0);

			if(MenuState > 6) MenuState = 3;
			ExitWithTimerF = (MenuState == 3);
		}
		
		
		
		else if(MenuState > 3) 
		{
			     if(Selector[1]==1 && Selector[2]==2 &&  Selector[3]==2 &&  Selector[4]==1) {MenuState -=3;} // 1CF_UP_DN_DELAY
			else if(Selector[1]==0 && Selector[2]==4 &&  Selector[3]==2 &&  Selector[4]==1) {MenuState -=4;} // Door Select
			else if(Selector[1]==0 && Selector[2]==2) 																			{MenuState -=4;} // Num & Talk
			//else if(Selector[1]==0 && Selector[2]==9 && Selector[3]==1)											{MenuState -=2;} // Select Speed
			else if(Selector[1]==0 && Selector[2]==9 && Selector[3]==2)											{MenuState -=3;} // CF3 Flag
			else if(Selector[1]==1 && Selector[2]==1	&& Selector[3]==0 && Selector[4]==6 && Selector[5]==1) {MenuState -=4;} // Drive Control
			//else if(Selector[1]==0 && Selector[2]==9 && Selector[3]==7)	{MenuState -=2;}//Level Distance
			
			else MenuState -=2;
		}
		else if(MenuState > 1) MenuState -=1;
		
		
		if(!MENU_COND_DAY_COUNTER) 
		{
			ExitWithTimerF = 0;
			FirstInDayCounterMenu = 1;
		}
	}
	if(MenuState > -3) {MENU_BACK}
}


