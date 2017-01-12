#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct card{
  char num[3];
  char suit;
} card;

typedef struct player{
  struct card hand[2];
  int bet; // amt of money already bet this round
  int money;
  int score;
  int status; //-1 fold, 0 check, 1 still betting
  //socket or whatever
  //IP thingy
} player;

struct player * players;

// cards removed from the deck are marked with num "N"
struct card * deck;

int makeDeck(struct card * deck);

void printDeck(struct card * deck, int size);

void printCard(struct card c);

int deal(struct player * players, int num_players, struct card * deck, int num_cards);

int top_card_pos(struct card *deck);

struct card remove_card(struct card *deck);

int bet(int amount, struct player *players, int playerNum);

int call(struct player);

int check(struct player *players, int playerNum);

int fold(struct player *play);

// returns 0 if player playerNum has met the highest_bet requirement and can check
// -1 otherwise (i.e. has to fold or call)
int can_check(struct player *players, int playerNum, int highest_bet);

int all_folded(struct player *players, int num_players);

int all_checked(struct player *players, int num_players);
