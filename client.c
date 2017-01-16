#include "game_mec.h"
#include "networking.c"

int main() {

  char *host = "149.89.150.103";
  int sd;

  sd = client_connect(host, 1379);

  char buffer[500];
  //  int i = 0;
  //while ( 1 ) {

  write( sd, "client to listener", sizeof("client to listener")  );
  
  read(sd, &buffer, sizeof(buffer));
  
  close(sd);
  
  printf("%s\n", buffer);
    //}    

  return 0;
}
