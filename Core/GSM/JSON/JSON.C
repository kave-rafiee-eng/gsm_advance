#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "json.h"

#include "../Inc/Variable.h"
#include "../Inc/Serial.h"

struct JSON_OUT	 json;
struct JSON_PROTOCOL json_protocol;

int strfind(char *str,char *substr){
	int ad = strstr(str,substr)  - (char *)&str[0];
	if( ad >= 0 )return ad;
	return -1;
}

int json_find(char *buffer_uart,char *command){
	
	if( strfind(buffer_uart,command) >= 0 ){
		int ad = strfind(buffer_uart,(char *)command);
		return ad+strlen((char *)command);
	}
	return -1;
}

char num[10];
int ad =0;
char json_get_data( char *buffer , char *command ){
    
		//strcpy( json.str_data , "");
		memset(json.str_data,0,strlen(json.str_data));
	
    
    ad = json_find(buffer,command);
	
		if( ad == -1 )return 0;
    
    char i=0;
    if( buffer[ad] == '"' ){
        while( buffer[ad+1+i] != ',' && buffer[ad+1+i] != '"' ){
    		json.str_data[i] = buffer[ad+i+1];
    		i++;
    	}
        return 1;        
    }
    else{
        memset(num,0,10);
        while( buffer[ad+i] != ',' &&  buffer[ad+i] != '}' ){
            
    		num[i] = buffer[ad+i];
    		i++;
					if(i>5)return 0;
    	}
    	json.data = atoi(num);
    	return 2;
    }
		
		return 0;
}


void read_protocol_json(){
	
	if( json_get_data(json.document , "\"t1\":") == TYPE_WORD ){
		strcpy( json_protocol.name_w1 , "");
		strcpy( json_protocol.name_w1 , json.str_data);
						
		if( json_get_data(json.document , "\"data_w1\":") == TYPE_STR ){
			strcpy( json_protocol.data_w1 , "");
			strcpy( json_protocol.data_w1 , json.str_data);
			json_protocol.data_w1_type = TYPE_STR;
		}
		else if( json_get_data(json.document , "\"data_w1\":") == TYPE_WORD ){
			json_protocol.data_w1_word = json.data;
			json_protocol.data_w1_type = TYPE_WORD;
		}    
	}
	
	
	if( json_get_data(json.document , "\"name_r1\":") == TYPE_STR ){
		strcpy( json_protocol.name_r1 , "");
		strcpy( json_protocol.name_r1 , json.str_data);
						
		if( json_get_data(json.document , "\"data_r1\":") == TYPE_STR ){
			strcpy( json_protocol.data_r1 , "");
			strcpy( json_protocol.data_r1 , json.str_data);
			json_protocol.data_r1_type = TYPE_STR;
		}
		else if( json_get_data(json.document , "\"data_r1\":") == TYPE_WORD ){
			json_protocol.data_r1_word = json.data;
			json_protocol.data_r1_type = TYPE_WORD;
		}    
	}

}

void reset_json(){
	
	json_protocol.data_r1_type=0;
	json_protocol.data_w1_type=0;
	json_protocol.data_r1_word=0;
	json_protocol.data_w1_word=0;
	
	memset( json_protocol.data_r1 ,0,strlen(json_protocol.data_r1));
	memset( json_protocol.data_w1 ,0,strlen(json_protocol.data_w1));
	
	memset( json_protocol.name_w1 ,0,strlen(json_protocol.name_w1));
	memset( json_protocol.name_r1 ,0,strlen(json_protocol.name_r1));
	
	memset( json.document ,0,strlen(json.document));
	
}

void json_get_data_dma(){
	
	strcpy( json.document ,"");
	strcpy(json.document,(const char*)HALL_RX_Buffer);
	//puts(json.document);
			
	//strcpy(HALL_RX_Buffer,"");
					
	DMA1_Stream2->CR  = 0; 
	DMA1_Stream2->NDTR = UART_BUFFER_SIZE;
	memset(HALL_RX_Buffer,0,UART_BUFFER_SIZE);
	DMA1_Stream2_Init();
	
}

int replacechar(char *str, char orig, char rep) {
    char *ix = str;
    int n = 0;
    while((ix = strchr(ix, orig)) != NULL) {
        *ix++ = rep;
        n++;
    }
    return n;
}

int server_protocol_arreay_select;
int server_protocol_byte_count;
int server_protocol_data;
int server_protocol_status;

char buffer_http_send_to_server[200];
char buffer_http_send_to_server_index=0;

void server_protocol_json(){
	
	memset(buffer_http_send_to_server,0,200);
	buffer_http_send_to_server_index=0;
	
	if( json_get_data(json.document , "\"ar1\":") == TYPE_WORD ){
		server_protocol_arreay_select = json.data;				
		if( json_get_data(json.document , "\"ad1\":") == TYPE_WORD ){
			server_protocol_byte_count = json.data;
			if( json_get_data(json.document , "\"da1\":") == TYPE_WORD ){
				server_protocol_data = json.data;	
				if( json_get_data(json.document , "\"st1\":") == TYPE_WORD ){
					server_protocol_status = json.data;
					
					memset(buffer_http_send_to_server,0,200);
					sprintf(buffer_http_send_to_server,"{\"serial\":\"100\",\"url\":\"");
					server_protocol_data_manage();
					strcat(buffer_http_send_to_server,"\"}");			
					modbus_master_write_register_MULTI(SLAVE_ADD,FC_WRITE_TO_SLAVE_MULTI,2,strlen(buffer_http_send_to_server),buffer_http_send_to_server);
					
					
				}
			}
		}	
	}	
}


/*

	if( json_get_data(json.document , "\"t1\":") == TYPE_WORD ){
		strcpy( json_protocol.name_w1 , "");
		strcpy( json_protocol.name_w1 , json.str_data);
						
		if( json_get_data(json.document , "\"data_w1\":") == TYPE_STR ){
			strcpy( json_protocol.data_w1 , "");
			strcpy( json_protocol.data_w1 , json.str_data);
			json_protocol.data_w1_type = TYPE_STR;
		}
		else if( json_get_data(json.document , "\"data_w1\":") == TYPE_WORD ){
			json_protocol.data_w1_word = json.data;
			json_protocol.data_w1_type = TYPE_WORD;
		} 


*/




