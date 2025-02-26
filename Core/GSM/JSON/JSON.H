
#ifndef __JSON_H
#define __JSON_H

#include "main.h"

#define TYPE_STR 1
#define TYPE_WORD 2

void server_protocol_json();

int strfind(char *str,char *substr);
int json_find(char *buffer_uart,char *command);
char json_get_data( char *buffer , char *command );
void reset_json();
void json_get_data_dma();
int replacechar(char *str, char orig, char rep);
void server_protocol_json();

struct JSON_OUT 
{
	char document[301];
  char str_data[100];
	int data;
};


// Structure to manage server protocol status
typedef struct {
    int array_select;      // server_protocol_arreay_select
    int byte_count;        // server_protocol_byte_count
    int data;              // server_protocol_data
    int status;            // server_protocol_status
} ServerProtocol;


#endif /* __JSON_H */