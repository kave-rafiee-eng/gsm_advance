#include "main.h"
#include "setting_adress.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../Inc/Variable.h"
#include "../Inc/Serial.h"

extern volatile	uint8_t			Stng[SETTING_ARRAY_SIZE];
extern volatile  uint8_t   	EEPROMSaveFlag;
extern char List_DoorControlType[][8];
extern char List_ServiceType[][17];
	
extern struct JSON_PROTOCOL json_protocol;

extern char write_data;
extern int device_serial;

void write_setting_manage(){
	
		if( strcmp(json_protocol.name_w1,travel_time_addres) == 0 ){ // write travel_time_addres	
				if( json_protocol.data_w1_type == TYPE_STR ){
						Stng[TRAVEL_TIME] = atoi(json_protocol.data_w1);
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
		if( strcmp(json_protocol.name_w1,number_of_stop_address) == 0 ){ // write number_of_stop_ADDRESS		
				if( json_protocol.data_w1_type == TYPE_STR ){
						Stng[NUM_OF_STOP] = atoi(json_protocol.data_w1);
						write_data=1;
				}				
		}		
		
		if( strcmp(json_protocol.name_w1,service_type_address) == 0 ){ // write service_type_address	
				if( json_protocol.data_w1_type == TYPE_STR ){
						replacechar(json_protocol.data_w1,'-',' ');					
						char i=0;
						for(i=0;i<4;i++){
								if( strcmp( json_protocol.data_w1 , List_ServiceType[i] ) == 0 ){
										Stng[SERVICE_TYPE] = i;
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


extern int server_protocol_arreay_select;
extern int server_protocol_byte_count;
extern int server_protocol_data;
extern int server_protocol_status;

extern char buffer_http_send_to_server[200];
extern char buffer_http_send_to_server_index;

void server_protocol_data_manage(  ){
	
	if( server_protocol_arreay_select == 0 ){

		if( server_protocol_status == 1 ){
				Stng[server_protocol_byte_count] = server_protocol_data;
		}
		else{
				char temp[50];
				buffer_http_send_to_server_index++;
				char num = buffer_http_send_to_server_index;
				sprintf(temp,"ar%d=%d&ad%d=%d&da%d=%d&",num,server_protocol_arreay_select,num,server_protocol_byte_count,num,Stng[server_protocol_byte_count]);
				strcat(buffer_http_send_to_server,temp);
		}
	}
	
}


