import random

# 카드 덱 초기화
suits = ('스페이드', '다이아몬드', '하트', '클로버')
ranks = ('2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A')
values = {'2': 2, '3': 3, '4': 4, '5': 5, '6': 6, '7': 7, '8': 8, '9': 9, '10': 10, 'J': 10, 'Q': 10, 'K': 10, 'A': 11}

# 카드 클래스 정의
class Card:
    def __init__(self, suit, rank):
        self.suit = suit
        self.rank = rank

    def __str__(self):
        return self.rank + '의 ' + self.suit

# 카드 덱 생성
class Deck:
    def __init__(self):
        self.deck = []
        for suit in suits:
            for rank in ranks:
                self.deck.append(Card(suit, rank))

    def shuffle(self):
        random.shuffle(self.deck)

    def deal(self):
        return self.deck.pop()

# 핸드 클래스 정의
class Hand:
    def __init__(self):
        self.cards = []
        self.value = 0
        self.aces = 0

    def add_card(self, card):
        self.cards.append(card)
        self.value += values[card.rank]
        if card.rank == 'A':
            self.aces += 1

    def adjust_for_ace(self):
        while self.value > 21 and self.aces:
            self.value -= 10
            self.aces -= 1

# 게임 진행
def blackjack_game():
    playing = True

    while True:
        # 게임 초기화
        deck = Deck()
        deck.shuffle()

        player_hand = Hand()
        dealer_hand = Hand()

        player_hand.add_card(deck.deal())
        player_hand.add_card(deck.deal())
        dealer_hand.add_card(deck.deal())
        dealer_hand.add_card(deck.deal())

        # 플레이어 차례
        while playing:
            print("\n플레이어 카드:")
            for card in player_hand.cards:
                print(card)
            print("플레이어 카드 합계:", player_hand.value)

            if player_hand.value == 21:
                print("블랙잭! 플레이어 승리!")
            elif player_hand.value > 21:
                print("카드 합계가 21을 초과했습니다. 딜러 승리!")
            else:
                action = input("카드를 더 받으시겠습니까? (hit/stay): ").lower()
                if action == 'hit':
                    player_hand.add_card(deck.deal())
                elif action == 'stay':
                    playing = False

        # 딜러 차례
        while dealer_hand.value < 17:
            dealer_hand.add_card(deck.deal())

        print("\n딜러 카드:")
        for card in dealer_hand.cards:
            print(card)
        print("딜러 카드 합계:", dealer_hand.value)

        if dealer_hand.value > 21:
            print("딜러가 버스트! 플레이어 승리!")
        elif dealer_hand.value > player_hand.value:
            print("딜러 승리!")
        elif dealer_hand.value < player_hand.value:
            print("플레이어 승리!")
        else:
            print("무승부!")

        # 게임 재시작 여부 확인
        play_again = input("\n게임을 다시 시작하시겠습니까? (yes/no): ").lower()
        if play_again != 'yes':
            break
        else:
            playing = True

# 게임 실행
if __name__ == "__main__":
    blackjack_game()
