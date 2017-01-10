#include "game_mec.h"

int makeDeck(struct * deck){
  char suits[4]={'H','D', 'C', 'S'}

  open("cards.txt", O_RDONLY);
  
  int i=0;
  int j;
  for(i;i<4;i++){
    for(j=0;j<13;j++){
      read
    }
  }
}

int deal(struct player * players){
  return 0;
}

int main(){
  players=(player *) malloc(sizeof(struct player)*4);
  players[0].bet=0;
  players[1].bet=0;
  printf("%d\n",players[0].bet);
  printf("%d\n",players[1].bet);
  deal(players);
  printf("%s\n", deck[0]);
  return 0;
}
