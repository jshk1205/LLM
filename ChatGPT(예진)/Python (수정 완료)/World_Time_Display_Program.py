import datetime
import pytz
import re
import threading

# 원래 이름은 세계 시간 표시 및 타이머 프로그램인데..
# 타이머가 작동하지 않습니다..

# 지피티가 똑같은 코드(기능 수정 안 된 거)만 계속 반복해서 줍니다..
# 타이머를 실행시키나요? 라는 질문에서 no를 입력한다면 다른 문제는 발생하지 않았습니다..
# 타이머 기능을 빼면 코드가 43줄밖에 안 되어서 클래스는 유지해두었습니다..
class Timer:
    def __init__(self, duration, callback):
        self.duration = duration
        self.callback = callback
        self.timer = None
        self.is_running = False

    def start(self):
        self.is_running = True
        self.timer = threading.Timer(self.duration, self._callback_wrapper)
        self.timer.start()
        print("Timer started.")

    def stop(self):
        if self.timer:
            self.timer.cancel()
            self.is_running = False

    def _callback_wrapper(self):
        self.is_running = False
        self.callback()


def display_world_time():
    print("World Time:")
    for tz in pytz.all_timezones:
        print(f"{tz}: {datetime.datetime.now(pytz.timezone(tz))}")

def display_time_for_timezone(timezone):
    time_now = datetime.datetime.now(pytz.timezone(timezone))
    print(f"Current time in {timezone}: {time_now}")

def parse_time_input(input_str):
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
        print("\n1. Display World Time and Set Timer")
        print("2. Display Current Time for Specific Timezone")
        print("3. Exit")
        choice = input("Choose your action: ")

        if choice == '1':
            display_world_time()
            timer_input = input("Do you want to set a timer? (yes/no): ")
            if timer_input.lower() == 'yes':
                time_input = input("Enter the time (in HH:MM format): ")
                duration = parse_time_input(time_input)
                if duration is not None:
                    timer = Timer(duration, lambda: print("Timer expired!"))
                    timers.append(timer)
                    timer.start()
                else:
                    print("Invalid format. Please try again.")
        elif choice == '2':
            timezone = input("Enter the timezone (e.g., Asia/Seoul, America/New_York): ")
            display_time_for_timezone(timezone)
        elif choice == '3':
            print("Exiting the program.")
            for timer in timers:
                timer.stop()
            break
        else:
            print("Invalid input. Please try again.")

if __name__ == "__main__":
    main()
