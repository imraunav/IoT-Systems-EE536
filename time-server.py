import socket
from debug import debug
from datetime import datetime

debug_flag = True

HOST = "raunav-rpi.local"
PORT = 2000

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
         s.bind((HOST, PORT)) # socket to listen on
         while True:
            try:
                s.listen() # listenning on socket
                conn, addr = s.accept() # when connection established
                with conn: # socket of client
                    print(f"Connected by {addr}")
                    request = conn.recv(1024).strip().decode('utf-8')
                    if request in ('T', 't'):
                        now = datetime.now()
                        current_time = now.strftime('%H:%M:%S')
                        response = f'Time is now: {current_time}'
                    else:
                        response = "Unkown request!"
                    conn.sendall(response.encode('utf-8'))
            except:
                print('Shutting down server.')
                break

if __name__ == "__main__":
    main()
