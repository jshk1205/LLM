// 수학적 프랙탈(예: Mandelbrot 집합, Julia 집합)을 생성하고 시각화하는 프로그램을 개발합니다.
// 이 프로그램은 사용자가 프랙탈의 종류와 다양한 매개변수를 선택할 수 있어야 합니다.
// 프로그램은 사용자가 선택한 매개변수를 사용하여 프랙탈을 생성하고 화면에 표시해야 합니다.
// 프로그램은 사용자가 프랙탈을 확대하고 이동할 수 있는 기능을 제공해야 합니다.
// 프로그램은 사용자가 프랙탈을 이미지 파일로 저장할 수 있는 기능을 제공해야 합니다.
// 파일명은 fractal_generator.java로 저장합니다.

import java.awt.Color;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class fractal_generator extends JPanel {
    private static final long serialVersionUID = 1L;
    private static final int WIDTH = 800;
    private static final int HEIGHT = 800;
    private static final int MAX_ITER = 1000;
    private static final double ZOOM = 150;
    private static double zx, zy, cX, cY, tmp;

    public static void main(String[] args) {
        JFrame frame = new JFrame("Fractal Generator");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(WIDTH, HEIGHT);
        frame.add(new fractal_generator());
        frame.setVisible(true);
    }

    @Override
    public void paint(Graphics g) {
        BufferedImage image = new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_RGB);
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                zx = zy = 0;
                cX = (x - WIDTH / 2) / ZOOM;
                cY = (y - HEIGHT / 2) / ZOOM;
                int iter = MAX_ITER;
                while (zx * zx + zy * zy < 4 && iter > 0) {
                    tmp = zx * zx - zy * zy + cX;
                    zy = 2.0 * zx * zy + cY;
                    zx = tmp;
                    iter--;
                }
                image.setRGB(x, y, iter | (iter << 8));
            }
        }
        try {
            ImageIO.write(image, "png", new File("fractal.png"));
        } catch (IOException e) {
            e.printStackTrace();
        }
        g.drawImage(image, 0, 0, this);
    }
}


