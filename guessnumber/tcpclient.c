/*

TCP guess number client

Liuqing Yang<yllqq#outlook.com>

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>
#define BUFSIZE 256

#define BUFSIZE 256

int main(int argc, char *argv[])
{
	if(argc != 2){
		printf("Usage : tcpclient IPAddress\n");
		exit(1);
	}
	
	int sd;
	int n;
	int bytesToRecv;
	int sendbuffer;
	int recvbuffer;
	char* movePtr;
	struct sockaddr_in serverAddr;
	int ret;
	
	sd = socket(PF_INET, SOCK_STREAM, 0);
	if(sd == -1){
		printf("Fail to socket!\n");
		close(sd);
		exit(1);
	}
	
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &serverAddr.sin_addr);
	serverAddr.sin_port = htons(3122);
	
	ret = connect(sd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret == -1){
		printf("Fail to connect server!\n");
		close(sd);
		exit(3);
	}
	
	while(1)
	{
		printf("Guess a num between 0 and 100 : ");
		scanf("%d", &sendbuffer);
		ret = send(sd, &sendbuffer, sizeof(sendbuffer), 0);
		if(ret == -1){
			printf("Fail to send!\n");
			close(sd);
			exit(3);
		}
		bytesToRecv = sizeof(recvbuffer);
		movePtr = (char*)(&recvbuffer);
		while((n = recv(sd, movePtr, bytesToRecv, 0)) > 0)
		{
			movePtr = movePtr + n;
			bytesToRecv = bytesToRecv - n;
			if(bytesToRecv == 0) break;
		}
		if(recvbuffer == -2)
			printf("Too big!\n\n");
		else if(recvbuffer == -1)
			printf("Too small!\n\n");
		else{
			printf("Right. You get it in %d time.\n", recvbuffer);
			break;
		}
	}
	
	close(sd);
	exit(0);
}

