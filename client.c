#include "game_mec.h"
#include "networking.h"

int main() {

  char *host = "149.89.150.103";
  int sd;

  sd = client_connect(host, 1379);

  int port; // this is the port to listen on

  // initiate connection
  write( sd, "client to listener", sizeof("client to listener")  );
  // get port to listen on
  read(sd, &port, sizeof(int));
  close(sd);
  
  printf("%d\n", port);

  // receive connection from actual server for game communications
  int connection;
  sd = server_setup(port);
  connection = secondary_server_connect(sd);

  char read_buffer[500];
  char write_buffer[500];

  // first operation should always be to listen
  while (1) {
    read(connection, &read_buffer, sizeof(read_buffer));
    if ( strcmp(read_buffer, "write") == 0 ) {
      printf("send something to server: \n");
      fgets(write_buffer, sizeof(write_buffer), stdin);
      *strchr(write_buffer, '\n') = 0; // get rid of new line
      write(connection, write_buffer, sizeof(write_buffer));
    }
    else if ( strcmp(read_buffer, "read") == 0 ) {
      read(connection, &read_buffer, sizeof(read_buffer));
      printf("here's something from the server\n");
      printf("%s\n", read_buffer);
    }
  }
  return 0;
}
