'''
echo-client.py
author-Raunav Ghosh
reference - https://realpython.com/python-sockets/
'''

import socket

debug_flag = False
HOST = "127.0.0.1"
PORT = 2000

def debug(message):
    if debug_flag == True:
        print(f'DEBUG: {message}')

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        while True:
            try:
                s.sendall(input('Echo message: ').encode('utf-8'))
                data = s.recv(1024)
                print(f'Response: {data}')
            except:
                print('Shutting down client.')
                break

if __name__ == "__main__":
    main()
