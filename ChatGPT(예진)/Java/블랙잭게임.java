import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;

public class 블랙잭게임 {

    // 카드 덱 초기화
    private static final String[] suits = {"스페이드", "다이아몬드", "하트", "클로버"};
    private static final String[] ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    private static final int[] values = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

    // 카드 클래스 정의
    static class Card {
        private final String suit;
        private final String rank;

        public Card(String suit, String rank) {
            this.suit = suit;
            this.rank = rank;
        }

        @Override
        public String toString() {
            return rank + "의 " + suit;
        }
    }

    // 카드 덱 생성
    static class Deck {
        private final List<Card> deck;

        public Deck() {
            this.deck = new ArrayList<>();
            for (String suit : suits) {
                for (String rank : ranks) {
                    this.deck.add(new Card(suit, rank));
                }
            }
        }

        public void shuffle() {
            Collections.shuffle(deck);
        }

        public Card deal() {
            return deck.remove(deck.size() - 1);
        }
    }

    // 핸드 클래스 정의
    static class Hand {
        private final List<Card> cards;
        private int value;
        private int aces;

        public Hand() {
            this.cards = new ArrayList<>();
            this.value = 0;
            this.aces = 0;
        }

        public void addCard(Card card) {
            this.cards.add(card);
            this.value += values[Arrays.asList(ranks).indexOf(card.rank)];
            if (card.rank.equals("A")) {
                this.aces++;
            }
        }

        public void adjustForAce() {
            while (this.value > 21 && this.aces > 0) {
                this.value -= 10;
                this.aces--;
            }
        }
    }

    // 게임 진행
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        while (true) {
            boolean playing = true;

            // 게임 초기화
            Deck deck = new Deck();
            deck.shuffle();

            Hand playerHand = new Hand();
            Hand dealerHand = new Hand();

            playerHand.addCard(deck.deal());
            playerHand.addCard(deck.deal());
            dealerHand.addCard(deck.deal());
            dealerHand.addCard(deck.deal());

            // 플레이어 차례
            while (playing) {
                System.out.println("\n플레이어 카드:");
                for (Card card : playerHand.cards) {
                    System.out.println(card);
                }
                System.out.println("플레이어 카드 합계: " + playerHand.value);

                if (playerHand.value == 21) {
                    System.out.println("블랙잭! 플레이어 승리!");
                    break;
                } else if (playerHand.value > 21) {
                    System.out.println("카드 합계가 21을 초과했습니다. 딜러 승리!");
                    break;
                } else {
                    System.out.print("카드를 더 받으시겠습니까? (hit/stay): ");
                    String action = scanner.nextLine().toLowerCase();
                    if (action.equals("hit")) {
                        playerHand.addCard(deck.deal());
                    } else if (action.equals("stay")) {
                        playing = false;
                    }
                }
            }

            if (!playing) {
                // 딜러 차례
                while (dealerHand.value < 17) {
                    dealerHand.addCard(deck.deal());
                }

                System.out.println("\n딜러 카드:");
                for (Card card : dealerHand.cards) {
                    System.out.println(card);
                }
                System.out.println("딜러 카드 합계: " + dealerHand.value);

                if (dealerHand.value > 21) {
                    System.out.println("딜러가 버스트! 플레이어 승리!");
                } else if (dealerHand.value > playerHand.value) {
                    System.out.println("딜러 승리!");
                } else if (dealerHand.value < playerHand.value) {
                    System.out.println("플레이어 승리!");
                } else {
                    System.out.println("무승부!");
                }
            }

            // 게임 재시작 여부 확인
            System.out.print("\n게임을 다시 시작하시겠습니까? (yes/no): ");
            String playAgain = scanner.nextLine().toLowerCase();
            if (!playAgain.equals("yes")) {
                break;
            }
        }
    }
}
