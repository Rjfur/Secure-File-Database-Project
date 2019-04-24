
ECE369
Part 5 - Integration - Timer Client

*

#ifndef INCLUSIONS														// To make sure not to declare the function more than once by including the header multiple times.
#define INCLUSIONS

// Inclusions
#include <arpa/inet.h>													// Includes in_addr_t & in_port_t structure and inet_addr & htons commands
#include <ctype.h>														// Includes toupper and tolower commands
#include <errno.h>														// Includes all error variables that would be defined to errno
#include <netinet/in.h>													
#include <signal.h>														
#include <stdio.h>														
#include <stdlib.h>														
#include <string.h>													
#include <sys/ipc.h>													
#include <sys/socket.h>													
#include <sys/types.h>													
#include <sys/wait.h>													
#include <unistd.h>														
#include <stdio.h>
#include <stdlib.h>

#endif


void SocketThis(int argc, char *argv[]);								// SocketThis Function Prototype



int main(int argc, char *argv[])
{
	

	
	if (argc != 2)														// Checking the command line: program name with arguments
	{
		printf("Usage: %s server_port\n", argv[0]);
		exit(1);														// This will exit if the number of arguments given at the command line was incorrect
	}
	else
	{
		SocketThis(argc,argv);											// Handles server socket connection
		return 0;
	}
}

//SERVER -> SOCKET


// Function that simulates a UDP server
void SocketThis(int argc, char *argv[])
{
	// Initializations
	int sock, hostport;
	int addr_len, bytes_read;
	int x;
	int running = 1;
	char recv_data[1024];
    char hostname[1024];
	struct sockaddr_in Server, Client;

	// Create a UDP server socket: AF_INET for IPv4 protocols
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	// Print info to screen
	gethostname(hostname, 1024);										// Get hostname
	hostport = atoi(argv[1]);											// Port number to Integer
	printf("Server runs on %s at %d",hostname,hostport);

	// Assign server info
	Server.sin_family = AF_INET;										// Assign Server Socket Type
	Server.sin_port = htons(hostport);									// Assign Server Port number
	Server.sin_addr.s_addr = INADDR_ANY;								// Assign Server IP Address
	bzero(&(Server.sin_zero),8);

	// Bind the socket to server address = '' any address the machine has & port = 80xx
	if (bind(sock,(struct sockaddr *)&Server,sizeof(struct sockaddr)) == -1)
	{
		perror("Bind");
		exit(1);
	}
	addr_len = sizeof(struct sockaddr);
	printf("\nUDP Server is waiting on port %d ...\n", hostport);
	fflush(stdout);

	// Wait for the client to initiate communication
	while (running)
	{
		// Receive a message from the client
		bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&Client, &addr_len);
		recv_data[bytes_read] = '\0';

		// Print to screen the received message
		printf("\nReceived message from %s on port %d: ",inet_ntoa(Client.sin_addr),ntohs(Client.sin_port));
		for (x = 0; x < bytes_read; x++)
			recv_data[x] = toupper(recv_data[x]);
		printf("%s\n", recv_data);
		fflush(stdout);

		// Echo the message in uppercase
		sendto(sock, recv_data, strlen(recv_data), 0,(struct sockaddr *)&Client, sizeof(struct sockaddr));

		// Shutdown upon client request
		if (strcmp(recv_data,"SHUTDOWN") == 0)
			running = 0;
	}
	printf("UDP Server shuts down!\n");

	// Close server socket
	close(sock);
}
