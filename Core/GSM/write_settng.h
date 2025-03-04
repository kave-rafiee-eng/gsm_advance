
#ifndef __WRITE_SETTNG_H
#define __WRITE_SETTNG_H

#include "main.h"

void server_protocol_data_manage();
void handle_status(volatile	uint8_t *stng_value);
void* get_field_by_index(void* struct_array, size_t struct_size, int struct_index, int field_index);
uint16_t convert_struct_to_uint16(void* struct_ptr, size_t struct_size);
void send_register(	uint8_t data);

#endif /* __WRITE_SETTNG_H */