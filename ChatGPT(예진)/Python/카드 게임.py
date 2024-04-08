import random

class Card:
    def __init__(self, suit, value):
        self.suit = suit
        self.value = value

    def __repr__(self):
        return f"{self.value} of {self.suit}"

class Deck:
    def __init__(self):
        self.cards = [Card(suit, value) for suit in ["Hearts", "Diamonds", "Clubs", "Spades"]
                      for value in range(2, 15)]

    def shuffle(self):
        random.shuffle(self.cards)

    def deal(self):
        if len(self.cards) > 0:
            return self.cards.pop()
        else:
            return None

class Player:
    def __init__(self, name):
        self.name = name
        self.hand = []

    def draw(self, deck, num_cards=1):
        for _ in range(num_cards):
            card = deck.deal()
            if card:
                self.hand.append(card)
            else:
                print("No more cards in the deck!")
                break

    def discard(self, index):
        if 0 <= index < len(self.hand):
            return self.hand.pop(index)
        else:
            return None

    def show_hand(self):
        print(f"{self.name}'s hand:")
        for card in self.hand:
            print(card)

def play_round(player1, player2, deck):
    player1.draw(deck, 3)
    player2.draw(deck, 3)

    player1.show_hand()
    player2.show_hand()

    while True:
        choice = input("Which card to discard (0-2)? Type 'none' to keep all cards: ")
        if choice.lower() == 'none':
            break
        try:
            index = int(choice)
            discarded_card = player1.discard(index)
            if discarded_card:
                print(f"{player1.name} discarded: {discarded_card}")
            else:
                print("Invalid choice. Please choose again.")
        except ValueError:
            print("Invalid input. Please enter a number or 'none'.")

    while len(player1.hand) < 3:
        player1.draw(deck)

    while len(player2.hand) < 3:
        player2.draw(deck)

    # Additional event: Check if player1 has a flush
    if all(card.suit == player1.hand[0].suit for card in player1.hand):
        print(f"{player1.name} has a flush!")
        # Implement additional action for flush

    if len(player1.hand) > len(player2.hand):
        return player1.name
    elif len(player2.hand) > len(player1.hand):
        return player2.name
    else:
        return "It's a tie!"

def main():
    deck = Deck()
    deck.shuffle()

    player1 = Player("Player 1")
    player2 = Player("Player 2")

    rounds_to_play = 3

    for round_num in range(1, rounds_to_play + 1):
        print(f"\nRound {round_num}:")
        round_winner = play_round(player1, player2, deck)
        print(f"Winner of round {round_num}: {round_winner}")

    print("\nFinal Result:")
    player1.show_hand()
    player2.show_hand()

if __name__ == "__main__":
    main()
