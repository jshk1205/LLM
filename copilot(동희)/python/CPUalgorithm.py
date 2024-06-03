#CPU스케줄러알고리즘을 만들려고해요
#비선점, 선점, 우선순위, 라운드로빈, SJF, SRTF, FCFS, HRRN을 만들려고해요
#우선순위는 1이 가장높고 10이 가장낮아요
#라운드로빈은 1초입니다
#프로세스는 최대 10개입니다
#프로세스는 이름, 도착시간, 실행시간, 우선순위를 가지고있어요
#프로세스는 이름은 1글자입니다
#프로세스는 도착시간은 0초이상입니다
#프로세스는 실행시간은 1초이상입니다
#프로세스는 우선순위는 1이상 10이하입니다
#프로세스는 이름은 중복되지않아요
#프로세스는 도착시간은 중복되지않아요
#프로세스는 실행시간은 중복되지않아요
#프로세스는 우선순위는 중복되지않아요
#프로세스는 도착시간이 빠른순으로 정렬되어있어요
#스케줄러알고리즘을 선택해서 알고리즘을 확인하는 방법으로 할래
#스케줄러알고리즘을 선택하면 프로세스들의 이름, 도착시간, 실행시간, 우선순위, 대기시간, 반환시간, Gantt Chart, 평균 대기시간, 평균 반환시간을 출력할래

#1. 비선점
#2. 선점
#3. 우선순위
#4. 라운드로빈
#5. SJF
#6. SRTF
#7. FCFS
#8. HRRN
#9. 종료

import os
import re
from collections import deque
from copy import deepcopy

class Process:
    def __init__(self, name, arrival_time, burst_time, priority):
        self.name = name
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.priority = priority
        self.waiting_time = 0
        self.turnaround_time = 0

    def __str__(self):
        return f"{self.name} {self.arrival_time} {self.burst_time} {self.priority} {self.waiting_time} {self.turnaround_time}"

    def __repr__(self):
        return self.__str__()
    
    def __lt__(self, other):
        return self.arrival_time < other.arrival_time
    
    def __eq__(self, other):
        return self.name == other.name
    
    def __hash__(self):
        return hash(self.name)
    
    def __deepcopy__(self, memo):
        return Process(self.name, self.arrival_time, self.burst_time, self.priority)
    
    def execute(self):
        self.burst_time -= 1

def non_preemptive(processes):
    processes = sorted(processes, key=lambda x: x.arrival_time)
    current_time = 0
    gantt_chart = []
    for process in processes:
        process.waiting_time = current_time - process.arrival_time
        process.turnaround_time = process.waiting_time + process.burst_time
        current_time += process.burst_time
        gantt_chart.append(process.name)
    return processes, gantt_chart

def preemptive(processes):
    processes = sorted(processes, key=lambda x: x.arrival_time)
    current_time = 0
    gantt_chart = []
    while processes:
        available_processes = [process for process in processes if process.arrival_time <= current_time]
        if available_processes:
            available_processes = sorted(available_processes, key=lambda x: x.priority)
            process = available_processes[0]
            process.execute()
            gantt_chart.append(process.name)
            if process.burst_time == 0:
                process.waiting_time = current_time - process.arrival_time
                process.turnaround_time = process.waiting_time + process.burst_time
                processes.remove(process)
            current_time += 1
        else:
            gantt_chart.append(" ")
            current_time += 1
    return processes, gantt_chart

def priority(processes):
    processes = sorted(processes, key=lambda x: x.arrival_time)
    current_time = 0
    gantt_chart = []
    while processes:
        available_processes = [process for process in processes if process.arrival_time <= current_time]
        if available_processes:
            available_processes = sorted(available_processes, key=lambda x: x.priority)
            process = available_processes[0]
            process.execute()
            gantt_chart.append(process.name)
            if process.burst_time == 0:
                process.waiting_time = current_time - process.arrival_time
                process.turnaround_time = process.waiting_time + process.burst_time
                processes.remove(process)
            current_time += 1
        else:
            gantt_chart.append(" ")
            current_time += 1
    return processes, gantt_chart

def round_robin(processes):
    processes = sorted(processes, key=lambda x: x.arrival_time)
    current_time = 0
    gantt_chart = []
    while processes:
        available_processes = [process for process in processes if process.arrival_time <= current_time]
        if available_processes:
            process = available_processes[0]
            process.execute()
            gantt_chart.append(process.name)
            if process.burst_time == 0:
                process.waiting_time = current_time - process.arrival_time
                process.turnaround_time = process.waiting_time + process.burst_time
                processes.remove(process)
            current_time += 1
        else:
            gantt_chart.append(" ")
            current_time += 1
    return processes, gantt_chart

def sjf(processes):
    processes = sorted(processes, key=lambda x: x.arrival_time)
    current_time = 0
    gantt_chart = []
    while processes:
        available_processes = [process for process in processes if process.arrival_time <= current_time]
        if available_processes:
            available_processes = sorted(available_processes, key=lambda x: x.burst_time)
            process = available_processes[0]
            process.execute()
            gantt_chart.append(process.name)
            if process.burst_time == 0:
                process.waiting_time = current_time - process.arrival_time
                process.turnaround_time = process.waiting_time + process.burst_time
                processes.remove(process)
            current_time += 1
        else:
            gantt_chart.append(" ")
            current_time += 1
    return processes, gantt_chart

def srtf(processes):
    processes = sorted(processes, key=lambda x: x.arrival_time)
    current_time = 0
    gantt_chart = []
    while processes:
        available_processes = [process for process in processes if process.arrival_time <= current_time]
        if available_processes:
            available_processes = sorted(available_processes, key=lambda x: x.burst_time)
            process = available_processes[0]
            process.execute()
            gantt_chart.append(process.name)
            if process.burst_time == 0:
                process.waiting_time = current_time - process.arrival_time
                process.turnaround_time = process.waiting_time + process.burst_time
                processes.remove(process)
            current_time += 1
        else:
            gantt_chart.append(" ")
            current_time += 1
    return processes, gantt_chart

def fcfs(processes):
    processes = sorted(processes, key=lambda x: x.arrival_time)
    current_time = 0
    gantt_chart = []
    for process in processes:
        process.waiting_time = current_time - process.arrival_time
        process.turnaround_time = process.waiting_time + process.burst_time
        current_time += process.burst_time
        gantt_chart.append(process.name)
    return processes, gantt_chart

def hrrn(processes):
    processes = sorted(processes, key=lambda x: x.arrival_time)
    current_time = 0
    gantt_chart = []
    while processes:
        available_processes = [process for process in processes if process.arrival_time <= current_time]
        if available_processes:
            available_processes = sorted(available_processes, key=lambda x: x.priority)
            process = available_processes[0]
            process.execute()
            gantt_chart.append(process.name)
            if process.burst_time == 0:
                process.waiting_time = current_time - process.arrival_time
                process.turnaround_time = process.waiting_time + process.burst_time
                processes.remove(process)
            current_time += 1
        else:
            gantt_chart.append(" ")
            current_time += 1
    return processes, gantt_chart

def main():
    processes = set()
    while True:
        print("1. 프로세스 추가")
        print("2. 스케줄러 알고리즘 선택")
        print("3. 종료")
        choice = input("선택: ")
        if choice == "1":
            name = input("프로세스 이름: ")
            arrival_time = int(input("도착 시간: "))
            burst_time = int(input("실행 시간: "))
            priority = int(input("우선순위: "))
            process = Process(name, arrival_time, burst_time, priority)
            processes.add(process)
        elif choice == "2":
            print("1. 비선점")
            print("2. 선점")
            print("3. 우선순위")
            print("4. 라운드로빈")
            print("5. SJF")
            print("6. SRTF")
            print("7. FCFS")
            print("8. HRRN")
            print("9. 종료")
            algorithm = input("선택: ")
            if algorithm == "1":
                new_processes, gantt_chart = non_preemptive(deepcopy(processes))
                for process in new_processes:
                    print(process)
                print("Gantt Chart:", gantt_chart)
            elif algorithm == "2":
                new_processes, gantt_chart = preemptive(deepcopy(processes))
                for process in new_processes:
                    print(process)
                print("Gantt Chart:", gantt_chart)
            elif algorithm == "3":
                new_processes, gantt_chart = priority(deepcopy(processes))
                for process in new_processes:
                    print(process)
                print("Gantt Chart:", gantt_chart)
            elif algorithm == "4":
                new_processes, gantt_chart = round_robin(deepcopy(processes))
                for process in new_processes:
                    print(process)
                print("Gantt Chart:", gantt_chart)
            elif algorithm == "5":
                new_processes, gantt_chart = sjf(deepcopy(processes))
                for process in new_processes:
                    print(process)
                print("Gantt Chart:", gantt_chart)
            elif algorithm == "6":
                new_processes, gantt_chart = srtf(deepcopy(processes))
                for process in new_processes:
                    print(process)
                print("Gantt Chart:", gantt_chart)
            elif algorithm == "7":
                new_processes, gantt_chart = fcfs(deepcopy(processes))
                for process in new_processes:
                    print(process)
                print("Gantt Chart:", gantt_chart)
            elif algorithm == "8":
                new_processes, gantt_chart = hrrn(deepcopy(processes))
                for process in new_processes:
                    print(process)
                print("Gantt Chart:", gantt_chart)
            elif algorithm == "9":
                break
            else:
                print("올바른 값을 입력하세요.")
        elif choice == "3":
            break
        else:
            print("올바른 값을 입력하세요.")

if __name__ == "__main__":
    main()

