// SHASI_H.h
#ifndef SHASI_H
#define SHASI_H

#include <stdint.h>
#include <stdbool.h>

#define UART_BUFFER_SIZE 200

void get_data_from_dma();
void test_shasi();

	
struct SHASI {
	
	uint8_t set;
	uint8_t cansel;
	
};
	
struct UART_RX_DATA {
	
	uint8_t buf[UART_BUFFER_SIZE];
	uint8_t len;
	
};

#endif // SHASI_H
