#include "main.h"

extern I2C_HandleTypeDef hi2c1;

#define		_EEPROM_SIZE_KBIT							64
#define		_EEPROM_I2C   								hi2c1
#define		_EEPROM_USE_FREERTOS          0
#define		_EEPROM_ADDRESS               0xA0
#define		_EEPROM_USE_WP_PIN            0


#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
//#include "ee24Config.h"

bool    ee24_isConnected(void);
bool    ee24_write(uint16_t address, uint8_t *data, size_t lenInBytes);	
bool    ee24_read(uint16_t address, uint8_t *data, size_t lenInBytes);
bool    ee24_eraseChip(void);

