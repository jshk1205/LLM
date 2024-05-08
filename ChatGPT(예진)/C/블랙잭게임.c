#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 카드 덱 초기화
char* suits[] = { "스페이드", "다이아몬드", "하트", "클로버" };
char* ranks[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
int values[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11 };

// 카드 구조체 정의
typedef struct {
    char* suit;
    char* rank;
} Card;

// 카드 덱 생성
typedef struct {
    Card deck[52];
    int current_card;
} Deck;

// 핸드 구조체 정의
typedef struct {
    Card cards[11]; // 최대 11장까지 카드를 가질 수 있다고 가정
    int num_cards;
    int value;
    int aces;
} Hand;

// 카드 덱 초기화
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

// 카드 섞기
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

// 카드 뽑기
Card deal_card(Deck* deck) {
    return deck->deck[deck->current_card++];
}

// 핸드 초기화
void initialize_hand(Hand* hand) {
    hand->num_cards = 0;
    hand->value = 0;
    hand->aces = 0;
}

// 카드 추가
void add_card_to_hand(Hand* hand, Card card) {
    hand->cards[hand->num_cards++] = card;
    int i;
    for (i = 0; i < 13; i++) {
        if (strcmp(card.rank, ranks[i]) == 0) {
            hand->value += values[i];
            if (strcmp(card.rank, "A") == 0) // 에이스 처리
                hand->aces++;
            break;
        }
    }
}

// 에이스 처리
void adjust_for_ace(Hand* hand) {
    while (hand->value > 21 && hand->aces) {
        hand->value -= 10;
        hand->aces--;
    }
}

// 플레이어 차례
void player_turn(Deck* deck, Hand* player_hand) {
    char action[10];
    while (1) {
        printf("\n플레이어 카드:\n");
        for (int i = 0; i < player_hand->num_cards; i++) {
            printf("%s의 %s\n", player_hand->cards[i].rank, player_hand->cards[i].suit);
        }
        printf("플레이어 카드 합계: %d\n", player_hand->value);

        if (player_hand->value == 21) {
            printf("블랙잭! 플레이어 승리!\n");
            break;
        }
        else if (player_hand->value > 21) {
            printf("카드 합계가 21을 초과했습니다. 딜러 승리!\n");
            break;
        }
        else {
            printf("카드를 더 받으시겠습니까? (hit/stay): ");
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

// 딜러 차례
void dealer_turn(Deck* deck, Hand* dealer_hand) {
    while (dealer_hand->value < 17) {
        add_card_to_hand(dealer_hand, deal_card(deck));
        adjust_for_ace(dealer_hand);
    }
}

// 게임 실행
void blackjack_game() {
    srand(time(NULL));

    while (1) {
        // 게임 초기화
        Deck deck;
        initialize_deck(&deck);
        shuffle_deck(&deck);

        Hand player_hand, dealer_hand;
        initialize_hand(&player_hand); // 플레이어 핸드 초기화
        initialize_hand(&dealer_hand);

        add_card_to_hand(&player_hand, deal_card(&deck));
        add_card_to_hand(&player_hand, deal_card(&deck));
        add_card_to_hand(&dealer_hand, deal_card(&deck));
        add_card_to_hand(&dealer_hand, deal_card(&deck));

        // 플레이어 차례
        player_turn(&deck, &player_hand);

        // 딜러 차례
        if (player_hand.value <= 21) {
            dealer_turn(&deck, &dealer_hand);
        }

        // 결과 출력
        printf("\n딜러 카드:\n");
        for (int i = 0; i < dealer_hand.num_cards; i++) {
            printf("%s의 %s\n", dealer_hand.cards[i].rank, dealer_hand.cards[i].suit);
        }
        printf("딜러 카드 합계: %d\n", dealer_hand.value);

        if (dealer_hand.value > 21) {
            printf("딜러가 버스트! 플레이어 승리!\n");
        }
        else if (dealer_hand.value > player_hand.value) {
            printf("딜러 승리!\n");
        }
        else if (dealer_hand.value < player_hand.value) {
            printf("플레이어 승리!\n");
        }
        else {
            printf("무승부!\n");
        }

        // 게임 재시작 여부 확인
        char play_again[4];
        printf("\n게임을 다시 시작하시겠습니까? (yes/no): ");
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
