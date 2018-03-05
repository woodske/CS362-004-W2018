/*****************************************************
* Random Test 1
* Kevin Woods
* 18Feb2018
* Card: Adventurer
*****************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define NUM_TESTS 100000
#define AVAILABLE_CARDS 16

#define RED     "\x1B[31m"
#define GRN     "\x1B[32m"
#define RESET   "\x1B[0m"

//Set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 0

int main() {
  printf("\n______________Adventurer Test Begin_________________\n");
  
  struct gameState G;
  srand(time(NULL));
	
  //Kingdom cards used
  int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
  
  //Cards to fill hand/deck/discard
  int cards[16] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room, estate, duchy, province,
      copper, silver, gold};
  
  //Variables for game intiation
  int player = 0,
    numPlayer = 2,
    randSeed,
    i, x, y, treasureMod, 
    handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
  
  //Counters for game state
  int preTreasureCount,
    postTreasureCount,
    preDiscardDeckCount,
    postDiscardDeckCount,
    preActionCount,
    postActionCount,
		totalTreasure;
   
  //Counters for pass/fails
  int failCount = 0,
    passCount = 0,
    treasurePass = 0,
    treasureFail = 0,
    discardDeckPass = 0,
    discardDeckFail = 0,
    actionPass = 0,
    actionFail = 0;
 
  for (i = 0; i < NUM_TESTS; i++) {
    
    randSeed = rand();
    
		memset(&G, 23, sizeof(struct gameState));               // clear the game state
    initializeGame(numPlayer, k, randSeed, &G);         		// initialize a new game
    
    //Generate a randomly sized deck/hand/discard
    G.deckCount[player] = rand() % MAX_DECK;
    G.discardCount[player] = rand() % MAX_DECK; 
    G.handCount[player] = rand() % MAX_HAND;
    
    //Fill deck/hand/discard with random dominion cards
    for (x = 0; x < G.deckCount[player]; x++) {
      y = rand() % AVAILABLE_CARDS;
      G.deck[player][x] = cards[y];
    }

    for (x = 0; x < G.discardCount[player]; x++) {
      y = rand() % AVAILABLE_CARDS;
      G.discard[player][x] = cards[y];
    }
    
    for (x = 0; x < G.handCount[player]; x++) {
      y = rand() % AVAILABLE_CARDS;
      G.hand[player][x] = cards[y];
    }
		
    // Reset game state counters
    preTreasureCount = 0;
    postTreasureCount = 0;
    preDiscardDeckCount = G.deckCount[player] + G.discardCount[player];
    preActionCount = G.numActions;
		totalTreasure = 0;
		
		//Count the number of treasure cards in hand/deck. Must have alternative pass/fail
		//conditions if the number of treasures is less than two.
    for (x = 0; x < G.deckCount[player]; x++) {
      if (G.deck[player][x] == copper ||
         G.deck[player][x] == silver ||
         G.deck[player][x] == gold) 
      {
        totalTreasure++;
      }
    }

    for (x = 0; x < G.discardCount[player]; x++) {
      if (G.discard[player][x] == copper ||
         G.discard[player][x] == silver ||
         G.discard[player][x] == gold) 
      {
        totalTreasure++;
      }
    }
    
    //Count the number of treasures in hand
    for (x = 0; x < G.handCount[player]; x++) {
      if (G.hand[player][x] == copper ||
         G.hand[player][x] == silver ||
         G.hand[player][x] == gold) 
      {
        preTreasureCount++;
      }
    }
    
    //call card effect and pass it this character to have adventurer played
    cardEffect(adventurer, choice1, choice2, choice3, &G, handpos, &bonus);
   
    //Count the number of treasures in hand
    for (x = 0; x < G.handCount[player]; x++) {
      if (G.hand[player][x] == copper ||
         G.hand[player][x] == silver ||
         G.hand[player][x] == gold) 
      {
        postTreasureCount++;
      }
    }
    
    postDiscardDeckCount = G.deckCount[player] + G.discardCount[player];
    postActionCount = G.numActions;
    
#if (NOISY_TEST == 1)
		printf("\nTesting case %d of %d\n", i+1, NUM_TESTS);
#endif
		
		//Expected changes in treasure count depending on how many treasures are in deck/discard
		if (totalTreasure == 0) {
			treasureMod = 0;
		} else if (totalTreasure == 1) {
			treasureMod = 1;
		} else {
			treasureMod = 2;
		}
		
		//Determine number of tests with 100% passing criteria
		if (preTreasureCount == postTreasureCount - treasureMod &&
			preDiscardDeckCount == postDiscardDeckCount + treasureMod &&
			preActionCount == postActionCount + 1)
		{
			passCount++;
		} else {
			failCount++;
		}

		//Determine number of tests that fail the final treasure in hand count
		if (preTreasureCount == postTreasureCount - treasureMod) {
			treasurePass++;
#if (NOISY_TEST == 1)
			printf(GRN "PreTreasure = %d, PostTreasure = %d, Expected = %d\n" RESET, preTreasureCount, postTreasureCount, preTreasureCount + treasureMod);
#endif
		} else {
			treasureFail++;
#if (NOISY_TEST == 1)
			printf(RED "PreTreasure = %d, PostTreasure = %d, Expected = %d\n" RESET, preTreasureCount, postTreasureCount, preTreasureCount + treasureMod);
#endif
		}

		//Determine number of tests that fail the final discard/deck card count
		if (preDiscardDeckCount == postDiscardDeckCount + treasureMod) {
			discardDeckPass++;
#if (NOISY_TEST == 1)
			printf(GRN "PreDiscardDeck = %d, PostDiscardDeck = %d, Expected = %d\n" RESET, preDiscardDeckCount, postDiscardDeckCount, preDiscardDeckCount - treasureMod);
#endif
		} else {
			discardDeckFail++;
#if (NOISY_TEST == 1)
			printf(RED "PreDiscardDeck = %d, PostDiscardDeck = %d, Expected = %d\n" RESET, preDiscardDeckCount, postDiscardDeckCount, preDiscardDeckCount - treasureMod);
#endif
		}

		//Determine number of tests that fail the final action count
		if (preActionCount == postActionCount + 1) {
			actionPass++;
#if (NOISY_TEST == 1)
			printf(GRN "PreActionCount = %d, PostActionCount = %d, Expected = %d\n" RESET, preActionCount, postActionCount, preActionCount - 1);
#endif
		} else {
			actionFail++;
#if (NOISY_TEST == 1)
			printf(RED "PreActionCount = %d, PostActionCount = %d, Expected = %d\n" RESET, preActionCount, postActionCount, preActionCount - 1);
#endif
		}    
	}
	
	//Print summary of results
  printf("%d Total Tests: %d Passed and %d Failed\n", NUM_TESTS, passCount, failCount);
  printf("%d Passing Treasure Count Check \t\t %d Failing Treasure Count Check\n", treasurePass, treasureFail);
  printf("%d Passing Discard and Deck Count Check \t %d Failing Discard and Deck Count Check\n", discardDeckPass, discardDeckFail);
  printf("%d Passing Action Count Check \t\t\t %d Failing Action Count Check\n", actionPass, actionFail);
	printf("\nSet NOISY_TEST to 1 to see each result.\n\n");
  
  printf("______________Adventurer Test End____________________\n\n");
  return 0;
}
