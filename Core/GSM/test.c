#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../Inc/Variable.h"
#include "../Inc/Serial.h"


extern char List_DoorControlType[][8];
	
extern volatile	uint8_t			Stng[SETTING_ARRAY_SIZE];
extern volatile  uint8_t   	EEPROMSaveFlag;

extern char 								HALL_TX_Buffer[UART_BUFFER_SIZE];
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

			
			
			server_protocol_json();
			
			
	}
	
}
	
char write_data=0;

void json_test(){

			read_protocol_json();
	
			if( json_protocol.data_w1_type > 0 ){
				
					//puts("/name = ");
					//puts( json_protocol.name_w1 );	
				
					/*if( json_protocol.data_w1_type == TYPE_STR ){
							//puts("/data_str = ");
							//puts( json_protocol.data_w1 );	
					}
					if( json_protocol.data_w1_type == TYPE_WORD ){
							char str[50];
							sprintf(str,"/data_word = %d",json_protocol.data_w1_word);
							//puts(str);    
							write_data=1;
					}	*/

				
					write_setting_manage();
					
			}

			if( json_protocol.data_r1_type > 0 ){
				
					read_setting_manage();
					
			}

		reset_json();
}

void gsm_init(){
	
	ini_cpu_timer();
	
}



//HALL_SendData(strlen(HALL_TX_Buffer));	

