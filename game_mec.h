#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct card{
  int num;
  char suit;
} card;

typedef struct player{
  struct card hand[5];
  int bet;
  int money;
  int score;
  int playerNumber;
  //socket or whatever
  //IP thingy
} player;

struct player * players;

struct card * deck;

int makeDeck(struct card * deck);

int deal(struct player * players);

int bet(int amount, struct player);

int call(struct player);

int check(struct player);

int fold(struct player);
