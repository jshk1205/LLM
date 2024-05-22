import random

# Initialize card deck
suits = ["Spades", "Diamonds", "Hearts", "Clubs"]
ranks = ["2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"]
values = [2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11]

# Define card class
class Card:
    def __init__(self, suit, rank):
        self.suit = suit
        self.rank = rank

# Create card deck
class Deck:
    def __init__(self):
        self.deck = [Card(suit, rank) for suit in suits for rank in ranks]
        self.current_card = 0

    def shuffle_deck(self):
        random.shuffle(self.deck)

    def deal_card(self):
        card = self.deck[self.current_card]
        self.current_card += 1
        return card

# Define hand class
class Hand:
    def __init__(self):
        self.cards = []
        self.num_cards = 0
        self.value = 0
        self.aces = 0

    def add_card_to_hand(self, card):
        self.cards.append(card)
        self.num_cards += 1
        card_value_index = ranks.index(card.rank)
        self.value += values[card_value_index]
        if card.rank == "A":
            self.aces += 1

    def adjust_for_ace(self):
        while self.value > 21 and self.aces:
            self.value -= 10
            self.aces -= 1

# Player's turn
def player_turn(deck, player_hand):
    while True:
        print("\nPlayer's cards:")
        for card in player_hand.cards:
            print(f"{card.rank} of {card.suit}")
        print(f"Player's card total: {player_hand.value}")

        if player_hand.value == 21:
            print("Blackjack! Player wins!")
            break
        elif player_hand.value > 21:
            print("Card total exceeds 21. Player busts!")
            break
        else:
            action = input("Would you like to hit or stay? (hit/stay): ")
            if action == "hit":
                player_hand.add_card_to_hand(deck.deal_card())
                player_hand.adjust_for_ace()
            elif action == "stay":
                break

# Dealer's turn
def dealer_turn(deck, dealer_hand):
    while dealer_hand.value < 17:
        dealer_hand.add_card_to_hand(deck.deal_card())
        dealer_hand.adjust_for_ace()

# Play the game
def blackjack_game():
    while True:
        # Initialize game
        deck = Deck()
        deck.shuffle_deck()

        player_hand = Hand()
        dealer_hand = Hand()

        player_hand.add_card_to_hand(deck.deal_card())
        player_hand.add_card_to_hand(deck.deal_card())
        dealer_hand.add_card_to_hand(deck.deal_card())
        dealer_hand.add_card_to_hand(deck.deal_card())

        # Player's turn
        player_turn(deck, player_hand)

        # Dealer's turn
        if player_hand.value <= 21:
            dealer_turn(deck, dealer_hand)

        # Print results
        print("\nDealer's cards:")
        for card in dealer_hand.cards:
            print(f"{card.rank} of {card.suit}")
        print(f"Dealer's card total: {dealer_hand.value}")

        if dealer_hand.value > 21:
            print("Dealer busts! Player wins!")
        elif dealer_hand.value > player_hand.value:
            print("Dealer wins!")
        elif dealer_hand.value < player_hand.value:
            print("Player wins!")
        else:
            print("It's a tie!")

        # Check if the player wants to play again
        play_again = input("\nDo you want to play again? (yes/no): ")
        if play_again.lower() != "yes":
            break

if __name__ == "__main__":
    blackjack_game()
