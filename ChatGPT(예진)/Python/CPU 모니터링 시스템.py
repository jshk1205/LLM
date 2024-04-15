import psutil
import time
import logging
import threading

def monitor_cpu(threshold=90, duration=10, max_continuous_duration=30, max_consecutive_threshold=95, interval=5, process_name=None):
    logging.basicConfig(filename='cpu_monitor.log', level=logging.INFO, format='%(asctime)s - %(message)s')
    consecutive_high_usage = 0
    continuous_high_usage = 0
    is_monitoring = True

    def monitor_keyboard_input():
        nonlocal is_monitoring
        while True:
            key = input("---Press 'p' to stop monitoring--- \n")
            if key == 'p':
                is_monitoring = False
                break
            elif key == 'c':
                print("What action would you like to perform?")
                print("1. Lower process priority")
                print("2. Kill specific process")
                choice = input("Enter your choice (1/2): ")
                if choice == '1':
                    if process_name:
                        lower_process_priority(process_name)
                    else:
                        print("No process name provided.")
                elif choice == '2':
                    process_to_kill = input("Enter the name of the process you want to kill: ")
                    kill_process(process_to_kill)

    def lower_process_priority(process_name):
        try:
            for proc in psutil.process_iter():
                if proc.name() == process_name:
                    proc.nice(psutil.BELOW_NORMAL_PRIORITY_CLASS)  # 프로세스 우선순위를 낮춤
                    print(f"Priority of process {process_name} has been lowered.")
                    logging.info(f"Priority of process {process_name} has been lowered.")
                    break
        except psutil.NoSuchProcess:
            print(f"Process {process_name} not found.")
            logging.warning(f"Process {process_name} not found.")

    def kill_process(process_name):
        try:
            for proc in psutil.process_iter():
                if proc.name() == process_name:
                    proc.kill()  # 프로세스 종료
                    print(f"Process {process_name} has been terminated.")
                    logging.info(f"Process {process_name} has been terminated.")
                    break
        except psutil.NoSuchProcess:
            print(f"Process {process_name} not found.")
            logging.warning(f"Process {process_name} not found.")

    keyboard_thread = threading.Thread(target=monitor_keyboard_input)
    keyboard_thread.start()

    while is_monitoring:
        cpu_percent = psutil.cpu_percent(interval=1)
        print(f"CPU Usage: {cpu_percent:.2f}%")

        logging.info(f"CPU Usage: {cpu_percent:.2f}%")

        if cpu_percent > threshold:
            consecutive_high_usage += 1
            continuous_high_usage += 1

            if consecutive_high_usage >= duration:
                print(f"WARNING: CPU usage has been above {threshold}% for {duration} seconds!")
                logging.warning(f"WARNING: CPU usage has been above {threshold}% for {duration} seconds!")
                # 여기서 다른 동작을 수행할 수 있습니다.
        else:
            consecutive_high_usage = 0

        if cpu_percent > max_consecutive_threshold:
            continuous_high_usage += 1

            if continuous_high_usage >= max_continuous_duration:
                print(f"ALERT: CPU usage spiked above {max_consecutive_threshold}% for {max_continuous_duration} seconds!")
                logging.warning(f"ALERT: CPU usage spiked above {max_consecutive_threshold}% for {max_continuous_duration} seconds!")
                # 여기서 추가적인 조치를 취할 수 있습니다.

        time.sleep(interval)

    keyboard_thread.join()

if __name__ == "__main__":
    monitor_cpu(process_name="notepad.exe")
