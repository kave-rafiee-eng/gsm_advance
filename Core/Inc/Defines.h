#include "main.h"

#define COMPILE_DAY			6
#define COMPILE_MONTH		6
#define	COMPILE_YEAR		1403

//#define TYPE_HYDRAULIC_DEFINE		1
//#define TYPE_3VF_DEFINE					1
//#define TYPE_2SPEED_DEFINE			1

#define DONT_OPEN_DOOR_AFTER_RELEVER_IN_HYD 	0
#define DONT_OPEN_DOOR_AFTER_PARK_IN_HYD     	0
#define STOPPING_TIME_BEFORE_RELEVEL_IN_HYD		15 //in sec

#define LCD_BACKLIGHT_TIME	1200		// 120 Sec
#define TO_CLOSE_66_TIME		700  //7 sec
#define CANCEL_PUSHB_TIME  	20 //*10 = 200ms

#define MAX_NOS  	60 //MAX Number Of Stop

#define	TURN_ON_BOARD_TIME	10 //1 Sec

#define ERROR_LIST_SIZE			70 		//It should not be more than 95
#define SETTING_ARRAY_SIZE  800	//It should not be more than 3499

#define	FTO_COOL 			0
#define FTO_WARM			1
#define FTO_HOT				2

#define NO_AIM           		255

#define SERVICE_TYPE_COLLECTIVE_DOWN		0
#define SERVICE_TYPE_FULL_COLLECTIVE		1
#define SERVICE_TYPE_PUSH_BUTTON				2

#define DOOR_CONTROL_TYPE_LOADED		0
#define DOOR_CONTROL_TYPE_69				1
#define DOOR_CONTROL_TYPE_68				2

#define STOP					0
#define SLOW					1
#define REV						2
#define MEDIUM				3
#define FAST					4
#define FORCE_STOP		5

#define NORMAL_OPEN		0
#define NORMAL_CLOSE	1

#define DISABLE 0
#define ENABLE	1


#define DOWN				0
#define UP 					1
#define MOVE_RESET	2

#define	RUN			1

#define OPEN		0
#define CLOSE		1

#define LEVEL				(In1CF == 0)
#define NO_LEVEL		(In1CF == 1)

#define DAY_COUNTER_STATUS_50_DAY			0
#define	DAY_COUNTER_STATUS_30_DAY			1
#define	DAY_COUNTER_STATUS_PERMANENT	2

#define RESET_MODE       0
#define MODE_REVESION    1
#define MODE_CALIBRATION 2
#define MODE_NORMAL      3
#define MODE_ERROR       4
#define MODE_EMERGENCY   5
#define MODE_START       6

#define PLAY_FLOOR_NUM     100
#define PLAY_MUSIC         120
#define PLAY_DISTURB_DOOR  114
#define PLAY_EX_CAPACITY   113
#define PLAY_EMERGENCY     111
#define PLAYER_STOP        130

#define TYPE_3VF		0
#define TYPE_HYD		1
#define	TYPE_2SP		2
#define TYPE_HEVOS	3

#define PB_TYPE_PARALLEL				0
#define PB_TYPE_SERIAL_EXT			1
#define PB_TYPE_SERIAL_PB				2
#define PB_TYPE_DOUBLEX_MASTER	3

#define NUM_TYPE_PARALLEL				0
#define NUM_TYPE_SERIAL_PB			1

#define	DRIVE_CONNECTION_PARALLEL	0
#define DRIVE_CONNECTION_ANALOG		1
#define	DRIVE_CONNECTION_485_DCP3	2
#define	DRIVE_CONNECTION_485_QMA	3





//Hardware+LCD+Menu+EEPROM	52.0%
//Fixed ID:    							1.5% -> 53.5%
//Day Counter: 							2.8% -> 56.3%
//CT Current Meas:					1.3% -> 57.6%
//Cabin Serial:							2.0% -> 59.6%
//Data Lost:								0.6% ->	61.2%
//HYD Valve Contorl: 				3.4% -> 64.6%  3VF:63.2%  2Speed:64.0%
//Door Control & PHC:				2.0% -> 66.6%
//Errors & Clear PB:				0.7% -> 67.3%
//Rev & 1cf/3cf Counter & Landing & Calibration & Relevel: 	4.5% -> 71.8%
//Normal Simple Serivice + Travel time + CF3 Delay:					2.2% -> 74.0%
//Collective Down & Full Collective & PushButton:						1.8% -> 75.8%
//Fire ,OVL ,GoToPark ,DoorPark:														0.5% -> 76.3%
//Standby ,FTO ,Emergency																		1.2% -> 77.5%
//Error_MotorSafe ,Error_DayCounte ,ExitFromMenu ,StartMode 1.2% -> 78.7%
//Errors & Alarm Show																				3.4% -> 82.1%
//FLTDRV ,EndDoorTime, Safety90, DoorOpenTime, Door68&69, ResetPBTimer, Cut 1CF-CF3: 0.5% -> 82.6%
//Activity:									0.8% ->	83.4%
//Hardware Input display:		1.5% -> 84.9%
//Set PB with menu:					5.9% -> 90.8%





