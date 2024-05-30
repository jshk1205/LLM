import matplotlib
matplotlib.use('TkAgg')  # Backend setting

import psutil
import time
import matplotlib.pyplot as plt
import warnings

def monitor_memory_with_history(interval=1, history_length=30, threshold=None):
    """
    Monitor memory usage at the given interval and store history.
    :param interval: Monitoring interval (seconds)
    :param history_length: Length of memory history
    :param threshold: Memory usage warning threshold (%)
    """
    memory_history = []
    timestamps = []

    # Ignore warning messages
    warnings.filterwarnings("ignore")

    # Create a graph
    fig, ax = plt.subplots()

    plt.ion()  # Set interactive mode

    while True:
        memory_info = psutil.virtual_memory()
        total_memory = memory_info.total
        used_memory = memory_info.used
        percent_memory_used = memory_info.percent

        memory_history.append(percent_memory_used)
        timestamps.append(time.time())

        # Limit the length of history to prevent too much data accumulation
        if len(memory_history) > history_length:
            memory_history.pop(0)
            timestamps.pop(0)

        # Update the graph
        ax.clear()
        ax.plot(timestamps, memory_history, marker='o')
        ax.set_xlabel('Time')
        ax.set_ylabel('Memory Usage (%)')
        ax.set_title('Memory Usage Change')
        plt.draw()
        plt.pause(0.01)

        # Print memory information to the console
        print(f"Total memory: {total_memory} bytes")
        print(f"Used memory: {used_memory} bytes")
        print(f"Memory usage: {percent_memory_used}%")

        # If a threshold is set and memory usage exceeds the threshold, print a warning message
        if threshold is not None and percent_memory_used > threshold:
            print(f"Warning: Memory usage exceeds {threshold}%.")

        # Prompt the user for options
        option = input("Press 'q' to quit, or 'p' to continue: ")
        if option == 'q':
            break
        elif option != 'p':
            print("Please select a valid option.")

        time.sleep(interval)

if __name__ == "__main__":
    # Run the program with a specified threshold
    monitor_memory_with_history(threshold=90)
