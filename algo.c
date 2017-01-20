#include "algo.h"
#include "game_mec.h"
#include <stdlib.h>
#include <sys/time.h>

//https://github.com/gTopa/PokeHerPokeMe/blob/master/Poker/Hand.pde

int updateScore(player player, int currentscore) {
  sort(player.hand);
  int newscore = currentscore + handValue(player.hand);
  return newscore;
}

int updateScores(player * players, int numplayers) { //update score of each player
  int i;
  for (i = 0; i < numplayers; i++) {
    players[i].score = updateScore(players[i], players[i].score);
  }
  return 0;
}



int winningHand(player * players, int numplayers){ //return a player_num
  int champ = 0;
  int i;
  for (i = 0; i < numplayers; i++){
    if (players[i].score > players[champ].score)
      champ = i;
  }
  return champ;
}
  

int swap (card *a, card *b){
  struct card temp;
  temp = *a;
  *a = *b;
  *b = temp;
  return 0;
}

int shuffle( card * deck) {
  srand ( time(NULL) );
  int i;
  for (i = 51; i>0; i--) {
    int j = rand() % (i+1);
    swap(&deck[i], &deck[j]);
  }
  return 0;
}

int compareTwo (card card1, card card2) { //Java equiv: card1.compareTo(card2)
  if (card1.num * 10 + card1.suit > card2.num * 10 + card2.suit)
    return 1;
  else if (card1.num * 10 + card1.suit > card2.num * 10 + card2.suit)
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
    swap(&hand[x], &hand[imin]);
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
      score+=80000;
      score+=findMax(hand);
    }else if(isStraight(hand)){
      score+=40000;
      score+=findMax(hand);
    }else if(isFlush(hand)){
      score+=50000;
      score+=findMax(hand);
    }else if(isPoker(hand)){
      score+=70000;
      score+=findMost(hand);
    }else if(isFull(hand)){
      score+=60000;
      score+=findMost(hand);
    }else if(isTwoPair(hand)){
      score+=20000;
      score+=findMost(hand);
    }else if(isThree(hand)){
      score+=30000;
      score+=findMost(hand);
    }else if(isPair(hand)){
      score+=10000;
      score+=findMost(hand);
      score+=findMax(hand);
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
  for (int i = 1; i < 5; i++) {
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


int main() {
  struct card c1, c2, c3, c4, c5;
  struct card hand[5];
  
  
  return 0;
}
