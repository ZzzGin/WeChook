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

    # Send data
    a = 1
    while a == 1:
	    message = ('{ \"cmd\": \"SendMessage\", \"args\": [\"filehelper\", \"HelloFromDll!\"] }'+'\0').encode("utf-8")
	    print('sending {!r}'.format(message.decode()))
	    sock.sendall(message)

	    # Look for the response
	    amount_received = 0
	    amount_expected = len(message)

	    while amount_received < amount_expected:
	        data = sock.recv(1000)
	        amount_received += len(data)
	        print('received {!r}'.format(data.decode()))
	    time.sleep(1)
	    a += 1

finally:
    print('closing socket')
    sock.close()