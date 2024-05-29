# Description: A simple1 client that connects to the server and sends and receives messages.
import socket
import threading

def send_msg(sock):
    while True:
        msg = input()
        if msg.lower() == 'q':
            sock.close()
            break
        sock.send(msg.encode())

def recv_msg(sock):
    while True:
        try:
            msg = sock.recv(1024).decode()
            if msg:
                print(f"Message from server: {msg}")
            else:
                break
        except:
            break
    sock.close()

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(('127.0.0.1', 12345))

    threading.Thread(target=send_msg, args=(sock,)).start()
    threading.Thread(target=recv_msg, args=(sock,)).start()

if __name__ == "__main__":
    main()
