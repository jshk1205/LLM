import socket
import threading

def receive_message(sock):
    while True:
        try:
            message = sock.recv(1024).decode()
            print(message)
        except:
            print("서버로부터 연결이 끊어졌습니다.")
            sock.close()
            break

def main():
    host = 'localhost'  # 서버의 호스트 주소, 필요에 따라 변경
    port = 12345  # 서버의 포트 번호, 서버 설정과 일치해야 함

    client_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        client_sock.connect((host, port))
    except:
        print("서버에 연결할 수 없습니다.")
        return

    threading.Thread(target=receive_message, args=(client_sock,)).start()

    while True:
        message = input('')
        if message == 'quit':  # quit를 입력하면 연결 종료
            break
        client_sock.send(message.encode())

    client_sock.close()

if __name__ == "__main__":
    main()