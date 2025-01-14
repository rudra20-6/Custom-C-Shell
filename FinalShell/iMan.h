#ifndef IMAN_H_INCLUDED  
#define IMAN_H_INCLUDED  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SERVER_IP "65.19.140.5" // IP address for man.he.net
#define SERVER_PORT 80
#define BUFFER_SIZE 10000

void displayMan(char command_name[]);


#endif
