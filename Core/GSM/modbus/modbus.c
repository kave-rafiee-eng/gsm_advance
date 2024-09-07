#include "main.h"
#include "modbus.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../Inc/Variable.h"
#include "../Inc/Serial.h"

struct MODBUS_RTU modbus;
struct MODBUS_SLAVE modbus_slave;


uint16_t slave_start_add =0;
uint8_t slave_data_get =0;
uint16_t slave_crc_get=0;
uint16_t slave_crc_calculate=0;
uint16_t slave_data_count=0;

char modbus_slave_manage(){
	
	if( SLAVE_ADD == modbus.buf_rx[0] &&  FC_WRITE_TO_SLAVE_SINGLE == modbus.buf_rx[1]  ){ //single write
		
		slave_start_add = (modbus.buf_rx[2]<<8) + modbus.buf_rx[3];
		slave_data_get = modbus.buf_rx[4];
		slave_crc_get = (modbus.buf_rx[6]<<8) + modbus.buf_rx[5];
		
		slave_crc_calculate = CRC_16(modbus.buf_rx,5);
		if( slave_crc_calculate == slave_crc_get ){
				modbus_slave.address = slave_start_add;
				modbus_slave.data = slave_data_get;
				return 1;
		}
		else return -1;
	}
	else if( SLAVE_ADD == modbus.buf_rx[0] &&  FC_WRITE_TO_SLAVE_MULTI == modbus.buf_rx[1] ){ //multi write
		
		slave_start_add = (modbus.buf_rx[2]<<8) + modbus.buf_rx[3];
		slave_data_count = (modbus.buf_rx[4]<<8) + modbus.buf_rx[5];
		
		slave_crc_get = (modbus.buf_rx[slave_data_count+7]<<8) + modbus.buf_rx[slave_data_count+6];		
		slave_crc_calculate = CRC_16(modbus.buf_rx,6+slave_data_count);
		
		if( slave_crc_calculate == slave_crc_get ){
			for(int i=0; i<slave_data_count; i++ ){			
					modbus_slave.buf[i] = modbus.buf_rx[i+6];
			}
			modbus_slave.data_count = slave_data_count;
			modbus_slave.address = slave_start_add;
			modbus_slave.F_new_data=1;
		}
		
	}
	else 0;
}


char modbus_master_write_register_single(uint8_t slave_addres , uint8_t fc_code , uint16_t start_addres , uint16_t data){
	
	uint8_t tx_buf[20];  uint16_t tx_crc=0;
	
	tx_buf[0] = slave_addres;
	tx_buf[1] = fc_code;	
	tx_buf[2] = start_addres >> 8 ; //start_addres H	
	tx_buf[3] = start_addres  ; //start_addres L		
	tx_buf[4] = data >> 8 ; //data H	
	tx_buf[5] = data  ; //data L	

	tx_crc = CRC_16(tx_buf,6);
	
	tx_buf[6] = tx_crc ; //CRC H
	tx_buf[7] = tx_crc >> 8 ; //CRC L	
	
	char i=0;
	for(i=0;i<8;i++){
		//MODBUS_ADVANCE_RS(1);
		stdout_putchar(tx_buf[i]);
		//MODBUS_ADVANCE_RS(0);
	}
}

void modbus_get_data_dma(){
	
	DMA1_Stream2->CR  = 0; 
	
	memset(modbus.buf_rx,0,UART_BUFFER_SIZE);
	
	for( char i=0; i<UART_BUFFER_SIZE - DMA1_Stream2->NDTR; i++ ){
		modbus.buf_rx[i]= HALL_RX_Buffer[i];
	}
					
	DMA1_Stream2->NDTR = UART_BUFFER_SIZE;
	memset(HALL_RX_Buffer,0,UART_BUFFER_SIZE);
	DMA1_Stream2_Init();
	
}
