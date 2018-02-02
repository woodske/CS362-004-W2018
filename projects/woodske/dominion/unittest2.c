/*
   Test for the scoreFor() function which returns the score of each player based on their victory points
   */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

int main() {
	int i;
	int seed = 1000;
	int numPlayer = 2;
	int handCount;
	int discardCount;
	int deckCount;
	int cardType;
	int x;
	int k[10] = {adventurer, council_room, feast, gardens, mine
		, remodel, smithy, village, baron, great_hall};
	struct gameState G;


	for (x = 0; x < 6; x++) {
		if (x == 4)
			cardType = 16; //Great Hall
		else if (x == 5)
			cardType = 10; //gardens
		else
			cardType = x; //curse, estate, duchy, province

		for (handCount = 0; handCount < 6; handCount++) {
			for (discardCount = 0; discardCount < 6; discardCount++) {
				for (deckCount = 0; deckCount < 6; deckCount++) {
					

					

					printf ("TESTING scoreFor():\n");

					printf("Test player %d with %d, %d, %d, cards in hand/discard/deck of type %d:\n", p, handCount, discardCount, deckCount, cardType);

					memset(&G, 23, sizeof(struct gameState));   			// clear the game state
					r = initializeGame(numPlayer, k, seed, &G); 			// initialize a new game
					G.handCount[p] = handCount;                 			// set the number of cards on hand
					G.discardCount[p] = discardCount;				// set the number of cards in discard pile
					G.deckCount[p] = deckCount;				        // set the number of cards in deck
					memcpy(G.hand[p], cardType, sizeof(int) * handCount);		// set all the cards in hand to cardType
					memcpy(G.discard[p], cardType, sizeof(int) * discardCount);     // set all the cards in descard to cardType
					memcpy(G.deck[p], cardType, sizeof(int) * deckCount);		// set all the cards in deck to cardType
					updateCoins(p, &G, bonus);
