#include "game_mec.h"
#include "networking.h"
#include "server.h"

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
// sets timeout if timeout == 1
int listener( int sd, unsigned int *ip_queue, int *queue_size, int timeout ) {
  int connection;
  connection = initial_server_connect(sd, &(ip_queue[*queue_size]), timeout);
  
  if ( connection == -1 ) {// only reached if timeout == 1
    close(connection);
    return -1;
  }

  char buffer[500];
  int port_buff = 3019;
  read( connection, &buffer, sizeof(buffer) );
  if ( strcmp(buffer, "client to listener") == 0 )
    write( connection, &port_buff, sizeof(int) ); // send port to listen on to client
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
  for ( i = 0; i < *queue_size - num_players; i++ ) {
    ip_queue[i] = ip_queue[i+j];
  }
  // update queue_size
  (*queue_size) = (*queue_size) - num_players;
  
  return num_players;
}

// returns 0 if the game of given pid is not running
// returns 1 if the game of given pid is running
int update_game_status(int pid) {
  if ( kill(pid, 0) == 0 ) // doesn't actually kill
    return 1;
  else
    return 0;
}

/*
// populates an array of players using player_IPs
// takes an array of 4 player structs
// takes an array of 4 ip addresses
// MOVE THIS TO GAME MECHANICS
int make_players(struct player players[], unsigned int player_IPs[], int num_players) {
  int i;
  for ( i = 0; i < num_players; i++ ) {
    
  }
  return 0;
} */

// DIAGNOSTIC only
// runs the game; takes an arrat of length 4 storing ip addresses
int game(unsigned int player_IPs[], int num_players){
  // establish socket connections with players
  int client_connections[num_players];
  int i;
  for ( i = 0; i < num_players; i++ ) {
    client_connections[i] = client_connectB(player_IPs[i], 3019); 
  }
  
  // main game loop
  i = 0;
  while (1) {
    char read_buffer[500];
    // get info from player i
    int curr_connect = client_connections[i];
    write(curr_connect, "write", sizeof("write"));
    read(curr_connect, &read_buffer, sizeof(read_buffer));
    printf("read from client: %s\n", read_buffer);

    // exit case
    if ( strcmp(read_buffer, "exit") == 0 )
      return -1;

    // send to all players
    int j;
    for ( j = 0; j < num_players; j++ ) {
      curr_connect = client_connections[j];
      write(curr_connect, "read", sizeof("read"));
      write(curr_connect, read_buffer, sizeof(read_buffer));
    }

    // update i
    if ( i == (num_players-1) )
      i = 0;
    else
      i++;
  }
    /*
  printf("Game players:\n");
  //  while (1) {
  int k = 0;
  while ( k < 10 ) {
    int i = 0;
    for ( i = 0; i < num_players; i++ ) {
      printf("Player %d: %d", i, player_IPs[i]);
    }
    printf("\n");
    sleep(2);
    k++;
  }
  */
  return 0;
}
  

int main() {
  
  int sd, connection;
  unsigned int ip_queue[20];
  int queue_size = 0;
  int game_running = 0; // 0 if no game running; 1 if there is
  int game_pid = 90;
  
  sd = server_setup(1379);
  
  while (1) {

    //printf("game_pid: %d\n", game_pid);
    //game_running = update_game_status(game_pid);
    //printf("game_running: %d\n", game_running);

    if ( queue_size > 1 && game_running == 0 ) { // ready to start game
      unsigned int player_IPs[4];
      int num_players;
      if ( queue_size > 4 )
	num_players = transfer_IPs(ip_queue, &queue_size, player_IPs, 4);
      else if ( queue_size >= 2 )
	num_players = transfer_IPs(ip_queue, &queue_size, player_IPs, queue_size);

      if ( game_running == 0 ) { // no game running
	game_running = 1;
	game_pid = fork();
	signal(SIGCHLD, SIG_IGN); // circumvents waiting
	if ( game_pid == 0 ) {  // child process
	  struct player * players=(struct player *) malloc(sizeof(struct player)*num_players);
	  makePlayers(players,ip_queue, num_players);
	  while(num_players>1){
	    struct card * deck=(struct card *) malloc(sizeof(struct card)*53);
	    struct card * river=(struct card *) malloc(sizeof(struct card)*5);
	    int len_river=0;
	    int highest_bet=0;
	    makeDeck(deck);
	    shuffle(deck);
	    deal(players, num_players, deck, 2);
	    betting(players, *highest_bet, num_players, river, len_river);
	    addCards(river, 3, *len_river);
	    betting(players, *highest_bet, num_players, river, len_river);
	    addCards(river, 1, *len_river);
	    betting(players, *highest_bet, num_players, river, len_river);
	    addCards(river, 1, *len_river);
	    betting(players, *highest_bet, num_players, river, len_river);
	    //score_players();
	    //find_max();
	    //move_money();
	    //check_if_broke();
	  }
	  //game(player_IPs, num_players); // start game
	  printf("hi\n");
	  exit(0);
	}
      }
    }
    game_running = update_game_status(game_pid);
    if ( ! (queue_size > 1) ) {
      listener(sd, ip_queue, &queue_size, 0); // listen w/o time outs
    }
    else if ( game_running == 1 ) {
      listener(sd, ip_queue, &queue_size, 1); // listen w/ time outs
    }
    game_running = update_game_status(game_pid);
  }   

  return 0;
}


/* DIAGNOSTIC RUNNING STUFF
int main(){
  players=(player *) calloc(4,sizeof(struct player));
  deck=(card *) malloc(sizeof(struct card)*53);

  int j;
  for(j=0;j<4;j++){
    players[j].money=1000;
    players[j].status=1;
  }
  int highest_bet=0;

  players[0].bet=0;
  players[1].bet=0;
  printf("%d\n",players[0].bet);
  printf("%d\n",players[1].bet);
  deal(players);
  
  makeDeck(deck);
  printDeck(deck, 52);
  printf("\n");

  deal(players, 4, deck, 2);

  int i;
  for ( i = 0; i < 4; i++ ) {
    printDeck(players[i].hand, 2);
    printf("\n");
  }

  printDeck(deck, 52);

  printf("\n\n %d\n",check(players, 0));
  printf("\n %d \n", bet(100,&highest_bet,players,1));
  printf("\n %d \n", bet(1100,&highest_bet,players,2));
  printf("\n %d \n", bet(10,&highest_bet,players,2));
  printf("\n\n %d\n",check(players, 2));
  printf("\n\n %d\n",call(players, 2, highest_bet));
  
  printf("\n highest_bet: %d\n", highest_bet);
  
  for(j=0;j<4;j++){
    printf("\n\nplayer %d's bets %d \n", j, players[j].bet);
    printf("\n\nplayer %d's money %d \n", j, players[j].money);
    printf("\n\nplayer %d's status %d \n", j, players[j].status);
  }
  free(players);
  free(deck);
  return 0;
}
*/
