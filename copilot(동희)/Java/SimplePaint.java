//자바로 간단한 그림판을 만들거야
//그림판은 선을 그리는 기능만 있으면 돼
//선을 그릴 때는 실시간으로 마우스가 움직이는 방향으로 선을 그려야 해
//마우스를 누른 상태로 움직이면 마우스 포인터 위치까지 선을 그어야 해
//마우스를 떼면 그리기를 멈춰야 해
//선의 색깔은 빨간색, 초록색, 파란색 중에 선택할 수 있어야 해
//선의 굵기는 1px, 3px, 5px 중에 선택할 수 있어야 해
//그림판의 배경색은 하얀색으로 해야 해
//그림판의 크기는 800x600 픽셀로 해야 해
//그림판의 이름은 "SimplePaint"로 해야 해
//그림판의 오른쪽 위에는 "선 색깔" 레이블과 빨간색, 초록색, 파란색 중에 선택할 수 있는 콤보 박스가 있어야 해
//그림판의 오른쪽 위에는 "선 굵기" 레이블과 1px, 3px, 5px 중에 선택할 수 있는 콤보 박스가 있어야 해
//그림판의 오른쪽 위에는 "지우개" 체크 박스가 있어야 해
//그림판의 오른쪽 위에는 "지우개 크기" 레이블과 1px, 3px, 5px 중에 선택할 수 있는 콤보 박스가 있어야 해
//그림판의 지우개 기능은 선을 지우는 기능이어야해
//선을 지울 때는 선을 그릴떄와 같은 메커니즘으로 선을 지워야 해
//마우스를 누른 상태로 움직이면 마우스 포인터 위치까지 선을 지워야 해
//마우스를 떼면 지우기를 멈춰야 해
//지우개의 크기는 1px, 3px, 5px 중에 선택할 수 있어야 해
//지우개의 색깔은 하얀색으로 해야 해

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.io.*;

public class SimplePaint extends JFrame {
    private static final long serialVersionUID = 1L;
    private static final String FILE_NAME = "paint.txt";

    private JButton btnRed, btnGreen, btnBlue;
    private JComboBox<String> cbThickness;
    private JCheckBox cbEraser;
    private JComboBox<String> cbEraserSize;
    private JPanel canvas;
    private int x, y;
    private int thickness = 1;
    private boolean isEraser = false;
    private int eraserSize = 1;
    private Color color = Color.BLACK;

    public SimplePaint() {
        setTitle("SimplePaint");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        Container c = getContentPane();
        c.setLayout(new BorderLayout());

        JPanel pControl = new JPanel();
        btnRed = new JButton("Red");
        btnGreen = new JButton("Green");
        btnBlue = new JButton("Blue");
        cbThickness = new JComboBox<>(new String[] { "1px", "3px", "5px" });
        cbEraser = new JCheckBox("Eraser");
        cbEraserSize = new JComboBox<>(new String[] { "1px", "3px", "5px" });
        pControl.add(btnRed);
        pControl.add(btnGreen);
        pControl.add(btnBlue);
        pControl.add(cbThickness);
        pControl.add(cbEraser);
        pControl.add(cbEraserSize);

        canvas = new JPanel() {
            private static final long serialVersionUID = 1L;

            @Override
            protected void paintComponent(Graphics g) {
                super.paintComponent(g);
                g.setColor(color);
                if (isEraser) {
                    g.setColor(Color.WHITE);
                }
                if (thickness == 1) {
                    g.drawLine(x, y, x, y);
                } else if (thickness == 3) {
                    g.drawLine(x - 1, y - 1, x + 1, y + 1);
                    g.drawLine(x - 1, y + 1, x + 1, y - 1);
                    g.drawLine(x - 1, y, x + 1, y);
                    g.drawLine(x, y - 1, x, y + 1);
                } else if (thickness == 5) {
                    g.drawLine(x - 2, y - 2, x + 2, y + 2);
                    g.drawLine(x - 2, y + 2, x + 2, y - 2);
                    g.drawLine(x - 2, y, x + 2, y);
                    g.drawLine(x, y - 2, x, y + 2);
                    g.drawLine(x - 1, y - 1, x + 1, y + 1);
                    g.drawLine(x - 1, y + 1, x + 1, y - 1);
                    g.drawLine(x - 1, y, x + 1, y);
                    g.drawLine(x, y - 1, x, y + 1);
                }
            }
        };
        canvas.setBackground(Color.WHITE);
        canvas.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                x = e.getX();
                y = e.getY();
            }
        });
        canvas.addMouseMotionListener(new MouseMotionAdapter() {
            @Override
            public void mouseDragged(MouseEvent e) {
                Graphics g = canvas.getGraphics();
                g.setColor(color);
                if (isEraser) {
                    g.setColor(Color.WHITE);
                }
                if (thickness == 1) {
                    g.drawLine(x, y, e.getX(), e.getY());
                } else if (thickness == 3) {
                    g.drawLine(x - 1, y - 1, e.getX() - 1, e.getY() - 1);
                    g.drawLine(x - 1, y + 1, e.getX() - 1, e.getY() + 1);
                    g.drawLine(x + 1, y - 1, e.getX() + 1, e.getY() - 1);
                    g.drawLine(x + 1, y + 1, e.getX() + 1, e.getY() + 1);
                    g.drawLine(x - 1, y, e.getX() - 1, e.getY());
                    g.drawLine(x + 1, y, e.getX() + 1, e.getY());
                    g.drawLine(x, y - 1, e.getX(), e.getY() - 1);
                    g.drawLine(x, y + 1, e.getX(), e.getY() + 1);
                } else if (thickness == 5) {
                    g.drawLine(x - 2, y - 2, e.getX() - 2, e.getY() - 2);
                    g.drawLine(x - 2, y + 2, e.getX() - 2, e.getY() + 2);
                    g.drawLine(x + 2, y - 2, e.getX() + 2, e.getY() - 2);
                    g.drawLine(x + 2, y + 2, e.getX() + 2, e.getY() + 2);
                    g.drawLine(x - 2, y, e.getX() - 2, e.getY());
                    g.drawLine(x + 2, y, e.getX() + 2, e.getY());
                    g.drawLine(x, y - 2, e.getX(), e.getY() - 2);
                    g.drawLine(x, y + 2, e.getX(), e.getY() + 2);
                    g.drawLine(x - 1, y - 1, e.getX() - 1, e.getY() - 1);
                    g.drawLine(x - 1, y + 1, e.getX() - 1, e.getY() + 1);
                    g.drawLine(x + 1, y - 1, e.getX() + 1, e.getY() - 1);
                    g.drawLine(x + 1, y + 1, e.getX() + 1, e.getY() + 1);
                    g.drawLine(x - 1, y, e.getX() - 1, e.getY());
                    g.drawLine(x + 1, y, e.getX() + 1, e.getY());
                    g.drawLine(x, y - 1, e.getX(), e.getY() - 1);
                    g.drawLine(x, y + 1, e.getX(), e.getY() + 1);
                }
                x = e.getX();
                y = e.getY();
            }
        });
        
        btnRed.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                color = Color.RED;
            }
        });
        btnGreen.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                color = Color.GREEN;
            }
        });
        btnBlue.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                color = Color.BLUE;
            }
        });
        cbThickness.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JComboBox<?> cb = (JComboBox<?>) e.getSource();
                String item = (String) cb.getSelectedItem();
                if (item.equals("1px")) {
                    thickness = 1;
                } else if (item.equals("3px")) {
                    thickness = 3;
                } else if (item.equals("5px")) {
                    thickness = 5;
                }
            }
        });
        cbEraser.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                isEraser = cbEraser.isSelected();
            }
        });
        cbEraserSize.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JComboBox<?> cb = (JComboBox<?>) e.getSource();
                String item = (String) cb.getSelectedItem();
                if (item.equals("1px")) {
                    eraserSize = 1;
                } else if (item.equals("3px")) {
                    eraserSize = 3;
                } else if (item.equals("5px")) {
                    eraserSize = 5;
                }
            }
        });

        c.add(pControl, BorderLayout.NORTH);
        c.add(canvas, BorderLayout.CENTER);

        setSize(800, 600);
        setVisible(true);
    }

    public static void main(String[] args) {
        new SimplePaint();
    }
}
