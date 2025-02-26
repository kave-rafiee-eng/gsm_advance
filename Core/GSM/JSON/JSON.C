#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "json.h"

#include "../Inc/Variable.h"
#include "../Inc/Serial.h"

struct JSON_OUT	 json;

// Find substring position
int strfind(char *str, char *substr) {
    char *pos = strstr(str, substr);
    return (pos) ? (pos - str) : -1;
}


// Locate a JSON field in the buffer
int json_find(char *buffer_uart, char *command) {
    int pos = strfind(buffer_uart, command);
    return (pos >= 0) ? pos + strlen(command) : -1;
}

// Extract data from JSON buffer
char json_get_data(char *buffer, char *command) {
    memset(json.str_data, 0, sizeof(json.str_data));

    int ad = json_find(buffer, command);
    if (ad == -1) return 0;

    char i = 0;
    if (buffer[ad] == '"') {  
        while (buffer[ad + 1 + i] != ',' && buffer[ad + 1 + i] != '"' && i < sizeof(json.str_data) - 1) {
            json.str_data[i] = buffer[ad + i + 1];
            i++;
        }
        json.str_data[i] = '\0';  // Ensure string termination
        return TYPE_STR;
    } else {
        char num[10] = {0};
        while (buffer[ad + i] != ',' && buffer[ad + i] != '}' && i < sizeof(num) - 1) {
            num[i] = buffer[ad + i];
            i++;
        }
        json.data = atoi(num);
        return TYPE_WORD;
    }
    return 0;
}

// Reset JSON protocol variables
void reset_json() {
    memset(json.document, 0, sizeof(json.document));
}


// Read JSON data from DMA buffer
void json_get_data_dma() {
    strncpy(json.document, (const char *)HALL_RX_Buffer, sizeof(json.document) - 1);
    json.document[sizeof(json.document) - 1] = '\0';

    // Reset DMA
    DMA1_Stream2->CR = 0;
    DMA1_Stream2->NDTR = UART_BUFFER_SIZE;
    memset(HALL_RX_Buffer, 0, UART_BUFFER_SIZE);
    DMA1_Stream2_Init();
}


// Replace a character in a string
int replacechar(char *str, char orig, char rep) {
    char *ix = str;
    int count = 0;
    while ((ix = strchr(ix, orig)) != NULL) {
        *ix++ = rep;
        count++;
    }
    return count;
}



// Declare a variable of type ServerProtocol
ServerProtocol server_protocol;

char buffer_http_send_to_server[200];
char buffer_http_send_to_server_index=0;
	
// Process server protocol JSON
void server_protocol_json(){
	
	buffer_http_send_to_server_index=0;
	
  memset(buffer_http_send_to_server, 0, sizeof(buffer_http_send_to_server));
  snprintf(buffer_http_send_to_server, sizeof(buffer_http_send_to_server), "{\"serial\":\"100\"");
	
	for( char n=0; n<10; n++){
		
			char str_cmp[50];
			sprintf(str_cmp,"\"ar%d\":",n);
			if( json_get_data(json.document , str_cmp)  > 0 ){
				if( json_get_data(json.document , str_cmp)  == TYPE_STR )server_protocol.array_select = atoi(json.str_data);
				if( json_get_data(json.document , str_cmp)  == TYPE_WORD )server_protocol.array_select = json.data;

				sprintf(str_cmp,"\"ad%d\":",n);	
				if( json_get_data(json.document , str_cmp) > 0 ){
					if( json_get_data(json.document , str_cmp)  == TYPE_STR )server_protocol.byte_count = atoi(json.str_data);
					if( json_get_data(json.document , str_cmp)  == TYPE_WORD )server_protocol.byte_count = json.data;			

					sprintf(str_cmp,"\"da%d\":",n);	
					if( json_get_data(json.document , str_cmp) > 0 ){
						if( json_get_data(json.document , str_cmp)  == TYPE_STR )server_protocol.data = atoi(json.str_data);
						if( json_get_data(json.document , str_cmp)  == TYPE_WORD )server_protocol.data = json.data;		

						sprintf(str_cmp,"\"st%d\":",n);	
						if( json_get_data(json.document , str_cmp) > 0){
							if( json_get_data(json.document , str_cmp)  == TYPE_STR )server_protocol.status = atoi(json.str_data);
							if( json_get_data(json.document , str_cmp)  == TYPE_WORD )server_protocol.status = json.data;		
								
								
								server_protocol_data_manage();
			
						}
					}
				}	
			}		
	}

	strcat(buffer_http_send_to_server,"}");		
	modbus_master_write_register_MULTI(SLAVE_ADD,FC_WRITE_TO_SLAVE_MULTI,2,strlen(buffer_http_send_to_server),buffer_http_send_to_server);	
		
}





