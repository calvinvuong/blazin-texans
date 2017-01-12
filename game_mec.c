#include "game_mec.h"
#include <string.h>


int makeDeck(struct card * deck){
  char suits[4]={'D','C', 'H', 'S'};
  char * nums[13]={"A","2","3","4","5","6","7","8","9","10","J","Q","K"};
  
  int i;
  int j;
  
  for(i=0;i<4;i++){
    for(j=0;j<13;j++){
      struct card tempCard;
      strcpy(tempCard.num, nums[j]);
      tempCard.suit=suits[i];
      deck[i*13+j]=tempCard;
    }
  }
  return 0;
}

void printCard(struct card c) {
  printf("%s%c", c.num, c.suit);
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
    int j = 0;
    while ( j < num_cards) {
      players[i].hand[j] = remove_card(deck);
      j++;
    }
    i++;
  }
  return 0;
}

// returns the position of the top card in the deck list
// returns -1 if deck is empty (should never happen)
int top_card_pos(struct card * deck) {
  int i = 0;
  while ( i < 52 ) {
    if ( strcmp(deck[i].num, "N") != 0 )
      return i;
    i++;
  }
  return -1;
}

// removes the first cards from the deck
// marks the removed spot with num "N"
// returns the card struct removed
struct card remove_card(struct card * deck) {
  int top_card = top_card_pos(deck);
  
  // copy top card
  struct card tmp_card; // card to be returned
  strcpy(tmp_card.num, (deck[top_card]).num);
  tmp_card.suit = (deck[top_card]).suit;

  // "remove" top card from deck
  strcpy((deck[top_card]).num, "N");

  return tmp_card;
}

int bet(int amount, struct player *players, int playerNum){
  if(amount>players[playerNum].money){
    return -1;
  }else{
    players[playerNum].bet+=amount;
    players[playerNum].money-=amount;
    return 0;
  }
}

int check(struct player *players, int playerNum){
  int i;
  for( i=0;i<playerNum;i++){
    if(players[i].status==1){
      return -1;
    }
  }
  players[playerNum].status=0;
  return 0;
}

int fold(struct player *players, int playerNum){
  players[playerNum].status=-1;
  return 0;
}

// returns 0 if player playerNum has met the highest_bet requirement and can check
// -1 otherwise (i.e. has to fold or call)
// use this function in determining which options the player can choose from
int can_check(struct player *players, int playerNum, int highest_bet) {
  if ( players[playerNum].bet == highest_bet )
    return 0;
  
  return -1;
}

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

int main(){
  players=(player *) malloc(sizeof(struct player)*4);
  deck=(card *) malloc(sizeof(struct card)*53);

  /*
  players[0].bet=0;
  players[1].bet=0;
  printf("%d\n",players[0].bet);
  printf("%d\n",players[1].bet);
  deal(players);
  */
  
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
  deck = free(deck);
  return 0;
}
