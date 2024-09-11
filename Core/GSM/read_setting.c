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

void read_setting_manage(){
	
		if( strcmp(json_protocol.name_r1,travel_time_addres) == 0 ){ // read travel_time_addres	
				char str[100];
				sprintf(str,"{\"serial\":\"%d\",\"name_w1\":\"%s\",\"data_w1\":%d,}",device_serial,travel_time_addres,Stng[TRAVEL_TIME]);
				modbus_master_write_register_MULTI(SLAVE_ADD,FC_WRITE_TO_SLAVE_MULTI,2,strlen(str),str);
		}
					
		if( strcmp(json_protocol.name_r1,door_control_type_ADDRESS) == 0 ){ // read door_control_type_ADDRESS	
				char str[100];
				sprintf(str,"{\"serial\":\"%d\",\"name_w1\":\"%s\",\"data_w1\":\"%s\",}",device_serial,door_control_type_ADDRESS,List_DoorControlType[Stng[DOOR_CONTROL_TYPE]]);
				modbus_master_write_register_MULTI(SLAVE_ADD,FC_WRITE_TO_SLAVE_MULTI,2,strlen(str),str);
		}

		if( strcmp(json_protocol.name_r1,number_of_stop_address) == 0 ){ // read number_of_stop_ADDRESS		
				char str[100];
				sprintf(str,"{\"serial\":\"%d\",\"name_w1\":\"%s\",\"data_w1\":\"%d\",}",device_serial,number_of_stop_address,Stng[NUM_OF_STOP]);
				modbus_master_write_register_MULTI(SLAVE_ADD,FC_WRITE_TO_SLAVE_MULTI,2,strlen(str),str);
		}
		
		if( strcmp(json_protocol.name_r1,service_type_address) == 0 ){ // read service_type_address		
				char str[100];
				sprintf(str,"{\"serial\":\"%d\",\"name_w1\":\"%s\",\"data_w1\":\"%s\",}",device_serial,service_type_address,List_ServiceType[Stng[SERVICE_TYPE]]);
				replacechar(str,' ','-');
				modbus_master_write_register_MULTI(SLAVE_ADD,FC_WRITE_TO_SLAVE_MULTI,2,strlen(str),str);
		}
	
}