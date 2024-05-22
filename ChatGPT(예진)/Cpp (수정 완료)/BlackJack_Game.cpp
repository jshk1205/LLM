#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

// Initialize the deck of cards
std::array<std::string, 4> suits = { "Spades", "Diamonds", "Hearts", "Clubs" };
std::array<std::string, 13> ranks = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
std::array<int, 13> values = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11 };

// Define the structure of a card
struct Card {
    std::string suit;
    std::string rank;
};

// Create the deck of cards
struct Deck {
    std::array<Card, 52> deck;
    int current_card;
};

// Define the structure of a hand
struct Hand {
    std::vector<Card> cards; // Assume a maximum of 11 cards per hand
    int num_cards;
    int value;
    int aces;
};

// Initialize the deck of cards
void initialize_deck(Deck& deck) {
    int k = 0;
    for (const auto& suit : suits) {
        for (const auto& rank : ranks) {
            deck.deck[k].suit = suit;
            deck.deck[k].rank = rank;
            k++;
        }
    }
    deck.current_card = 0;
}

// Shuffle the deck of cards
void shuffle_deck(Deck& deck) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.deck.begin(), deck.deck.end(), g);
}

// Deal a card from the deck
Card deal_card(Deck& deck) {
    return deck.deck[deck.current_card++];
}

// Initialize a hand
void initialize_hand(Hand& hand) {
    hand.num_cards = 0;
    hand.value = 0;
    hand.aces = 0;
}

// Add a card to a hand
void add_card_to_hand(Hand& hand, Card card) {
    hand.cards.push_back(card);
    auto it = std::find(ranks.begin(), ranks.end(), card.rank);
    if (it != ranks.end()) {
        int index = std::distance(ranks.begin(), it);
        hand.value += values[index];
        if (card.rank == "A") // Ace handling
            hand.aces++;
    }
}

// Adjust for aces in a hand
void adjust_for_ace(Hand& hand) {
    while (hand.value > 21 && hand.aces) {
        hand.value -= 10;
        hand.aces--;
    }
}

// Player's turn
void player_turn(Deck& deck, Hand& player_hand) {
    std::string action;
    while (true) {
        std::cout << "\nPlayer's cards:\n";
        for (const auto& card : player_hand.cards) {
            std::cout << card.rank << " of " << card.suit << '\n';
        }
        std::cout << "Player's card total: " << player_hand.value << '\n';

        if (player_hand.value == 21) {
            std::cout << "Blackjack! Player wins!\n";
            break;
        }
        else if (player_hand.value > 21) {
            std::cout << "Card total exceeds 21. Dealer wins!\n";
            break;
        }
        else {
            std::cout << "Would you like to hit or stay? (hit/stay): ";
            std::cin >> action;
            if (action == "hit") {
                add_card_to_hand(player_hand, deal_card(deck));
                adjust_for_ace(player_hand);
            }
            else if (action == "stay") {
                break;
            }
        }
    }
}

// Dealer's turn
void dealer_turn(Deck& deck, Hand& dealer_hand) {
    while (dealer_hand.value < 17) {
        add_card_to_hand(dealer_hand, deal_card(deck));
        adjust_for_ace(dealer_hand);
    }
}

// Run the game
void blackjack_game() {
    std::random_device rd;
    std::mt19937 g(rd());

    while (true) {
        // Initialize the game
        Deck deck;
        initialize_deck(deck);
        shuffle_deck(deck);

        Hand player_hand, dealer_hand;
        initialize_hand(player_hand); // Initialize player's hand
        initialize_hand(dealer_hand);

        add_card_to_hand(player_hand, deal_card(deck));
        add_card_to_hand(player_hand, deal_card(deck));
        add_card_to_hand(dealer_hand, deal_card(deck));
        add_card_to_hand(dealer_hand, deal_card(deck));

        // Player's turn
        player_turn(deck, player_hand);

        // Dealer's turn
        if (player_hand.value <= 21) {
            dealer_turn(deck, dealer_hand);
        }

        // Print results
        std::cout << "\nDealer's cards:\n";
        for (const auto& card : dealer_hand.cards) {
            std::cout << card.rank << " of " << card.suit << '\n';
        }
        std::cout << "Dealer's card total: " << dealer_hand.value << '\n';

        if (dealer_hand.value > 21) {
            std::cout << "Dealer busts! Player wins!\n";
        }
        else if (dealer_hand.value > player_hand.value) {
            std::cout << "Dealer wins!\n";
        }
        else if (dealer_hand.value < player_hand.value) {
            std::cout << "Player wins!\n";
        }
        else {
            std::cout << "It's a tie!\n";
        }

        // Ask if the player wants to play again
        std::string play_again;
        std::cout << "\nWould you like to play again? (yes/no): ";
        std::cin >> play_again;
        if (play_again == "no") {
            break;
        }
    }
}

int main() {
    blackjack_game();
    return 0;
}
