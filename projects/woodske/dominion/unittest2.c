/*
   Test for the scoreFor() function which returns the score of each player based on their victory points
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

//Set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 0

int main() {
	int i;
	int seed = 1000;
	int numPlayer = 2;
	int p = 2;
	int handCount;
	int discardCount;
	int deckCount;
	int cardType;
	int failedTest = 0;
	int passedTest = 0;
	int r, x, Gscore, Escore, cardValue;
	int k[10] = {adventurer, council_room, feast, gardens, mine
		, remodel, smithy, village, baron, great_hall};
	struct gameState G;
	
	printf("Testing scoreFor()...\n\n");	
/************************************************************************
*******Testing for Curse, Estate, Duchy, Province, and Great Hall *******
*************************************************************************/
	for (x = 0; x < 5; x++) {
		if (x == 4)
			cardType = 16; //Great Hall
		else
			cardType = x; //curse, estate, duchy, province

		for (handCount = 0; handCount < 3; handCount++) {
			for (discardCount = 0; discardCount < 3; discardCount++) {
				for (deckCount = 0; deckCount < 3; deckCount++) {
#if (NOISY_TEST == 1)
					printf("Test player %d with %d, %d, %d, cards in hand/discard/deck of type %d:\n", p, handCount, discardCount, deckCount, cardType);
#endif
					memset(&G, 23, sizeof(struct gameState));   			// clear the game state
					r = initializeGame(numPlayer, k, seed, &G); 			// initialize a new game
					G.handCount[p] = handCount;                 			// set the number of cards on hand
					G.discardCount[p] = discardCount;				// set the number of cards in discard pile
					G.deckCount[p] = deckCount;				        // set the number of cards in deck
				
				
					for (r = 0; r < handCount; r++) {
						G.hand[p][r] = cardType;
					}

					for (r = 0; r < discardCount; r++) {
						G.discard[p][r] = cardType;
					}

					for (r = 0; r < deckCount; r++) {
						G.deck[p][r] = cardType;
					}

					Gscore = scoreFor(p, &G);

					switch(cardType) {

						case curse:
							cardValue = -1;
							break;
						case estate:
							cardValue = 1;
							break;
						case duchy:
							cardValue = 3;
							break;
						case province:
							cardValue = 6;
							break;
						case great_hall:
							cardValue = 1;
							break;
						default:
							break;
					}
					
					Escore = handCount * cardValue + discardCount * cardValue + deckCount * cardValue;

					if (Gscore == Escore)
						passedTest++;
					else
						failedTest++;

#if (NOISY_TEST == 1)
					printf("G.scoreFor = %d, expected = %d\n", Gscore, Escore);
					
					if (Gscore == Escore) {
						printf(GRN "**********************TEST PASSED**********************\n\n" RESET);
					} else {
						printf(RED "**********************TEST FAILED**********************\n\n" RESET);
					}
#endif
				}
			}
		}
	}
	printf("For Curse, Estate, Duchy, Province, Great Hall -- # Passed Tests: %d\t# Failed Tests: %d\n", passedTest, failedTest);
	printf("Set NOISY_TEST to 1 to see testing output.\n\n");
					
					
/*********************************
*******Testing for Gardens *******
**********************************/
					
	passedTest = 0;
	failedTest = 0;	
	for (handCount = 0; handCount < 7; handCount++) {
		for (discardCount = 0; discardCount < 7; discardCount++) {
			for (deckCount = 0; deckCount < 7; deckCount++) {

#if (NOISY_TEST == 1)
				printf("Test player %d with %d, %d, %d, cards in hand/discard/deck of type gardens:\n", p, handCount, discardCount, deckCount);
#endif
				memset(&G, 23, sizeof(struct gameState));   			// clear the game state
				r = initializeGame(numPlayer, k, seed, &G); 			// initialize a new game
				G.handCount[p] = handCount;                 			// set the number of cards on hand
				G.discardCount[p] = discardCount;				// set the number of cards in discard pile
				G.deckCount[p] = deckCount;				        // set the number of cards in deck


				for (r = 0; r < handCount; r++) {
					G.hand[p][r] = gardens;
				}

				for (r = 0; r < discardCount; r++) {
					G.discard[p][r] = gardens;
				}

				for (r = 0; r < deckCount; r++) {
					G.deck[p][r] = gardens;
				}

				Gscore = scoreFor(p, &G);
				int gardensScore = (handCount + discardCount + deckCount)/10;
				Escore = handCount * gardensScore + discardCount * gardensScore + deckCount * gardensScore;

				if (Gscore == Escore)
					passedTest++;
				else
					failedTest++;

#if (NOISY_TEST == 1)
				printf("G.scoreFor = %d, expected = %d\n", Gscore, Escore);
				if (Gscore == Escore) {
					printf(GRN "**********************TEST PASSED**********************\n\n" RESET);
				} else {
					printf(RED "**********************TEST FAILED**********************\n\n" RESET);
				}
#endif
			}
		}
	}

	printf("For Gardens -- # Passed Tests: %d\t# Failed Tests: %d\n", passedTest, failedTest);
	printf("Set NOISY_TEST to 1 to see testing output.\n\n");

	return 0;
}


