# UDP P2P Framework



# Import socket module and system module
import errno
import os
import socket
import sys
import threading
import time

#UDP CLIENT	
def UDPclient(peer_address, client_port):

	
	time.sleep(1)
	running = 1

	clientSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	print 'Client: Type "quit" or "shutdown" to turnoff the peer2peer connection'
	
	# Client takes message from user input, sends it to the server, and receives its echo
	while running:
	
		# Parse message input
		parsing = 1
		message = raw_input ("Client: Type a message: ")
		while parsing:
			if message == '':
				print 'Invalid input. Please try again.'
				message = raw_input ("\rClient: Type a message: ")
			else: 
				parsing = 0

	
		try:
			clientSocket.sendto(message,(peer_address,int(client_port)))
			modifiedMessage, sourceAddress = clientSocket.recvfrom(1024)
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

#UDP SERVER	

running = 1
if len(sys.argv) <= 3:
	print 'Usage: "python UDPpeer.py peer_address server_port peer_server_port"'
	sys.exit(2)


serverSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

serverSocket.bind(('', int(sys.argv[2])))
# Set blocking to zero 
serverSocket.setblocking(0)
# Create new thread for client for peer
t1 = threading.Thread( target=UDPclient, args=(sys.argv[1],sys.argv[3]) )	
t1.daemon = True
t1.start()

print '\nServer: UDP Server is waiting on port ', sys.argv[2], '...'

# Listen to the connection requests for communication
while running:

	# Receive a message from the client
	try:
		
		if not t1.is_alive():
			break
	
		# Fetch received message
		message, sourceAddress = serverSocket.recvfrom(1024)
		print '\rServer: Received message: ', message
		
		
		serverSocket.sendto(message.upper(),sourceAddress)
	
				
		if message.upper() == 'QUIT' or message.upper() == 'SHUTDOWN':
			running = 0
			break
		else:
			sys.stdout.write('Client: Type a message: ')
	except socket.error as e:	# If no new information 
		if e.errno == errno.EAGAIN or e.errno == errno.EWOULDBLOCK:
			if not t1.is_alive():
				running = 0
				break

print 'Server: UDP Peer2Peer Connection has been closed.'

# Close the server welcome socket before exiting
serverSocket.close()
sys.exit(0)