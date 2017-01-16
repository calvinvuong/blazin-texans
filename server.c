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

// transfers correct # of ip addresses in queue to a player list
// removes transferred ip addresses form ip_queue
// returns num_players
int transfer_IPs( unsigned int *ip_queue, int *queue_size, unsigned int *player_IPs, int num_players ) {
  int i;
  // fill player_IPs w/ zeroes
  for ( i = 0; i < 4; i++ )
    player_IPs[i] = 0;
  // fill player_IPs with IPs FROM ip_queue
  for ( i = 0; i < num_players; i++ )
    player_IPs[i] = ip_queue[i];

  // remove transferred ips from ip_queue
  int j = num_players;
  for ( i = 0; i < *queue_size - num_players - 1; i++ ) {
    ip_queue[i] = ip_queue[i+j];
  }
  // update queue_size
  (*queue_size) = (*queue_size) - num_players;
  
  return num_players;
}

// runs the game; takes an arrat of length 4 storing ip addresses
int game(unsigned int player_IPs[], int num_players){
  printf("Game players:\n");
  while (1) {
    int i = 0;
    for ( i = 0; i < num_players; i++ ) {
      printf("Player %d: %d", i, player_IPs[i]);
    }
    printf("\n");
    sleep(2);
  }
}
  

int main() {
  
  int sd, connection;
  unsigned int ip_queue[20];
  int queue_size = 0;
  int game_running; // 0 if no game running; 1 if there is
  
  sd = server_setup(1379);
  
  while (1) {
    listener(sd, ip_queue, &queue_size);
    if ( queue_size > 1 ) { // ready to start game
      unsigned int player_IPs[4];
      int num_players;
      if ( queue_size > 4 )
	num_players = transfer_IPs(ip_queue, &queue_size, player_IPs, 4);
      else if ( queue_size >= 2 )
	num_players = transfer_IPs(ip_queue, &queue_size, player_IPs, queue_size);

      // start_game();
      if ( game_running == 0 ) { // no game running
	int f = fork();
	if ( f == 0 ) // child process
	  game(player_IPs, num_players); // start game
      }

    }
    // if only 1 player connected, pass
    
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
