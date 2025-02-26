#include "main.h"
#include "modbus_rtu_8bit.h"
#include <string.h>
#include "../Inc/Hardware.h"

// UART send function placeholder
void UART_Send(uint8_t *data, uint8_t length) {
  // Implement UART transmission here
	for(uint8_t i=0;i<=length;i++){
			stdout_putchar(data[i]);
	}
}

void Modbus_SendFrame(uint8_t deviceAddress, uint8_t functionCode, uint8_t *data, uint8_t dataLength) {
    uint8_t frame[dataLength+5];
    uint16_t crc;

    // Construct the Modbus frame
    frame[0] = deviceAddress;       // Device address
    frame[1] = functionCode;        // Function code

    // Add data
    memcpy(&frame[2], data, dataLength);

    // Calculate CRC
    crc = Modbus_CalculateCRC(frame, 2 + dataLength);

    // Add CRC to the frame
    frame[2 + dataLength] = crc & 0xFF;          // Low byte of CRC
    frame[3 + dataLength] = (crc >> 8) & 0xFF;   // High byte of CRC

    // Send the frame via UART
	HW_DIR_HALL(1);
    UART_Send(frame, 4 + dataLength);  // Total length: Address + Function Code + Data + CRC
	HW_DIR_HALL(0);
}

ModbusFrame Modbus_ReceiveFrame(uint8_t *buffer, uint8_t length) {
    ModbusFrame receivedFrame;

    // Initialize the structure with default values
    receivedFrame.isValid = false;
    receivedFrame.deviceAddress = 0;
    receivedFrame.functionCode = 0;
    receivedFrame.dataLength = 0;

    // Check if the frame length is valid (minimum: address + function code + CRC = 4 bytes)
    if (length < 4) {
        return receivedFrame; // Invalid frame
    }

    // Extract CRC from the frame
    uint16_t receivedCRC = (buffer[length - 1] << 8) | buffer[length - 2];

    // Calculate CRC for the received data (excluding CRC bytes)
    uint16_t calculatedCRC = Modbus_CalculateCRC(buffer, length - 2);

    // Compare received CRC with calculated CRC
    if (receivedCRC != calculatedCRC) {
        return receivedFrame; // CRC mismatch
    }

    // Extract device address and function code
    receivedFrame.deviceAddress = buffer[0];
    receivedFrame.functionCode = buffer[1];

    // Extract data length
    receivedFrame.dataLength = length - 4; // Exclude address, function code, and CRC

    // Copy data
    memcpy(receivedFrame.data, &buffer[2], receivedFrame.dataLength);

    receivedFrame.isValid = true;

    return receivedFrame;
}

uint16_t Modbus_CalculateCRC(uint8_t *buffer, uint8_t length) {
    uint16_t crc = 0xFFFF;
    for (uint8_t i = 0; i < length; i++) {
        crc ^= buffer[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}
