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

extern char F_save_EEPROM;

void server_protocol_data_manage(){
	
	if( server_protocol.array_select == 0 ){ // STNG processing	
			handle_status(&Stng[server_protocol.byte_count]);
	}
	else if( server_protocol.array_select == 1 &&  server_protocol.byte_count>=100 ){ // FLOOR processing
			if( server_protocol.byte_count%100 == 2 )handle_status(&Floor[server_protocol.byte_count/100-1].Door1Select);
			if( server_protocol.byte_count%100 == 3 )handle_status(&Floor[server_protocol.byte_count/100-1].Door2Select);
			if( server_protocol.byte_count%100 == 4 )handle_status(&Floor[server_protocol.byte_count/100-1].Door3Select);
			if( server_protocol.byte_count%100 == 5 )handle_status(&Floor[server_protocol.byte_count/100-1].SegL);
			if( server_protocol.byte_count%100 == 6 )handle_status(&Floor[server_protocol.byte_count/100-1].SegR);
			if( server_protocol.byte_count%100 == 7 )handle_status(&Floor[server_protocol.byte_count/100-1].Talk);
			if( server_protocol.byte_count%100 == 8 )handle_status(&Floor[server_protocol.byte_count/100-1].Jump1Speed);
			if( server_protocol.byte_count%100 == 9 )handle_status(&Floor[server_protocol.byte_count/100-1].Jump1Flag);
			if( server_protocol.byte_count%100 == 10 )handle_status(&Floor[server_protocol.byte_count/100-1].JumpNFlag);				
			if( server_protocol.byte_count%100 == 11 )handle_status(&Floor[server_protocol.byte_count/100-1].LevelDistance);
	}
	else if( server_protocol.array_select == 2 && server_protocol.byte_count > 0  ){
		
			uint16_t packed_data=0;
			
			if( server_protocol.byte_count == 1 )  packed_data = convert_struct_to_uint16(&MIn, sizeof(MIn));
			if( server_protocol.byte_count == 2 )  packed_data = convert_struct_to_uint16(&MOut, sizeof(MOut));
			if( server_protocol.byte_count == 3 )  packed_data = convert_struct_to_uint16(&CIn, sizeof(CIn));
			if( server_protocol.byte_count == 4 )  packed_data = convert_struct_to_uint16(&CIn, sizeof(CIn));
		
			if( server_protocol.byte_count == 5 )  packed_data = convert_struct_to_uint16(&PB.Hall, sizeof(PB.Hall));
			if( server_protocol.byte_count == 6 )  packed_data = convert_struct_to_uint16(&PB.Cabin, sizeof(PB.Cabin));
		
			if( server_protocol.byte_count == 7 )  { 
					packed_data |= (Safety_90      & 0x1) << 0;
					packed_data |= (Safety_71      & 0x1) << 1;
					packed_data |= (Safety_66      & 0x1) << 2;
					packed_data |= (Safety_69      & 0x1) << 3;
					packed_data |= (Safety_68      & 0x1) << 4;
					packed_data |= (PreSafety_66   & 0x1) << 5;
					packed_data |= (Short110To24   & 0x1) << 6;
			}
		
			if( server_protocol.byte_count == 8 )  packed_data = convert_struct_to_uint16(&SerialExtInput, sizeof(SerialExtInput));
			
			
			if( server_protocol.byte_count == 9 )  { 
				
				packed_data |= (Error_RLS          & 0x1) << 0;
				packed_data |= (Error_RLSCut       & 0x1) << 1;
				packed_data |= (Error_DRC          & 0x1) << 2;
				packed_data |= (Error_DRCCut       & 0x1) << 3;
				packed_data |= (Error_RLS_RelevelF & 0x1) << 4;
				packed_data |= (Error_FTO          & 0x1) << 5;
				packed_data |= (Error_TravelTimeout & 0x1) << 6;
				packed_data |= (Error_DoorCloseTimeout & 0x1) << 7;
				packed_data |= (Error_URA          & 0x1) << 8;
				packed_data |= (MainErrorF         & 0x1) << 9;
				packed_data |= (Error_CutSerial    & 0x1) << 10;
				packed_data |= (Error_DayCounter   & 0x1) << 11;				
			}
			
			if( server_protocol.byte_count == 10 )  { 
					if (Error_Safety90)          packed_data |= (1 << 0);
					if (Error_SafetyCircuitCut)  packed_data |= (1 << 1);
					if (Error_Cut66)            packed_data |= (1 << 2);
					if (Error_Cut68)            packed_data |= (1 << 3);
					if (Error_Cut69)            packed_data |= (1 << 4);
					if (Error_LevelingTimeout)  packed_data |= (1 << 5);
					if (Error_FLT_UNB)         packed_data |= (1 << 6);
					if (Error_PHR)             packed_data |= (1 << 7);
					if (Error_PHL)             packed_data |= (1 << 8);
					if (Error_OverCurrent)     packed_data |= (1 << 9);
					if (Error_LowCurrent)      packed_data |= (1 << 10);
					if (Error_CA1CAN)         packed_data |= (1 << 11);
					if (Error_1CF)            packed_data |= (1 << 12);
					if (Error_CF3)            packed_data |= (1 << 13);		
			}
			
			if( server_protocol.byte_count == 11 )  { 
				
				if (Error_EndDoorTime)   packed_data |= (1 << 0);
				if (Error_FLT_DRV)       packed_data |= (1 << 1);
				if (Error_Cut_1CF_CF3)   packed_data |= (1 << 2);
				if (Error_RLS_S)         packed_data |= (1 << 3);
				if (Error_RLSCut_S)      packed_data |= (1 << 4);
				
			}
			


						
			char temp[50];
			buffer_http_send_to_server_index++;
			int num = buffer_http_send_to_server_index;

			sprintf(temp, ",\"ar%d\":\"%d\",\"ad%d\":\"%d\",\"da%d\":\"%d\"", 
					num, server_protocol.array_select, 
					num, server_protocol.byte_count, 
					num, packed_data);
							
			strcat(buffer_http_send_to_server, temp);
			
			
			
	}
	else if( server_protocol.array_select == 3 && server_protocol.byte_count > 0  ){
		
			if( server_protocol.byte_count == 1 )  send_register(Ph_R_RMS);
			if( server_protocol.byte_count == 2 )  send_register(Ph_S_RMS);
			if( server_protocol.byte_count == 3 )  send_register(Ph_T_RMS);
			if( server_protocol.byte_count == 4 )  send_register(PowerVoltageMainBoard);
			if( server_protocol.byte_count == 5 )  send_register(PowerVoltageCabin);
			if( server_protocol.byte_count == 6 )  send_register(CT1_RMS);
			if( server_protocol.byte_count == 7 )  send_register(CT2_RMS);
		
	}
	else if( server_protocol.array_select == 4 && server_protocol.byte_count > 0  ){
		
			if( server_protocol.byte_count == 1 )  send_register(SegUp);
			if( server_protocol.byte_count == 2 )  send_register(SegDn);
		
	}
		
		
	
	
	
}

			
void send_register(	uint8_t data) {
	
		char temp[50];
    buffer_http_send_to_server_index++;
    int num = buffer_http_send_to_server_index;

    sprintf(temp, ",\"ar%d\":\"%d\",\"ad%d\":\"%d\",\"da%d\":\"%d\"", 
        num, server_protocol.array_select, 
        num, server_protocol.byte_count, 
        num, data);
            
		strcat(buffer_http_send_to_server, temp);
}
			
void handle_status( volatile	uint8_t *stng_value) {
    if (server_protocol.status == 1) {
        *stng_value = server_protocol.data;
				F_save_EEPROM=1;
    }
		
		char temp[50];
    buffer_http_send_to_server_index++;
    int num = buffer_http_send_to_server_index;

    sprintf(temp, ",\"ar%d\":\"%d\",\"ad%d\":\"%d\",\"da%d\":\"%d\"", 
        num, server_protocol.array_select, 
        num, server_protocol.byte_count, 
        num, *stng_value);
            
		strcat(buffer_http_send_to_server, temp);
}



uint16_t convert_struct_to_uint16(void* struct_ptr, size_t struct_size) {
    uint16_t result = 0;
    uint8_t* byte_ptr = (uint8_t*)struct_ptr;
    
    for (size_t i = 0; i < struct_size && i < 16; i++) {
        result |= ((byte_ptr[i] & 0x1) << i);
    }
    
    return result;
}



			/*if( server_protocol.status == 1 ){
					Stng[server_protocol.byte_count] = server_protocol.data;
					server_protocol.status=0;
			}
			if( server_protocol.status == 0 ){
					char temp[50];
					buffer_http_send_to_server_index++;
					char num = buffer_http_send_to_server_index;
					
					sprintf(temp,",\"ar%d\":\"%d\",\"ad%d\":\"%d\",\"da%d\":\"%d\"",num,server_protocol.array_select,num,server_protocol.byte_count,num,Stng[server_protocol.byte_count]);
					strcat(buffer_http_send_to_server,temp);
			}*/


			/*if( server_protocol.status == 1 ){
				
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
			}*/

