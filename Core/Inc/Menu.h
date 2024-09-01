#include "main.h"
#include "n1202.h"

//((MenuState != 0 && EnterWithTimerF == 0) || KeyTime[1] > 300)
#define MENU_ENT  {  if( K_Ent && (EnterWithTimerF == 0 || KeyTime[1] > 300) ) {MenuState++; Selector[MenuState]=0; MenuBar[MenuState]=0; FirstToMenu=1; K_Ent=0;} }
#define MENU_BACK {  if( K_Ext && (ExitWithTimerF  == 0 || KeyTime[3] > 300) ) {if(MenuState==1) {EEPROMSaveFlag=1;} MenuState--; FirstToMenu=1;	K_Ext=0;} }

void Keys4Menu(void);
void Menu(void);
void SelChg(uint8_t Max,uint8_t Dir);
void SetCarcodecTransferHWAddressArray(void);

///Menu Condition 
#define MENU_COND_MAINMENU             		(MenuState==1)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////   GENERAL MENU   ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	#define MENU_COND_GENERAL     						(MenuState==2 		&& Selector[1]==0) //General

		#define MENU_COND_NUM_OF_STOP     				(MenuState==3 	&& Selector[1]==0 && Selector[2]==0) 	//Num Of Stop
		
		#define MENU_COND_SERVICE_TYPE     				(MenuState==3 	&& Selector[1]==0 && Selector[2]==1)  //Service Type


		#define MENU_COND_NUM_TALK     						(MenuState>=3 && MenuState<=6	&& Selector[1]==0 && Selector[2]==2) 	//Num & Talk

		#define MENU_COND_TRAVEL_TIME     				(MenuState==3 	&& Selector[1]==0 && Selector[2]==3)  //Travel Time

		#define MENU_COND_DOOR	     							(MenuState==3 	&& Selector[1]==0 && Selector[2]==4)  //Door
			#define MENU_COND_DOOR_TIME								(MenuState==4	&& Selector[1]==0 && Selector[2]==4 && Selector[3]==0) //Door Time
					#define MENU_COND_DOOR_OPEN_DELAY				(MenuState==5 && Selector[1]==0 && Selector[2]==4 && Selector[3]==0 &&  Selector[4]==0) //Door Open Delay
					#define MENU_COND_DOOR_CLOSE_DELAY			(MenuState==5 && Selector[1]==0 && Selector[2]==4 && Selector[3]==0 &&  Selector[4]==1) //Door Close Delay
					#define MENU_COND_END_DOOR_TIME					(MenuState==5 && Selector[1]==0 && Selector[2]==4 && Selector[3]==0 &&  Selector[4]==2) //End Door Time
					#define MENU_COND_DOOR_CLOSE_TIMEOUT		(MenuState==5 && Selector[1]==0 && Selector[2]==4 && Selector[3]==0 &&  Selector[4]==3) //Door Close Timeout
					#define MENU_COND_DOOR_PARK_TIME				(MenuState==5 && Selector[1]==0 && Selector[2]==4 && Selector[3]==0 &&  Selector[4]==4) //Door Park Time
					#define MENU_COND_69_DEBOUNCER					(MenuState==5 && Selector[1]==0 && Selector[2]==4 && Selector[3]==0 &&  Selector[4]==5) //69 Debouncer
					#define MENU_COND_68_DEBOUNCER					(MenuState==5 && Selector[1]==0 && Selector[2]==4 && Selector[3]==0 &&  Selector[4]==6) //68 Debouncer
					#define MENU_COND_DOOR_OPEN_TIME				(MenuState==5 && Selector[1]==0 && Selector[2]==4 && Selector[3]==0 &&  Selector[4]==7) //Door Open Time
			#define MENU_COND_DOOR_CONTROL_TYPE				(MenuState==4	&& Selector[1]==0 && Selector[2]==4 && Selector[3]==1) //Door Control Type
			#define MENU_COND_DOOR_SECOND							(MenuState==4	&& Selector[1]==0 && Selector[2]==4 && Selector[3]==2) //Second Door
				#define MENU_COND_NUM_OF_DOOR							(MenuState==5 && Selector[1]==0 && Selector[2]==4 && Selector[3]==2 &&  Selector[4]==0) //Num Of Door
				#define MENU_COND_DOOR_SELECT							((MenuState>=5 && MenuState<=8) && Selector[1]==0 && Selector[2]==4 &&  Selector[3]==2 &&  Selector[4]==1) //Door Select

		#define MENU_COND_SOUND	     							(MenuState==3 	&& Selector[1]==0 && Selector[2]==5)  //Sound
			#define MENU_COND_MUSIC_VOLUME						(MenuState==4	&& Selector[1]==0 && Selector[2]==5 && Selector[3]==0) //Music Volume
			#define MENU_COND_TALK_VOLUME							(MenuState==4	&& Selector[1]==0 && Selector[2]==5 && Selector[3]==1) //Talk Volume
			#define MENU_COND_WELCOME_FLOOR						(MenuState==4	&& Selector[1]==0 && Selector[2]==5 && Selector[3]==2) //Welcom Floor
			#define MENU_COND_GANG_SELECT							(MenuState==4	&& Selector[1]==0 && Selector[2]==5 && Selector[3]==3) //Gang Select
			
		#define MENU_COND_FLOOR	     							(MenuState==3 	&& Selector[1]==0 && Selector[2]==6)  //Floor
			#define MENU_COND_PARK_FLOOR	     				(MenuState==4	&& Selector[1]==0 && Selector[2]==6 && Selector[3]==0) //Park Floor
			#define MENU_COND_TIME_TO_PARK	     			(MenuState==4	&& Selector[1]==0 && Selector[2]==6 && Selector[3]==1) //Time To Park
			#define MENU_COND_FIRE_FLOOR	     				(MenuState==4	&& Selector[1]==0 && Selector[2]==6 && Selector[3]==2) //Fire Floor
			#define MENU_COND_WAITING_FLOOR	     			(MenuState==4	&& Selector[1]==0 && Selector[2]==6 && Selector[3]==3) //Waiting Floor
			
		#define MENU_COND_MOTOR_SAFE							(MenuState==3 	&& Selector[1]==0 && Selector[2]==7)  //Motor Safe
			#define MENU_COND_MOTOR_SAFE_ENABLE				(MenuState==4 && Selector[1]==0 && Selector[2]==7 && Selector[3]==0) 		//Motor Safe Enable
			#define MENU_COND_FAST_OC									(MenuState==4 && Selector[1]==0 && Selector[2]==7 && Selector[3]==1) 		//Fast Over Current
			#define MENU_COND_SLOW_OC									(MenuState==4 && Selector[1]==0 && Selector[2]==7 && Selector[3]==2) 		//Slow Over Current
			#define MENU_COND_TIME_OC									(MenuState==4 && Selector[1]==0 && Selector[2]==7 && Selector[3]==3) 		//Time Over Current
			
		#define MENU_COND_HYD_SETTING							(MenuState==3 	&& Selector[1]==0 && Selector[2]==8)	//HYD Setting
			#define MENU_COND_HYD_START_SLOW_DEL			(MenuState==4 && Selector[1]==0 && Selector[2]==8 && Selector[3]==0) 		//Start Slow Delay
			#define MENU_COND_HYD_START_FAST_DEL			(MenuState==4 && Selector[1]==0 && Selector[2]==8 && Selector[3]==1) 		//Start Fast Delay
			#define MENU_COND_HYD_STAR_TO_DELTA				(MenuState==4 && Selector[1]==0 && Selector[2]==8 && Selector[3]==2) 		//Star To Delta
			#define MENU_COND_MOTOR_START_DEL					(MenuState==4 && Selector[1]==0 && Selector[2]==8 && Selector[3]==3) 		//Motor Start Delay
			#define MENU_COND_MOTOR_STOP_DEL					(MenuState==4 && Selector[1]==0 && Selector[2]==8 && Selector[3]==4) 		//Motor Stop Delay
			
			
		#define MENU_COND_LANDING									(MenuState==3 	&& Selector[1]==0 && Selector[2]==9)	//Landing
			#define MENU_COND_LANDING_BASED						(MenuState==4 && Selector[1]==0 && Selector[2]==9 && Selector[3]==0)	//Landing Based On
			#define MENU_COND_JUMP1_SPEED							((MenuState>=4 && MenuState<=5) && Selector[1]==0 && Selector[2]==9 && Selector[3]==1)	//Select Speed
			#define MENU_COND_CF3_FLAG								((MenuState>=4 && MenuState<=6) && Selector[1]==0 && Selector[2]==9 && Selector[3]==2)	//CF3 Flag
			#define MENU_COND_CF3_DELAY								(MenuState==4 && Selector[1]==0 && Selector[2]==9 && Selector[3]==3)	//CF3 Delay
			#define MENU_COND_MOTOR_SPEED							(MenuState==4 && Selector[1]==0 && Selector[2]==9 && Selector[3]==4)	//Motor Speed
			#define MENU_COND_ACCELERATION						(MenuState==4 && Selector[1]==0 && Selector[2]==9 && Selector[3]==5)	//Acceleration
			#define MENU_COND_DECCELERATION						(MenuState==4 && Selector[1]==0 && Selector[2]==9 && Selector[3]==6)	//Decceleration
			#define MENU_COND_LEVEL_DISTANCE					((MenuState>=4 && MenuState<=5) && Selector[1]==0 && Selector[2]==9 && Selector[3]==7)	//Level Distance
			
		#define MENU_COND_CALIBRATION							(MenuState==3 	&& Selector[1]==0 && Selector[2]==10)	//Calibration
			#define MENU_COND_CALIBRATION_DIR					(MenuState==4 && Selector[1]==0 && Selector[2]==10 && Selector[3]==0)	//Calibration Direction
			#define MENU_COND_CALIBRATION_SPEED				(MenuState==4 && Selector[1]==0 && Selector[2]==10 && Selector[3]==1) //Calibration Speed
		
		
		#define MENU_COND_EMERGENCY								(MenuState==3 	&& Selector[1]==0 && Selector[2]==11)	//Emergency
			#define MENU_COND_EMERGENCY_ENABLE				(MenuState==4 && Selector[1]==0 && Selector[2]==11 && Selector[3]==0) //Emergency Enable
			#define MENU_COND_EMERGENCY_DIR						(MenuState==4 && Selector[1]==0 && Selector[2]==11 && Selector[3]==1) //Emergency Directiorn
		
		#define MENU_COND_1CF_ERROR								(MenuState==3 	&& Selector[1]==0 && Selector[2]==12)	//1CF Error
			#define MENU_COND_1CF_ERROR_ENABLE	 			(MenuState==4 && Selector[1]==0 && Selector[2]==12 && Selector[3]==0) //1CF Error Enable
			#define MENU_COND_1CF_ERROR_TIMEOUT	 			(MenuState==4 && Selector[1]==0 && Selector[2]==12 && Selector[3]==1) //1CF Error Timeout
			
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////   ADVANCE MENU   ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	#define MENU_COND_ADVANCE     						(MenuState==2 		&& Selector[1]==1) //Advance

		#define MENU_COND_TYPE_ELEVATOR	     			(MenuState==3 	&& Selector[1]==1 && Selector[2]==0) 	//Type Elevator
		
		#define MENU_COND_HARDWARE								(MenuState==3 	&& Selector[1]==1 && Selector[2]==1) 	//Hardware
			#define MENU_COND_HW_MAINBOARD						(MenuState==4	&& Selector[1]==1 && Selector[2]==1 && Selector[3]==0)	//Main Board HW
				#define MENU_COND_HW_MAIN_INPUTS					((MenuState>=5 && MenuState<=6)	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==0 && Selector[4]==0)	//Main Inputs
				#define MENU_COND_HW_MAIN_OUTPUTS					((MenuState>=5 && MenuState<=6)	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==0 && Selector[4]==1)	//Main outputs
				#define MENU_COND_HW_MAIN_IN_TYPE					((MenuState>=5 && MenuState<=6)	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==0 && Selector[4]==2)	//Main Inputs Type
				#define MENU_COND_HW_MAIN_OUT_TYPE				((MenuState>=5 && MenuState<=6)	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==0 && Selector[4]==3)	//Main outputs Type
				#define MENU_COND_HW_MAIN_PUSHBUTTON			(MenuState==5	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==0 && Selector[4]==4)	//Main Push Button
					#define MENU_COND_HW_MAIN_PB_TYPE					(MenuState==6	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==0 && Selector[4]==4 && Selector[5]==0)	//Main PB Type
					#define MENU_COND_HW_MAIN_PB_PARALLEL			((MenuState>=6 && MenuState<=7)	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==0 && Selector[4]==4 && Selector[5]==1)	//Main Parallel PB
					#define MENU_COND_HW_MAIN_PB_CANCEL_ENABLE (MenuState==6	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==0 && Selector[4]==4 && Selector[5]==2)	//Main Cancel Time
				#define MENU_COND_HW_MAIN_NUMRATOR				(MenuState==5	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==0 && Selector[4]==5)	//Main Numrator
					#define MENU_COND_HW_MAIN_NUM_TYPE				(MenuState==6	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==0 && Selector[4]==5 && Selector[5]==0)	//Main Num Type
					#define MENU_COND_HW_MAIN_NUM_PARALLEL		((MenuState>=6 && MenuState<=7)	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==0 && Selector[4]==5 && Selector[5]==1)	//Main Parallel Num
					#define MENU_COND_MAIN_NUM_SLEEP_LIGHT		(MenuState==6	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==0 && Selector[4]==5 && Selector[5]==2)	//Main Num sleep light
					#define MENU_COND_MAIN_NUM_ACTIVE_LIGHT		(MenuState==6	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==0 && Selector[4]==5 && Selector[5]==3)	//Main Num active light
					
				#define MENU_COND_HW_DRIVE								(MenuState==5	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==0 && Selector[4]==6)	//Drive
					#define MENU_COND_HW_DRIVE_CONNCTION			(MenuState==6	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==0 && Selector[4]==6 && Selector[5]==0)	//Drive Connection
					#define MENU_COND_HW_DRIVE_CONTROL				((MenuState>=6 && MenuState<=9)	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==0 && Selector[4]==6 && Selector[5]==1)	//Drive Control
					
			#define MENU_COND_HW_CARCODEC							(MenuState==4	&& Selector[1]==1 && Selector[2]==1 && Selector[3]==1)	//Carcodec HW
				#define MENU_COND_HW_CAR_INPUTS						((MenuState>=5 && MenuState<=6)	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==1 && Selector[4]==0)	//CAR Inputs
				#define MENU_COND_HW_CAR_OUTPUTS					((MenuState>=5 && MenuState<=6)	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==1 && Selector[4]==1)	//CAR outputs
				#define MENU_COND_HW_CAR_IN_TYPE					((MenuState>=5 && MenuState<=6)	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==1 && Selector[4]==2)	//CAR Inputs Type
				#define MENU_COND_HW_CAR_OUT_TYPE					((MenuState>=5 && MenuState<=6)	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==1 && Selector[4]==3)	//CAR outputs Type
				#define MENU_COND_HW_CAR_PUSHBUTTON				(MenuState==5	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==1 && Selector[4]==4)	//CAR Push Button
					#define MENU_COND_HW_CAR_PB_TYPE					(MenuState==6	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==1 && Selector[4]==4 && Selector[5]==0)	//CAR PB Type
					#define MENU_COND_HW_CAR_PB_PARALLEL			((MenuState>=6 && MenuState<=7)	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==1 && Selector[4]==4 && Selector[5]==1)	//CAR Parallel PB
					#define MENU_COND_HW_CAR_PB_CANCEL_ENABLE	(MenuState==6	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==1 && Selector[4]==4 && Selector[5]==2)	//CAR Cancel Time
				#define MENU_COND_HW_CAR_NUMRATOR				(MenuState==5	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==1 && Selector[4]==5)	//CAR Numrator
					#define MENU_COND_HW_CAR_NUM_TYPE					(MenuState==6	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==1 && Selector[4]==5 && Selector[5]==0)	//CAR Num Type
					#define MENU_COND_HW_CAR_NUM_PARALLEL			((MenuState>=6 && MenuState<=7)	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==1 && Selector[4]==5 && Selector[5]==1)	//CAR Parallel Num
					#define MENU_COND_CAR_NUM_SLEEP_LIGHT			(MenuState==6	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==1 && Selector[4]==5 && Selector[5]==2)	//CAR Num sleep light
					#define MENU_COND_CAR_NUM_ACTIVE_LIGHT		(MenuState==6	&& Selector[1]==1 && Selector[2]==1	&& Selector[3]==1 && Selector[4]==5 && Selector[5]==3)	//CAR Num active light
					
		#define MENU_COND_TIMER_SETTING						(MenuState==3 	&& Selector[1]==1 && Selector[2]==2) 	//Timer Setting
			#define MENU_COND_UD_RELEASE_DELAY	 			(MenuState==4	&& Selector[1]==1 && Selector[2]==2 && Selector[3]==0) //U/D Release Delay
			#define MENU_COND_TC_RELEASE_DELAY	 			(MenuState==4	&& Selector[1]==1 && Selector[2]==2 && Selector[3]==1) //TC Release Delay
			#define MENU_COND_1CF_DELAY								(MenuState==4	&& Selector[1]==1 && Selector[2]==2 && Selector[3]==2)	//1CF Delay
				#define MENU_COND_1CF_ALL_DELAY	 					(MenuState==5 && Selector[1]==1 && Selector[2]==2 &&  Selector[3]==2 &&  Selector[4]==0) //1CF All Floors Delay
				#define MENU_COND_1CF_UP_DN_DELAY	 				((MenuState>=5 && MenuState<=7) && Selector[1]==1 && Selector[2]==2 &&  Selector[3]==2 &&  Selector[4]==1) //1CF UP/DOWN Delay
			#define MENU_COND_LEVELING_TIME						(MenuState==4	&& Selector[1]==1 && Selector[2]==2 && Selector[3]==3) //Leveling Time
			#define MENU_COND_STANDBY_TIME						(MenuState==4	&& Selector[1]==1 && Selector[2]==2 && Selector[3]==4) //Standby Time
			#define MENU_COND_URA_DELAY								(MenuState==4	&& Selector[1]==1 && Selector[2]==2 && Selector[3]==5) //URA Delay
			#define MENU_COND_RELAY_TIME							(MenuState==4	&& Selector[1]==1 && Selector[2]==2 && Selector[3]==6) //Relay Delay&TIME
				#define MENU_COND_RELAY_ON_DELAY	 				(MenuState==5 && Selector[1]==1 && Selector[2]==2 &&  Selector[3]==6 &&  Selector[4]==0)	//Relay On Delay
				#define MENU_COND_RELAY_ON_TIME	 					(MenuState==5 && Selector[1]==1 && Selector[2]==2 &&  Selector[3]==6 &&  Selector[4]==1)	//Relay On Time
				
		#define MENU_COND_PHASE_CONTROL						(MenuState==3 	&& Selector[1]==1 && Selector[2]==3) 	//Phase Control
			#define MENU_COND_PHASE_REVERSE						(MenuState==4 && Selector[1]==1 && Selector[2]==3 && Selector[3]==0)	//Phase Reverse
			#define MENU_COND_PHASE_FAULT							(MenuState==4 && Selector[1]==1 && Selector[2]==3 && Selector[3]==1) 	//Phase Fault
			#define MENU_COND_ONE_PHASE								(MenuState==4 && Selector[1]==1 && Selector[2]==3 && Selector[3]==2) 	//One Phase
			
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////   INFORMATION MENU   //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	#define MENU_COND_INFORMATION     				(MenuState==2 		&& Selector[1]==2) //Advance

		#define MENU_COND_DAY_COUNTER	     				(MenuState>=3 	&& Selector[1]==2 && Selector[2]==0) //Day Counter
		
		#define MENU_COND_STARTING_LOG	     			(MenuState==3 	&& Selector[1]==2 && Selector[2]==1) //Start Counter
		
		#define MENU_COND_PROGRAM_DATE	    			(MenuState==3 	&& Selector[1]==2 && Selector[2]==3) //Program Date
		
		
		
		
		
		
		
		
		
		
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////   Setting & EEPROM Array Address   ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		

/////////////////////////	For Stng & EEPROM Arrays
//Size: 1 byte
#define	TYPE_ELEVATOR					1
#define NUM_OF_STOP						2
#define	SERVICE_TYPE					3
#define	DOOR_CONTROL_TYPE			4
#define	ENABLE_MOTOR_SAFE			5
#define CALIBRATION_DIR				6
#define	CALIBRATION_SPEED			7
#define	ENABLE_1CF_ERROR			8
#define	ENABLE_EMERGENCY			9
#define	EMERGENCY_DIR					10
#define	ENABLE_PHASE_REVERSE	11
#define ENABLE_PHASE_FAULT		12
#define	ENABLE_ONE_PHASE			13
#define TRAVEL_TIME						14
#define	DOOR_OPEN_DELAY				15
#define	DOOR_CLOSE_DELAY			16
#define	DOOR_END_TIME					17
#define	DOOR_CLOSE_TIMEOUT		18
#define	DOOR_PARK_TIME				19
#define	DEBOUNCER69						20
#define	DEBOUNCER68						21
#define	NUM_OF_DOOR						22
#define	MUSIC_VOLUME					23
#define	TALK_VOLUME						24
#define	WELCOME_FLOOR					25
#define	GANG_SELECT						26
#define	DOOR_OPEN_TIME				27
#define	PARK_FLOOR						28
#define	TIME_TO_PARK					29
#define	FIRE_FLOOR						30
#define	WAITING_FLOOR					31
#define	UD_RELEASE_DELAY			32
#define	TC_RELEASE_DELAY			33
#define	LEVELING_TIME					34
#define	STANDBY_TIME					35
#define	MOTOR_FAN_TIME				36
#define	URA_DELAY							37
#define	CF3_DELAY							38
#define	ERROR_1CF_TIMEOUT			39
#define	FAST_OVERCURRENT			40
#define	SLOW_OVERCURRENT			41
#define	TIME_OVERCURRENT			42
#define	START_SLOW_DELAY			43
#define	START_FAST_DELAY			44
#define	START_TO_DELTA				45
#define	MOTOR_START_DELAY			46
#define	MOTOR_STOP_DELAY			47
#define	ALL_FLOOR_1CF_DELAY		48
#define	MAIN_PB_TYPE					49
#define	CAR_PB_TYPE						50
#define	MAIN_NUM_TYPE					51
#define	CAR_NUM_TYPE					52
#define	DRIVE_CONNECTION			53
#define	MOTOR_SPEED						54
#define	ACCELERATION					55
#define	DECCELERATION					56
#define	DAY_COUNTER_DAY				57
#define	DAY_COUNTER_HOUR			58
#define	DAY_COUNTER_MINUTES		59
#define	DAY_COUNTER_STATUS		60
#define	LANDING_BASED					61
#define	MAIN_NUM_SLEEP_LIGHT	62
#define	MAIN_NUM_ACTIVE_LIGHT	63
#define	CAR_NUM_SLEEP_LIGHT		64
#define	CAR_NUM_ACTIVE_LIGHT	65
#define RELAY_ON_DELAY				66
#define RELAY_ON_TIME					67
#define MAIN_PB_CANCEL_ENABLE	68
#define CAR_PB_CANCEL_ENABLE	69

//Size: 50 byte
#define	MAIN_IN_TYPE_ADD			100
#define	MAIN_OUT_TYPE_ADD			150
#define MAIN_IN_HWADD					200
#define	MAIN_OUT_HWADD				250
#define	MAIN_PB_ADD						300
#define MAIN_NUM_ADD					350
#define	CAR_IN_TYPE_ADD				400
#define	CAR_OUT_TYPE_ADD			450
#define CAR_IN_HWADD					500
#define	CAR_OUT_HWADD					550
#define	CAR_PB_ADD						600
#define CAR_NUM_ADD						650 


////////////////////////////	Just For EEPROM Array
//Size: MAX_NOS byte
#define DELAY_1CF_UP_ADD			700
#define DELAY_1CF_DN_ADD			780	//(DELAY_1CF_UP_ADD + MAX_NOS)
#define SEGMENTS_L_ADD				860	//(DELAY_1CF_DN_ADD + MAX_NOS)
#define SEGMENTS_R_ADD				940	//(SEGMENTS_L_ADD 	+ MAX_NOS)
#define TALK_ADD							1020//(SEGMENTS_R_ADD 	+ MAX_NOS)
#define JUMP1_SPEED						1100//(TALK_ADD 	+ MAX_NOS)
#define JUMP1_FLAG						1180//(JUMP1_SPEED 				+ MAX_NOS)
#define	JUMPN_FLAG						1260//(JUMP1_FLAG 			+ MAX_NOS)
#define LEVEL_DISTANCE_ADD		1340//(JUMPN_FLAG 			+ MAX_NOS)

#define DOOR_SELECT_ADD				1420//(LEVEL_DISTANCE_ADD 			+ MAX_NOS)


#define DRIVE_PARALLEL_ADD		1500	//(LEVEL_DISTANCE_ADD + MAX_NOS) //Size: 10 byte max //3 values (V0,V1,V2) together in one byte








		
		
		

		
		
		
		
		
		
		
		
		
