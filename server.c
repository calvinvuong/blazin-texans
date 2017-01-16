#include "game_mec.h"
#include "networking.c"

/*STANDARDS:
use port 1379 for the initial connection to the listener
use port 3019 for the connection between game and client
*/

void print_IP( unsigned int queue[], int size ) {
  printf("IP Queue: ");
  int i;
  for ( i = 0; i < size; i++ ) {
    //printf("%s ", inet_ntoa(queue[i]));
    printf("%d, ", queue[i]);
  }
  printf("\n");
}

// actively listens for new connections and adds their ip to ip_queue
int listener( int sd, unsigned int *ip_queue, int *queue_size ) {
  int connection;
  connection = initial_server_connect(sd, &(ip_queue[*queue_size]));
  
  char buffer[500];
  read( connection, &buffer, sizeof(buffer) );
  if ( strcmp(buffer, "client to listener") == 0 )
    write( connection, "received client to listener; please standby", sizeof("received client to listener; please standby") );
  close(connection);

  (*queue_size)++;
  print_IP(ip_queue, *queue_size);

  return 0;
}

int main() {
  
  int sd, connection;
  unsigned int ip_queue[20];
  int queue_size = 0;
  
  sd = server_setup(1379);
  
  while (1) {
    listener(sd, ip_queue, &queue_size);
    
    /*
    connection = initial_server_connect(sd, &(ip_queue[queue_size]));
    queue_size++;
    
    char buffer[500];
    read( connection, &buffer, sizeof(buffer) );
    if ( strcmp(buffer, "client to listener") == 0 )
      write( connection, "received client to listener; please standby", sizeof("received client to listener; please standby") );
    close(connection);

    
    print_IP(ip_queue, queue_size);
    */
  }    

  return 0;
}
