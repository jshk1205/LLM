import threading
import time
from flask import Flask, request, render_template
import pyautogui

app = Flask(__name__)
macro_thread = None  # 매크로 쓰레드를 저장할 변수


def send_keys(text, interval):
    print(f"키 입력 매크로를 시작합니다. {text}를 {interval}초 간격으로 반복 입력합니다.")
    try:
        while True:
            pyautogui.write(text)  # 지정된 텍스트를 입력합니다.
            time.sleep(interval)  # 지정된 간격만큼 대기합니다.
    except KeyboardInterrupt:
        print("\n프로그램을 종료합니다.")


@app.route('/', methods=['GET', 'POST'])
def index():
    global macro_thread  # 전역 변수로 선언

    if request.method == 'POST':
        text = request.form['text']
        interval = float(request.form['interval'])

        # 매크로가 이미 실행 중인지 확인
        if macro_thread and macro_thread.is_alive():
            return "매크로가 이미 실행 중입니다."

        # 쓰레드를 이용하여 매크로 함수를 백그라운드에서 실행합니다.
        macro_thread = threading.Thread(target=send_keys, args=(text, interval))
        macro_thread.start()

        return "매크로가 시작되었습니다. 매크로를 멈추려면 프로그램을 종료시키세요."

    return render_template('index.html')


@app.route('/stop', methods=['GET'])
def stop_macro():
    global macro_thread  # 전역 변수로 선언

    # 매크로가 실행 중인지 확인
    if macro_thread and macro_thread.is_alive():
        macro_thread.join()  # 매크로 쓰레드가 종료될 때까지 대기
        return "매크로가 중지되었습니다."
    else:
        return "매크로가 실행 중이 아닙니다."


if __name__ == "__main__":
    app.run(debug=True)
