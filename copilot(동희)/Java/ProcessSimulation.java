//프로세스 시뮬레이션 프로그램을 Java언어로 만들어줘
// 프로세스의 이름이 나와야됨
// FCFS, SJF, RR, Priority 스케줄링 알고리즘을 구현해줘
// 프로세스의 이름은 P1, P2, P3, P4, P5로 설정
// 프로세스의 정보는 표로 출력해줘
// 사용자가 스케줄링 알고리즘을 선택할 수 있게 해줘 (FCFS, SJF, RR, Priority)
// 사용자가 화면에서 직접 선택할 수 있게 GUI를 사용해 출력해줘
// FCFS, SJF, RR, Priority 스케줄링 알고리즘을 선택하면 해당 알고리즘으로 프로세스를 스케줄링해줘
// 사용자가 프로세스의 도착 시간, 실행 시간을 직접 입력할 수 있게 해줘
// 사용자가 프로세스의 도착 시간, 실행 시간을 입력하면 프로세스의 도착 시간, 실행 시간, 종료 시간, 대기 시간, 반환 시간을 출력해줘
// 자바 swing을 사용해서 화면에 출력해줘
// FCFS, SJF, RR, Priority  스케줄링 알고리즘이 로직이 무조건 구현되어야함
//사용자가 스케줄링 알고리짐을 선택하기 전에 각 프로세스에다가 직접 도착 시간, 실행 시간을 입력할 수 있게 해줘
// 사용자가 스케줄링 알고리즘을 선택하면 프로세스의 도착 시간, 실행 시간, 종료 시간, 대기 시간, 반환 시간을 출력해줘
// RR, priority 스케줄링 알고리즘을 선택하면 quantum, priority를 입력할 수 있게 해줘, 입력하는데 둘이 텍스트가 겹치면 안돼 그리고 오류가 발생하지 않도록 로직을 짜줘

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.util.LinkedList;
import java.util.Queue;

public class ProcessSimulation extends JFrame implements ActionListener {
    private static final long serialVersionUID = 1L;
    private static final String[] COLUMN_NAMES = { "Process Name", "Arrival Time", "Burst Time", "Completion Time",
            "Turnaround Time", "Waiting Time" };
    private static final String[] ALGORITHMS = { "FCFS", "SJF", "RR", "Priority" };
    private static final String[] PROCESSES = { "P1", "P2", "P3", "P4", "P5" };
    private static final int MAX_PROCESSES = 5;
    private static final int MAX_ALGORITHMS = 4;
    private static final int MAX_QUANTUM = 10;
    private static final int MAX_PRIORITY = 10;

    private JComboBox<String> algorithmComboBox;
    private DefaultTableModel tableModel;
    private JTable table;
    private JButton scheduleButton;
    private JTextField[] arrivalTimeFields;
    private JTextField[] burstTimeFields;
    private JTextField quantumField;
    private JTextField priorityField;

    public ProcessSimulation() {
        setTitle("Process Scheduling Simulation");
        setSize(800, 600);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        JPanel topPanel = new JPanel();
        algorithmComboBox = new JComboBox<>(ALGORITHMS);
        algorithmComboBox.addActionListener(this);
        topPanel.add(algorithmComboBox);
        add(topPanel, BorderLayout.NORTH);

        tableModel = new DefaultTableModel(COLUMN_NAMES, 0);
        table = new JTable(tableModel);
        add(new JScrollPane(table), BorderLayout.CENTER);

        JPanel bottomPanel = new JPanel();
        scheduleButton = new JButton("Schedule");
        scheduleButton.addActionListener(this);
        bottomPanel.add(scheduleButton);
        add(bottomPanel, BorderLayout.SOUTH);

        arrivalTimeFields = new JTextField[MAX_PROCESSES];
        burstTimeFields = new JTextField[MAX_PROCESSES];
        // 왼쪽에 패널들을 세로로 나열하기 위한 JPanel 생성
        JPanel leftPanel = new JPanel();
        leftPanel.setLayout(new BoxLayout(leftPanel, BoxLayout.Y_AXIS)); // 세로로 나열 설정

        for (int i = 0; i < MAX_PROCESSES; i++) {
            JPanel processPanel = new JPanel();
            processPanel.add(new JLabel(PROCESSES[i])); // 프로세스 라벨 추가
            arrivalTimeFields[i] = new JTextField(5);
            burstTimeFields[i] = new JTextField(5);
            processPanel.add(new JLabel("Arrival Time:"));
            processPanel.add(arrivalTimeFields[i]);
            processPanel.add(new JLabel("Burst Time:"));
            processPanel.add(burstTimeFields[i]);

            leftPanel.add(processPanel); // 수정된 부분: 각 프로세스 패널을 leftPanel에 추가
        }

        add(leftPanel, BorderLayout.WEST); // leftPanel을 프레임의 왼쪽에 추가

        quantumField = new JTextField(5);
        priorityField = new JTextField(5);
        JPanel inputPanel = new JPanel();
        inputPanel.add(new JLabel("Quantum"));
        inputPanel.add(quantumField);
        inputPanel.add(new JLabel("Priority"));
        inputPanel.add(priorityField);
        add(inputPanel, BorderLayout.EAST);
    }

    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == scheduleButton) {
            String algorithm = (String) algorithmComboBox.getSelectedItem();
            if (algorithm.equals("FCFS")) {
                // FCFS scheduling
                int[] arrivalTimes = new int[MAX_PROCESSES];
                int[] burstTimes = new int[MAX_PROCESSES];
                for (int i = 0; i < MAX_PROCESSES; i++) {
                    arrivalTimes[i] = Integer.parseInt(arrivalTimeFields[i].getText());
                    burstTimes[i] = Integer.parseInt(burstTimeFields[i].getText());
                }

                // Calculate completion time, turnaround time, and waiting time for each process
                int[] completionTimes = new int[MAX_PROCESSES];
                int[] turnaroundTimes = new int[MAX_PROCESSES];
                int[] waitingTimes = new int[MAX_PROCESSES];
                int currentTime = 0;
                for (int i = 0; i < MAX_PROCESSES; i++) {
                    if (currentTime < arrivalTimes[i]) {
                        currentTime = arrivalTimes[i];
                    }
                    completionTimes[i] = currentTime + burstTimes[i];
                    turnaroundTimes[i] = completionTimes[i] - arrivalTimes[i];
                    waitingTimes[i] = turnaroundTimes[i] - burstTimes[i];
                    currentTime = completionTimes[i];
                }

                // Clear the table
                tableModel.setRowCount(0);

                // Populate the table with process information
                for (int i = 0; i < MAX_PROCESSES; i++) {
                    Object[] rowData = { PROCESSES[i], arrivalTimes[i], burstTimes[i], completionTimes[i],
                            turnaroundTimes[i], waitingTimes[i] };
                    tableModel.addRow(rowData);
                }

            } else if (algorithm.equals("SJF")) {
                // SJF scheduling
                int[] arrivalTimes = new int[MAX_PROCESSES];
                int[] burstTimes = new int[MAX_PROCESSES];
                for (int i = 0; i < MAX_PROCESSES; i++) {
                    arrivalTimes[i] = Integer.parseInt(arrivalTimeFields[i].getText());
                    burstTimes[i] = Integer.parseInt(burstTimeFields[i].getText());
                }

                // Sort the processes based on burst time in ascending order
                for (int i = 0; i < MAX_PROCESSES - 1; i++) {
                    for (int j = 0; j < MAX_PROCESSES - i - 1; j++) {
                        if (burstTimes[j] > burstTimes[j + 1]) {
                            int tempBurstTime = burstTimes[j];
                            burstTimes[j] = burstTimes[j + 1];
                            burstTimes[j + 1] = tempBurstTime;

                            int tempArrivalTime = arrivalTimes[j];
                            arrivalTimes[j] = arrivalTimes[j + 1];
                            arrivalTimes[j + 1] = tempArrivalTime;
                        }
                    }
                }

                // Calculate completion time, turnaround time, and waiting time for each process
                int[] completionTimes = new int[MAX_PROCESSES];
                int[] turnaroundTimes = new int[MAX_PROCESSES];
                int[] waitingTimes = new int[MAX_PROCESSES];
                int currentTime = 0;
                for (int i = 0; i < MAX_PROCESSES; i++) {
                    if (currentTime < arrivalTimes[i]) {
                        currentTime = arrivalTimes[i];
                    }
                    completionTimes[i] = currentTime + burstTimes[i];
                    turnaroundTimes[i] = completionTimes[i] - arrivalTimes[i];
                    waitingTimes[i] = turnaroundTimes[i] - burstTimes[i];
                    currentTime = completionTimes[i];
                }

                // Clear the table
                tableModel.setRowCount(0);

                // Populate the table with process information
                for (int i = 0; i < MAX_PROCESSES; i++) {
                    Object[] rowData = { PROCESSES[i], arrivalTimes[i], burstTimes[i], completionTimes[i],
                            turnaroundTimes[i], waitingTimes[i] };
                    tableModel.addRow(rowData);
                }

            } else if (algorithm.equals("RR")) {
                // RR scheduling
                int quantum = Integer.parseInt(quantumField.getText());

                // Create a queue to store the processes
                Queue<Integer> processQueue = new LinkedList<>();

                // Add the processes to the queue
                for (int i = 0; i < MAX_PROCESSES; i++) {
                    int burstTime = Integer.parseInt(burstTimeFields[i].getText());
                    if (burstTime > 0) {
                        processQueue.add(i);
                    }
                }

                // Calculate completion time, turnaround time, and waiting time for each process
                int[] completionTimes = new int[MAX_PROCESSES];
                int[] turnaroundTimes = new int[MAX_PROCESSES];
                int[] waitingTimes = new int[MAX_PROCESSES];
                int currentTime = 0;

                while (!processQueue.isEmpty()) {
                    int processIndex = processQueue.poll();
                    int burstTime = Integer.parseInt(burstTimeFields[processIndex].getText());

                    if (burstTime <= quantum) {
                        // Process completes within the quantum time
                        currentTime += burstTime;
                        completionTimes[processIndex] = currentTime;
                        turnaroundTimes[processIndex] = completionTimes[processIndex];
                        waitingTimes[processIndex] = turnaroundTimes[processIndex]
                                - Integer.parseInt(arrivalTimeFields[processIndex].getText());
                        burstTimeFields[processIndex].setText("0");
                    } else {
                        // Process still has remaining burst time after the quantum time
                        currentTime += quantum;
                        burstTime -= quantum;
                        burstTimeFields[processIndex].setText(Integer.toString(burstTime));
                        processQueue.add(processIndex);
                    }
                }

                // Clear the table
                tableModel.setRowCount(0);

                // Populate the table with process information
                for (int i = 0; i < MAX_PROCESSES; i++) {
                    Object[] rowData = { PROCESSES[i], Integer.parseInt(arrivalTimeFields[i].getText()),
                            Integer.parseInt(burstTimeFields[i].getText()), completionTimes[i], turnaroundTimes[i],
                            waitingTimes[i] };
                    tableModel.addRow(rowData);
                }
            } else if (algorithm.equals("Priority")) {
                // Priority scheduling
                int[] arrivalTimes = new int[MAX_PROCESSES];
                int[] burstTimes = new int[MAX_PROCESSES];
                int[] priorities = new int[MAX_PROCESSES];
                for (int i = 0; i < MAX_PROCESSES; i++) {
                    arrivalTimes[i] = Integer.parseInt(arrivalTimeFields[i].getText());
                    burstTimes[i] = Integer.parseInt(burstTimeFields[i].getText());
                    priorities[i] = Integer.parseInt(priorityField.getText());
                }

                // Sort the processes based on priority in ascending order
                for (int i = 0; i < MAX_PROCESSES - 1; i++) {
                    for (int j = 0; j < MAX_PROCESSES - i - 1; j++) {
                        if (priorities[j] > priorities[j + 1]) {
                            int tempBurstTime = burstTimes[j];
                            burstTimes[j] = burstTimes[j + 1];
                            burstTimes[j + 1] = tempBurstTime;

                            int tempArrivalTime = arrivalTimes[j];
                            arrivalTimes[j] = arrivalTimes[j + 1];
                            arrivalTimes[j + 1] = tempArrivalTime;

                            int tempPriority = priorities[j];
                            priorities[j] = priorities[j + 1];
                            priorities[j + 1] = tempPriority;
                        }
                    }
                }

                // Calculate completion time, turnaround time, and waiting time for each process
                int[] completionTimes = new int[MAX_PROCESSES];
                int[] turnaroundTimes = new int[MAX_PROCESSES];
                int[] waitingTimes = new int[MAX_PROCESSES];
                int currentTime = 0;
                for (int i = 0; i < MAX_PROCESSES; i++) {
                    if (currentTime < arrivalTimes[i]) {
                        currentTime = arrivalTimes[i];
                    }
                    completionTimes[i] = currentTime + burstTimes[i];
                    turnaroundTimes[i] = completionTimes[i] - arrivalTimes[i];
                    waitingTimes[i] = turnaroundTimes[i] - burstTimes[i];
                    currentTime = completionTimes[i];
                }

                // Clear the table
                tableModel.setRowCount(0);

                // Populate the table with process information
                for (int i = 0; i < MAX_PROCESSES; i++) {
                    Object[] rowData = { PROCESSES[i], arrivalTimes[i], burstTimes[i], completionTimes[i],
                            turnaroundTimes[i], waitingTimes[i] };
                    tableModel.addRow(rowData);
                }
            }
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            new ProcessSimulation().setVisible(true);
        });
    }
}
