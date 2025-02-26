#include "main.h"
#include "modbus.h"
#include "../Inc/Hardware.h"
#include "../gsm.h"
// Function to write multiple registers to a Modbus slave device
char modbus_master_write_register_MULTI(uint8_t slave_address, uint8_t fc_code, uint16_t start_address, uint16_t word_count, uint8_t *data) {

    uint16_t tx_crc = 0;            // Variable to store the calculated CRC
    uint8_t tx_buf[word_count + 20]; // Buffer to store the Modbus message to be transmitted
    int i = 0;                      // Loop counter

    // Set up the Modbus frame with the slave address and function code
    tx_buf[0] = slave_address;               // Slave address
    tx_buf[1] = fc_code;                     // Function code (e.g., write multiple registers)
    tx_buf[2] = start_address >> 8;          // Start address high byte
    tx_buf[3] = start_address;               // Start address low byte
    tx_buf[4] = word_count >> 8;             // Word count high byte
    tx_buf[5] = word_count;                  // Word count low byte

    // Copy the data to the buffer starting from byte 6
    for (i = 0; i < word_count; i++) {
        tx_buf[6 + i] = data[i];  // Copy each data byte into the buffer
    }

    // Calculate the CRC for the message
    tx_crc = CRC_16(tx_buf, 6 + word_count);

    // Append the CRC to the buffer (CRC is 2 bytes)
    tx_buf[6 + word_count] = tx_crc;          // CRC high byte
    tx_buf[7 + word_count] = tx_crc >> 8;     // CRC low byte

    // Send the Modbus frame byte by byte
    for (i = 0; i < word_count + 9; i++) {
        MODBUS_DRIVE(1);              // Set direction for hardware interface
        UART_GSM_send(tx_buf[i]);   // Transmit the byte via the hardware interface
        MODBUS_DRIVE(0);              // Reset the direction for hardware interface
    }
}
