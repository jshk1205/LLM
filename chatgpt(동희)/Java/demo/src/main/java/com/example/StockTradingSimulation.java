package com.example;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;

public class StockTradingSimulation extends JFrame {
    private Stock[] stockList = {
            new Stock("Samsung Electronics", 50000.0),
            new Stock("LG Chem", 30000.0),
            new Stock("Hyundai Motor", 40000.0)
    };
    private int[] holdingStockQuantity = {0, 0, 0}; // Initialize the quantity of held stocks
    private double balance = 1000000.0; // Set initial balance
    private double investmentAmount = 0.0; // Set initial investment amount

    private JLabel balanceLabel;
    private JLabel investmentAmountLabel;
    private JPanel stockPanel;
    private JTextArea logTextArea;

    public StockTradingSimulation() {
        setTitle("Advanced Stock Trading Simulation");
        setSize(1200, 400);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        JPanel topPanel = new JPanel();
        topPanel.setLayout(new GridLayout(1, 3));

        balanceLabel = new JLabel("Balance: " + (int) balance);
        topPanel.add(balanceLabel);

        investmentAmountLabel = new JLabel("Investment Amount: " + (int) investmentAmount);
        topPanel.add(investmentAmountLabel);

        stockPanel = new JPanel();
        stockPanel.setLayout(new GridLayout(stockList.length, 1));
        displayStocks();
        topPanel.add(stockPanel);

        add(topPanel, BorderLayout.NORTH);

        logTextArea = new JTextArea(15, 30);
        JScrollPane scrollPane = new JScrollPane(logTextArea);
        add(scrollPane, BorderLayout.CENTER);

        setVisible(true);

        // Start price fluctuation thread
        Thread priceFluctuationThread = new Thread(new Runnable() {
            public void run() {
                while (true) {
                    try {
                        Thread.sleep(5000); // Price fluctuation every 5 seconds
                        for (Stock stock : stockList) {
                            double fluctuationRate = new Random().nextDouble() * 0.1 - 0.05; // Random fluctuation rate from -0.05 to 0.05
                            double newPrice = stock.getPrice() * (1 + fluctuationRate);
                            stock.setPrice(newPrice);
                        }
                        displayStocks();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        });
        priceFluctuationThread.start();
    }

    private void displayStocks() {
        investmentAmount = 0.0; // Initialize investment amount
        stockPanel.removeAll();
        for (int i = 0; i < stockList.length; i++) {
            JPanel stockRowPanel = new JPanel();
            stockRowPanel.setLayout(new FlowLayout());

            JLabel nameLabel = new JLabel(stockList[i].getName());
            JLabel priceLabel = new JLabel(String.format("%d", (int) stockList[i].getPrice()));
            double initialPrice = stockList[i].getInitialPrice();
            double currentPrice = stockList[i].getPrice();
            double fluctuationRate = (currentPrice - initialPrice) / initialPrice * 100;
            JLabel fluctuationLabel = new JLabel(String.format("%+.2f%%", fluctuationRate));
            if (fluctuationRate > 0) {
                fluctuationLabel.setForeground(Color.RED);
            } else if (fluctuationRate < 0) {
                fluctuationLabel.setForeground(Color.BLUE);
            }

            JLabel holdingQuantityLabel = new JLabel("Holding Quantity: " + holdingStockQuantity[i]);

            stockRowPanel.add(nameLabel);
            stockRowPanel.add(priceLabel);
            stockRowPanel.add(fluctuationLabel);
            stockRowPanel.add(holdingQuantityLabel);

            investmentAmount += stockList[i].getPrice() * holdingStockQuantity[i]; // Calculate current investment amount for each stock

            JButton buyButton = new JButton("Buy");
            JButton sellButton = new JButton("Sell");

            final int index = i;
            buyButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    String quantityInput = JOptionPane.showInputDialog("Enter the quantity to buy:");
                    if (quantityInput != null && !quantityInput.isEmpty()) {
                        int quantity = Integer.parseInt(quantityInput);
                        double price = stockList[index].getPrice();
                        buy(index, quantity, price);
                    }
                }
            });

            sellButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    String quantityInput = JOptionPane.showInputDialog("Enter the quantity to sell:");
                    if (quantityInput != null && !quantityInput.isEmpty()) {
                        int quantity = Integer.parseInt(quantityInput);
                        double price = stockList[index].getPrice();
                        sell(index, quantity, price);
                    }
                }
            });

            stockRowPanel.add(buyButton);
            stockRowPanel.add(sellButton);

            stockPanel.add(stockRowPanel);
        }
        balanceLabel.setText("Balance: " + (int) balance);
        investmentAmountLabel.setText("Investment Amount: " + (int) investmentAmount);
        revalidate();
        repaint();
    }

    private void buy(int index, int quantity, double price) {
        double purchaseCost = price * quantity;
        if (balance >= purchaseCost) {
            holdingStockQuantity[index] += quantity;
            balance -= purchaseCost;
            displayStocks();
            logTextArea.append(stockList[index].getName() + " stocks purchased " + quantity + " shares at " + (int) price + " won.\n");
        } else {
            JOptionPane.showMessageDialog(this, "Insufficient balance.");
        }
    }

    private void sell(int index, int quantity, double price) {
        if (holdingStockQuantity[index] >= quantity) {
            double saleProceeds = price * quantity;
            holdingStockQuantity[index] -= quantity;
            balance += saleProceeds;
            displayStocks();
            logTextArea.append(stockList[index].getName() + " stocks sold " + quantity + " shares at " + (int) price + " won.\n");
        } else {
            JOptionPane.showMessageDialog(this, "Insufficient stocks.");
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new StockTradingSimulation();
            }
        });
    }

    private class Stock {
        private String name;
        private double price;
        private double initialPrice;

        public Stock(String name, double price) {
            this.name = name;
            this.price = price;
            this.initialPrice = price;
        }

        public String getName() {
            return name;
        }

        public double getPrice() {
            return price;
        }

        public void setPrice(double price) {
            this.price = price;
        }

        public double getInitialPrice() {
            return initialPrice;
        }
    }
}
