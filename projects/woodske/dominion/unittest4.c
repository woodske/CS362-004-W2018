/*
   Test for the isGameOver() function which returns the score of each player based on their victory points
   */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define RED 	"\x1B[31m"
#define GRN		"\x1B[32m"
#define RESET	"\x1B[0m"

int main() {
	int seed = 1000;
	int numPlayer = 2;
	int p = 2;
	int r, gameOver, x, y;
	int k[10] = {adventurer, council_room, feast, gardens, mine
		, remodel, smithy, village, baron, great_hall};
	struct gameState G;

	printf("Testing isGameOver()...\n\n");	

	/************************************************************************
	********************** Testing for game start ***************************
	*************************************************************************/
		
	memset(&G, 23, sizeof(struct gameState));   			// clear the game state
	r = initializeGame(numPlayer, k, seed, &G); 			// initialize a new game
	
	printf("Testing if game is over after initialization... \n");

	gameOver = isGameOver(&G);
				
	if (gameOver == 0) {
		printf(GRN "**********************TEST PASSED**********************\n\n" RESET);
	} else {
		printf(RED "**********************TEST FAILED**********************\n\n" RESET);
	}

	/************************************************************************
	********************** Testing for empty province ***********************
	*************************************************************************/

	memset(&G, 23, sizeof(struct gameState));   			// clear the game state
	r = initializeGame(numPlayer, k, seed, &G); 			// initialize a new game

	printf("Testing if game is over after provinces are gone... \n");

	G.supplyCount[province] = 0;

	gameOver = isGameOver(&G);
				
	if (gameOver == 1) {
		printf(GRN "**********************TEST PASSED**********************\n\n" RESET);
	} else {
		printf(RED "**********************TEST FAILED**********************\n\n" RESET);
	}

	/************************************************************************
	********************** Testing for 1 - 4 empty piles ************************
	*************************************************************************/

	printf("Testing if game is over after 1 - 4 piles are gone... \n");

	for (x = 0; x < 4; x++) {

		memset(&G, 23, sizeof(struct gameState));   			// clear the game state
		r = initializeGame(numPlayer, k, seed, &G); 			// initialize a new game

		for (y = 0; y < x + 1; y++) {
			G.supplyCount[k[y]] = 0;
		}

		gameOver = isGameOver(&G);
		printf("%d piles are empty\n", x+1);
		
		printf("Gameover value: %d\n", gameOver);

		if (x >= 2) {			
			if (gameOver == 1) {
				printf(GRN "**********************TEST PASSED**********************\n\n" RESET);
			} else {
				printf(RED "**********************TEST FAILED**********************\n\n" RESET);
			}
		} else {
			if (gameOver == 0) {
				printf(GRN "**********************TEST PASSED**********************\n\n" RESET);
			} else {
				printf(RED "**********************TEST FAILED**********************\n\n" RESET);
			}
		}
	}

	
	return 0;

}

	

	