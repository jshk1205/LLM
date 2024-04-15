import time
import datetime
import pytz
import threading
import re

class Timer:
    def __init__(self, duration, callback):
        self.duration = duration
        self.callback = callback
        self.thread = None
        self.is_running = False

    def start(self):
        self.is_running = True
        self.thread = threading.Thread(target=self._timer_thread)
        self.thread.start()

    def stop(self):
        self.is_running = False
        if self.thread:
            self.thread.join()

    def _timer_thread(self):
        time.sleep(self.duration)
        if self.callback:
            self.callback()

def display_world_time():
    print("세계 시간:")
    for tz in pytz.all_timezones:
        print(f"{tz}: {datetime.datetime.now(pytz.timezone(tz))}")

def display_time_for_timezone(timezone):
    time_now = datetime.datetime.now(pytz.timezone(timezone))
    print(f"현재 {timezone} 시간: {time_now}")

def parse_time_input(input_str):
    # 정규표현식을 사용하여 입력 문자열에서 시간과 분 추출
    match = re.match(r'(\d+):(\d+)', input_str)
    if match:
        hours = int(match.group(1))
        minutes = int(match.group(2))
        return hours * 3600 + minutes * 60
    else:
        return None

def main():
    timers = []

    while True:
        print("\n1. 세계 시간 표시 및 타이머 설정")
        print("2. 특정 시간대의 현재 시간 표시")
        print("3. 종료")
        choice = input("원하는 작업을 선택하세요: ")

        if choice == '1':
            display_world_time()
            timer_input = input("타이머를 설정하시겠습니까? (예/아니오): ")
            if timer_input.lower() == '예':
                time_input = input("시간을 입력하세요 (HH:MM 형식): ")
                duration = parse_time_input(time_input)
                if duration is not None:
                    timer = Timer(duration, lambda: print("타이머 종료!"))
                    timers.append(timer)
                    timer.start()
                else:
                    print("잘못된 형식입니다. 다시 시도하세요.")
        elif choice == '2':
            timezone = input("시간을 확인할 시간대를 입력하세요: ")
            display_time_for_timezone(timezone)
        elif choice == '3':
            print("프로그램을 종료합니다.")
            for timer in timers:
                timer.stop()
            break
        else:
            print("잘못된 입력입니다. 다시 시도하세요.")

if __name__ == "__main__":
    main()
