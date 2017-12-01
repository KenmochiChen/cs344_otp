#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 5


void enc(char plaintext[], char key[], char ciphertext_return[]){



    char ciphertext[80000];
    int i,plaintext_num,key_num,temp_num;
    char temp_num_string[3],temp_ciphertext[3];
    for(i=0;i<strlen(plaintext);i++){
        if(plaintext[i]==' '){
            plaintext_num=27;
        }
        else{
            sprintf(temp_num_string,"%d",plaintext[i]);
            plaintext_num=atoi(temp_num_string)-65;
        }

        if(key[i]==' '){
            key_num=27;
        }
        else{
            sprintf(temp_num_string,"%d",key[i]);
            key_num=atoi(temp_num_string)-65;
        }
        
        temp_num=plaintext_num+key_num;
        temp_num=temp_num%27;
        if(temp_num<0)
            temp_num=temp_num+27;
        if(temp_num==26)
            strcpy(temp_ciphertext," ");
        else sprintf(temp_ciphertext,"%c",temp_num+65);

		

        
		ciphertext[i]=temp_ciphertext[0];
        
        
    }
    ciphertext[i]='\0';
    strcpy(ciphertext_return,ciphertext);

	//printf("enc:%s------------\n",ciphertext);


}


void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues

int main(int argc, char *argv[])
{
	int listenSocketFD, establishedConnectionFD, portNumber, charsRead, status;
	socklen_t sizeOfClientInfo;
	char buffer[160000];
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

		waitpid(-1, &status, WNOHANG);

		
		


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
			memset(buffer, '\0', 80000);
			charsRead = recv(establishedConnectionFD, buffer, 9999, 0); // Read the client's message from the socket
			if (charsRead < 0) error("ERROR reading from socket");
            //printf("server:%s\n",buffer);
			if (strcmp(buffer, "verification") == 0) {
					//fprintf(stderr,"invalid client\n");
					//write error back to client


					char response[] = "accept";
					send(establishedConnectionFD, response, sizeof(response), 0);
					//printf("&&&&&&&&&&&&&&&&&&&&&&&\n");
					
			} 
			else {
					//write confirmation back to client
					char response[]  = "invalid";
					send(establishedConnectionFD, response, sizeof(response), 0);
					exit(2);
			}
			//printf("SERVER: I received this from the client: \"%s\"\n", buffer);
			//memset(buffer, '\0', 100);
			//printf("+++++++++++++\n%s\n",buffer);

			memset(buffer, '\0', 80000);
			char receive_buffer[LENGTH+1]="\0";
			int bytes_received = 0;
			while ((bytes_received = recv(establishedConnectionFD, receive_buffer, LENGTH, 0)) > 0){
				strcat(buffer,receive_buffer);
				memset(receive_buffer, '\0', strlen(receive_buffer));

				if (bytes_received == 0 || bytes_received != LENGTH)
				{
					break;
				}
			}











			
			//charsRead = recv(establishedConnectionFD, buffer, 9999, 0);
			//printf("-------------\n%s\n",buffer);




			int i,p;
			char plaintext[80000]="\0";
			char key[80000]="\0";
			for(i=0;i<strlen(buffer);i++){
				if(buffer[i] != '\n'){
					plaintext[i]=buffer[i];
				}else{
					plaintext[i+1]='\0';
					break;
				}
			}
			p=i+1;
			for(i=0;i<strlen(buffer);i++){
				if(buffer[p+i] != '\n'){
					key[i]=buffer[p+i];
				}else{
					key[i+1]='\0';
					break;
				}
			}

			printf("server plaintext:%d\n",strlen(plaintext));
			printf("server key:%d\n",strlen(key));

			
			// int x = 0; 
			// printf("CHAR INT         plaintext\n"); 
			// for (x = 0; x < strlen(plaintext); x++) 
			// 	printf(" %c   %d\n", plaintext[x], plaintext[x]);


			// printf("CHAR INT         key\n"); 
			// for (x = 0; x < strlen(key); x++) 
			// 	printf(" %c   %d\n", key[x], key[x]);



			char ciphertext[80000]="\0";
			enc(plaintext,key,ciphertext);

			//printf("server:%s\n",ciphertext);





			char send_buffer[LENGTH+1]="\0";
			for(i=0;i<strlen(ciphertext);){
				strncpy(send_buffer,ciphertext+i,LENGTH);
				i=i+5;
				if (send(establishedConnectionFD, send_buffer, strlen(send_buffer), 0) < 0)
				{
					printf("[otp_enc] Error: Failed to send file.\n");
					break;
				}
				memset(send_buffer, '\0', strlen(send_buffer));
			}










\
			//charsRead = send(establishedConnectionFD, ciphertext, strlen(ciphertext), 0); // Send success back
			//if (charsRead < 0) error("ERROR writing to socket");

			return 0;





		}
		else{
			close(establishedConnectionFD);

			//printf("pid:%d-----------------\n",pid);
			while (pid > 0){ 	//parent process. wait for children to finish
				//printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
				pid = waitpid(-1, &status, WNOHANG);
				//printf(".");
			}
		}
	}



	close(listenSocketFD); // Close the listening socket
	return 0; 
}
