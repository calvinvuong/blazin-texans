#include "game_mec.h"
#include "networking.h"
#include "algo.h"

/* FOR REFERENCE ONLY
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
*/

// for this to work struct player player_list[4]; must be statically initialized in server and passed in
// if this doesn't work, it might have to take in pointers and dereference
int makePlayers(struct player player_list[], unsigned int player_IPs[], int num_players) {
  int i;
  for ( i = 0; i < num_players; i++ ) {
    // empty hand
    player_list[i].hand[0].num = -1;
    player_list[i].hand[1].num = -1;

    // player info
    player_list[i].player_num = i;
    player_list[i].bet = 0;
    player_list[i].money = 500;
    player_list[i].score = 0;
    player_list[i].status = 1; // CHECK THIS

    //networking stuff
    player_list[i].ip_address = player_IPs[i];
    player_list[i].socket_connection = client_connectB(player_IPs[i], 3019); 
  }
  return 0;
  }

int makeDeck(struct card * deck){
  int suits[4]={0, 1, 2, 3}; // from lowest suit to highest
  // if num == -1, card is removed
  // Ace is 12
  int nums[13]={0,1,2,3,4,5,6,7,8,9,10,11,12};
  
  int i;
  int j;
  
  for(i=0;i<4;i++){
    for(j=0;j<13;j++){
      struct card tempCard;
      tempCard.num = nums[j];
      tempCard.suit = suits[i];
      deck[i*13+j] = tempCard;
    }
  }
  return 0;
}

void printCard(struct card c) {
  char suit_chars[4] = {'D', 'C', 'H', 'S'};
  char * num_strs[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
  if ( c.num == -1 ) // do nothing if card does not exist
    return;
  else 
    printf("%s%c", num_strs[c.num], suit_chars[c.suit]);  
}

// doesn't necessarily have to be a deck, just an array of cards
// prints size number of cards
void printDeck(struct card * list, int size) {
  int i = 0;
  while ( i < size ) {
    printCard(list[i]);
    printf(", ");
    i++;
  }
}

// deals num_cards cards from card list deck to list of players
int deal(struct player * players, int num_players, struct card * deck, int num_cards){
  int i = 0;
  int top_card = top_card_pos(deck);
  
  while ( i < num_players ) {
    if(players[i].money>0){
      int j = 0;
      while ( j < num_cards) {
	struct card * tmp=(struct card *)malloc(sizeof(struct card));
	players[i].hand[j] = remove_card(deck, *tmp);
	j++;
	free(tmp);
      }
    }
    i++;
  }
  return 0;
}

//puts cards in river
int addCards(struct card * river, int num_cards, struct card * deck, int *len_river){
  int i=0;
  while(i<num_cards){
    struct card * tmp=(struct card *)malloc(sizeof(struct card));
    river[*len_river]=remove_card(deck, *tmp);
    i++;
    (*len_river)++;
    free(tmp);
  }
  return 0;
}

// returns the position of the top card in the deck list
// returns -1 if deck is empty (should never happen)
int top_card_pos(struct card * deck) {
  int i = 0;
  while ( i < 52 ) {
    if ( deck[i].num != -1 )
      return i;
    i++;
  }
  return -1;
}

// removes the first cards from the deck
// marks the removed spot with num "N"
// returns the card struct removed
struct card remove_card(struct card * deck, struct card rtrn) {
  int top_card = top_card_pos(deck);
  
  // copy top card
  struct card tmp_card; // card to be returned
  tmp_card.num = (deck[top_card]).num;
  tmp_card.suit = (deck[top_card]).suit;

  // "remove" top card from deck
  (deck[top_card]).num = -1;

  rtrn = tmp_card;
  return rtrn;
}

// takes an int array of size 4 containing player options
// populates array w/ options; 0 = no option, 1 = yes option
// [fold, check, call, bet]
int get_options(int option_list[], struct player players[], int player_num, int highest_bet) {
  option_list[0] = 1; // can always fold
  option_list[1] = can_check(players, player_num, highest_bet);
  option_list[2] = can_call(players, player_num, highest_bet);
  option_list[3] = can_bet(players, player_num, highest_bet);
  return 0;
}

int bet(int amount_to_bet, int *highest_bet, struct player *players, int playerNum){
  if(amount_to_bet > players[playerNum].money || (amount_to_bet + players[playerNum].bet) < *highest_bet){
    return -1;
  }else{
    //(*highest_bet) = (*highest_bet) + (amount_to_bet - players[playerNum].bet);
    (*highest_bet) = amount_to_bet + players[playerNum].bet;
    players[playerNum].bet+=amount_to_bet;
    players[playerNum].money-=amount_to_bet;
    return 0;
  }
}

int check(struct player *players, int playerNum){
  //if(playerNum-1<0 && players[playerNum-1].status==1){
  //  return -1;
  //}
  players[playerNum].status=0;
  return 0;
}

int fold(struct player *players, int playerNum){
  players[playerNum].status=-1;
  return 0;
}

int call(struct player * players, int playerNum, int highest_bet){
  int amount_to_bet=highest_bet-players[playerNum].bet; 
  if(amount_to_bet>players[playerNum].money){
    return -1;
  }

  players[playerNum].bet=highest_bet;
  players[playerNum].money-=amount_to_bet;
  return 0;
}

// returns 1 if player playerNum has met the highest_bet requirement and can check
// 0 otherwise (i.e. has to fold or call)
// use this function in determining which options the player can choose from
int can_check(struct player *players, int playerNum, int highest_bet) {
  if ( (players[playerNum].bet == highest_bet) || (players[playerNum].money == 0) )
    return 1;
  
  return 0;
}

// returns 1 if the player playerNum can call
// 0 otherwise
int can_call(struct player *players, int playerNum, int highest_bet) {
  if ( can_check(players, playerNum, highest_bet) == 1 ) // if you can check, you can't call
    return 0;

  if ( (players[playerNum].money) >= (highest_bet - players[playerNum].bet) )
    return 1;
  else
    return 0;
}


// returns 1 if the player playerNum can bet more money than the highest bet
// 0 otherwise
int can_bet(struct player *players, int playerNum, int highest_bet) {
  if ( (players[playerNum].money) > (highest_bet - players[playerNum].bet) )
    return 1;
  else
    return 0;
}

/*
// returns 0 if all players have folded
// -1 otherwise
int all_folded(struct player *players, int num_players) {
  int i;
  for ( i = 0; i < num_players; i++ ) {
    if ( players[i].status != -1 )
      return -1;
  }
  return 0;
}

// returns 0 if all players have checked or folded
// -1 if not all players have checked
int all_checked(struct player *players, int num_players) {
  int i;
  for ( i = 0; i < num_players; i++ ) {
    if ( players[i].status != 0 )
      return -1;
  }
  return 0;
}
*/
int all_ready(struct player * players, int num_players, int highest_bet){
  int i;
  for(i=0;i<num_players;i++){
    if( (players[i].money > 0) && ( (players[i].status==0&&players[i].bet-highest_bet)||(players[i].status==1&&players[i].bet-highest_bet) ) ){
      return 1;
    }
  }
  return 0;
}


// sends an int array of 4
int send_possible_moves(struct player * players, int player_num, int high_bet) {

  struct packet_server_to_client pack;
  pack.type = 2;
  pack.highest_bet = high_bet;
  pack.player_num = player_num;
  
  //int opt_list[4];
  get_options(pack.options, players, player_num, high_bet);
  //pack.options = opt_list;
  
  write(players[player_num].socket_connection, &pack, sizeof(pack));

  return 0;
}


// returns response number from client; modifies high_bet if necessary
// 0: fold
// 1: check
// 2: call
// 3: bet
int get_move_response(struct player * players, int player_num, int * bet_response) {
  struct packet_client_to_server pack;
  read(players[player_num].socket_connection, &pack, sizeof(pack));
  *bet_response = pack.bet_amount; // bet checking is done client side
  return pack.option_choice;
}

// sends to all clients info about the game
int update_client(struct player * players, int num_players, int high_bet, struct card river[], int river_len) {
  int i;
  for ( i = 0; i < num_players; i++ ) {
    if ( ! (players[i].money < 0) ) { // if player is not dead
      struct packet_server_to_client pack;
      pack.type = 0;
      pack.highest_bet = high_bet;
      
      // populate pack.stream w/ contents of river
      int j;
      for ( j = 0; j < river_len; j++ )
	pack.stream[j] = river[j];
      //pack.stream = river;
      pack.stream_length = river_len;
      
      // populate player list
      for ( j = 0; j < num_players; j++ )
	pack.player_list[j] = players[j];
      
      pack.player_length = num_players;
      pack.player_num = i;
      
      write(players[i].socket_connection, &pack, sizeof(pack));
    }
  }

  return 0;
}


int print_game_info(struct card * river, int river_len, struct player * players, int player_num, int num_players){
  int i;
  for(i=0;i<num_players;i++){
    char status_msg[50];
    if ( players[i].money < 0 )
      strcpy(status_msg, "dead");
    else if ( players[i].status == -1 )
      strcpy(status_msg, "folded");
    else if ( players[i].status == 0 )
      strcpy(status_msg, "checked");
    else if ( players[i].status == 1 )
      strcpy(status_msg, "still betting");
    else
      strcpy(status_msg, "status was not updated");
    
    printf("Player %d: status: %s \n bet: %d \n money: %d \n", i, status_msg, players[i].bet, players[i].money);
    
  }
  printf("Your player number: %d\n", player_num);
  printf("Your hand: ");
  printDeck(players[player_num].hand,2);
  printf("\n");

  printf("Cards on Table: ");
  printDeck(river, river_len);
  printf("\n");
  return 0;
}

int check_if_broke(struct player * players, int * num_alive, int num_players){
  int i;
  *num_alive=0;
  for(i=0;i<num_players;i++){
    if(players[i].money >= 0){
      (*num_alive)++;
    }
  }
  return 0;
}


int score(struct player * players, int num_players, card * table_cards) {
  updateScores(players, num_players, table_cards);
  int winner = winningHand(players, num_players); // player num of the winner
  int pot = 0;
  int i;
  // determine pot and reset bet to 0
  for ( i = 0; i < num_players; i++ ) {
    if ( (players[i].money >= 0) && (players[i].bet >= 0) )
      pot += players[i].bet;
    players[i].bet = 0;
  }

  // give rewards to winner
  players[winner].money += pot;
  // tell players who won
  for ( i = 0; i < num_players; i++ ) {
    if (players[i].money >= 0) { // send to all alive players
      struct packet_server_to_client pack;
      pack.type = 3;
      pack.winner_id = winner;
      write(players[i].socket_connection, &pack, sizeof(pack));
    }
  }

  // reset vars of players
  for ( i = 0; i < num_players; i++ ) {
    players[i].status = 1;
    players[i].bet = 0;
    players[i].score = 0;
    players[i].hand[0].num = -1;
    players[i].hand[1].num = -1;
  }
  
  // purge dead players
  for ( i = 0; i < num_players; i++ ) {
    if ( players[i].money == 0 ) { // player died this hand
      players[i].money = -500; // this makes it so i don't have to tell him he's dead again
      struct packet_server_to_client pack;
      pack.type = -1;
      write(players[i].socket_connection, &pack, sizeof(pack));
    }
  }
  return 0;
}


// make sure you call betting with &highest_bet
int betting(struct player * players, int * highest_bet, int numPlayers, struct card river[], int river_len){ 

  int i;
  int done;
  int ready=1; //1 until everyone either folded or at highest_bet, then goes to 0
  while(ready){
    for(i=0;i<numPlayers;i++){
      done=1;
      printf("highest_bet: %d\n", *highest_bet);
      if ( players[i].money < 0 ){ // already dead;
	done = 0;
	printf("money<0\n\n");
      }else if(players[i].status==-1 || players[i].money == 0){ // folded or went all in
	// send to all players what happened
	update_client(players, numPlayers, *highest_bet, river, river_len);
	done=0;
	printf("status==-1 or money==0\n\n");
      }
      while(done){ // can still bet
	// send to all players what happened
	update_client(players, numPlayers, *highest_bet, river, river_len);
	printf("updated client\n\n");
	//send possible moves, pot, highest bet, cards, river
	send_possible_moves(players, i, *highest_bet);	
	//getresponse
	printf("sent moves\n\n");
	int bet_response;
	int response = get_move_response(players, i, &bet_response);
	
	//if(strcmp(response, "check")){
	if (response == 1) { //check
	  check(players, i);
	  done=0;
	}

	//if(strcmp(response, "call")){
	else if ( response == 2 ) { //call
	  call(players, i, *highest_bet);
	  done=0;
	}

	//if(strcmp(response, "bet")){
	else if (response == 3 ) { // bet
	  bet(bet_response, highest_bet, players, i);
	  done=0;
	}
	//if(strcmp(response, "fold")){
	else if ( response == 0 ) { // fold
	  fold(players, i);
	  done=0;
	}
      }
    }
    ready=all_ready(players, numPlayers, *highest_bet);
  }
  return 0;
}
/*
put int server:
deal()
betting()
flop()
betting()
turn_river()
betting()
turn_river()
betting()
score() // lost players will exit;
*/
/*
int main(){
  players=(player *) calloc(4,sizeof(struct player));
  deck=(card *) malloc(sizeof(struct card)*53);

  int j;
  for(j=0;j<4;j++){
    players[j].money=1000;
    players[j].status=1;
  }
  int highest_bet=0;
  
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
*/
  /*
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
*/
  /*
  free(players);
  free(deck);
  return 0;
}
*/
