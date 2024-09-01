#include "main.h"
#include "Menu.h"
#include "AT24Cxx.h"
#include "EEPROM.h"
#include "Defines.h"
#include "stdio.h"
#include "n1202.h"
#include "Serial.h"
#include "Hardware.h"
#include "Variable.h"

const uint8_t	ResetFactory[EE_BUFFER_SIZE]={
	
123,  0, 12,  0,  0,  0,  0,  2,  1,  0,  0,  0,  0,  0, 60,  0,  0,  3, 10,  0,  5,  5,  0,  7,  8,  0,  2,  5,  0,  0,
  0,  0,  0,  0, 30,  8,  0, 10,  0, 30,  6, 10,  0,  8,  6,  0, 10,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0, 20,  0, 20, 11,  2,  3,  4,  5,  6,  9,  1,  2,  3,  4,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  0,  0,  0,  7,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  7,  8,  9, 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 13, 14, 15, 16, 17, 18, 19, 20, 21,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  8,  9, 10, 11, 12, 13, 14, 15,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11,  2,  3,  4,  5,  6,  7,  8,  9, 10,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 25,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  2,  4,  7,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  8,  0,  2,  0,  0,  0,  6,  0
};


void SaveHwKeyPassword(void)
{
	EEPROM_Buffer[0] 	= (HwPass[0] >> 24) & 0x000000FF;
	EEPROM_Buffer[1] 	= (HwPass[0] >> 16) & 0x000000FF;
	EEPROM_Buffer[2]	= (HwPass[0] >> 8) 	& 0x000000FF;
	EEPROM_Buffer[3] 	= (HwPass[0]) & 0x000000FF;
	
	EEPROM_Buffer[4] 	= (HwPass[1] >> 24) & 0x000000FF;
	EEPROM_Buffer[5] 	= (HwPass[1] >> 16) & 0x000000FF;
	EEPROM_Buffer[6] 	= (HwPass[1] >> 8) 	& 0x000000FF;
	EEPROM_Buffer[7] 	= (HwPass[1]) & 0x00FF;
	
	EEPROM_Buffer[8] 	= (HwPass[2] >> 24) & 0x000000FF;
	EEPROM_Buffer[9] 	= (HwPass[2] >> 16) & 0x000000FF;
	EEPROM_Buffer[10]	= (HwPass[2] >> 8) 	& 0x000000FF;
	EEPROM_Buffer[11]	= (HwPass[2]) & 0x000000FF;
	
	ee24_write(HW_KEY_PASSWORD_START_ADDRESS, EEPROM_Buffer, 12);
}

void ReadHwKeyPassword(void)
{
	ee24_read(HW_KEY_PASSWORD_START_ADDRESS,EEPROM_Buffer,12);
	
	uint32_t	Byte3=0,Byte2=0,Byte1=0,Byte0=0;
		
	Byte3 = EEPROM_Buffer[0] 	& 0x000000FF;
	Byte2 = EEPROM_Buffer[1] 	& 0x000000FF;
	Byte1 = EEPROM_Buffer[2] 	& 0x000000FF;
	Byte0 = EEPROM_Buffer[3] 	& 0x000000FF;
	HwPass[0] = (Byte3 << 24) | (Byte2 << 16) | (Byte1 << 8) | Byte0;
	Byte3 = EEPROM_Buffer[4] 	& 0x000000FF;
	Byte2 = EEPROM_Buffer[5] 	& 0x000000FF;
	Byte1 = EEPROM_Buffer[6] 	& 0x000000FF;
	Byte0 = EEPROM_Buffer[7] 	& 0x000000FF;
	HwPass[1] = (Byte3 << 24) | (Byte2 << 16) | (Byte1 << 8) | Byte0;
	Byte3 = EEPROM_Buffer[8] 	& 0x000000FF;
	Byte2 = EEPROM_Buffer[9] 	& 0x000000FF;
	Byte1 = EEPROM_Buffer[10] & 0x000000FF;
	Byte0 = EEPROM_Buffer[11] & 0x000000FF;
	HwPass[2] = (Byte3 << 24) | (Byte2 << 16) | (Byte1 << 8) | Byte0;
}

void SaveDayCounterValues(void)
{
	EEPROM_Buffer[0] 	= DayCounter_Status;
	EEPROM_Buffer[1] 	= DayCounter_Day;
	EEPROM_Buffer[2] 	= DayCounter_Hour;
	EEPROM_Buffer[3] 	= DayCounter_Minutes;
	EEPROM_Buffer[4] 	= (SavedPermanentPass >> 8) & 0x00FF;
	EEPROM_Buffer[5] 	=	SavedPermanentPass & 0x00FF;
	EEPROM_Buffer[6] 	= DayCounter_Day ^ (0x23);
	
	ee24_write(DAY_COUNTER_START_ADDRESS, EEPROM_Buffer, 7);
}

void ReadDayCounterValues(void)
{
	ee24_read(DAY_COUNTER_START_ADDRESS,EEPROM_Buffer,7);
	
	DayCounter_Status = EEPROM_Buffer[0];
	DayCounter_Day 		= EEPROM_Buffer[1];
	DayCounter_Hour 	= EEPROM_Buffer[2];
	DayCounter_Minutes= EEPROM_Buffer[3];
	SavedPermanentPass = EEPROM_Buffer[4];
	SavedPermanentPass <<= 8;
	SavedPermanentPass |= EEPROM_Buffer[5];
	DayAccuracy = EEPROM_Buffer[6] ^ (0x23);
	
	
	
	if(DayCounter_Status > 2) 
	{
		DayCounter_Day = 50;
		DayCounter_Hour = 23;
		DayCounter_Minutes = 59;
		DayCounter_Status = DAY_COUNTER_STATUS_50_DAY;
		SaveDayCounterValues();
	}
}


void SaveErrorList(void)
{
	uint8_t iw=0;
	for(iw=0;iw<ERROR_LIST_SIZE;iw++)
	{
		EEPROM_Buffer[iw] = ErrorList[iw];
	}
	EEPROM_Buffer[iw] = ErrorListCounter;
	iw++;
	EEPROM_Buffer[iw] = LastOKInErrorList;
	
	ee24_write(ERROR_LIST_START_ADDRESS, EEPROM_Buffer, ERROR_LIST_SIZE+2);
}


void ReadErrorList(void)
{
	ee24_read(ERROR_LIST_START_ADDRESS,EEPROM_Buffer,ERROR_LIST_SIZE+2);
	
	uint8_t iw=0;
	
	for(iw=0;iw<ERROR_LIST_SIZE;iw++)
	{
		ErrorList[iw] = EEPROM_Buffer[iw];
	}
	ErrorListCounter = EEPROM_Buffer[iw];
	iw++;
	LastOKInErrorList = EEPROM_Buffer[iw];
}

void ReadBoardFixedID(void)
{
	ee24_read(FIXED_ID_START_ADDRESS,EEPROM_Buffer,8);
	
	BoardFixedID = 0;
	for(uint8_t i=0;i<6;i++) 
	{
		BoardFixedID *= 10;
		BoardFixedID += EEPROM_Buffer[i];
	}
}

void SaveDataToEEPROM(void)
{
	uint16_t	ie=0;
	
	EEPROM_Buffer[0] 	= 123;	//Signs after the first program
	
	
	
	for(ie=1;ie < DELAY_1CF_UP_ADD;ie++) //1 to 699
	{
		EEPROM_Buffer[ie] = Stng[ie];
	}
	
	/// Put Floor struct values in Stng array
	for(ie = 0;ie < MAX_NOS;ie++) //700 - 1499
	{
		EEPROM_Buffer[DELAY_1CF_UP_ADD + ie] = Floor[ie].Delay_1CF_Up;
		
		EEPROM_Buffer[DELAY_1CF_DN_ADD + ie] = Floor[ie].Delay_1CF_Down;
		
		EEPROM_Buffer[SEGMENTS_L_ADD + ie] = Floor[ie].SegL;
		
		EEPROM_Buffer[SEGMENTS_R_ADD + ie] = Floor[ie].SegR;
		
		EEPROM_Buffer[TALK_ADD + ie] = Floor[ie].Talk;
		
		EEPROM_Buffer[JUMP1_SPEED + ie] = Floor[ie].Jump1Speed;
		
		EEPROM_Buffer[JUMP1_FLAG + ie] = Floor[ie].Jump1Flag;
		
		EEPROM_Buffer[JUMPN_FLAG + ie] = Floor[ie].JumpNFlag;
		
		EEPROM_Buffer[LEVEL_DISTANCE_ADD + ie] = Floor[ie].LevelDistance;
		
		EEPROM_Buffer[DOOR_SELECT_ADD + ie]	=	(Floor[ie].Door3Select << 4) | (Floor[ie].Door2Select << 2) | (Floor[ie].Door1Select);
	}
	
	//Drive Parallel Signal
	for(ie = 0;ie < 8;ie++) //1500-1507
	{
		EEPROM_Buffer[DRIVE_PARALLEL_ADD + ie]	=	(uint8_t) ((uint8_t)Stng_DriveParallelV2[ie] << 2) | ((uint8_t)Stng_DriveParallelV1[ie] << 1) | (Stng_DriveParallelV0[ie]);
	}
	
	
	ee24_write(MAIN_DATA_START_ADDRESS, EEPROM_Buffer, EE_BUFFER_SIZE);
}


void ReadDataFromEEPROM(void)
{
	uint16_t	ie=0;
	
	ee24_read(MAIN_DATA_START_ADDRESS,EEPROM_Buffer,EE_BUFFER_SIZE);
	
	if(EEPROM_Buffer[0] == 123) //Signs after the first program
	{
		for(ie=1;ie < DELAY_1CF_UP_ADD;ie++) //1 to 699
		{
			Stng[ie] = EEPROM_Buffer[ie];
		}
		
		
		/// Put Floor struct values in Stng array
		for(ie = 0;ie < MAX_NOS;ie++) //700-1499
		{
			Floor[ie].Delay_1CF_Up = EEPROM_Buffer[DELAY_1CF_UP_ADD + ie];
			
			Floor[ie].Delay_1CF_Down = EEPROM_Buffer[DELAY_1CF_DN_ADD + ie];
			
			Floor[ie].SegL = EEPROM_Buffer[SEGMENTS_L_ADD + ie];
			
			Floor[ie].SegR = EEPROM_Buffer[SEGMENTS_R_ADD + ie];
			
			Floor[ie].Talk = EEPROM_Buffer[TALK_ADD + ie];
			
			Floor[ie].Jump1Speed = EEPROM_Buffer[JUMP1_SPEED + ie];
			
			Floor[ie].Jump1Flag = EEPROM_Buffer[JUMP1_FLAG + ie];
			
			Floor[ie].JumpNFlag = EEPROM_Buffer[JUMPN_FLAG + ie];
			
			Floor[ie].LevelDistance = EEPROM_Buffer[LEVEL_DISTANCE_ADD + ie];
			
			uint8_t Data = EEPROM_Buffer[DOOR_SELECT_ADD + ie];	
			Floor[ie].Door1Select = Data & 0x03;
			Floor[ie].Door2Select = (Data >> 2) & 0x03;
			Floor[ie].Door3Select = (Data >> 4) & 0x03;
		}
		
		//Drive Parallel Signal
		for(ie = 0;ie < 8;ie++) //1500-1507
		{
			uint8_t Data = EEPROM_Buffer[DRIVE_PARALLEL_ADD + ie];	
			Stng_DriveParallelV0[ie] = Data & 0x01;
			Stng_DriveParallelV1[ie] = Data & 0x02;
			Stng_DriveParallelV2[ie] = Data & 0x04;
		}
		
		
	}
	
	else
	{
		LoadDefaultValues();
	}
	
}



void LoadDefaultValues(void)
{
	WDT_RESET;
	ee24_read(DEFAULT_DATA_START_ADDRESS,EEPROM_Buffer,EE_BUFFER_SIZE);
	
	if(EEPROM_Buffer[0] != 123)
	{
		
		///////////////////////// Clear Error List
		for(uint8_t iw=0;iw<ERROR_LIST_SIZE;iw++)
		{
			EEPROM_Buffer[iw] = 0;
		}
		ee24_write(ERROR_LIST_START_ADDRESS, EEPROM_Buffer, ERROR_LIST_SIZE);
		
		
		SetFixedBoardID();
		
		DayCounter_Day = 50;
		DayCounter_Hour = 23;
		DayCounter_Minutes = 59;
		DayCounter_Status = DAY_COUNTER_STATUS_50_DAY;
		SaveDayCounterValues();
		
		///////////////////////// Load factory values from ROM
		for(uint16_t iw=0;iw<EE_BUFFER_SIZE;iw++)
		{
			EEPROM_Buffer[iw] = ResetFactory[iw];
		}
		
		ee24_write(MAIN_DATA_START_ADDRESS, EEPROM_Buffer, EE_BUFFER_SIZE);
		ee24_write(DEFAULT_DATA_START_ADDRESS, EEPROM_Buffer, EE_BUFFER_SIZE);
	}
	else 
	{
		ee24_write(MAIN_DATA_START_ADDRESS, EEPROM_Buffer, EE_BUFFER_SIZE);
	}
	WDT_RESET;
	HAL_Delay(10);
	WDT_RESET;
	ReadDataFromEEPROM();
}



void SetFixedBoardID(void)
{
	
	uint8_t SelectNum=0,ValueNum=0,ID_Num[8];
	for(uint8_t i=0;i<8;i++) {ID_Num[i] = 0;}
	
	while(1)
	{
		WDT_RESET;
		
		if(K_Ent)
		{
			SelectNum++;
			if(SelectNum > 5) {SelectNum = 0;}
			ValueNum = ID_Num[SelectNum];
			K_Ent = 0;
		}
		else if(K_Ext)
		{
			SelectNum--;
			if(SelectNum > 5) {SelectNum = 5;}
			ValueNum = ID_Num[SelectNum];
			K_Ext = 0;
		}
		else
		{
			if(K_Up)
			{
				ValueNum++;
				if(ValueNum > 9) ValueNum = 0;
				K_Up = 0;
			}
			if(K_Dn)
			{
				ValueNum--;
				if(ValueNum > 9) ValueNum = 9;
				K_Dn = 0;
			}
			
			ID_Num[SelectNum] = ValueNum;
		}
		
		if(KeyTime[1] > 200)
		{
			for(uint8_t i=0;i<8;i++) 
			{
				EEPROM_Buffer[i] = ID_Num[i];
			}
			
			ee24_write(FIXED_ID_START_ADDRESS, EEPROM_Buffer, 8);
			
			break;
		}
		
		if(LCD_UpdateF == 0)
		{
			sprintf(LcdStr,"Fixed ID");
			LCD_write_str_middle(1,LcdStr);

			sprintf(LcdStr,"%u %u %u %u %u %u",ID_Num[0],ID_Num[1],ID_Num[2],ID_Num[3],ID_Num[4],ID_Num[5]);
			LCD_write_str_middle(30,LcdStr);
			sprintf(LcdStr,"--");
			LCD_write_str(13+SelectNum*12,38,LcdStr);
			
			sprintf(LcdStr,"%u",KeyTime[1]);
			LCD_write_str_middle(51,LcdStr);
			
			LCD_Update();
		}
	}
	
	ReadBoardFixedID();

}

void 	PrintAndSaveDefaultValue(void)
{
	ee24_read(MAIN_DATA_START_ADDRESS, EEPROM_Buffer, EE_BUFFER_SIZE);
	ee24_write(DEFAULT_DATA_START_ADDRESS, EEPROM_Buffer, EE_BUFFER_SIZE); //Load Default Address
	
	LCD_Clear();
	sprintf(LcdStr,"Print Values");
	LCD_write_str_middle(22,LcdStr);
	LCD_Update();
	
	PR_SendData(sprintf(PR_TX_Buffer,"\r\n\r\n////////////////////////////////////\r\n"));
	HAL_Delay(50);
	PR_SendData(sprintf(PR_TX_Buffer,"//////////////  START  /////////////\r\n"));
	HAL_Delay(50);
	PR_SendData(sprintf(PR_TX_Buffer,"////////////////////////////////////\r\n"));
	HAL_Delay(50);
	
	uint8_t	EnterT = 0;
	for(uint16_t iw=0;iw<EE_BUFFER_SIZE;iw++)
	{
		PR_SendData(sprintf(PR_TX_Buffer,"%3u,",EEPROM_Buffer[iw]));
		HAL_Delay(6);
		EnterT++;
		if(EnterT > 29)
		{
			PR_SendData(sprintf(PR_TX_Buffer,"\r\n"));
			EnterT = 0;
			HAL_Delay(4);
			
		}
	}
	
	//HAL_Delay(500);
}


void CheckLoadAndSaveDefault(void)
{
	if(KEY_UP && KEY_DN)  			
	{
		Show_PleaseWait();
		PrintAndSaveDefaultValue();
	}
	else if(KEY_EXT && KEY_ENT) 
	{
		LCD_Clear();
		sprintf(LcdStr,"Release ENT To");
		LCD_write_str_middle(17,LcdStr);
		sprintf(LcdStr,"Load Default");
		LCD_write_str_middle(34,LcdStr);
		LCD_Update();
		while(KEY_EXT || KEY_ENT);
		
		sprintf(LcdStr,"Do you want");
		LCD_write_str(1,1,LcdStr);
		sprintf(LcdStr,"Load default?");
		LCD_write_str(1,17,LcdStr);
		sprintf(LcdStr,"Yes: ENT");
		LCD_write_str(1,34,LcdStr);
		sprintf(LcdStr,"No: EXT");
		LCD_write_str(1,51,LcdStr);
		LCD_Update();
		
		while(!KEY_EXT && !KEY_ENT);
		
		if(KEY_ENT)
		{
			Show_PleaseWait();
			LoadDefaultValues();
			HAL_Delay(300);
		}
	}
}




void Show_PleaseWait(void)
{
	LCD_Clear();
	sprintf(LcdStr,"Please Wait");
	LCD_write_str_middle(22,LcdStr);
	LCD_Update();
}









