#include "main.h"
#include "shasi.h"

#include "../Inc/Variable.h"
#include "../Inc/Serial.h"

struct SHASI shasi_recive; // SHASI DATA RECIVE
struct UART_RX_DATA uart_rx_data; // UART RX BUFFER

ModbusFrame txFrame; //MODBUS 8 BIT TX FREAM
ModbusFrame RXframe; //MODBUS 8 BIT RX FREAM

extern struct cpu_timer_basic_10bit_auto_reset tbr_g1[def_num_tbr_g1];

extern volatile	char	HALL_RX_Buffer[UART_BUFFER_SIZE];
extern int i_get;
extern int last_i_get;


	
void test_shasi(){
	
	//---------------------------- send to shasi
	
	txFrame.dataLength = 8;
	txFrame.functionCode = 0x10;
	txFrame.deviceAddress = 10;
	
	txFrame.data[0] = 0; //set
	txFrame.data[1] = 0; //clear
	txFrame.data[2] = 0x04; //blinker
	txFrame.data[3] = 0; //landing
	txFrame.data[4] = 2; //command
	txFrame.data[5] = 6; //seg_a
	txFrame.data[6] = 128; //seg_b
	txFrame.data[7] = 0; //reserve
	
	tbr_g1[tbr_g1_TEST_SHASI].EN=1; // timer enable
	tbr_g1[tbr_g1_TEST_SHASI].C_set_time=500; // 500ms timer
	
	if( tbr_g1[tbr_g1_TEST_SHASI].F_end ){ tbr_g1[tbr_g1_TEST_SHASI].F_end=0; // timer flag

		Modbus_SendFrame(txFrame.deviceAddress, txFrame.functionCode, txFrame.data, txFrame.dataLength);
		
	}
	
	//---------------------------- recive 
	
	i_get = UART_BUFFER_SIZE - DMA1_Stream2->NDTR;
	
	if( i_get != last_i_get ) { 
		last_i_get = i_get;
		tbr_g1[tbr_g1_SHASI_RS485_GET].I_time=0;
	}
	
	if ( i_get > 0 )tbr_g1[tbr_g1_SHASI_RS485_GET].EN=1;
	else tbr_g1[tbr_g1_SHASI_RS485_GET].EN=0;
	
	tbr_g1[tbr_g1_SHASI_RS485_GET].C_set_time=5;
	
	if( tbr_g1[tbr_g1_SHASI_RS485_GET].F_end ){ tbr_g1[tbr_g1_SHASI_RS485_GET].F_end=0;			
		tbr_g1[tbr_g1_TEST].EN=0;

		get_data_from_dma();
			
	  RXframe = Modbus_ReceiveFrame( (uint8_t*)uart_rx_data.buf, uart_rx_data.len );
		
		if (RXframe.isValid == 1) {
		
			shasi_recive.set = 	RXframe.data[0];
			shasi_recive.cansel = 	RXframe.data[1];
		}	
	}
	
}

void get_data_from_dma(){
	
	DMA1_Stream2->CR  = 0; 
	
	memset( uart_rx_data.buf , 0 , UART_BUFFER_SIZE );
	
	uart_rx_data.len =  UART_BUFFER_SIZE - DMA1_Stream2->NDTR;
	for( char i=0; i<uart_rx_data.len; i++ ){
		uart_rx_data.buf[i]= HALL_RX_Buffer[i];
	}
					
	DMA1_Stream2->NDTR = UART_BUFFER_SIZE;
	memset( HALL_RX_Buffer , 0 , UART_BUFFER_SIZE );
	DMA1_Stream2_Init();
	
}
