#실시간 채팅 애플리케이션을 만들어줘
#이 코드에는 서버에 대한 코드를 작성해야함
#서버는 클라이언트의 요청을 받아서 다른 클라이언트에게 전달해주는 역할을 함
#서버에서 채팅을 치면 클라이언트는 서버 : 내용 이런식으로 출력되어야함
#클라이언트에서 채팅을 치면 서버는 클라이언트 : 내용 이런식으로 출력되어야함
#서버는 클라이언트가 접속할 때까지 기다려야함
#서버는 클라이언트가 접속하면 클라이언트와 연결되었다고 출력해야함
#서버는 클라이언트가 보낸 메시지를 다른 클라이언트에게 전달해야함
#서버는 클라이언트가 접속을 끊으면 클라이언트와 연결이 끊어졌다고 출력해야함
#서버는 클라이언트가 접속을 끊으면 다른 클라이언트에게 알려줘야함
#서버는 클라이언트가 접속을 끊으면 다른 클라이언트에게 메시지를 전달하지 않아야함

import socket
import threading

def handle_client(client_socket, addr):
    print(f'클라이언트와 연결되었습니다. {addr}')
    while True:
        data = client_socket.recv(1024)
        if not data:
            print(f'클라이언트와 연결이 끊어졌습니다. {addr}')
            break
        message = f'클라이언트 : {data.decode()}'
        print(message)
        broadcast_message(message.encode(), client_socket)
    client_socket.close()
    clients.remove(client_socket)

def broadcast_message(message, sender_socket):
    for client in clients:
        if client != sender_socket:  # 메시지를 보낸 클라이언트 제외
            client.send(message)

def accept_connections():
    while True:
        client_socket, addr = server.accept()
        clients.append(client_socket)
        threading.Thread(target=handle_client, args=(client_socket, addr)).start()

def server_send_message():
    while True:
        message = input("")
        if message:
            formatted_message = f"서버: {message}"
            broadcast_message(formatted_message.encode(), None)

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(('', 12345))
server.listen()

clients = []

# 클라이언트 연결을 처리하는 스레드
threading.Thread(target=accept_connections).start()

# 서버 메시지 전송 기능을 처리하는 스레드
threading.Thread(target=server_send_message).start()