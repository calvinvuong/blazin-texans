typedef struct player{
  struct card hand[5];
  int bet;
  int money;
  int score;
  int playerNumber;
  //socket or whatever
  //IP thingy
} player;

typedef struct card{
  int num;
  char suit;
} card;

char deck[52][4]={"AC","AS","AH","AD","2C","2S","2H","2D","3C","3S","3H","3D","4C","4S","4H","4D","5C","5S","5H","5D","6C","6S","6H","6D","7C","7S","7H","7D","8C","8S","8H","8D","9C","9S","9H","9D","10C","10S","10H","10D","JC","JS","JH","JD","QC","QS","QH","QD","KC","KS","KH","KD"};
  
int deal(char * deck, struct player * players);

int bet(int amount, struct player);

int call(struct player);

int check(struct player);

int fold(struct player);
