#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "write_settng.h"

#include "../Inc/Variable.h"
#include "../Inc/Serial.h"

//-------------------------------- RESA
extern volatile	uint8_t			Stng[SETTING_ARRAY_SIZE];
extern volatile  uint8_t   	EEPROMSaveFlag;
extern char List_DoorControlType[][8];
extern char List_ServiceType[][17];

//-------------------------------- KAVE
extern struct JSON_PROTOCOL json_protocol;
extern ServerProtocol server_protocol;

extern char buffer_http_send_to_server[200];
extern char buffer_http_send_to_server_index;

//--------------------------------

void server_protocol_data_manage(){
	
	if( server_protocol.array_select == 0 ){ // STNG processing

			if( server_protocol.status == 1 ){
					Stng[server_protocol.byte_count] = server_protocol.data;
					server_protocol.status=0;
			}
			if( server_protocol.status == 0 ){
					char temp[50];
					buffer_http_send_to_server_index++;
					char num = buffer_http_send_to_server_index;
					
					sprintf(temp,",\"ar%d\":\"%d\",\"ad%d\":\"%d\",\"da%d\":\"%d\"",num,server_protocol.array_select,num,server_protocol.byte_count,num,Stng[server_protocol.byte_count]);
					strcat(buffer_http_send_to_server,temp);
			}
	}
	if( server_protocol.array_select == 1 &&  server_protocol.byte_count>=100 ){ // FLOOR processing

			if( server_protocol.status == 1 ){
				
				if( server_protocol.byte_count%100 == 2 )Floor[server_protocol.byte_count/100-1].Door1Select=server_protocol.data;
				if( server_protocol.byte_count%100 == 3 )Floor[server_protocol.byte_count/100-1].Door2Select=server_protocol.data;
				if( server_protocol.byte_count%100 == 4 )Floor[server_protocol.byte_count/100-1].Door3Select=server_protocol.data;
				
				if( server_protocol.byte_count%100 == 5 )Floor[server_protocol.byte_count/100-1].SegL=server_protocol.data;
				if( server_protocol.byte_count%100 == 6 )Floor[server_protocol.byte_count/100-1].SegR=server_protocol.data;
				if( server_protocol.byte_count%100 == 7 )Floor[server_protocol.byte_count/100-1].Talk=server_protocol.data;
				if( server_protocol.byte_count%100 == 8 )Floor[server_protocol.byte_count/100-1].Jump1Speed=server_protocol.data;
				if( server_protocol.byte_count%100 == 9 )Floor[server_protocol.byte_count/100-1].Jump1Flag=server_protocol.data;
				if( server_protocol.byte_count%100 == 10 )Floor[server_protocol.byte_count/100-1].JumpNFlag=server_protocol.data;				
				if( server_protocol.byte_count%100 == 11 )Floor[server_protocol.byte_count/100-1].LevelDistance=server_protocol.data;
				server_protocol.status=0;
			}
			if( server_protocol.status == 0 ){
				
				int data=0;
				
				if( server_protocol.byte_count%100 == 2 )data=Floor[server_protocol.byte_count/100-1].Door1Select;
				if( server_protocol.byte_count%100 == 3 )data=Floor[server_protocol.byte_count/100-1].Door2Select;
				if( server_protocol.byte_count%100 == 4 )data=Floor[server_protocol.byte_count/100-1].Door3Select;
				
				if( server_protocol.byte_count%100 == 5 )data=Floor[server_protocol.byte_count/100-1].SegL;
				if( server_protocol.byte_count%100 == 6 )data=Floor[server_protocol.byte_count/100-1].SegR;
				if( server_protocol.byte_count%100 == 7 )data=Floor[server_protocol.byte_count/100-1].Talk;
				if( server_protocol.byte_count%100 == 8 )data=Floor[server_protocol.byte_count/100-1].Jump1Speed;
				if( server_protocol.byte_count%100 == 9 )data=Floor[server_protocol.byte_count/100-1].Jump1Flag;
				if( server_protocol.byte_count%100 == 10 )data=Floor[server_protocol.byte_count/100-1].JumpNFlag;
				
				if( server_protocol.byte_count%100 == 11 )data=Floor[server_protocol.byte_count/100-1].LevelDistance;
				
					char temp[50];
					buffer_http_send_to_server_index++;
					char num = buffer_http_send_to_server_index;
					sprintf(temp,",\"ar%d\":\"%d\",\"ad%d\":\"%d\",\"da%d\":\"%d\"",num,server_protocol.array_select,num,server_protocol.byte_count,num,data);
					strcat(buffer_http_send_to_server,temp);
			}
	}
	
}


