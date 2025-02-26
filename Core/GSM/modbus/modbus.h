#ifndef __MODBUS_H
#define __MODBUS_H

#include "main.h"

#define SLAVE_ADD 1
#define FC_READ_FROM_SLAVE 1

#define FC_WRITE_TO_SLAVE_SINGLE 2
#define FC_WRITE_TO_SLAVE_MULTI 3

char modbus_slave_manage();
char modbus_master_write_register_MULTI( uint8_t slave_addres , uint8_t fc_code , uint16_t start_addres , uint16_t world_count , uint8_t *data );
	
struct MODBUS_RTU {
	
	volatile uint8_t buf_rx[200];
	uint8_t len;
	
};

struct MODBUS_SLAVE {
	
	volatile uint8_t address;
	volatile uint8_t data;
	volatile uint8_t buf[200];
	volatile uint16_t data_count;
	char F_new_data :1;
};


#endif