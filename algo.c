#include "algo.h"
#include "game_mec.h"
#include <stdlib.h>
#include <sys/time.h>

//https://github.com/gTopa/PokeHerPokeMe/blob/master/Poker/Hand.pde

int updateScore(player player, int currentscore, card * tablecards) {
  struct card * sevencards;
  struct card a;
  sevencards = calloc(7,sizeof(a));
  int k;
  for (k = 0; k < 2; k++)
    sevencards[k] = player.hand[k];
  for (k = 2; k < 7; k++)
    sevencards[k] = tablecards[k-2];
  struct card * hand;
  hand = calloc(5, sizeof(a));
  hand = determineHand(sevencards);
  int newscore = currentscore + handValue(hand);
  free(sevencards);
  free(hand);
  return newscore;
}

int updateScores(player * players, int numplayers, card * tablecards) { //update score of each player
  int i;
  for (i = 0; i < numplayers; i++) {
    if ( players[i].money < 0 ) // player already dead
      players[i].score = -500;
    else if (players[i].status == -1)
      players[i].score = -500;
    else
      players[i].score = updateScore(players[i], players[i].score, tablecards);
  }
  return 0;
}



int winningHand(player * players, int numplayers){ //return a player_num
  int champ = 0;
  int i;
  for (i = 0; i < numplayers; i++){
    if ( (players[i].status != -1) && (players[i].score > players[champ].score) )
      champ = i;
  }
  return champ;
}
  

int swap (card * deck, int a, int b){
  struct card temp;
  temp = deck[a];
  deck[a] = deck[b];
  deck[b] = temp;
  return 0;
}

int shuffle( card * deck) {
  srand ( time(NULL) );
  int i;
  for (i = 51; i>0; i--) {
    int j = rand() % (i+1);
    swap(deck, i, j);
  }
  return 0;
}

int compareTwo (card card1, card card2) { //Java equiv: card1.compareTo(card2)
  if (card1.num * 10 + card1.suit > card2.num * 10 + card2.suit)
    return 1;
  else if (card1.num * 10 + card1.suit < card2.num * 10 + card2.suit)
    return -1;
  else return 0;
}

int sort(card * hand) {
  int x, y, imin, temp;
  for (x = 0; x < 5; x++) {
    imin = x;
    for (y = x; y < 5; y++) {
      if (compareTwo(hand[y],hand[imin]) == -1) {
	imin = y;
      }
    }
    swap(hand, x, imin);
  }
  return 0;
}

int findMax( card * hand){
  return hand[4].num * 10 + hand[4].suit;
}

int findMost(card * hand){
  int n[14];
  int i, max, val;
  for (i = 0; i < 5; i++) {
    n[hand[i].num] += 1;
  }
  max = 0;
  val = 0;
  for (i = 0; i< 14; i++) {
    if(n[i] >= max) {
      max = n[i];
      val = i;
    }
  }
  return val * 100;
}

int handValue(card * hand) {
  int score = 0;
  sort(hand);
  if(isStraight(hand)&&isFlush(hand)){
    //printf("Straight flush \n");
    score+=80000;
    score+=findMax(hand);
  }else if(isStraight(hand)){
    //printf("Straight \n");
    score+=40000;
    score+=findMax(hand);
  }else if(isFlush(hand)){
    //printf("Flush \n");
    score+=50000;
    score+=findMax(hand);
  }else if(isPoker(hand)){
    //printf("Poker \n");
    score+=70000;
    score+=findMost(hand);
  }else if(isFull(hand)){
    //printf("Full \n");
    score+=60000;
    score+=findMost(hand);
  }else if(isTwoPair(hand)){
    //printf("Two Pair \n");
    score+=20000;
    score+=findMost(hand);
  }else if(isThree(hand)){
    //printf("Three \n");
    score+=30000;
    score+=findMost(hand);
  }else if(isPair(hand)){
    //printf("Pair \n");
    score+=10000;
    score+=findMost(hand);
    //printf("most: %d\n", findMost(hand));
    score+=findMax(hand);
    //printf("max: %d\n", findMax(hand));
  }else{
    score+=findMax(hand);
  }
  return score;
}

int isStraight(card * hand) {
  int i;
  for (i = 1; i<5; i++) {
    if (!(hand[i-1].num+1==hand[i].num||(i==4&&hand[i].num==13&&hand[0].num==1))){
      return 0;
    }
  }
  return 1;
}

int isFlush(card * hand) {
  int i;
  for (i = 1; i < 5; i++) {
    if (hand[i-1].suit != hand[i].suit)
      return 0;
  }
  return 1;
}

int isPoker(card * hand) {
  if(hand[0].num==hand[3].num||hand[1].num==hand[4].num)
    return 1;
  return 0;
}

int isThree(card * hand) {
  int i, numdiff;
  numdiff = 0;
  for (i = 1; i < 5; i++) {
    if (hand[i-1].num != hand[i].num)
      numdiff += 1;
  }
  if (numdiff > 2)
    return 0;
  else
    return 1;
}

int isPair(card * hand) {
  int i;
  for (i = 1; i < 5; i++) {
    if (hand[i-1].num == hand[i].num)
      return 1;
  }
  return 0;
}

int isTwoPair(card * hand) {
  int onePair = 0;
  int pairNum = 0;
  int i;
  for (i = 1; i < 5; i++) {
    if (hand[i-1].num == hand[i].num) {
      if (onePair && hand[i].num != pairNum)
	return 1;
      else {
	onePair = 1;
	pairNum = hand[i].num;
      }
    }
  }
  return 0;
}

int isFull(card * hand) {
  int i, numdiff;
  numdiff = 0;
  for (i = 1; i < 5; i++) {
    if (hand[i-1].num != hand[i].num)
      numdiff++;
  }
  if (numdiff == 1)
    return 1;
  else
    return 0;
}


card * determineHand(card * sevencards) {
  int i, j, k, x, hc, hn;
  i = 1;
  j = 0;
  x = 0;
  struct card a;
  struct card * currBestHand = calloc(5, sizeof(a));
  struct card * newHand;
  for (k = 0; k < 7; k++) {
    if (k != i && k != j) {
      currBestHand[x] = sevencards[k];
      x++;
    }
  }
  hc = handValue(currBestHand);
  //printf("hc: %d\n", hc);


  for (i = 2; i < 7; i++) {
    for (j = 0; j < 7; j++) {
      if (i == j)
	break;
      else {
	//printf("i = %d, j = %d \n", i, j);
	newHand = calloc(5, sizeof(a));
	x = 0;
	for (k = 0; k < 7; k++) {
	  if (k != i && k != j) {
	    newHand[x] = sevencards[k];
	    x++;
	  }
	}
	hn = handValue(newHand);
	//printf("hn: %d\n", hn);
	if (hn >= hc) {
	  //printf("switch\n");
	  currBestHand = newHand;
	  hc = handValue(currBestHand);
	  //printf("hc: %d\n", hc);
	}
	//free(newHand);
	//printf("\n");
      }
    }
  }
  
  
  return currBestHand;
}

/*
int main() {
  
  struct card tablecards[5];
  struct player players[4];
  int k;
 
  tablecards[0].num = 2;
  tablecards[0].suit = 1;
  tablecards[1].num = 11;
  tablecards[1].suit = 3;
  tablecards[2].num = 2;
  tablecards[2].suit = 2;
  tablecards[3].num = 6;
  tablecards[3].suit = 3;
  tablecards[4].num = 5;
  tablecards[4].suit = 3;

  
  printf("Comparing Two Cards\n");
  printf("%d of %d vs %d of %d\n", tablecards[0].num, tablecards[0].suit, tablecards[1].num, tablecards[1].suit);
  int c;
  c = compareTwo(tablecards[0], tablecards[1]);
  printf("%d\n\n", c);

  printf("Tablecards\n");
  for (k = 0; k < 5; k++)
    printf("Card %d: %d of %d \n", k, tablecards[k].num, tablecards[k].suit);
  
  sort(tablecards);
  printf("\nTablecards Sorted\n");
  for (k = 0; k < 5; k++)
    printf("Card %d: %d of %d \n", k, tablecards[k].num, tablecards[k].suit);
  
  swap(tablecards, 0, 1);
  printf("\nTablecards Swap 0 and 1\n");
  for (k = 0; k < 5; k++)
    printf("Card %d: %d of %d \n", k, tablecards[k].num, tablecards[k].suit);

  swap(tablecards, 0, 1);
  printf("\nTablecards Swap Back\n");
  for (k = 0; k < 5; k++)
    printf("Card %d: %d of %d \n", k, tablecards[k].num, tablecards[k].suit);
  printf("\n");
  
  int i;
  for(i = 0; i < 4; i++){
    players[i].score = 0;
    players[i].player_num = i;
  }

  players[0].hand[0].num = 12;
  players[0].hand[0].suit = 1;
  players[0].hand[1].num = 2;
  players[0].hand[1].suit = 0;

  players[1].hand[0].num = 12;
  players[1].hand[0].suit = 3;
  players[1].hand[1].num = 7;
  players[1].hand[1].suit = 3;

  players[2].hand[0].num = 11;
  players[2].hand[0].suit = 2;
  players[2].hand[1].num = 11;
  players[2].hand[1].suit = 0;

  players[3].hand[0].num = 3;
  players[3].hand[0].suit = 0;
  players[3].hand[1].num = 4;
  players[3].hand[1].suit = 0;

  for (i = 0; i < 4; i++) {
    struct card * sevencards;
    struct card a;
    sevencards = calloc(7, sizeof(a));
    for (k = 0; k < 2; k++) {
      sevencards[k] = players[i].hand[k];
    }
    for (k = 2; k < 7; k++) {
      sevencards[k] = tablecards[k-2];
    }
    printf("Player %d 7 cards \n", i);
    for (k = 0; k < 7; k++) { 
      printf("Card %d: %d of %d\n", k,sevencards[k].num, sevencards[k].suit);
    }
    printf("\n");
    struct card * hand;
    hand = calloc(5, sizeof(a));
    hand = determineHand(sevencards);
    free(sevencards);
  
    printf("Player %d hand: \n", i);
    for (k = 0; k <5; k++)
      printf("Card %d: %d of %d \n", k, hand[k].num, hand[k].suit);
    printf("\n");
    free(hand);
  }
  
  printf("\n");
  updateScores(players, 4, tablecards);
  for (i = 0; i < 4; i++)
    printf("Player %d score: %d\n", i, players[i].score);
  printf("\n");

  
  int w;
  w = winningHand(players, 4);
  
  printf("The winning hand is player %d \n", w);
  printf("\n");

  struct card deck[52];
  int suits[4]={0, 1, 2, 3}; // from lowest suit to highest
  // if num == -1, card is removed
  // Ace is 12
  int nums[13]={0,1,2,3,4,5,6,7,8,9,10,11,12};
  int j;
  for(i=0;i<4;i++){
    for(j=0;j<13;j++){
      struct card tempCard;
      tempCard.num = nums[j];
      tempCard.suit = suits[i];
      deck[i*13+j] = tempCard;
    }
  }
  shuffle(deck);
  for (k = 0; k < 52; k++) {
    printf("Card %d: %d of %d\n", k, deck[k].num, deck[k].suit);
  }
  
  return 0;
}
*/
