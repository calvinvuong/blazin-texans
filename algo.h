#include "game_mec.h"
#include <stdlib.h>
#include <sys/time.h>

#ifndef ALGO_H
#define ALGO_H

int updateScore(player player, int numplayers);

int updateScores(player * players, int numplayers);

int winningHand(player * players, int numplayers);

int swap(card * a, card * b);

int shuffle(card * deck);

int compareTwo(card card1, card card2);

int sort(card * hand);

int findMax(card * hand);

int findMost(card * hand);

int handValue(card * hand);

int isStraight(card * hand);

int isFlush(card * hand);

int isPoker(card * hand);

int isThree(card * hand);

int isPair(card * hand);

int isTwoPair(card * hand);

int isTwoPair(card * hand);

int isFull(card * hand);

#endif
