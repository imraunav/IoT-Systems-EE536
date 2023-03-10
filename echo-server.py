'''
echo-server.py
author - Raunav Ghosh
referenced from - https://realpython.com/python-sockets/
'''
import socket

debug_flag = False

HOST = "127.0.0.1" # Loopback address
PORT = 2000 # Port to listen on

def debug(message):
    if debug_flag == True:
        print(f'DEBUG: {message}')

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()
        conn, addr = s.accept()
        with conn:
            print(f"Connected by {addr}")
            while True:
                try:
                    data = conn.recv(1024)
                    conn.sendall(data)
                except:
                    print('Shutting down server.')
                    break

if __name__ == "__main__":
    main()
