/*
NAME: Henry Unruh
CLASS: CS360
ASSIGNMENT: Assignment 5, Dining Philosophers
SYNOPSIS: This is a program that tackles a computer science concept known as the "Dining Philosophers" problem; deadlocking and cycles.
		  Program/assignment uses threads instead of system calls.
*/

//include statements
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "random.h"

int csticks[5];
pthread_mutex_t cstick_mutex;

//Function declarations
void diningPhilosopher(int *thinkID);
void eatingCycle(int *eatingTimeTotal, int thinkID);
void ponderingCycle(int *ponderTimeTotal, int thinkerID);

//Main function
int main(int argc, char *argv[]){
	int *thinkID;	
	pthread_t thinkers[5];
	pthread_mutex_init(&cstick_mutex, NULL);	
	
	int i = 0;
	while(i < 5){
		csticks[i] = 1;
		i++;
	}
	
	for(int i = 0; i < 5; i++){
		thinkID = (int*)malloc(sizeof(int));
		*thinkID = i;
		pthread_create(&thinkers[i], NULL, (void*)diningPhilosopher, (void*)thinkID);
	}
	
	for(int i = 0; i < 5; i++){
		pthread_join(thinkers[i], NULL);
	}
	
return 0;
}

//Dining Philosophers process
void diningPhilosopher(int *thinkID){
	int eatingTimeTotal = 0;
	int ponderTimeTotal = 0;
	
	ponderingCycle(&ponderTimeTotal, *thinkID); //Begin thinking.
	
	while(eatingTimeTotal < 100){ //Philosophers have to eat for at least 100 seconds.
		pthread_mutex_lock(&cstick_mutex); //check for two chopsticks availablity, unlock mutex if unable.
		
		if(csticks[*thinkID] == 1 && csticks[(*thinkID + 1) % 5] == 1){ 
		
			//Eating cycle
			csticks[*thinkID] = csticks[(*thinkID + 1) % 5] = 0; //take two chopsticks and unlock.
			pthread_mutex_unlock(&cstick_mutex);
			eatingCycle(&eatingTimeTotal, *thinkID);
			
			//Thinking cycle
			pthread_mutex_lock(&cstick_mutex); //lock to return chopsticks
			csticks[*thinkID] = csticks[(*thinkID + 1) % 5] = 1;
			pthread_mutex_unlock(&cstick_mutex);
			ponderingCycle(&ponderTimeTotal, *thinkID);
			
		}else{
			pthread_mutex_unlock(&cstick_mutex);
			sleep(5);
		}
	}
}

//Thinking function
void ponderingCycle(int *ponderTimeTotal, int thinkerID){
	int ponderingTime = randomGaussian(11, 7); //randomGaussian(thinking mean, thinking std. dev.)
	
	if(ponderingTime < 0){//If they don't think... they're not thinking.
		ponderingTime = 0;
	}
	*ponderTimeTotal += ponderingTime;
	
	printf("Philosopher %d has been thinking for %d seconds. (Total time thinking = %d)\n", thinkerID, ponderingTime, *ponderTimeTotal);
	sleep(ponderingTime);
}

//Eating function
void eatingCycle(int *eatingTimeTotal, int thinkID){
	int eatingTime = randomGaussian(9, 3); //randomGaussian(eating mean, eating std. dev.)
	
	if(eatingTime < 0){//If they don't eat... they're not eating.
		eatingTime = 0;
	}
	*eatingTimeTotal += eatingTime;
	
	printf("Philosopher %d has been eating for %d seconds. (Total time eating = %d)\n", thinkID, eatingTime, *eatingTimeTotal);
	sleep(eatingTime);
}