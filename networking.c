#include "networking.h"

void error_check( int i, char *s ) {
  if ( i < 0 ) {
    printf("%d\n", i);
    printf("[%s] error %d: %s\n", s, errno, strerror(errno) );
    exit(1);
  }
}

int server_setup(int port) {
  
  int sd;
  int i;
  
  sd = socket( AF_INET, SOCK_STREAM, 0 );
  error_check( sd, "server socket" );
  
  struct sockaddr_in sock;
  sock.sin_family = AF_INET;
  sock.sin_addr.s_addr = INADDR_ANY;
  sock.sin_port = htons(port);
  i = bind( sd, (struct sockaddr *)&sock, sizeof(sock) );
  error_check( i, "server bind" );
  
  return sd;
}

// to be run by client; similar to server setup
// port should be 3019
int client_setup(int port) {
  int sd;
  int i;
  
  sd = socket( AF_INET, SOCK_STREAM, 0 );
  error_check( sd, "client socket" );
  
  struct sockaddr_in sock;
  sock.sin_family = AF_INET;
  sock.sin_addr.s_addr = INADDR_ANY;
  sock.sin_port = htons(port);
  i = bind( sd, (struct sockaddr *)&sock, sizeof(sock) );
  error_check( i, "server bind" );
  
  return sd;
}

int initial_server_connect(int sd, unsigned int *ip, int timeout) {
  int connection, i;
  if ( timeout == 1 ) {
    struct timeval time;      
    time.tv_sec = 5;
    time.tv_usec = 0;
    setsockopt (sd, SOL_SOCKET, SO_RCVTIMEO, (char *)&time, sizeof(time));
  }
  i = listen(sd, 1);
  error_check( i, "server listen" );
  
  struct sockaddr_in sock1;
  unsigned int sock1_len = sizeof(sock1);
  connection = accept( sd, (struct sockaddr *)&sock1, &sock1_len );
  //error_check( connection, "server accept" );

  if ( connection == -1 ) // if timeout occurred, then do not alter ip list
    return connection;

  printf("[server] connected to %s\n", inet_ntoa( sock1.sin_addr ) );
  *ip = sock1.sin_addr.s_addr;
  
  return connection;
}

// to be run by the client
int secondary_server_connect(int sd) {
  int connection, i;

  i = listen(sd, 1);
  error_check( i, "server listen" );
  
  struct sockaddr_in sock1;
  unsigned int sock1_len = sizeof(sock1);
  connection = accept( sd, (struct sockaddr *)&sock1, &sock1_len );
  error_check( connection, "server accept" );
  
  printf("[real client] connected to %s\n", inet_ntoa( sock1.sin_addr ) );

  return connection;
}


int client_connect( char *host, int port ) {
  int sd, i;
  
  sd = socket( AF_INET, SOCK_STREAM, 0 );
  error_check( sd, "client socket" );
  
  struct sockaddr_in sock;
  sock.sin_family = AF_INET;
  inet_aton( host, &(sock.sin_addr));
  sock.sin_port = htons(port);
  
  printf("[client] connecting to: %s\n", host );
  i = connect( sd, (struct sockaddr *)&sock, sizeof(sock) );
  error_check( i, "client connect");
  
  return sd;
}

// same as client_connect(), but takes in the host ip in binary
int client_connectB( unsigned int host, int port ){
  int sd, i;
  
  sd = socket( AF_INET, SOCK_STREAM, 0 );
  error_check( sd, "client socket" );
  
  struct sockaddr_in sock;
  sock.sin_family = AF_INET;
  //inet_aton( host, &(sock.sin_addr));
  sock.sin_addr.s_addr = host;
  sock.sin_port = htons(port);
  
  printf("[client] connecting to: %d\n", host );
  i = connect( sd, (struct sockaddr *)&sock, sizeof(sock) );
  error_check( i, "client connect");
  
  return sd;
}

