#include "main.h"
#include "modbus.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../Inc/Variable.h"
#include "../Inc/Serial.h"

// Define Modbus RTU structure and Modbus slave structure
struct MODBUS_RTU modbus;
struct MODBUS_SLAVE modbus_slave;

// Function to manage Modbus slave communication
char modbus_slave_manage() {
    
    uint16_t slave_start_add = 0;      // Variable to store the starting address of the slave
    uint8_t slave_data_get = 0;        // Variable to store the data to be written to the slave
    uint16_t slave_crc_get = 0;        // Variable to store the CRC received from the slave
    uint16_t slave_crc_calculate = 0;  // Variable to store the calculated CRC
    uint16_t slave_data_count = 0;     // Variable to store the count of data to be written to the slave

		if (SLAVE_ADD == modbus.buf_rx[0] && FC_WRITE_TO_SLAVE_MULTI == modbus.buf_rx[1]) {
        // Multi write to slave
        slave_start_add = (modbus.buf_rx[2] << 8) + modbus.buf_rx[3];   // Get the starting address from the received buffer
        slave_data_count = (modbus.buf_rx[4] << 8) + modbus.buf_rx[5];  // Get the number of data bytes to be written
        
        // Get the CRC received after the data
        slave_crc_get = (modbus.buf_rx[slave_data_count + 7] << 8) + modbus.buf_rx[slave_data_count + 6];
        
        // Calculate CRC for the received data
        slave_crc_calculate = CRC_16(modbus.buf_rx, 6 + slave_data_count);
        
        // Check if the calculated CRC matches the received CRC and if the data count is valid (less than 200)
        if (slave_crc_calculate == slave_crc_get && slave_data_count < 200) {
            // If CRC matches, copy the data to the Modbus slave buffer
            for (int i = 0; i < slave_data_count; i++) {
                modbus_slave.buf[i] = modbus.buf_rx[i + 6];  // Copy data to slave buffer
            }
            // Update the Modbus slave structure with the data count and address
            modbus_slave.data_count = slave_data_count;
            modbus_slave.address = slave_start_add;
            modbus_slave.F_new_data = 1;  // Indicate that new data is available
        }
    } else {
        return 0;  // Return 0 if the message is not for a valid operation
    }
}



