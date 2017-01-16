#include "game_mec.h"
#include "networking.h"

int main() {

  char *host = "149.89.150.103";
  int sd;

  sd = client_connect(host);

  char buffer[500];
  
  while (1) {
    unsigned int ip;
    ip = inet_aton("149.89.150.104");
    
    write( sd, ip, sizeof(ip) );

    read(sd, buffer, sizeof(buffer));
    
    close(connection);

    printf("%s\n", buffer);
  }    

  return 0;
}
