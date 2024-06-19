#로그파일을 열어서 로그를 분석하는 프로그램을 작성하시오.
#로그파일의 각 줄은 다음과 같이 구성되어 있다.
#로그시간 로그내용
#로그시간은 24시간제 시:분:초 형식이다.
#로그내용은 공백으로 구분된 단어들로 구성되어 있다.
#로그파일을 읽어서 다음과 같은 정보를 출력하시오.

#1.로그의 개수
#2.로그의 종류의 개수
#3.각 로그의 내용과 그 로그의 개수
#4.각 로그의 시간대별 개수
#5.가장 많은 로그를 남긴 시간대와 그 로그의 개수
#6.가장 많은 로그를 남긴 시간대의 로그 내용과 그 개수

#로그파일은 log.txt 파일을 사용하시오.



import os
import re

def log_analysis():
    with open("copilot(동희)/python/로그분석/log.txt", "r", encoding='utf-8') as f:
        logs = f.readlines()
    log_count = len(logs)
    log_types = set()
    log_contents = {}
    log_times = {}
    for log in logs:
        log_time, log_content = log.split(maxsplit=1)
        log_types.add(log_content)
        if log_content in log_contents:
            log_contents[log_content] += 1
        else:
            log_contents[log_content] = 1
        if log_time in log_times:
            log_times[log_time] += 1
        else:
            log_times[log_time] = 1
    max_log_time = max(log_times, key=log_times.get)
    max_log_time_count = log_times[max_log_time]
    max_log_time_contents = {}
    for log in logs:
        log_time, log_content = log.split(maxsplit=1)
    if log_time == max_log_time:
        if log_content in max_log_time_contents:
            max_log_time_contents[log_content] += 1
        else:
            max_log_time_contents[log_content] = 1
    if max_log_time_contents:
        max_log_time_content = max(max_log_time_contents, key=max_log_time_contents.get)
        max_log_time_content_count = max_log_time_contents[max_log_time_content]
    else:
        max_log_time_content = None
        max_log_time_content_count = 0
    print("로그의 개수:", log_count)
    print("로그의 종류의 개수:", len(log_types))
    print("각 로그의 내용과 그 로그의 개수:")
    for log_content, count in log_contents.items():
        print(log_content, count)
    print("각 로그의 시간대별 개수:")
    for log_time, count in log_times.items():
        print(log_time, count)
    print("가장 많은 로그를 남긴 시간대와 그 로그의 개수:", max_log_time, max_log_time_count)
    print("가장 많은 로그를 남긴 시간대의 로그 내용과 그 개수:", max_log_time_content, max_log_time_content_count)

log_analysis()






    
# Path: copilot%28%EB%8F%99%ED%9D%AC%29/python/%EB%A1%9C%EA%B7%B8%EB%B6%84%EC%84%9D.py