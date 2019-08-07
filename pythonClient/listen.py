import socket
import sys
import time

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = ('localhost', 50526)
print('connecting to {} port {}'.format(*server_address))
sock.connect(server_address)

try:
    while True:
	    data = sock.recv(1000)
	    print('received {!r}'.format(data.decode()))

finally:
    print('closing socket')
    sock.close()