/*

TCP guess number server

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
#define SOCKET_NUMBER 312


int main()
{
	int listensd;
	int connectsd;
	int n;
	int processID;
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	int clAddrLen;
	int sd;
	srand(time(NULL));
	
	listensd = socket(PF_INET, SOCK_STREAM, 0);
	if(listensd == -1){
		printf("Fail to socket!\n");
		close(listensd);
		exit(1);
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(3122);
	sd = bind(listensd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(sd == -1){
		printf("Fail to bind!\n");
		close(listensd);
		exit(2);
	}
	
	sd = listen(listensd, 5);
	if(sd == -1){
		printf("Fail to listen!\n");
		close(listensd);
		exit(3);
	}
	
	while(1)
	{
		connectsd = accept(listensd, (struct sockaddr*)&clientAddr, &clAddrLen);
		if(connectsd == -1){
			printf("Fail to accept!\n");
			close(listensd);
			exit(4);
		}
		int num = (int)((double)(rand()) / RAND_MAX * 100);
		processID = fork();
		if(processID == 0)
		{
			close(listensd);
			int guess;
			int count = 0;
			int sendbuffer;
			printf("A client start to guess %d\n", num);
			while(1)
			{
				int bytesToRecv = sizeof(guess);
				char* movePtr = (char*)(&guess);
				count++;
				while((n = recv(connectsd, movePtr, bytesToRecv, 0)) > 0)
				{
					movePtr += n;
					bytesToRecv -= n;
					if(bytesToRecv == 0) break;
				}
				if(guess > num)
					sendbuffer = -2;
				else if(guess < num)
					sendbuffer = -1;
				else sendbuffer = count;
				sd = send(connectsd, &sendbuffer, sizeof(sendbuffer), 0);
				if(sd == -1){
					printf("A connection failed(error:%d)!\n", errno);
					exit(0);
				}
				if(guess == num){
					printf("client guess %d in %d time\n",num, count);
					break;
				}
			}
			_exit(0);
		}
		close(connectsd);
	}
}

