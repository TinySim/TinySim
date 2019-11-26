#include "config.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "coap.h"
#include <time.h>

// #ifdef _cplusplus
// extern "C" {
// #endif

/*
 * the return value is some string from server
 */
void my_defined_coap_send(char ip[],char port[],char path[], int methodType,char infotext[],char result_info[],int *interval, int *delay);

void coap_send_data_by_get(char host[] ,char port[],char path[],char result_info[]);

// void  my_defined_coap_send(char myip[],char myport[],char mypath[], int methodType,char infotext[],char result_info[], int *interval, int *delay) {

// #ifdef _cplusplus
// }
// #endif