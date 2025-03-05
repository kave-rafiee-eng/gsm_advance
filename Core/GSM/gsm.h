
#ifndef __GSM_H
#define __GSM_H

#include "main.h"

void GSM_Processing();
void modbus_get_data_dma();
int UART_GSM_send (int ch);

// Macro to check if DMA transfer is complete (TCIF4 flag is set)
#define IS_DMA_TRANSFER_COMPLETE()  (DMA1->HISR & DMA_HISR_TCIF4)

// Macro to clear the DMA transfer complete flag (CTCIF4)
#define CLEAR_DMA_TRANSFER_FLAG()   (DMA1->HIFCR |= DMA_HIFCR_CTCIF4)

// Macro to switch RS485 to receive mode (DE = 0)
#define RS485_RECEIVE_MODE()        HW_DIR_HALL(0)


#define GSM_SendData(x) HALL_SendData(x)

#define GSM_DMA_Init() DMA1_Stream2_Init()

#define GSM_TX_Buffer HALL_TX_Buffer
#define GSM_RX_Buffer HALL_RX_Buffer

#define DMA_CR DMA1_Stream2->CR
#define DMA_BUFFER HALL_RX_Buffer
#define DMA_NDTR DMA1_Stream2->NDTR

#define UART_DR UART4->DR
#define UART_SR UART4->SR



#endif /* __GSM_H */