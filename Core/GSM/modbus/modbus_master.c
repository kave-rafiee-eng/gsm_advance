#include "main.h"
#include "modbus.h"
#include "../Inc/Hardware.h"


char modbus_master_write_register_MULTI( uint8_t slave_addres , uint8_t fc_code , uint16_t start_addres , uint16_t world_count , uint8_t *data ){
	
	uint16_t tx_crc=0;
	uint8_t tx_buf[world_count+20]; 
	int i=0;
	
	tx_buf[0] = slave_addres;
	tx_buf[1] = fc_code;	
	tx_buf[2] = start_addres >> 8 ; //start_addres H	
	tx_buf[3] = start_addres  ; //start_addres L		
	tx_buf[4] = world_count >> 8 ; //world_count H	
	tx_buf[5] = world_count  ; //world_count L	

	for( i=0; i<world_count; i++ ){
		
		tx_buf[6+i] = data[i];
	}
	
	tx_crc = CRC_16(tx_buf,6+world_count);
	
	tx_buf[6+world_count] = tx_crc ; //CRC H
	tx_buf[7+world_count] = tx_crc >> 8 ; //CRC L	
	
	for(i=0;i<world_count+9;i++){
		HW_DIR_HALL(1);
			stdout_putchar(tx_buf[i]);
		HW_DIR_HALL(0);
	}	
	
}

char modbus_master_write_register_SINGLE(uint8_t slave_addres , uint8_t fc_code , uint16_t start_addres , uint8_t data){
	
	uint8_t tx_buf[20];  uint16_t tx_crc=0;
	
	tx_buf[0] = slave_addres;
	tx_buf[1] = fc_code;	
	tx_buf[2] = start_addres >> 8 ; //start_addres H	
	tx_buf[3] = start_addres  ; //start_addres L		
	tx_buf[4] = data  ; //data L	

	tx_crc = CRC_16(tx_buf,5);
	
	tx_buf[5] = tx_crc ; //CRC H
	tx_buf[6] = tx_crc >> 8 ; //CRC L	
	
	char i=0;
	for(i=0;i<7;i++){
		HW_DIR_HALL(1);
			stdout_putchar(tx_buf[i]);
		HW_DIR_HALL(0);
	}
}

