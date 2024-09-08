#include "main.h"
#include "test.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

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
extern struct MODBUS_SLAVE modbus_slave;

int time=0;
int timer_eeprom=0;

int last_i_get=0;

int device_serial=100;

void json_test();
	
void test_modbus(){
	
	i_get = UART_BUFFER_SIZE - DMA1_Stream2->NDTR;
	
	if( i_get != last_i_get ) { 
		last_i_get = i_get;
		tbr_g1[tbr_g1_TEST].I_time=0;
	}
	
	if ( i_get > 0 )tbr_g1[tbr_g1_TEST].EN=1;
	else tbr_g1[tbr_g1_TEST].EN=0;
	
	tbr_g1[tbr_g1_TEST].C_set_time=20;
	
	if( tbr_g1[tbr_g1_TEST].F_end ){ tbr_g1[tbr_g1_TEST].F_end=0;
		if( i_get > 0 ){
				modbus_get_data_dma();
				modbus_slave_manage();
				
				tbr_g1[tbr_g1_TEST].EN=0;
		}				
	}
		
	if( modbus_slave.F_new_data ){ modbus_slave.F_new_data=0;
		
			memset( json.document,0,strlen(json.document));
			for( int i=0; i< modbus_slave.data_count; i++ ){
					json.document[i] = modbus_slave.buf[i];
			}

			json_test();
	}
	
}
	
void json_test(){

			char write_data=0;
	
			read_protocol_json();
	
			if( json_protocol.data_w1_type > 0 ){
				
					//puts("/name = ");
					//puts( json_protocol.name_w1 );	
				
					if( json_protocol.data_w1_type == TYPE_STR ){
							//puts("/data_str = ");
							//puts( json_protocol.data_w1 );	
					}
					if( json_protocol.data_w1_type == TYPE_WORD ){
							char str[50];
							sprintf(str,"/data_word = %d",json_protocol.data_w1_word);
							//puts(str);    
							write_data=1;
						
					}	
					if( strcmp(json_protocol.name_w1,travel_time_addres) == 0 ){ // write travel_time_addres	
							if( json_protocol.data_w1_type == TYPE_STR ){
									//puts("/travel_time$ = ");
									Stng[TRAVEL_TIME] = atoi(json_protocol.data_w1);
									timer_eeprom = 500;
									write_data=1;
							}			
					}
					if( strcmp(json_protocol.name_w1,door_control_type_ADDRESS) == 0 ){ // write door_control_type_ADDRESS	
							if( json_protocol.data_w1_type == TYPE_STR ){			
									char i=0;
									for(i=0;i<3;i++){
											if( strcmp( json_protocol.data_w1 , List_DoorControlType[i] ) == 0 ){
												Stng[DOOR_CONTROL_TYPE] = i;
												write_data=1;										
											}
									}
							}			
					}		

					if( write_data ){
						char str[100];
						sprintf(str,"{\"serial\":\"%d\",\"name_w1\":\"OK\",\"data_w1\":\"OK\",}",device_serial);
						modbus_master_write_register_MULTI(SLAVE_ADD,FC_WRITE_TO_SLAVE_MULTI,2,strlen(str),str);				
					}
					
			}

			if( json_protocol.data_r1_type > 0 ){
				
					if( strcmp(json_protocol.name_r1,travel_time_addres) == 0 ){ // read travel_time_addres	
						char str[100];
						sprintf(str,"{\"serial\":\"%d\",\"name_w1\":\"%s\",\"data_w1\":%d,}",device_serial,travel_time_addres,Stng[TRAVEL_TIME]);
						//puts(str); 
						modbus_master_write_register_MULTI(SLAVE_ADD,FC_WRITE_TO_SLAVE_MULTI,2,strlen(str),str);

					}
					
					if( strcmp(json_protocol.name_r1,door_control_type_ADDRESS) == 0 ){ // read door_control_type_ADDRESS	
						char str[100];
						sprintf(str,"{\"serial\":\"%d\",\"name_w1\":\"%s\",\"data_w1\":\"%s\",}",device_serial,door_control_type_ADDRESS,List_DoorControlType[Stng[DOOR_CONTROL_TYPE]]);
						//puts(str); 
						modbus_master_write_register_MULTI(SLAVE_ADD,FC_WRITE_TO_SLAVE_MULTI,2,strlen(str),str);
					}
										
			}

		reset_json();
}

void gsm_init(){
	
	ini_cpu_timer();
	
}



//HALL_SendData(strlen(HALL_TX_Buffer));	

