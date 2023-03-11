from debug import debug 

debug_flag = False

import socket

HOST = "raunav-rpi.local" # carefull with this
PORT = 2000

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall("T".encode('utf-8'))
        print(f'Response: {s.recv(1024)}')
        print('Shutting down client.')


if __name__ == "__main__":
    main()
