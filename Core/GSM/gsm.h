
#ifndef __GSM_H
#define __GSM_H

#include "main.h"

void GSM_Processing();
void modbus_get_data_dma();
int UART_GSM_send (int ch);

#define DMA_CR DMA1_Stream2->CR
#define DMA_BUFFER HALL_RX_Buffer
#define DMA_NDTR DMA1_Stream2->NDTR

#define UART_DR UART4->DR
#define UART_SR UART4->SR

#define MODBUS_DRIVE(x) HW_DIR_HALL(x)

#endif /* __GSM_H */