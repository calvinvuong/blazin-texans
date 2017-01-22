#include "game_mec.h"
#include "networking.h"
#include "algo.h"

// returns socket descriptor that will handle all communications
int custom_connect() {
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

  //close(sd);
  return connection;
}

// takes a list of options available from received packet
// prompts user for an input
// checks if bet is high enough
// returns action number: 0 = fold, 1 = check, 2 = call, 3 = bet
int get_action(int options[], int high_bet) {
  char print_msg[500];
  char input[50];
  
  strcpy(print_msg, "Please select an option NUMBER below: \n");
  if ( options[0] == 1 )
    strcat(print_msg, "1. fold\n");
  if ( options[1] == 1 )
    strcat(print_msg, "2. check\n");
  if ( options[2] == 1 )
    strcat(print_msg, "3. call\n");
  if ( options[3] == 1 )
    strcat(print_msg, "4. bet\n");

  printf("%s", print_msg);
  fgets(input, sizeof(input), stdin);
  *strchr(input, '\n') = 0; // get rid of newline
  int opt_choice = atoi(input);

  return opt_choice - 1;
}

int get_bet_amount(int high_bet, int player_money, int amt_betted) {
  // get amount to bet
  char input_bet[10];
  int bet_amount;
  fgets(input_bet, sizeof(input_bet), stdin);
  *strchr(input_bet, '\n') = 0;
  bet_amount = atoi(input_bet);
  while ( (bet_amount > player_money) || ((bet_amount + amt_betted) < high_bet) ) { // bet not valid
    fgets(input_bet, sizeof(input_bet), stdin);
    *strchr(input_bet, '\n') = 0;
    bet_amount = atoi(input_bet);
  }
  return bet_amount;
}

int main() {
  int connection = custom_connect();
  
  char read_buffer[500];
  char write_buffer[500];

  packet_server_to_client read_pack;
  packet_client_to_server write_pack;

  // game info
  int player_id;
  int high_bet;
  int player_money;
  int amount_betted;
  
  // first operation should always be to listen
  // one loop per listen
  while (1) {
    read(connection, &read_pack, sizeof(read_pack));
    player_id = read_pack.player_num;
    if ( read_pack.type == -1 ) {
      printf("You have no more money! Bye bye!\n");
      exit(0);
    }
    else if ( read_pack.type == 100 ) {
      printf("Yay! You won!\n");
      exit(0);
    }
    else if ( read_pack.type == 3 ) {
      printf("Player: %d won the hand!\n", read_pack.winner_id);
    }
    else if ( read_pack.type == 0 ) {
      system("clear"); // clear screen
      high_bet = read_pack.highest_bet;
      player_money = read_pack.player_list[player_id].money;
      amount_betted = read_pack.player_list[player_id].bet;

      print_game_info(read_pack.stream, read_pack.stream_length, read_pack.player_list, player_id, read_pack.player_length);
    }
    //OBSOLETE: else if ( read_pack.type == 1 )
    // if a response is needed
    else if ( read_pack.type == 2 ) {
      printf("type==2\n\n");
      int action = get_action(read_pack.options, high_bet);
      int bet_amt; // only will be sent if action == 3 bet
      if ( action == 3 )
	bet_amt = get_bet_amount(high_bet, player_money, amount_betted);
      
      write_pack.option_choice = action;
      write_pack.bet_amount = bet_amt;
      write(connection, &write_pack, sizeof(write_pack));
    }
  }    
  /*
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
  */
  return 0;
}
