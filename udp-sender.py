'''
Reference-
https://wiki.python.org/moin/UdpCommunication
'''
import socket

UDP_IP = "10.7.8.116"
UDP_PORT = 5005
sock = socket.socket(socket.AF_INET, # Internet
                    socket.SOCK_DGRAM) # UDP
print("UDP target IP: %s" % UDP_IP)
print("UDP target port: %s" % UDP_PORT)
while True:
    MESSAGE = input("Enter message to send over UDP: ").encode("utf-8")
    # print("message: %s" % MESSAGE)  
    sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
