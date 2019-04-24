# TCP P2P Framework

# Import socket module and system module
import errno
import os
import socket
import sys
import threading
import time



#TCP CLIENT

def TCPclient(peer_address, client_port):
	

	time.sleep(3)
	running = 1

	
	clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	
	# Request a TCP connection to the TCP server welcome socket
	try:
		clientSocket.connect((peer_address,int(client_port)))
		time.sleep(3)
		print 'Client: Type "quit" or "shutdown" to turnoff the peer2peer connection'
	except: 
		print("Client: Cannot connect to destination.")
		sys.exit(0)
	
	
	while running:
	
		
		parsing = 1
		message = raw_input ("Client: Type a message: ")
		while parsing:
			if message == '':
				print 'Invalid input. Please try again.'
				message = raw_input ("\rClient: Type a message: ")
			else: 
				parsing = 0

		
		try:
			clientSocket.send(message)
			modifiedMessage = clientSocket.recv(1024)
			print 'Client: Received echo: ' , modifiedMessage
			if message == 'quit' or message == 'shutdown':
				running = 0
				break
		except:	# Broken socket
			print("Client: Cannot send to destination.")
			clientSocket.shutdown(0)
			clientSocket.close()

	# Close the client socket
	clientSocket.close()
# End of Function



#TCP SERVER

running = 1
if len(sys.argv) <= 3:
	print 'Usage: "python TCPpeer.py peer_address server_port peer_server_port"'
	sys.exit(2)

# Create a TCP server welcome socket: (AF_INET for IPv4 protocols, SOCK_STREAM for TCP)
serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

serverSocket.bind(('', int(sys.argv[2])))

serverSocket.listen(1)

t1 = threading.Thread( target=TCPclient, args=(sys.argv[1],sys.argv[3]) )	
t1.daemon = True
t1.start()

# Listen to the connection requests for communication
while running:
	
	print '\nServer: TCP Server is listening on port ', sys.argv[2], '...'
	
	# Wait in-case no connection can be made in time
	time.sleep(5)
	if not t1.is_alive():
		break

	# Accept a new connection from the client
	connectionSocket,address = serverSocket.accept()
	connectionSocket.setblocking(0)
	
	print 'Server: Accepted a connection from ', address
	
	
	connecting = 1
	while connecting:
		
		# Receive a message from the client
		try:
			# Check if thread is down before fetch
			if not t1.is_alive():
				break
			
			# Fetch received message
			message = connectionSocket.recv(1024)
			print '\rServer: Received message: ', message
			
			# Echo the message in uppercase
			connectionSocket.send(message.upper())
		
			# Close the server connection socket upon client request			
			if message == 'quit' or message == 'shutdown':
				connectionSocket.close()
				running = 0
				connecting = 0
				break
			else:
				sys.stdout.write('Client: Type a message: ')
		except socket.error as e:	# If no new information 	
			if e.errno == errno.EAGAIN or e.errno == errno.EWOULDBLOCK:
				if not t1.is_alive():
					connectionSocket.close()
					running = 0
					connecting = 0
					break

print 'Server: TCP Peer2Peer Connection has been closed.'

# Close the server welcome socket before exiting
serverSocket.close()
sys.exit(0)