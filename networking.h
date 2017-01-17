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


void error_check( int i, char *s );

int server_setup(int port);
int initial_server_connect(int sd, unsigned int *ip, int timeout);


int client_connect( char *host, int port );

#endif
