#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define LENGTH 5

void error(const char *msg) { perror(msg); exit(0); }

int main(int argc, char *argv[])
{
	int socketFD, portNumber, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char buffer[160000];
	char auth_message[50]="verification_dec";
    
	if (argc < 4) { fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); exit(0); }

	memset((char*)&serverAddress, '\0', sizeof(serverAddress));
	portNumber = atoi(argv[3]);
	serverAddress.sin_family = AF_INET; 
	serverAddress.sin_port = htons(portNumber);
	serverHostInfo = gethostbyname("localhost"); 
	if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length);


	socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFD < 0) {
		perror("CLIENT: ERROR opening socket"); 
		exit(2);
	}
	

	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
		perror("CLIENT: ERROR connecting"); 
		exit(2);
	}


	memset(buffer, '\0', sizeof(buffer));
	

	send(socketFD, auth_message, strlen(auth_message), 0);
	//printf("****************\n");
	recv(socketFD, buffer, 9999, 0);
	//printf("****************2\n");
    //printf("client:%s\n",buffer);
	if (strcmp(buffer, "accept_dec") != 0) {
        fprintf(stderr,"fial to contact otp_dec_d\n");
        exit(2);
    }

	char plaintext[80000];
	FILE *plaintext_fp = fopen(argv[1], "r");
	fgets(plaintext,80000,plaintext_fp);
	plaintext[strlen(plaintext)]='\0';
	//plaintext[strlen(plaintext)+1]='\0';
	int i;
	for(i=0;i<strlen(plaintext);i++){
		if(plaintext[0] < 'A' || plaintext[0] > 'Z' || plaintext[0] != ' '){
			fprintf(stderr,"bad characters \n");
			exit(1);
		}
	}

	char key[80000];
	FILE *key_fp = fopen(argv[2], "r");
	fgets(key,80000,key_fp);
	key[strlen(key)-1]='\0';
	//key[strlen(key)+1]='\0';

	//printf("client:%s\n",plaintext);
	//printf("client:%s\n",key);

	if (strlen(plaintext) > (strlen(key)+1)){
    	 fprintf(stderr, "key is too short\n"); 
    	 exit(1);
    }

	strcat(plaintext,key);
	//printf("client:%s\n",plaintext);


	char send_buffer[LENGTH+1]="\0";
	for(i=0;i<strlen(plaintext);){
		strncpy(send_buffer,plaintext+i,LENGTH);
		i=i+5;
		if (send(socketFD, send_buffer, strlen(send_buffer), 0) < 0)
		{
			printf("[otp_dec] Error: Failed to send file.\n");
			break;
		}
		memset(send_buffer, '\0', strlen(send_buffer));
	}








	//send(socketFD, plaintext, strlen(plaintext), 0);
	
	//send(socketFD, key, strlen(key), 0);

	//printf("============\n");




	
	memset(buffer, '\0', sizeof(buffer));






	memset(buffer, '\0', 80000);
	char receive_buffer[LENGTH+1]="\0";
	int bytes_received = 0;
	while ((bytes_received = recv(socketFD, receive_buffer, LENGTH, 0)) > 0){
		strcat(buffer,receive_buffer);
		memset(receive_buffer, '\0', strlen(receive_buffer));

		if (bytes_received == 0 || bytes_received != LENGTH)
		{
			break;
		}
	}







	//charsRead = recv(socketFD, buffer, 9999, 0);

	//if (charsRead < 0) error("ERROR reading from socket");

	printf("%s\n",buffer);
	//printf("client:%d\n",strlen(buffer));

	close(socketFD);
	return 0;
}
