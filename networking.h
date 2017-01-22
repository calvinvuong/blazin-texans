#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>


#ifndef NETWORKING_H
#define NETWORKING_H

#define MESSAGE_BUFFER_SIZE 1000

#define HOST_IP "149.89.150.103"
#define PORT1 1379
#define PORT2 3019

void error_check( int i, char *s );

int server_setup(int port);
int initial_server_connect(int sd, unsigned int *ip, int timeout);
int secondary_server_connect(int sd);

int client_connect( char *host, int port );
int client_connectB( unsigned int host, int port );
#endif
