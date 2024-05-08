import subprocess
import re
import statistics
import time
import socket
import matplotlib.pyplot as plt


def get_host_by_ip(ip):
    """
    Get the host name corresponding to the given IP address.
    """
    try:
        host = socket.gethostbyaddr(ip)[0]
        return host
    except socket.herror:
        return None


def get_ip_by_host(host):
    """
    Get the IP address corresponding to the given host name.
    """
    try:
        ip = socket.gethostbyname(host)
        return ip
    except socket.gaierror:
        return None


def ping(host, count=4, interval=1):
    """
    Ping the specified host multiple times with a specified interval between pings.
    Return average response time and packet loss rate.
    """
    command = ['ping', '-c', str(count), '-i', str(interval), host]
    response_times = []

    try:
        for _ in range(count):
            result = subprocess.run(command, capture_output=True, timeout=10, text=True)
            times = re.findall(r"time=([0-9.]+)", result.stdout)
            if times:
                response_times.extend([float(t) for t in times])
            else:
                response_times.append(0)
            time.sleep(interval)

        avg_time = statistics.mean(response_times)
        packet_loss_rate = (count - len(response_times)) / count * 100

        return avg_time, packet_loss_rate, response_times
    except (subprocess.TimeoutExpired, ValueError):
        return None, None, None


def traceroute(host):
    """
    Perform traceroute to the specified host and return the list of hops.
    """
    command = ['traceroute', '-q', '1', host]

    try:
        result = subprocess.run(command, capture_output=True, timeout=20, text=True)
        hops = result.stdout.splitlines()
        return hops
    except subprocess.TimeoutExpired:
        return None


def save_to_file(data, filename):
    """
    Save data to a file.
    """
    with open(filename, 'w') as file:
        file.write(data)


# 사용자로부터 호스트나 IP 주소를 입력 받습니다.
target = input("Enter a host name or IP address: ")

# 입력된 값이 IP 주소인지 호스트 이름인지 확인하고 해당 정보를 출력합니다.
if re.match(r"\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}", target):
    host = get_host_by_ip(target)
    if host:
        print(f"Host name for {target}: {host}")
    else:
        print(f"Failed to resolve host name for {target}")
else:
    ip = get_ip_by_host(target)
    if ip:
        print(f"IP address for {target}: {ip}")
    else:
        print(f"Failed to resolve IP address for {target}")

# Ping 및 Traceroute 실행
count = 10
interval = 1

avg_response_time, packet_loss_rate, response_times = ping(target, count, interval)
if avg_response_time is not None and packet_loss_rate is not None:
    print(f"Avg response time: {avg_response_time} ms")
    print(f"Packet loss rate: {packet_loss_rate}%")

    ping_data = f"Avg response time: {avg_response_time} ms\nPacket loss rate: {packet_loss_rate}%"
    save_to_file(ping_data, 'ping_result.txt')

    plt.plot(range(1, len(response_times) + 1), response_times, marker='o')
    plt.xlabel('Ping Count')
    plt.ylabel('Response Time (ms)')
    plt.title('Response Time Over Time')
    plt.grid(True)
    plt.savefig('ping_plot.png')
    plt.show()
else:
    print(f"Failed to ping {target}")

hops = traceroute(target)
if hops is not None:
    print("Traceroute Results:")
    for hop in hops:
        print(hop)

    traceroute_data = "\n".join(hops)
    save_to_file(traceroute_data, 'traceroute_result.txt')
else:
    print(f"Failed to traceroute to {target}")
