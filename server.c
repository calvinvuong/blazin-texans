#include "game_mec.h"
#include "networking.c"

void print_IP( unsigned int queue[], int size ) {
  printf("IP Queue: ");
  int i;
  for ( i = 0; i < size; i++ ) {
    //printf("%s ", inet_ntoa(queue[i]));
    printf("%d, ", queue[i]);
  }
  printf("\n");
}


int main() {
  
  int sd, connection;
  unsigned int ip_queue[20];
  int queue_size = 0;

  sd = server_setup();
  
  while (1) {
    connection = server_connect(sd, &(ip_queue[queue_size]));
    
    unsigned int buffer;
    read( connection, &buffer, sizeof(buffer) );
    //ip_queue[queue_size] = buffer;
    queue_size++;
    
    write( connection, "received", sizeof("received") );
    close(connection);

    print_IP(ip_queue, queue_size);
   
  }    

  return 0;
}
