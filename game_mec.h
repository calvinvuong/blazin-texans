typedef struct player{
  struct card hand[5];
  int bet;
  int money;
  int score;
  int playerNumber;
  //socket or whatever
} player;
typedef struct card{
  int num;
  char suit;
} card;

