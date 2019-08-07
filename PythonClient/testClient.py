import socket
import sys
import time
import json
import threading

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = ('localhost', 50526)
print('connecting to {} port {}'.format(*server_address))
sock.connect(server_address)

def recv():
	while True:
	    data = sock.recv(1000)
	    print('received {!r}'.format(data.decode()))
	    recv_d = json.loads(data.decode())
	    if recv_d["cmd"]=="EjectHooks_R":
	    	sock.close()
	    	sys.exit()

threading.Thread(target=recv).start()

messages =[
	"testMessages",
	"这是个测试消息。。",
	"你看我居然能发送中文！",
	"是不是很酷？",
	"Of course, I need some more messages",
	"...just for testing...",
	"message1",
	"message2",
	"message3",
	"Ok done."
]
# Send data
for m in messages:
	dToBeSent = {"cmd":  "SendMessage", "args": ["filehelper", m]}
	messageString = (json.dumps(dToBeSent) + "\0").encode("utf-8")
	print('sending {!r}'.format(messageString.decode()))
	sock.sendall(messageString)
	time.sleep(2)
dToBeSent = {"cmd":  "EjectHooks", "args": ["1", "2"]}
messageString = (json.dumps(dToBeSent) + "\0").encode("utf-8")
print('sending {!r}'.format(messageString.decode()))
sock.sendall(messageString)
time.sleep(10)