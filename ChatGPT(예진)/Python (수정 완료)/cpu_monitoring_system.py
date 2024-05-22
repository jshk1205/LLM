import psutil
import threading
import time

# Constants
DEFAULT_THRESHOLD = 90
DEFAULT_DURATION = 10
DEFAULT_MAX_CONTINUOUS_DURATION = 30
DEFAULT_MAX_CONSECUTIVE_THRESHOLD = 95
DEFAULT_INTERVAL = 5

is_monitoring = True

def monitor_keyboard_input():
    global is_monitoring
    while True:
        print("---Press 'p' to stop monitoring---")
        key = input()
        if key == 'p':
            is_monitoring = False
            break
        elif key == 'c':
            print("What action would you like to perform?")
            print("1. Lower process priority")
            print("2. Kill specific process")
            choice = input("Enter your choice (1/2): ")
            if choice == '1':
                process_name = input("Enter the name of the process you want to lower priority for: ")
                try:
                    for proc in psutil.process_iter(['pid', 'name']):
                        if proc.info['name'] == process_name:
                            p = psutil.Process(proc.info['pid'])
                            p.nice(psutil.BELOW_NORMAL_PRIORITY_CLASS)
                            print("Process priority lowered successfully.")
                            break
                except psutil.NoSuchProcess:
                    print("Process not found.")
            elif choice == '2':
                process_name = input("Enter the name of the process you want to kill: ")
                try:
                    for proc in psutil.process_iter(['pid', 'name']):
                        if proc.info['name'] == process_name:
                            p = psutil.Process(proc.info['pid'])
                            p.terminate()
                            print(f"Process '{process_name}' terminated successfully.")
                            break
                except psutil.NoSuchProcess:
                    print("Process not found.")

def monitor_cpu():
    threshold = DEFAULT_THRESHOLD
    consecutive_count = 0
    while is_monitoring:
        cpu_percent = psutil.cpu_percent(interval=DEFAULT_INTERVAL)
        print(f"CPU Usage: {cpu_percent:.2f}%")

        if cpu_percent >= threshold:
            consecutive_count += 1
            if consecutive_count >= DEFAULT_MAX_CONTINUOUS_DURATION / DEFAULT_INTERVAL:
                print("CPU usage exceeded the threshold for too long.")
                print("Taking necessary action...")
                print("The CPU usage has been consistently high. Consider checking for any resource-intensive processes.")
                consecutive_count = 0
        else:
            consecutive_count = 0


def main():
    keyboard_thread = threading.Thread(target=monitor_keyboard_input)
    keyboard_thread.start()

    cpu_thread = threading.Thread(target=monitor_cpu)
    cpu_thread.start()

    keyboard_thread.join()
    cpu_thread.join()

if __name__ == "__main__":
    main()
