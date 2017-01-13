#include "game_mec.h"
#include "networking.h"

int main() {
  
  int sd, connection;
  sd = server_setup();

  while (1) {
    connection = server_connect(sd);
    
    // read from connection to get client ip and port
    int f = fork();
    if ( f == 0 ) {
      close(sd);
      exit(0);
    }
    else
      close(connection);
  }
    


  return 0;
}
