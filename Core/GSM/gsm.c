#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "gsm.h"

#include "../Inc/Variable.h"
#include "../Inc/Serial.h"

// CPU Timer
extern struct cpu_timer_8bit_reset_contorol_Seconds tbrc_s1[def_num_tbrc_s1];
extern struct cpu_timer_basic_10bit_auto_reset tbr_g1[def_num_tbr_g1];

// External variables
extern volatile uint8_t EEPROMSaveFlag;
extern char HALL_TX_Buffer[UART_BUFFER_SIZE];
extern volatile char HALL_RX_Buffer[UART_BUFFER_SIZE];


// External structures
extern struct JSON_OUT json;
extern struct MODBUS_SLAVE modbus_slave;

char F_save_EEPROM=0;

// 
int i_get = 0;
int last_i_get = 0;

void GSM_Processing() {


	  tbr_g1[tbr_g1_SECOUND].EN = 1;  
    tbr_g1[tbr_g1_SECOUND].C_set_time = 1000;
    if (tbr_g1[tbr_g1_SECOUND].F_end) { tbr_g1[tbr_g1_SECOUND].F_end = 0;
			timer_second_manager();
		}
			
			
		if( F_save_EEPROM == 1 ){ F_save_EEPROM=0;
			tbrc_s1[tbrc_s1_EEPROM_SAVE].EN=1;
			tbrc_s1[tbrc_s1_EEPROM_SAVE].AUTO=0;
			tbrc_s1[tbrc_s1_EEPROM_SAVE].C_set_time=4;		
		}
		if( tbrc_s1[tbrc_s1_EEPROM_SAVE].F_end ){ tbrc_s1[tbrc_s1_EEPROM_SAVE].F_end=0;
				EEPROMSaveFlag=1;
				tbrc_s1[tbrc_s1_EEPROM_SAVE].EN=0;
		}
	
		if (IS_DMA_TRANSFER_COMPLETE()) {
				CLEAR_DMA_TRANSFER_FLAG();  // Clear transfer complete flag
				RS485_RECEIVE_MODE();       // Set RS485 to receive mode
		}

    // Calculate the number of received bytes in the buffer
    i_get = UART_BUFFER_SIZE - DMA_NDTR;
    
    // Check if the received byte count has changed
    if (i_get != last_i_get) {
        last_i_get = i_get;
        tbr_g1[tbr_g1_TEST].I_time = 0; // Reset timer on new data
    }
    
    // Enable or disable the timer based on the received data
    if (i_get > 0) {
        tbr_g1[tbr_g1_TEST].EN = 1;  // Enable timer
    } else {
        tbr_g1[tbr_g1_TEST].EN = 0;  // Disable timer
    }
    
    // Set the timer's cycle time (5 ms)
    tbr_g1[tbr_g1_TEST].C_set_time = 5;
    
    // Check if the timer has ended
    if (tbr_g1[tbr_g1_TEST].F_end) {
        tbr_g1[tbr_g1_TEST].F_end = 0;  // Reset timer end flag
        
        // If data is available, process Modbus data
        if (i_get > 0) {
            modbus_get_data_dma();  // Get data via DMA
            modbus_slave_manage();   // Manage Modbus slave communication
            tbr_g1[tbr_g1_TEST].EN = 0;  // Disable timer after data processing
        }
    }
    
    // If new data is received from Modbus slave, process it
    if (modbus_slave.F_new_data) {
        modbus_slave.F_new_data = 0;  // Reset new data flag
        
        // Clear the previous JSON document
        memset(json.document, 0, strlen(json.document));
        
        // Copy Modbus data to the JSON document
        for (int i = 0; i < modbus_slave.data_count; i++) {
            json.document[i] = modbus_slave.buf[i];
        }
        
        // Check if the serial number is present in the Modbus data
        if (json_get_data(json.document, "\"serial\":") == TYPE_STR) {
            // If the serial number exists, we can proceed with specific actions
            // For now, just call server_protocol_json
            server_protocol_json();
        } else {
            // Otherwise, call the server_protocol_json function
            server_protocol_json();
        }
    }
}


void gsm_init(){
	
	ini_cpu_timer();
	
}



extern struct MODBUS_RTU modbus;

void modbus_get_data_dma(){
	
	DMA_CR  = 0; 
	
	memset(modbus.buf_rx,0,UART_BUFFER_SIZE);
	
	modbus.len =  UART_BUFFER_SIZE - DMA_NDTR;
	for( char i=0; i<modbus.len; i++ ){
		modbus.buf_rx[i]= GSM_RX_Buffer[i];
	}
					
	DMA_NDTR = UART_BUFFER_SIZE;
	memset(GSM_RX_Buffer,0,UART_BUFFER_SIZE);
	GSM_DMA_Init();
	
}


int UART_GSM_send (int ch) {
 
	UART_DR = ch;
			
	while ( READ_BIT(UART_SR, USART_SR_TXE) == 0){}

  return 1;
}


