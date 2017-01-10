#include "game_mec.h"

int makeDeck(struct card * deck){
  char suits[4]={'H','D', 'C', 'S'};
  char * nums[13]={"A","2","3","4","5","6","7","8","9","10","J","Q","K"};
  
  int i;
  int j;
  
  for(i=0;i<4;i++){
    for(j=0;j<13;j++){
      struct card tempCard;
      tempCard.num=nums[j];
      tempCard.suit=suits[i];
      deck[i*13+j]=tempCard;
    }
  }
  return 0;
}


int deal(struct player * players){
  return 0;
}

int main(){
  players=(player *) malloc(sizeof(struct player)*4);
  deck=(card *) malloc(sizeof(struct card)*52);
  players[0].bet=0;
  players[1].bet=0;
  printf("%d\n",players[0].bet);
  printf("%d\n",players[1].bet);
  deal(players);
  makeDeck(deck);
  printf("%s", deck[4].num);
  printf("%c", deck[4].suit);
  return 0;
}
