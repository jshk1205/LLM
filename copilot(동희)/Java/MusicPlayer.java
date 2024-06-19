//java로 MusciPlayer를 만들어보자
//음악을 재생하고 일시정지하고 정지할 수 있어야해
//음악을 재생할 때는 음악파일을 선택할 수 있어야해
//음악을 재생할 때는 음악파일의 제목을 출력해줘
//음악을 재생할 때는 음악파일의 재생시간을 출력해줘
//음악을 재생할 때는 음악파일의 재생시간을 실시간으로 출력해줘
//음악을 일시정지할 때는 음악파일의 재생시간을 일시정지된 시간으로 출력해줘
//음악을 다음곡, 이전곡으로 넘길 수 있어야해
//음악을 다음곡으로 넘길 때는 다음곡의 제목을 출력해줘
//음악을 이전곡으로 넘길 때는 이전곡의 제목을 출력해줘
//해당 기능들은 시작이 되어야합니다.
//mp3파일을 재생할 수 있는 라이브러리를 사용해야합니다.


//-------------------------------------------------------------

// 수정중인 코드입니다.

//-------------------------------------------------------------
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
import java.util.*;

public class MusicPlayer extends JFrame {
    private static final long serialVersionUID = 1L;
    private static final String FILE_NAME = "music.txt";

    private JButton btnPlay, btnPause, btnStop, btnNext, btnPrev;
    private JLabel lblTitle, lblDuration, lblCurrentTime;
    private JFileChooser fileChooser;
    private File selectedFile;
    private Timer timer;
    private int duration = 0;
    private int currentTime = 0;
    private boolean isPlaying = false;

    public MusicPlayer() {
        setTitle("Music Player");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        Container c = getContentPane();
        c.setLayout(new BorderLayout());

        JPanel pControl = new JPanel();
        btnPlay = new JButton("Play");
        btnPause = new JButton("Pause");
        btnStop = new JButton("Stop");
        btnNext = new JButton("Next");
        btnPrev = new JButton("Previous");
        pControl.add(btnPlay);
        pControl.add(btnPause);
        pControl.add(btnStop);
        pControl.add(btnNext);
        pControl.add(btnPrev);

        JPanel pInfo = new JPanel();
        lblTitle = new JLabel("Title: ");
        lblDuration = new JLabel("Duration: ");
        lblCurrentTime = new JLabel("Current Time: ");
        pInfo.add(lblTitle);
        pInfo.add(lblDuration);
        pInfo.add(lblCurrentTime);

        c.add(pControl, BorderLayout.NORTH);
        c.add(pInfo, BorderLayout.CENTER);

        fileChooser = new JFileChooser();
        fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);

        btnPlay.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                int result = fileChooser.showOpenDialog(MusicPlayer.this);
                if (result == JFileChooser.APPROVE_OPTION) {
                    selectedFile = fileChooser.getSelectedFile();
                    lblTitle.setText("Title: " + selectedFile.getName());
                    duration = getDuration(selectedFile);
                    lblDuration.setText("Duration: " + duration + "s");
                    currentTime = 0;
                    lblCurrentTime.setText("Current Time: " + currentTime + "s");
                    isPlaying = true;
                    timer = new Timer(1000, new ActionListener() {
                        @Override
                        public void actionPerformed(ActionEvent e) {
                            if (isPlaying) {
                                currentTime++;
                                lblCurrentTime.setText("Current Time: " + currentTime + "s");
                                if (currentTime >= duration) {
                                    isPlaying = false;
                                    timer.stop();
                                }
                            }
                        }
                    });
                    timer.start();
                }
            }
        });

        btnPause.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                isPlaying = !isPlaying;
            }
        });

        btnStop.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                isPlaying = false;
                currentTime = 0;
                lblCurrentTime.setText("Current Time: " + currentTime + "s");
                timer.stop();
            }
        });

        btnNext.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (selectedFile != null) {
                    currentTime = 0;
                    lblCurrentTime.setText("Current Time: " + currentTime + "s");
                    isPlaying = true;
                }
            }
        });

        btnPrev.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (selectedFile != null) {
                    currentTime = 0;
                    lblCurrentTime.setText("Current Time: " + currentTime + "s");
                    isPlaying = true;
                }
            }
        });

        setSize(400, 200);
        setVisible(true);
    }

    private int getDuration(File file) {
        try {
            ProcessBuilder pb = new ProcessBuilder("ffprobe", "-i", file.getAbsolutePath(), "-show_entries", "format=duration", "-v", "quiet", "-of", "csv=p=0");
            Process p = pb.start();
            BufferedReader br = new BufferedReader(new InputStreamReader(p.getInputStream()));
            return (int) Double.parseDouble(br.readLine());
        } catch (Exception e) {
            e.printStackTrace();
            return 0;
        }
    }

    public static void main(String[] args) {
        new MusicPlayer();
    }
}


