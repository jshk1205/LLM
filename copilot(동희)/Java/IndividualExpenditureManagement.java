//자바로 개인지출관리 프로그램을 만들려고해
//지출내역을 입력받아서 총 지출액을 계산하고, 지출내역을 출력하는 프로그램을 만들어봐
//지출내역은 최대 10개까지 입력받을 수 있어
//지출 내역에는 내용, 카테고리, 날짜, 비용이 포함돼
//카테고리는 음식, 교통, 문화, 기타로 나눠
//입력받은 지출내역을 출력할 때는 내용, 카테고리, 날짜, 비용을 모두 출력해야해
//총 지출액은 모든 지출내역의 비용을 더한 값이야
//지출내역을 모두 출력한 뒤에는 총 지출액을 출력해줘
//지출내역을 입력받을 때는 내용, 카테고리, 날짜, 비용을 입력받고, 지출내역을 출력할 때는 내용, 카테고리, 날짜, 비용, 총 지출액을 출력해야해
//자바의 그래픽기능을 이용해서 사용자가 편히 작성하게 해줘
//해당 개인지출관리 프로그램을 작성후 저장하는 기능이 있어야함
//저장된 파일을 불러와서 지출내역을 출력할 수 있어야함
//저장된 파일을 불러와서 총 지출액을 출력할 수 있어야함
//저장된 파일을 불러와서 지출내역을 수정할 수 있어야함
//저장된 파일을 불러와서 지출내역을 삭제할 수 있어야함
//저장된 파일을 불러와서 총 지출액을 수정할 수 있어야함
//저장된 파일을 불러와서 총 지출액을 삭제할 수 있어야함
//저장된 파일을 불러와서 총 지출액을 추가할 수 있어야함


import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
import java.util.*;

public class IndividualExpenditureManagement extends JFrame {
    private static final long serialVersionUID = 1L;
    private static final int MAX_EXPENDITURE = 10;
    private static final String FILE_NAME = "expenditure.txt";
    private static final String[] CATEGORIES = { "음식", "교통", "문화", "기타" };

    private JTextField tfContent, tfDate, tfCost;
    private JComboBox<String> cbCategory;
    private JTextArea taExpenditure;
    private JButton btnAdd, btnSave, btnLoad, btnModify, btnDelete, btnAddTotal, btnModifyTotal, btnDeleteTotal;
    private int totalCost = 0;
    private ArrayList<Expenditure> expenditures = new ArrayList<>();

    public IndividualExpenditureManagement() {
        setTitle("개인 지출 관리");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        Container c = getContentPane();
        c.setLayout(new BorderLayout());

        JPanel pInput = new JPanel();
        pInput.setLayout(new GridLayout(1, 5));
        tfContent = new JTextField();
        cbCategory = new JComboBox<>(CATEGORIES);
        tfDate = new JTextField();
        tfCost = new JTextField();
        btnAdd = new JButton("추가");
        pInput.add(tfContent);
        pInput.add(cbCategory);
        pInput.add(tfDate);
        pInput.add(tfCost);
        pInput.add(btnAdd);

        taExpenditure = new JTextArea();
        taExpenditure.setEditable(false);

        JPanel pButton = new JPanel();
        pButton.setLayout(new GridLayout(1, 6));
        btnSave = new JButton("저장");
        btnLoad = new JButton("불러오기");
        btnModify = new JButton("수정");
        btnDelete = new JButton("삭제");
        btnAddTotal = new JButton("총 지출액 추가");
        btnModifyTotal = new JButton("총 지출액 수정");
        btnDeleteTotal = new JButton("총 지출액 삭제");
        pButton.add(btnSave);
        pButton.add(btnLoad);
        pButton.add(btnModify);
        pButton.add(btnDelete);
        pButton.add(btnAddTotal);
        pButton.add(btnModifyTotal);
        pButton.add(btnDeleteTotal);

        c.add(pInput, BorderLayout.NORTH);
        c.add(new JScrollPane(taExpenditure), BorderLayout.CENTER);
        c.add(pButton, BorderLayout.SOUTH);

        setSize(800, 400);
        setVisible(true);

        btnAdd.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (expenditures.size() >= MAX_EXPENDITURE) {
                    JOptionPane.showMessageDialog(null, "최대 10개까지 입력할 수 있습니다.");
                    return;
                }
                String content = tfContent.getText();
                String category = (String) cbCategory.getSelectedItem();
                String date = tfDate.getText();
                int cost = Integer.parseInt(tfCost.getText());
                expenditures.add(new Expenditure(content, category, date, cost));
                totalCost += cost;
                updateExpenditure();
            }
        });

        btnSave.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(FILE_NAME))) {
                    oos.writeObject(expenditures);
                    oos.writeInt(totalCost);
                    JOptionPane.showMessageDialog(null, "저장되었습니다.");
                } catch (IOException ex) {
                    ex.printStackTrace();
                }
            }
        });

        btnLoad.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(FILE_NAME))) {
                    expenditures = (ArrayList<Expenditure>) ois.readObject();
                    totalCost = ois.readInt();
                    updateExpenditure();
                    JOptionPane.showMessageDialog(null, "불러왔습니다.");
                } catch (IOException | ClassNotFoundException ex) {
                    ex.printStackTrace();
                }
            }
        });

        btnModify.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                String content = JOptionPane.showInputDialog("수정할 내용을 입력하세요.");
                for (Expenditure expenditure : expenditures) {
                    if (expenditure.getContent().equals(content)) {
                        totalCost -= expenditure.getCost();
                        expenditure.setContent(JOptionPane.showInputDialog("수정할 내용을 입력하세요."));
                        expenditure.setCategory(JOptionPane.showInputDialog("수정할 카테고리를 입력하세요."));
                        expenditure.setDate(JOptionPane.showInputDialog("수정할 날짜를 입력하세요."));
                        expenditure.setCost(Integer.parseInt(JOptionPane.showInputDialog("수정할 비용을 입력하세요.")));
                        totalCost += expenditure.getCost();
                        updateExpenditure();
                        return;
                    }
                }
                JOptionPane.showMessageDialog(null, "해당 내용이 없습니다.");
            }
        });

        btnDelete.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                String content = JOptionPane.showInputDialog("삭제할 내용을 입력하세요.");
                for (Expenditure expenditure : expenditures) {
                    if (expenditure.getContent().equals(content)) {
                        totalCost -= expenditure.getCost();
                        expenditures.remove(expenditure);
                        updateExpenditure();
                        return;
                    }
                }
                JOptionPane.showMessageDialog(null, "해당 내용이 없습니다.");
            }
        });

        btnAddTotal.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                totalCost += Integer.parseInt(JOptionPane.showInputDialog("추가할 총 지출액을 입력하세요."));
                updateExpenditure();
            }
        });

        btnModifyTotal.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                totalCost = Integer.parseInt(JOptionPane.showInputDialog("수정할 총 지출액을 입력하세요."));
                updateExpenditure();
            }
        });

        btnDeleteTotal.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                totalCost = 0;
                updateExpenditure();
            }
        });
    }

    private void updateExpenditure() {
        taExpenditure.setText("");
        for (Expenditure expenditure : expenditures) {
            taExpenditure.append(expenditure.toString() + "\n");
        }
        taExpenditure.append("총 지출액: " + totalCost + "\n");
    }

    class Expenditure implements Serializable {
        private static final long serialVersionUID = 1L;
        private String content;
        private String category;
        private String date;
        private int cost;

        public Expenditure(String content, String category, String date, int cost) {
            this.content = content;
            this.category = category;
            this.date = date;
            this.cost = cost;
        }

        public String getContent() {
            return content;
        }

        public void setContent(String content) {
            this.content = content;
        }

        public String getCategory() {
            return category;
        }

        public void setCategory(String category) {
            this.category = category;
        }

        public String getDate() {
            return date;
        }

        public void setDate(String date) {
            this.date = date;
        }

        public int getCost() {
            return cost;
        }

        public void setCost(int cost) {
            this.cost = cost;
        }

        public String toString() {
            return "내용: " + content + ", 카테고리: " + category + ", 날짜: " + date + ", 비용: " + cost;
        }
    }
    public static void main(String[] args) {
        new IndividualExpenditureManagement();
    }
}

