#include "main.h"
#include "test.h"
#include "stdio.h"


#include "../Inc/Variable.h"
#include "../Inc/Serial.h"

#define travel_time_addres "general*travel_time"
#define door_control_type_ADDRESS "general*door*control_type"

extern char List_DoorControlType[][8];
	
extern volatile	uint8_t			Stng[SETTING_ARRAY_SIZE];
extern volatile  uint8_t   	EEPROMSaveFlag;

extern char 									HALL_TX_Buffer[UART_BUFFER_SIZE];
extern volatile	char				HALL_RX_Buffer[UART_BUFFER_SIZE];

extern struct cpu_timer_basic_10bit_auto_reset tbr_g1[def_num_tbr_g1];


char flag_test=0;
int i_get=0;
char last_get=0;

extern struct JSON_OUT	 json;
extern struct JSON_PROTOCOL json_protocol;

int time=0;
int timer_eeprom=0;

void test(){
	
	
	tbr_g1[tbr_g1_TEST].EN=1;
	tbr_g1[tbr_g1_TEST].C_set_time=10;
	
	if( tbr_g1[tbr_g1_TEST].F_end ){ tbr_g1[tbr_g1_TEST].F_end=0;
		
		//puts("a");
		
		i_get = UART_BUFFER_SIZE - DMA1_Stream2->NDTR;
		
		last_get=0;
		int i=0;
		for(i=0; i<UART_BUFFER_SIZE; i++ ){
			
			if( HALL_RX_Buffer[i] == '}' ){
				last_get = '}';
				break;
			}
		}
		//last_get = HALL_RX_Buffer[i_get-1];
		
		if( last_get == '}' ){
			
			json_get_data_dma();

			read_protocol_json();
	
			if( json_protocol.data_w1_type > 0 ){
				
					//puts("/name = ");
					//puts( json_protocol.name_w1 );	
				
					if( json_protocol.data_w1_type == TYPE_STR ){
							//puts("/data_str = ");
							//puts( json_protocol.data_w1 );	
					}
					else if( json_protocol.data_w1_type == TYPE_WORD ){
							char str[50];
							sprintf(str,"/data_word = %d",json_protocol.data_w1_word);
							//puts(str);    
					}
					
					if( strcmp(json_protocol.name_w1,travel_time_addres) == 0 ){ // write travel_time_addres	
							if( json_protocol.data_w1_type == TYPE_STR ){
									//puts("/travel_time$ = ");
									Stng[TRAVEL_TIME] = atoi(json_protocol.data_w1);
									timer_eeprom = 500;
							}			
					}
					
					if( strcmp(json_protocol.name_w1,door_control_type_ADDRESS) == 0 ){ // write door_control_type_ADDRESS	
							if( json_protocol.data_w1_type == TYPE_STR ){			
									char i=0;
									for(i=0;i<3;i++){
											if( strcmp( json_protocol.data_w1 , List_DoorControlType[i] ) == 0 ){
												Stng[DOOR_CONTROL_TYPE] = i;
												timer_eeprom = 500;											
											}
									}
							}			
					}			
					
			}

			if( json_protocol.data_r1_type > 0 ){
				
					if( strcmp(json_protocol.name_r1,travel_time_addres) == 0 ){ // read travel_time_addres	
						char str[100];
						sprintf(str,"{\"name_w1\":\"%s\",\"data_w1\":%d,}",travel_time_addres,Stng[TRAVEL_TIME]);
						puts(str); 
						time=1;	
		
					}
					
					if( strcmp(json_protocol.name_r1,door_control_type_ADDRESS) == 0 ){ // read door_control_type_ADDRESS	
						char str[100];
						sprintf(str,"{\"name_w1\":\"%s\",\"data_w1\":\"%s\",}",door_control_type_ADDRESS,List_DoorControlType[Stng[DOOR_CONTROL_TYPE]]);
						puts(str); 
						time=1;	
		
					}
					
					
					
									
			}
			else{
				time=100;
			}
			
			reset_json();
			
		}
		
		if(time>0)time--;
		
		if(timer_eeprom>0)timer_eeprom++;
		if( timer_eeprom > 500 ){ timer_eeprom=0;
			EEPROMSaveFlag=1;
		}
		
	}
	
	
	if( time == 0 ){ time=500;
		char str[100];
		sprintf(str,"{\"name_w1\":\"-\",\"data_w1\":\"-\",}");
		puts(str); 					
	}

	//EEPROMSaveFlag=1;
		
}

void gsm_init(){
	
	ini_cpu_timer();
	
}



//HALL_SendData(strlen(HALL_TX_Buffer));	

