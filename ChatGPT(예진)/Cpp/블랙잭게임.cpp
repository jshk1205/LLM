#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

// 카드 덱 초기화
std::array<std::string, 4> suits = { "스페이드", "다이아몬드", "하트", "클로버" };
std::array<std::string, 13> ranks = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
std::array<int, 13> values = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11 };

// 카드 구조체 정의
struct Card {
    std::string suit;
    std::string rank;
};

// 카드 덱 생성
struct Deck {
    std::array<Card, 52> deck;
    int current_card;
};

// 핸드 구조체 정의
struct Hand {
    std::vector<Card> cards; // 최대 11장까지 카드를 가질 수 있다고 가정
    int num_cards;
    int value;
    int aces;
};

// 카드 덱 초기화
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

// 카드 섞기
void shuffle_deck(Deck& deck) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.deck.begin(), deck.deck.end(), g);
}

// 카드 뽑기
Card deal_card(Deck& deck) {
    return deck.deck[deck.current_card++];
}

// 핸드 초기화
void initialize_hand(Hand& hand) {
    hand.num_cards = 0;
    hand.value = 0;
    hand.aces = 0;
}

// 카드 추가
void add_card_to_hand(Hand& hand, Card card) {
    hand.cards.push_back(card);
    auto it = std::find(ranks.begin(), ranks.end(), card.rank);
    if (it != ranks.end()) {
        int index = std::distance(ranks.begin(), it);
        hand.value += values[index];
        if (card.rank == "A") // 에이스 처리
            hand.aces++;
    }
}

// 에이스 처리
void adjust_for_ace(Hand& hand) {
    while (hand.value > 21 && hand.aces) {
        hand.value -= 10;
        hand.aces--;
    }
}

// 플레이어 차례
void player_turn(Deck& deck, Hand& player_hand) {
    std::string action;
    while (true) {
        std::cout << "\n플레이어 카드:\n";
        for (const auto& card : player_hand.cards) {
            std::cout << card.rank << "의 " << card.suit << '\n';
        }
        std::cout << "플레이어 카드 합계: " << player_hand.value << '\n';

        if (player_hand.value == 21) {
            std::cout << "블랙잭! 플레이어 승리!\n";
            break;
        }
        else if (player_hand.value > 21) {
            std::cout << "카드 합계가 21을 초과했습니다. 딜러 승리!\n";
            break;
        }
        else {
            std::cout << "카드를 더 받으시겠습니까? (hit/stay): ";
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

// 딜러 차례
void dealer_turn(Deck& deck, Hand& dealer_hand) {
    while (dealer_hand.value < 17) {
        add_card_to_hand(dealer_hand, deal_card(deck));
        adjust_for_ace(dealer_hand);
    }
}

// 게임 실행
void blackjack_game() {
    std::random_device rd;
    std::mt19937 g(rd());

    while (true) {
        // 게임 초기화
        Deck deck;
        initialize_deck(deck);
        shuffle_deck(deck);

        Hand player_hand, dealer_hand;
        initialize_hand(player_hand); // 플레이어 핸드 초기화
        initialize_hand(dealer_hand);

        add_card_to_hand(player_hand, deal_card(deck));
        add_card_to_hand(player_hand, deal_card(deck));
        add_card_to_hand(dealer_hand, deal_card(deck));
        add_card_to_hand(dealer_hand, deal_card(deck));

        // 플레이어 차례
        player_turn(deck, player_hand);

        // 딜러 차례
        if (player_hand.value <= 21) {
            dealer_turn(deck, dealer_hand);
        }

        // 결과 출력
        std::cout << "\n딜러 카드:\n";
        for (const auto& card : dealer_hand.cards) {
            std::cout << card.rank << "의 " << card.suit << '\n';
        }
        std::cout << "딜러 카드 합계: " << dealer_hand.value << '\n';

        if (dealer_hand.value > 21) {
            std::cout << "딜러가 버스트! 플레이어 승리!\n";
        }
        else if (dealer_hand.value > player_hand.value) {
            std::cout << "딜러 승리!\n";
        }
        else if (dealer_hand.value < player_hand.value) {
            std::cout << "플레이어 승리!\n";
        }
        else {
            std::cout << "무승부!\n";
        }

        // 게임 재시작 여부 확인
        std::string play_again;
        std::cout << "\n게임을 다시 시작하시겠습니까? (yes/no): ";
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
