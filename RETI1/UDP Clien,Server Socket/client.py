from socket import *
import time

serverName = 'localhost'
serverPort = 12000

clientSocket = socket(AF_INET, SOCK_DGRAM)

#clientSocket.settimeout(2.0)
#clientSocket.setblocking(0)

message = input('Input lowercase sentence:')

clientSocket.sendto(message.encode(), (serverName, serverPort))

#time.sleep(2)
modifiedMessage, serverAddress = clientSocket.recvfrom(2048)

print(modifiedMessage.decode())

clientSocket.close()

