#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues

int main(int argc, char *argv[])
{
	int listenSocketFD, establishedConnectionFD, portNumber, charsRead, status;
	socklen_t sizeOfClientIn	fo;
	char buffer[256];
	struct sockaddr_in serverAddress, clientAddress;
	pid_t pid, sid;

	if (argc =! 2) { fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1); } // Check usage & args

	// Set up the address struct for this process (the server)
	memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

	// Set up the socket
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (listenSocketFD < 0) error("ERROR opening socket");

	// Enable the socket to begin listening
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
		error("ERROR on binding");
	listen(listenSocketFD, 5); // Flip the socket on - it can now receive up to 5 connections

	// Accept a connection, blocking if one is not available until one connects
	
	while(1){


		sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
		establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
		if (establishedConnectionFD < 0) error("ERROR on accept");


		pid = fork();
		if (pid < 0){
			error("Server: ERROR forking process");
			exit(1);
		}

		if (pid == 0){


			// Get the message from the client and display it
			memset(buffer, '\0', 256);
			charsRead = recv(establishedConnectionFD, buffer, 255, 0); // Read the client's message from the socket
			if (charsRead < 0) error("ERROR reading from socket");
			if (strcmp(buffer, "verification") == 0) {
					//fprintf(stderr,"invalid client\n");
					//write error back to client


					char response[] = "accept";
					send(establishedConnectionFD, response, sizeof(response)-1, 0);
					printf("&&&&&&&&&&&&&&&&&&&&&&&\n");
					
			} 
			else {
					//write confirmation back to client
					char response[]  = "invalid";
					send(establishedConnectionFD, response, sizeof(response)-1, 0);
					exit(2);
			}
			//printf("SERVER: I received this from the client: \"%s\"\n", buffer);
			//memset(buffer, '\0', 256);
			printf("+++++++++++++\n%s\n",buffer);
			memset(buffer, '\0', 256);
			charsRead = recv(establishedConnectionFD, buffer, 255, 0);
			printf("-------------\n%s\n",buffer);

			// Send a Success message back to the client
			charsRead = send(establishedConnectionFD, "I am the server, and I got your message", 39, 0); // Send success back
			if (charsRead < 0) error("ERROR writing to socket");
			// Close the existing socket which is connected to the client
			
			printf("$$$$$$$$$$$$$$$$$$$$$$$\n");

			return 0;





		}

		close(establishedConnectionFD);


		while (pid > 0){ 	//parent process. wait for children to finish
			pid = waitpid(-1, &status, WNOHANG);
		}


	}



	close(listenSocketFD); // Close the listening socket
	return 0; 
}
