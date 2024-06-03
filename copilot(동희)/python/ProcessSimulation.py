#프로세스 시뮬레이션이라는 주제로 프로그램을 만들려고함
#프로세스는 프로그램이 실행되는 동안의 상태를 나타내는 것으로, 프로그램이 실행되는 동안에는 프로세스가 생성되고 종료됨
#프로세스는 프로그램이 실행되는 동안에는 프로세스가 생성되고 종료됨
#기능은
#1. 프로세스 생성
#2. 프로세스 종료
#3. 프로세스 상태 확인
#4. 프로세스 목록 확인
#5. 프로세스 실행
#6. 프로세스 종료

import os
import re

def process_simulation():
    processes = []
    while True:
        print("1. 프로세스 생성")
        print("2. 프로세스 종료")
        print("3. 프로세스 상태 확인")
        print("4. 프로세스 목록 확인")
        print("5. 프로세스 실행")
        print("6. 프로세스 종료")
        print("7. 종료")
        menu = input("메뉴를 선택하세요: ")
        if menu == '1':
            process_name = input("프로세스 이름을 입력하세요: ")
            process = {"name": process_name, "status": "created"}
            processes.append(process)
            print("프로세스가 생성되었습니다.")
        elif menu == '2':
            process_name = input("종료할 프로세스 이름을 입력하세요: ")
            for process in processes:
                if process["name"] == process_name:
                    process["status"] = "terminated"
                    print("프로세스가 종료되었습니다.")
                    break
            else:
                print("프로세스를 찾을 수 없습니다.")
        elif menu == '3':
            process_name = input("상태를 확인할 프로세스 이름을 입력하세요: ")
            for process in processes:
                if process["name"] == process_name:
                    print("프로세스 상태:", process["status"])
                    break
            else:
                print("프로세스를 찾을 수 없습니다.")
        elif menu == '4':
            print("프로세스 목록:")
            for process in processes:
                print(process["name"], process["status"])
        elif menu == '5':
            process_name = input("실행할 프로세스 이름을 입력하세요: ")
            for process in processes:
                if process["name"] == process_name:
                    process["status"] = "running"
                    print("프로세스가 실행되었습니다.")
                    break
            else:
                print("프로세스를 찾을 수 없습니다.")
        elif menu == '6':
            process_name = input("종료할 프로세스 이름을 입력하세요: ")
            for process in processes:
                if process["name"] == process_name:
                    process["status"] = "terminated"
                    print("프로세스가 종료되었습니다.")
                    break
            else:
                print("프로세스를 찾을 수 없습니다.")
        elif menu == '7':
            break
        else:
            print("잘못된 메뉴입니다.")

if __name__ == "__main__":
    process_simulation()
    