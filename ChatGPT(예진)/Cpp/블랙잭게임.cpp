#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

// ī�� �� �ʱ�ȭ
std::array<std::string, 4> suits = { "�����̵�", "���̾Ƹ��", "��Ʈ", "Ŭ�ι�" };
std::array<std::string, 13> ranks = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
std::array<int, 13> values = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11 };

// ī�� ����ü ����
struct Card {
    std::string suit;
    std::string rank;
};

// ī�� �� ����
struct Deck {
    std::array<Card, 52> deck;
    int current_card;
};

// �ڵ� ����ü ����
struct Hand {
    std::vector<Card> cards; // �ִ� 11����� ī�带 ���� �� �ִٰ� ����
    int num_cards;
    int value;
    int aces;
};

// ī�� �� �ʱ�ȭ
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

// ī�� ����
void shuffle_deck(Deck& deck) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.deck.begin(), deck.deck.end(), g);
}

// ī�� �̱�
Card deal_card(Deck& deck) {
    return deck.deck[deck.current_card++];
}

// �ڵ� �ʱ�ȭ
void initialize_hand(Hand& hand) {
    hand.num_cards = 0;
    hand.value = 0;
    hand.aces = 0;
}

// ī�� �߰�
void add_card_to_hand(Hand& hand, Card card) {
    hand.cards.push_back(card);
    auto it = std::find(ranks.begin(), ranks.end(), card.rank);
    if (it != ranks.end()) {
        int index = std::distance(ranks.begin(), it);
        hand.value += values[index];
        if (card.rank == "A") // ���̽� ó��
            hand.aces++;
    }
}

// ���̽� ó��
void adjust_for_ace(Hand& hand) {
    while (hand.value > 21 && hand.aces) {
        hand.value -= 10;
        hand.aces--;
    }
}

// �÷��̾� ����
void player_turn(Deck& deck, Hand& player_hand) {
    std::string action;
    while (true) {
        std::cout << "\n�÷��̾� ī��:\n";
        for (const auto& card : player_hand.cards) {
            std::cout << card.rank << "�� " << card.suit << '\n';
        }
        std::cout << "�÷��̾� ī�� �հ�: " << player_hand.value << '\n';

        if (player_hand.value == 21) {
            std::cout << "����! �÷��̾� �¸�!\n";
            break;
        }
        else if (player_hand.value > 21) {
            std::cout << "ī�� �հ谡 21�� �ʰ��߽��ϴ�. ���� �¸�!\n";
            break;
        }
        else {
            std::cout << "ī�带 �� �����ðڽ��ϱ�? (hit/stay): ";
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

// ���� ����
void dealer_turn(Deck& deck, Hand& dealer_hand) {
    while (dealer_hand.value < 17) {
        add_card_to_hand(dealer_hand, deal_card(deck));
        adjust_for_ace(dealer_hand);
    }
}

// ���� ����
void blackjack_game() {
    std::random_device rd;
    std::mt19937 g(rd());

    while (true) {
        // ���� �ʱ�ȭ
        Deck deck;
        initialize_deck(deck);
        shuffle_deck(deck);

        Hand player_hand, dealer_hand;
        initialize_hand(player_hand); // �÷��̾� �ڵ� �ʱ�ȭ
        initialize_hand(dealer_hand);

        add_card_to_hand(player_hand, deal_card(deck));
        add_card_to_hand(player_hand, deal_card(deck));
        add_card_to_hand(dealer_hand, deal_card(deck));
        add_card_to_hand(dealer_hand, deal_card(deck));

        // �÷��̾� ����
        player_turn(deck, player_hand);

        // ���� ����
        if (player_hand.value <= 21) {
            dealer_turn(deck, dealer_hand);
        }

        // ��� ���
        std::cout << "\n���� ī��:\n";
        for (const auto& card : dealer_hand.cards) {
            std::cout << card.rank << "�� " << card.suit << '\n';
        }
        std::cout << "���� ī�� �հ�: " << dealer_hand.value << '\n';

        if (dealer_hand.value > 21) {
            std::cout << "������ ����Ʈ! �÷��̾� �¸�!\n";
        }
        else if (dealer_hand.value > player_hand.value) {
            std::cout << "���� �¸�!\n";
        }
        else if (dealer_hand.value < player_hand.value) {
            std::cout << "�÷��̾� �¸�!\n";
        }
        else {
            std::cout << "���º�!\n";
        }

        // ���� ����� ���� Ȯ��
        std::string play_again;
        std::cout << "\n������ �ٽ� �����Ͻðڽ��ϱ�? (yes/no): ";
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
