/*****************************************************
* Random Test 3
* Kevin Woods
* 18Feb2018
* Card: Smithy
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
  printf("\n______________Smithy Test Begin_________________\n");
  
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
    i, x, y, cardMod,
    handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
  
  //Counters for game state
   int preHandCount,
    postHandCount,
    preActionCount,
    postActionCount,
		preDiscardDeckCount,
    postDiscardDeckCount;
   
  //Counters for pass/fails
   int failCount = 0,
    passCount = 0,
    handPass = 0,
    handFail = 0,
    actionPass = 0,
    actionFail = 0,
		discardDeckPass = 0,
    discardDeckFail = 0;
 
  for (i = 0; i < NUM_TESTS; i++) {
    randSeed = rand();
    
    memset(&G, 23, sizeof(struct gameState));               // clear the game state
    initializeGame(numPlayer, k, randSeed, &G);         // initialize a new game
    
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
		preDiscardDeckCount = G.deckCount[player] + G.discardCount[player];
    preHandCount = G.handCount[player];
    preActionCount = G.numActions;
    
    //call card effect and pass it this character to have adventurer played
    cardEffect(smithy, choice1, choice2, choice3, &G, handpos, &bonus);
    
     // Update game state counters
    postDiscardDeckCount = G.deckCount[player] + G.discardCount[player];
		postHandCount = G.handCount[player];
    postActionCount = G.numActions;
    
#if (NOISY_TEST == 1)
    printf("\nTesting case %d of %d\n", i+1, NUM_TESTS);
#endif
		
		//If there are less than three cards in deck/discard, 
		//only that many should be added to the hand
		if (preDiscardDeckCount == 0) {
			cardMod = 0;
		} else if (preDiscardDeckCount == 1) {
			cardMod = 1;
		} else if (preDiscardDeckCount == 2) {
			cardMod = 2;
		} else {
			cardMod = 3;
		}
		
    // Count total pass/fails
    if (preHandCount == postHandCount - cardMod + 1 &&
				preDiscardDeckCount == postDiscardDeckCount + cardMod &&
      	preActionCount == postActionCount + 1)
    {
      passCount++;
    } else {
      failCount++;
    }
   
		//Determine number of tests that fail the final hand count
    if (preHandCount == postHandCount - cardMod + 1) {
      handPass++;
#if (NOISY_TEST == 1)
      printf(GRN "PreHand = %d, PostHand = %d, Expected = %d\n" RESET, preHandCount, postHandCount, preHandCount + cardMod - 1);
#endif
    } else {
      handFail++;
#if (NOISY_TEST == 1)
      printf(RED "PreHand = %d, PostHand = %d, Expected = %d\n" RESET, preHandCount, postHandCount, preHandCount + cardMod - 1);
#endif
    }
		
		//Determine number of tests that fail the final discard/deck card count
		if (preDiscardDeckCount == postDiscardDeckCount + cardMod) {
			discardDeckPass++;
#if (NOISY_TEST == 1)
			printf(GRN "PreDiscardDeck = %d, PostDiscardDeck = %d, Expected = %d\n" RESET, preDiscardDeckCount, postDiscardDeckCount, preDiscardDeckCount - cardMod);
#endif
		} else {
			discardDeckFail++;
#if (NOISY_TEST == 1)
			printf(RED "PreDiscardDeck = %d, PostDiscardDeck = %d, Expected = %d\n" RESET, preDiscardDeckCount, postDiscardDeckCount, preDiscardDeckCount - cardMod);
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

  printf("%d Total Tests: %d Passed and %d Failed\n", NUM_TESTS, passCount, failCount);
  printf("%d Passing Hand Count Check \t %d Failing Hand Count Check\n", handPass, handFail);
	printf("%d Passing Discard and Deck Count Check \t %d Failing Discard and Deck Count Check\n", discardDeckPass, discardDeckFail);
  printf("%d Passing Action Count Check \t %d Failing Action Count Check\n", actionPass, actionFail);
  printf("Set NOISY_TEST to 1 to see each result.\n\n");
  
  
  
  printf("______________Smithy Test End____________________\n\n");
  return 0;
}