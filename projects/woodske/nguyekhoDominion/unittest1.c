/*
   Test for the gainCard() function which adds a card to a player's discard, deck, or hand
   */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define RED     "\x1B[31m"
#define GRN     "\x1B[32m"
#define RESET   "\x1B[0m"

int main() {
    int seed = 1000;
    int numPlayer = 2;
    int p = 2;
    int r, gainCardStatus, x, y, cardType;
    int deckCount, handCount, discardCount, supplyCount;
    int k[10] = {adventurer, council_room, feast, gardens, mine
        , remodel, smithy, village, baron, great_hall};
    struct gameState G;

    printf("Testing gainCard()...\n\n");  

    /************************************************************************
    ********************** Testing supply count check ***********************
    *************************************************************************/
        
    printf("Testing if gain card fails if supply is empty... \n");

    memset(&G, 23, sizeof(struct gameState));               // clear the game state
    r = initializeGame(numPlayer, k, seed, &G);             // initialize a new game

    //Attempt to gain adventurer to hand with an empty adventurer supply
    G.supplyCount[adventurer] = 0;

    gainCardStatus = gainCard(adventurer, &G, 2, 2);
                
    if (gainCardStatus == -1) {
        printf(GRN "**********************TEST PASSED**********************\n\n" RESET);
    } else {
        printf(RED "**********************TEST FAILED**********************\n\n" RESET);
    }

    printf("Testing if gain card fails if card is not in game... \n");

    memset(&G, 23, sizeof(struct gameState));               // clear the game state
    r = initializeGame(numPlayer, k, seed, &G);             // initialize a new game

    //Attempt to gain adventurer to hand with an empty adventurer supply
    
    gainCardStatus = gainCard(steward, &G, 2, 2);
                
    if (gainCardStatus == -1) {
        printf(GRN "**********************TEST PASSED**********************\n\n" RESET);
    } else {
        printf(RED "**********************TEST FAILED**********************\n\n" RESET);
    }

    /************************************************************************
    ********************** Testing status of card piles *********************
    *************************************************************************/
        
    printf("Testing status of cards piles after gainCard()... \n");

    for (p = 0; p < numPlayer; p++) {       //num players
        for (y = 0; y < 3; y++) {           //toFlag
            for (x = 0; x < 2; x++) {       //supplyPos
                switch(x) {
                    case 0:
                        cardType = adventurer;
                        break;
                    case 1:
                        cardType = great_hall;
                        break;
                    default:
                        break;
                }

            memset(&G, 23, sizeof(struct gameState));               // clear the game state
            r = initializeGame(numPlayer, k, seed, &G);             // initialize a new game

            discardCount = G.discardCount[p];
            deckCount = G.deckCount[p];
            handCount = G.handCount[p];
            supplyCount = G.supplyCount[cardType];

            gainCardStatus = gainCard(cardType, &G, y, p);

            printf("Discard, deck, hand, and supply counts of gainCard() type %d:\n", cardType); 
            printf("Pre: %d, %d, %d, %d\n", discardCount, deckCount, handCount, supplyCount);
            printf("Post: %d, %d, %d, %d\n", G.discardCount[p], G.deckCount[p], G.handCount[p], G.supplyCount[cardType]);

            switch(y) {
                case 0:     //discard
                    if (discardCount == G.discardCount[p] - 1 
                        && deckCount == G.deckCount[p] 
                        && handCount == G.handCount[p]
                        && supplyCount == G.supplyCount[cardType] + 1) {

                        printf(GRN "**********************TEST PASSED**********************\n\n" RESET);
                    } else {
                        printf(RED "**********************TEST FAILED**********************\n\n" RESET);
                    }
                    break;
                case 1:     //deck
                        if (discardCount == G.discardCount[p] 
                        && deckCount == G.deckCount[p] - 1
                        && handCount == G.handCount[p]
                        && supplyCount == G.supplyCount[cardType] + 1) {

                        printf(GRN "**********************TEST PASSED**********************\n\n" RESET);
                    } else {
                        printf(RED "**********************TEST FAILED**********************\n\n" RESET);
                    }
                    break;
                case 2:     //hand
                        if (discardCount == G.discardCount[p] 
                        && deckCount == G.deckCount[p]
                        && handCount == G.handCount[p] - 1
                        && supplyCount == G.supplyCount[cardType] + 1) {

                        printf(GRN "**********************TEST PASSED**********************\n\n" RESET);
                    } else {
                        printf(RED "**********************TEST FAILED**********************\n\n" RESET);
                    }
                    break;
                }
                    
            }
        }
    }
    return 0;
}