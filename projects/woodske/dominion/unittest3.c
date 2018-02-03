/*
   Test for the fullDeckCount() function which returns the score of each player based on their victory points
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
	int i;
	int seed = 1000;
	int numPlayer = 2;
	int p = 2;
	int handCount;
	int discardCount;
	int deckCount;
	int cardType;
	int r, x, FDCscore, Escore;
	int k[10] = {adventurer, council_room, feast, gardens, mine
		, remodel, smithy, village, baron, great_hall};
	struct gameState G;

	printf("Testing fullDeckCount()...\n\n");	
/************************************************************************
*******Testing for Adventurer, Council Room, Feast, and Gardens *********
*************************************************************************/
	//Fill hand, discard, and deck with cards then count using fullDeckCount
	for (cardType = 0; cardType < 4; cardType++) { 
		for (handCount = 0; handCount < 3; handCount++) {
			for (discardCount = 0; discardCount < 3; discardCount++) {
				for (deckCount = 0; deckCount < 3; deckCount++) {

					printf("Test player %d with %d, %d, %d, cards in hand/discard/deck of type %d:\n", p, handCount, discardCount, deckCount, cardType);

					memset(&G, 23, sizeof(struct gameState));   			// clear the game state
					r = initializeGame(numPlayer, k, seed, &G); 			// initialize a new game
					G.handCount[p] = handCount;                 			// set the number of cards on hand
					G.discardCount[p] = discardCount;				// set the number of cards in discard pile
					G.deckCount[p] = deckCount;				        // set the number of cards in deck
				
					for (r = 0; r < handCount; r++) {
						G.hand[p][r] = k[cardType];
					}

					for (r = 0; r < discardCount; r++) {
						G.discard[p][r] = k[cardType];
					}

					for (r = 0; r < deckCount; r++) {
						G.deck[p][r] = k[cardType];
					}

					FDCscore = fullDeckCount(p, k[cardType], &G);

					Escore = handCount + discardCount + deckCount;

					printf("Deck count = %d, expected = %d\n", FDCscore, Escore);

					if (FDCscore == Escore) {
						printf(GRN "**********************TEST PASSED**********************\n\n" RESET);
					} else {
						printf(RED "**********************TEST FAILED**********************\n\n" RESET);
					}	
				}
			}
		}
	}
	return 0;
}
