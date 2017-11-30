#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg) { perror(msg); exit(0); }

int main(int argc, char *argv[])
{
	int socketFD, portNumber, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char buffer[10000];
	char auth_message[50]="verification";
    
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
	printf("****************\n");
	recv(socketFD, buffer, sizeof(buffer), 0);
	printf("****************2\n");
    printf("client:%s\n",buffer);
	if (strcmp(buffer, "accept") != 0) {
        fprintf(stderr,"fial to contact otp_enc_d\n");
        exit(2);
    }

	char plaintext[10000];
	FILE *plaintext_fp = fopen(argv[1], "r");
	fgets(plaintext,10000,plaintext_fp);
	plaintext[strlen(plaintext)]='\0';
	//plaintext[strlen(plaintext)+1]='\0';
	int i;
	for(i=0;i<strlen(plaintext);i++){
		if(plaintext[0] < 'A' || plaintext[0] > 'Z'){
			fprintf(stderr,"bad characters \n");
			exit(1);
		}
	}

	char key[10000];
	FILE *key_fp = fopen(argv[2], "r");
	fgets(key,10000,key_fp);
	key[strlen(key)-1]='\0';
	//key[strlen(key)+1]='\0';

	printf("client:%s\n",plaintext);
	printf("client:%s\n",key);

	if (strlen(plaintext) > (strlen(key)+1)){
    	 fprintf(stderr, "key is too short"); 
    	 exit(1);
    }

	strcat(plaintext,key);
	printf("client:%s\n",plaintext);
	send(socketFD, plaintext, strlen(plaintext)+strlen(key), 0);
	
	//send(socketFD, key, strlen(key), 0);

	printf("============\n");




	
	memset(buffer, '\0', sizeof(buffer));
	charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0);
	printf("^^^^^^^^^^^^^^\n");
	if (charsRead < 0) error("ERROR reading from socket");
	printf("client:%s\n",buffer);

	close(socketFD);
	return 0;
}
