#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#ifndef GAME_MEC_H
#define GAME_MEC_H

typedef struct card{
  int num;
  int suit;
} card;

typedef struct player{
  struct card hand[2];
  int player_num; 
  int bet; // amt of money already bet this round
  int money;
  int score;
  int status; //-1 fold, 0 check, 1 still betting
  unsigned int ip_address;
  int socket_connection;
} player;

// will contain info sent from server to client
// whether it asks for something or is just an update
typedef struct packet_server_to_client {
  int type; // 0 if just for updating info; 1 for dealing cards; 2 if need a response back
  int highest_bet;

  // stuff for updating
  struct card stream[5]; // cards on the table
  int stream_length;

  struct player player_list[4]; // client will use info from player structs to display game
  int player_length;
  int player_num; // this players num
  // consider a next player var?

  
  // stuff for prompting
  int options[4];
  // [fold, check, call, bet]
  // [1, 0, 1, 1]     if option is available, corresponding position is on w/ 1; off w/ 0
} packet_server_to_client;

typedef struct packet_client_to_server {
  int option_choice;
  int bet_amount; // for bet option only; bet amount for call is determined server side
} packet_client_to_server;

struct player * players;

// cards removed from the deck are marked with num "N"
struct card * deck;

struct card * river;

int makePlayers(struct player player_list[], unsigned int player_IPs[], int num_players);

int makeDeck(struct card * deck);

void printDeck(struct card * deck, int size);

void printCard(struct card c);

int deal(struct player * players, int num_players, struct card * deck, int num_cards);

int top_card_pos(struct card *deck);

struct card remove_card(struct card *deck, struct card rtrn);

int get_options(int option_list[], struct player players[], int player_num, int highest_bet);

int bet(int amount_to_bet, int *highest_bet, struct player *players, int playerNum);

int call(struct player * players, int playerNum, int highest_bet);

int check(struct player *players, int playerNum);

int fold(struct player *players, int playerNum);

// returns 0 if player playerNum has met the highest_bet requirement and can check
// -1 otherwise (i.e. has to fold or call)
int can_check(struct player *players, int playerNum, int highest_bet);

int can_call(struct player *players, int playerNum, int highest_bet);

int can_bet(struct player *players, int playerNum, int highest_bet);

int all_folded(struct player *players, int num_players);

int all_checked(struct player *players, int num_players);

int betting(struct player * players, int * highest_bet, int num_players, struct card river[], int river_len);

int print_player_info(struct card * river, struct player * players, int player_num, int num_players);

int all_ready(struct player * players, int num_players, int highest_bet);

int addCards(struct card * river, int num_cards, struct card * deck, int *len_river);

// reset player bet amount and status
int round_reset(struct player players[], int player_num);

// networking send stuff
int send_possible_moves(struct player *players, int player_num, int high_bet);

int get_move_response(struct player * players, int player_num, int * high_bet);

int update_client(struct player * players, int num_players, int high_bet, struct card river[], int river_len);
#endif

