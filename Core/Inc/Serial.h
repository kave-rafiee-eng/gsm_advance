#include "main.h"
#include "EEPROM.h"


//// Print Serial
#define	PR_BUFFER_SIZE  				(EE_BUFFER_SIZE + 10)
#define	PR_TX_DMA_CNT  					DMA2_Stream7->NDTR
#define	PR_RX_DMA_CNT   				(PR_BUFFER_SIZE	-	DMA2_Stream5->NDTR)
#define	MODULE_EEPROM_SERIAL_ID	0xAB
#define MODULE_READY						0x12
#define	MODULE_GET_DATA					0x22
#define	MODULE_SEND_DATA				0x32
#define	MODULE_GET_KEY					0x42


#define UART_BUFFER_SIZE				300

//// Doublex Serial
#define	DBLX_TX_DMA_CNT  				DMA1_Stream6->NDTR
#define	DBLX_RX_DMA_CNT   			(UART_BUFFER_SIZE	-	DMA1_Stream5->NDTR)

//// Drive Serial
#define	DRV_TX_DMA_CNT  				DMA1_Stream7->NDTR
#define	DRV_RX_DMA_CNT  				(UART_BUFFER_SIZE	-	DMA1_Stream0->NDTR)


//// Hall Station Serial
#define	HALL_TX_DMA_CNT  				DMA1_Stream4->NDTR
#define	HALL_RX_DMA_CNT  				(UART_BUFFER_SIZE	-	DMA1_Stream2->NDTR)


//// Cabin Serial
#define	CAB_TX_DMA_CNT  				DMA1_Stream3->NDTR
#define	CAB_RX_DMA_CNT  				(UART_BUFFER_SIZE	-	DMA1_Stream1->NDTR)

#define	CAB_SERIAL_ID								0xAA
#define SERIAL_EXTENTION_SERIAL_ID	0xBB
#define	PARALLEL_ANALOG_MODULE_ID		0xCC




void CAB_SerialDataTransmition(void);
void HALL_SerialExtentionDataTransmition(void);

void ParallelAnalog_OD_Module_SerialDataTransmition(void);

void Module_SerialDataTransmition(void);


