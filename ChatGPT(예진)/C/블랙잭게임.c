#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ī�� �� �ʱ�ȭ
char* suits[] = { "�����̵�", "���̾Ƹ��", "��Ʈ", "Ŭ�ι�" };
char* ranks[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
int values[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11 };

// ī�� ����ü ����
typedef struct {
    char* suit;
    char* rank;
} Card;

// ī�� �� ����
typedef struct {
    Card deck[52];
    int current_card;
} Deck;

// �ڵ� ����ü ����
typedef struct {
    Card cards[11]; // �ִ� 11����� ī�带 ���� �� �ִٰ� ����
    int num_cards;
    int value;
    int aces;
} Hand;

// ī�� �� �ʱ�ȭ
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

// ī�� ����
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

// ī�� �̱�
Card deal_card(Deck* deck) {
    return deck->deck[deck->current_card++];
}

// �ڵ� �ʱ�ȭ
void initialize_hand(Hand* hand) {
    hand->num_cards = 0;
    hand->value = 0;
    hand->aces = 0;
}

// ī�� �߰�
void add_card_to_hand(Hand* hand, Card card) {
    hand->cards[hand->num_cards++] = card;
    int i;
    for (i = 0; i < 13; i++) {
        if (strcmp(card.rank, ranks[i]) == 0) {
            hand->value += values[i];
            if (strcmp(card.rank, "A") == 0) // ���̽� ó��
                hand->aces++;
            break;
        }
    }
}

// ���̽� ó��
void adjust_for_ace(Hand* hand) {
    while (hand->value > 21 && hand->aces) {
        hand->value -= 10;
        hand->aces--;
    }
}

// �÷��̾� ����
void player_turn(Deck* deck, Hand* player_hand) {
    char action[10];
    while (1) {
        printf("\n�÷��̾� ī��:\n");
        for (int i = 0; i < player_hand->num_cards; i++) {
            printf("%s�� %s\n", player_hand->cards[i].rank, player_hand->cards[i].suit);
        }
        printf("�÷��̾� ī�� �հ�: %d\n", player_hand->value);

        if (player_hand->value == 21) {
            printf("����! �÷��̾� �¸�!\n");
            break;
        }
        else if (player_hand->value > 21) {
            printf("ī�� �հ谡 21�� �ʰ��߽��ϴ�. ���� �¸�!\n");
            break;
        }
        else {
            printf("ī�带 �� �����ðڽ��ϱ�? (hit/stay): ");
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

// ���� ����
void dealer_turn(Deck* deck, Hand* dealer_hand) {
    while (dealer_hand->value < 17) {
        add_card_to_hand(dealer_hand, deal_card(deck));
        adjust_for_ace(dealer_hand);
    }
}

// ���� ����
void blackjack_game() {
    srand(time(NULL));

    while (1) {
        // ���� �ʱ�ȭ
        Deck deck;
        initialize_deck(&deck);
        shuffle_deck(&deck);

        Hand player_hand, dealer_hand;
        initialize_hand(&player_hand); // �÷��̾� �ڵ� �ʱ�ȭ
        initialize_hand(&dealer_hand);

        add_card_to_hand(&player_hand, deal_card(&deck));
        add_card_to_hand(&player_hand, deal_card(&deck));
        add_card_to_hand(&dealer_hand, deal_card(&deck));
        add_card_to_hand(&dealer_hand, deal_card(&deck));

        // �÷��̾� ����
        player_turn(&deck, &player_hand);

        // ���� ����
        if (player_hand.value <= 21) {
            dealer_turn(&deck, &dealer_hand);
        }

        // ��� ���
        printf("\n���� ī��:\n");
        for (int i = 0; i < dealer_hand.num_cards; i++) {
            printf("%s�� %s\n", dealer_hand.cards[i].rank, dealer_hand.cards[i].suit);
        }
        printf("���� ī�� �հ�: %d\n", dealer_hand.value);

        if (dealer_hand.value > 21) {
            printf("������ ����Ʈ! �÷��̾� �¸�!\n");
        }
        else if (dealer_hand.value > player_hand.value) {
            printf("���� �¸�!\n");
        }
        else if (dealer_hand.value < player_hand.value) {
            printf("�÷��̾� �¸�!\n");
        }
        else {
            printf("���º�!\n");
        }

        // ���� ����� ���� Ȯ��
        char play_again[4];
        printf("\n������ �ٽ� �����Ͻðڽ��ϱ�? (yes/no): ");
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
