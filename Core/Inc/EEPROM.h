#include "main.h"

#define		EE_BUFFER_SIZE	3500

#define 	MAIN_DATA_START_ADDRESS								0			//Size: 3500 Byte
#define  	DEFAULT_DATA_START_ADDRESS						3500	//Size: 3500 Byte
#define 	ERROR_LIST_START_ADDRESS							7000	//Size: 100 Byte
#define 	FIXED_ID_START_ADDRESS 								7100	//Size: 10 Byte
#define 	HW_KEY_PASSWORD_START_ADDRESS					7110	//Size: 20 Byte
#define 	DAY_COUNTER_START_ADDRESS							7130 	//Size: 10 Byte

void SaveHwKeyPassword(void);
void ReadHwKeyPassword(void);
void SaveDayCounterValues(void);
void ReadDayCounterValues(void);
void SaveDataToEEPROM(void);
void ReadDataFromEEPROM(void);
void LoadDefaultValues(void);
void CheckLoadAndSaveDefault(void);
void Show_PleaseWait(void);
void SetFixedBoardID(void);
void SaveErrorList(void);
void ReadErrorList(void);
void ReadBoardFixedID(void);


