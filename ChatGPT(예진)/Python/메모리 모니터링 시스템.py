import matplotlib
matplotlib.use('TkAgg')  # 백엔드 설정

import psutil
import time
import matplotlib.pyplot as plt
import warnings

def monitor_memory_with_history(interval=1, history_length=30, threshold=None):
    """
    주어진 간격으로 메모리 사용량을 모니터링하고 이력을 저장합니다.
    :param interval: 모니터링 간격(초)
    :param history_length: 메모리 이력의 길이
    :param threshold: 메모리 사용량 경고 임계값 (%)
    """
    memory_history = []
    timestamps = []

    # 경고 메시지 표시 무시
    warnings.filterwarnings("ignore")

    # 그래프 생성
    fig, ax = plt.subplots()

    plt.ion()  # 대화형 모드 설정

    while True:
        memory_info = psutil.virtual_memory()
        total_memory = memory_info.total
        available_memory = memory_info.available
        used_memory = memory_info.used
        percent_memory_used = memory_info.percent

        memory_history.append(percent_memory_used)
        timestamps.append(time.time())

        # 이력의 길이를 제한하여 너무 많은 데이터가 쌓이지 않도록 함
        if len(memory_history) > history_length:
            memory_history.pop(0)
            timestamps.pop(0)

        # 그래프 업데이트
        ax.clear()
        ax.plot(timestamps, memory_history, marker='o')
        ax.set_xlabel('시간')
        ax.set_ylabel('메모리 사용량 (%)')
        ax.set_title('메모리 사용량 변화')
        plt.draw()
        plt.pause(0.01)

        # 콘솔에 메모리 정보 출력
        print(f"전체 메모리: {total_memory} bytes")
        print(f"사용 가능한 메모리: {available_memory} bytes")
        print(f"사용 중인 메모리: {used_memory} bytes")
        print(f"메모리 사용량 비율: {percent_memory_used}%")

        # 임계값이 설정되어 있고 사용량이 임계값을 초과할 경우 경고 메시지 출력
        if threshold is not None and percent_memory_used > threshold:
            print(f"경고: 메모리 사용량이 {threshold}%를 초과했습니다.")

        # 사용자에게 옵션을 입력받음
        option = input("프로그램을 종료하려면 'q', 계속 이어가려면 'p'를 누르세요: ")
        if option == 'q':
            break
        elif option != 'p':
            print("올바른 옵션을 선택하세요.")

        time.sleep(interval)

if __name__ == "__main__":
    # 임계값을 설정하여 프로그램 실행
    monitor_memory_with_history(threshold=90)
