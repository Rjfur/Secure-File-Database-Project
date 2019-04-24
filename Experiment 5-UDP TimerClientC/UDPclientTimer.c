
ECE369
Part 5 - Integration - Timer Client



#ifndef INCLUSIONS														
#define INCLUSIONS

// Inclusions
#include <arpa/inet.h>													// Includes in_addr_t & in_port_t structure and inet_addr & htons commands
#include <ctype.h>														// Includes toupper and tolower commands
#include <errno.h>														// Includes all error variables that would be defined to errno
#include <netinet/in.h>													// Includes sockaddr_in structure
#include <signal.h>														
#include <stdio.h>														
#include <stdlib.h>														
#include <string.h>														
#include <sys/ipc.h>													
#include <sys/socket.h>													
#include <sys/types.h>													
#include <sys/wait.h>													
#include <sys/time.h>													
#include <time.h>														
#include <unistd.h>														
#include <stdio.h>
#include <stdlib.h>

#endif



void SocketThis(int argc, char *argv[]);								// SocketThis Function Prototype



// Main Function
int main(int argc, char *argv[])
{
	

	// Syntax Check
	if (argc != 3)														// Checking the command line: program name with arguments
	{
		printf("Usage: %s server_address server_port\n", argv[0]);
		exit(1);														// This will exit if the number of arguments given at the command line was incorrect
	}
	else
	{
		SocketThis(argc,argv);											// Handles server socket connection
		return 0;
	}
}


                                                
//CLIENT->SERVER

// Function that simulates a UDP client
void SocketThis(int argc, char *argv[])
{
	// Initializations
	int sock;
	int bytes_read;
	int running = 1;
	struct timeval tv, tv_send, tv_receive;
	char send_data[1024];
	struct sockaddr_in Server;

	// Create a UDP client socket: SOCK_DGRAM for UDP
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("Socket error.");
		exit(1);
	}

	// Assign server info
	Server.sin_family = AF_INET;										// Assign Server Socket Type
	Server.sin_port = htons(atoi(argv[2]));								// Assign Server Port number
	Server.sin_addr.s_addr = inet_addr(argv[1]);						// Assign Server IP Address
	bzero(&(Server.sin_zero),8);

	// Client takes message from user input, sends it to the server, and receives its echo
	while (running)
	{
		// User input => send data
		printf("Type a message: ");
		gets(send_data);

		// Set socket timeout as 1 second
		tv.tv_sec = 1; 	// 1 second timeout
		tv.tv_usec = 0; // 0 microseconds timeout
		setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

		// Send a UDP packet
		gettimeofday(&tv_send,NULL);  // Set initial time
		sendto(sock, send_data, strlen(send_data), 0,(struct sockaddr *)&Server, sizeof(struct sockaddr));

		// Receive the server response
		bytes_read = recvfrom(sock,send_data,1024,0,(struct sockaddr *)&Server, sizeof(struct sockaddr));
		send_data[bytes_read] = '\0';

		if ((bytes_read == -1) && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
		{
			// Server does not respond, so assume the packet is lost
			printf("Time out! Message is lost.\n");
		}
		else
		{
			// Record receive_time
			gettimeofday(&tv_receive,NULL);  // Set ending time

			// Print the RTT along with the echo message
			printf("Received echo: ");
			ntohs(Server.sin_port);
			printf("%s in RTT: %f seconds.\n", send_data, (double) (tv_receive.tv_usec - tv_send.tv_usec) / 1000000 + (double) (tv_receive.tv_sec - tv_send.tv_sec));
			fflush(stdout);

			// Quit upon user request
			if (strcmp(send_data,"SHUTDOWN") == 0 || strcmp(send_data,"QUIT") == 0)
				running = 0;
		}
	}
	printf("UDP Client quits!\n");

	// Close server socket
	close(sock);

}
