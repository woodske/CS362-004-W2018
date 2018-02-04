
/*
 * Testing Adventurer
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "adventurer"
#define RED     "\x1B[31m"
#define GRN     "\x1B[32m"
#define RESET   "\x1B[0m"

int main() {

    int i, j, m;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int preHandCount, preDiscardCount, preCoinCount, postHandCount, postDiscardCount, postCoinCount;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n----------------- Testing Card: %s ----------------\n", TESTCARD);

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	// change player's deck to estate, copper, and silver
	testG.deck[thisPlayer][2] = estate;
	testG.deck[thisPlayer][1] = copper;
	testG.deck[thisPlayer][0] = silver;
	testG.deckCount[thisPlayer] = 3;

	// change player's hand to estate, estate, copper
	testG.hand[thisPlayer][0] = estate;
	testG.hand[thisPlayer][1] = estate;
	testG.hand[thisPlayer][2] = gold;
	testG.handCount[thisPlayer] = 3;

	// Save initial values
	preHandCount = testG.handCount[thisPlayer];
	preDiscardCount = testG.discardCount[thisPlayer];
	i = updateCoins(thisPlayer, &testG, bonus);
	preCoinCount = testG.coins;

	printf("Pre-Adventurer: handcount = %d, discard = %d, coins = %d\n", preHandCount, preDiscardCount, preCoinCount);
	cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);
	i = updateCoins(thisPlayer, &testG, bonus);
	postHandCount = testG.handCount[thisPlayer];
	postDiscardCount = testG.discardCount[thisPlayer];
	postCoinCount = testG.coins;

	printf("Post-Adventurer: handcount = %d, discard = %d, coins = %d\n", postHandCount, postDiscardCount, postCoinCount);
	printf("Expected Post-Adventurer: handcount = %d, discard = %d, coins = %d\n", preHandCount + 2, preDiscardCount + 1, preCoinCount + 3);

	if (postHandCount == preHandCount +2 && postDiscardCount == preDiscardCount + 1 && postCoinCount == preCoinCount + 3) {
        printf(GRN "**********************TEST PASSED**********************\n\n" RESET);
    } else {
        printf(RED "**********************TEST FAILED**********************\n\n" RESET);
    }

	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);

	return 0;
}


