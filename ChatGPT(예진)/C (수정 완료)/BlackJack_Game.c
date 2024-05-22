#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Initialize card deck
char* suits[] = { "Spades", "Diamonds", "Hearts", "Clubs" };
char* ranks[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
int values[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11 };

// Define card structure
typedef struct {
    char* suit;
    char* rank;
} Card;

// Create card deck
typedef struct {
    Card deck[52];
    int current_card;
} Deck;

// Define hand structure
typedef struct {
    Card cards[11]; // Assume a hand can hold a maximum of 11 cards
    int num_cards;
    int value;
    int aces;
} Hand;

// Initialize card deck
void initialize_deck(Deck* deck) {
    int i, j, k = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 13; j++) {
            deck->deck[k].suit = suits[i];
            deck->deck[k].rank = ranks[j];
            k++;
        }
    }
    deck->current_card = 0;
}

// Shuffle the deck
void shuffle_deck(Deck* deck) {
    int i, j;
    Card temp;
    for (i = 0; i < 52; i++) {
        j = rand() % 52;
        temp = deck->deck[i];
        deck->deck[i] = deck->deck[j];
        deck->deck[j] = temp;
    }
}

// Deal a card
Card deal_card(Deck* deck) {
    return deck->deck[deck->current_card++];
}

// Initialize hand
void initialize_hand(Hand* hand) {
    hand->num_cards = 0;
    hand->value = 0;
    hand->aces = 0;
}

// Add a card to hand
void add_card_to_hand(Hand* hand, Card card) {
    hand->cards[hand->num_cards++] = card;
    int i;
    for (i = 0; i < 13; i++) {
        if (strcmp(card.rank, ranks[i]) == 0) {
            hand->value += values[i];
            if (strcmp(card.rank, "A") == 0) // Handle ace
                hand->aces++;
            break;
        }
    }
}

// Adjust for ace
void adjust_for_ace(Hand* hand) {
    while (hand->value > 21 && hand->aces) {
        hand->value -= 10;
        hand->aces--;
    }
}

// Player's turn
void player_turn(Deck* deck, Hand* player_hand) {
    char action[10];
    while (1) {
        printf("\nPlayer's cards:\n");
        for (int i = 0; i < player_hand->num_cards; i++) {
            printf("%s of %s\n", player_hand->cards[i].rank, player_hand->cards[i].suit);
        }
        printf("Player's card total: %d\n", player_hand->value);

        if (player_hand->value == 21) {
            printf("Blackjack! Player wins!\n");
            break;
        }
        else if (player_hand->value > 21) {
            printf("Card total exceeds 21. Dealer wins!\n");
            break;
        }
        else {
            printf("Would you like to hit or stay? (hit/stay): ");
            scanf("%s", action);
            if (strcmp(action, "hit") == 0) {
                add_card_to_hand(player_hand, deal_card(deck));
                adjust_for_ace(player_hand);
            }
            else if (strcmp(action, "stay") == 0) {
                break;
            }
        }
    }
}

// Dealer's turn
void dealer_turn(Deck* deck, Hand* dealer_hand) {
    while (dealer_hand->value < 17) {
        add_card_to_hand(dealer_hand, deal_card(deck));
        adjust_for_ace(dealer_hand);
    }
}

// Play the game
void blackjack_game() {
    srand(time(NULL));

    while (1) {
        // Initialize game
        Deck deck;
        initialize_deck(&deck);
        shuffle_deck(&deck);

        Hand player_hand, dealer_hand;
        initialize_hand(&player_hand); // Initialize player's hand
        initialize_hand(&dealer_hand);

        add_card_to_hand(&player_hand, deal_card(&deck));
        add_card_to_hand(&player_hand, deal_card(&deck));
        add_card_to_hand(&dealer_hand, deal_card(&deck));
        add_card_to_hand(&dealer_hand, deal_card(&deck));

        // Player's turn
        player_turn(&deck, &player_hand);

        // Dealer's turn
        if (player_hand.value <= 21) {
            dealer_turn(&deck, &dealer_hand);
        }

        // Print results
        printf("\nDealer's cards:\n");
        for (int i = 0; i < dealer_hand.num_cards; i++) {
            printf("%s of %s\n", dealer_hand.cards[i].rank, dealer_hand.cards[i].suit);
        }
        printf("Dealer's card total: %d\n", dealer_hand.value);

        if (dealer_hand.value > 21) {
            printf("Dealer busts! Player wins!\n");
        }
        else if (dealer_hand.value > player_hand.value) {
            printf("Dealer wins!\n");
        }
        else if (dealer_hand.value < player_hand.value) {
            printf("Player wins!\n");
        }
        else {
            printf("It's a tie!\n");
        }

        // Check if the player wants to play again
        char play_again[4];
        printf("\nDo you want to play again? (yes/no): ");
        scanf("%s", play_again);
        if (strcmp(play_again, "no") == 0) {
            break;
        }
    }
}

int main() {
    blackjack_game();
    return 0;
}