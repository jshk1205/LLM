package com.example;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;

public class 주식거래시뮬레이션 extends JFrame {
    private Stock[] 주식목록 = {
            new Stock("삼성전자", 50000.0),
            new Stock("LG화학", 30000.0),
            new Stock("현대차", 40000.0)
    };
    private int[] 보유주식수량 = {0, 0, 0}; // 보유한 주식 수량 초기화
    private double 잔고 = 1000000.0; // 초기 잔고 설정
    private double 투자금 = 0.0; // 초기 투자 금액 설정

    private JLabel 잔고라벨;
    private JLabel 투자금액라벨;
    private JPanel 주식패널;
    private JTextArea 로그텍스트영역;

    public 주식거래시뮬레이션() {
        setTitle("고급 주식 거래 시뮬레이션");
        setSize(1200, 400);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        JPanel 상단패널 = new JPanel();
        상단패널.setLayout(new GridLayout(1, 3));

        잔고라벨 = new JLabel("잔고: " + (int) 잔고);
        상단패널.add(잔고라벨);

        투자금액라벨 = new JLabel("투자 금액: " + (int) 투자금);
        상단패널.add(투자금액라벨);

        주식패널 = new JPanel();
        주식패널.setLayout(new GridLayout(주식목록.length, 1));
        주식목록표시();
        상단패널.add(주식패널);

        add(상단패널, BorderLayout.NORTH);

        로그텍스트영역 = new JTextArea(15, 30);
        JScrollPane 스크롤패널 = new JScrollPane(로그텍스트영역);
        add(스크롤패널, BorderLayout.CENTER);

        setVisible(true);

        // 가격 변동 스레드 시작
        Thread 가격변동스레드 = new Thread(new Runnable() {
            public void run() {
                while (true) {
                    try {
                        Thread.sleep(5000); // 5초마다 가격 변동
                        for (Stock 주식 : 주식목록) {
                            double 변동률 = new Random().nextDouble() * 0.1 - 0.05; // -0.05부터 0.05까지의 랜덤 변동률
                            double 새가격 = 주식.get가격() * (1 + 변동률);
                            주식.set가격(새가격);
                        }
                        주식목록표시();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        });
        가격변동스레드.start();
    }

    private void 주식목록표시() {
        투자금 = 0.0; // 투자 금액 초기화
        주식패널.removeAll();
        for (int i = 0; i < 주식목록.length; i++) {
            JPanel 주식행패널 = new JPanel();
            주식행패널.setLayout(new FlowLayout());

            JLabel 이름라벨 = new JLabel(주식목록[i].get이름());
            JLabel 가격라벨 = new JLabel(String.format("%d", (int) 주식목록[i].get가격()));
            double 초기가격 = 주식목록[i].get초기가격();
            double 현재가격 = 주식목록[i].get가격();
            double 변동률 = (현재가격 - 초기가격) / 초기가격 * 100;
            JLabel 변동라벨 = new JLabel(String.format("%+.2f%%", 변동률));
            if (변동률 > 0) {
                변동라벨.setForeground(Color.RED);
            } else if (변동률 < 0) {
                변동라벨.setForeground(Color.BLUE);
            }

            JLabel 보유수량라벨 = new JLabel("보유수량: " + 보유주식수량[i]);

            주식행패널.add(이름라벨);
            주식행패널.add(가격라벨);
            주식행패널.add(변동라벨);
            주식행패널.add(보유수량라벨);

            투자금 += 주식목록[i].get가격() * 보유주식수량[i]; // 각 주식의 현재 투자 금액 계산

            JButton 매수버튼 = new JButton("매수");
            JButton 매도버튼 = new JButton("매도");

            final int 인덱스 = i;
            매수버튼.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    String 수량입력 = JOptionPane.showInputDialog("매수할 수량을 입력하세요:");
                    if (수량입력 != null && !수량입력.isEmpty()) {
                        int 수량 = Integer.parseInt(수량입력);
                        double 가격 = 주식목록[인덱스].get가격();
                        매수(인덱스, 수량, 가격);
                    }
                }
            });

            매도버튼.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    String 수량입력 = JOptionPane.showInputDialog("매도할 수량을 입력하세요:");
                    if (수량입력 != null && !수량입력.isEmpty()) {
                        int 수량 = Integer.parseInt(수량입력);
                        double 가격 = 주식목록[인덱스].get가격();
                        매도(인덱스, 수량, 가격);
                    }
                }
            });

            주식행패널.add(매수버튼);
            주식행패널.add(매도버튼);

            주식패널.add(주식행패널);
        }
        잔고라벨.setText("잔고: " + (int) 잔고);
        투자금액라벨.setText("투자 금액: " + (int) 투자금);
        revalidate();
        repaint();
    }

    private void 매수(int 인덱스, int 수량, double 가격) {
        double 구매비용 = 가격 * 수량;
        if (잔고 >= 구매비용) {
            보유주식수량[인덱스] += 수량;
            잔고 -= 구매비용;
            주식목록표시();
            로그텍스트영역.append(주식목록[인덱스].get이름() + " 주식 " + 수량 + "주를 " + (int) 가격 + "원에 매수하였습니다.\n");
        } else {
            JOptionPane.showMessageDialog(this, "잔고가 부족합니다.");
        }
    }

    private void 매도(int 인덱스, int 수량, double 가격) {
        if (보유주식수량[인덱스] >= 수량) {
            double 판매비용 = 가격 * 수량;
            보유주식수량[인덱스] -= 수량;
            잔고 += 판매비용;
            주식목록표시();
            로그텍스트영역.append(주식목록[인덱스].get이름() + " 주식 " + 수량 + "주를 " + (int) 가격 + "원에 매도하였습니다.\n");
        } else {
            JOptionPane.showMessageDialog(this, "보유한 주식이 부족합니다.");
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new 주식거래시뮬레이션();
            }
        });
    }

    private class Stock {
        private String 이름;
        private double 가격;
        private double 초기가격;

        public Stock(String 이름, double 가격) {
            this.이름 = 이름;
            this.가격 = 가격;
            this.초기가격 = 가격;
        }

        public String get이름() {
            return 이름;
        }

        public double get가격() {
            return 가격;
        }

        public void set가격(double 가격) {
            this.가격 = 가격;
        }

        public double get초기가격() {
            return 초기가격;
        }
    }
}
