#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>



#define SHMKEY		1234
#define PORT 		7777
#define FAILURE		10000
#define SUCCESS		10001
#define TRUE		10002
#define FALSE		10003

struct sockaddr_in server_addr;
struct sockaddr_in client_addr;


struct info1
{
	char flag[10];
	char music[20];
};
typedef struct info1 Info1;


int sockfd;
Info1 SendInfo;
int ret;

void StopClient(int num)
{
	strcpy(SendInfo.flag,"quit");

	ret = send(sockfd, &SendInfo, sizeof(SendInfo), 0);
	if(ret == -1)
	{
		perror("send");
	}
	else
	{
		printf("sned quit success \n");
	}
	close(sockfd);
	printf("BYE ! \n");
	exit(1);
}


int main()
{
	char choise[10];
	
	signal(SIGINT, StopClient); 
	
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == sockfd)
	{
		perror("socket");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr("192.168.162.128");
	ret = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(-1 == ret)
	{
		perror("connect");
		exit(1);
	}


	while(1)
	{
		printf("sendinfo is : %s \n", SendInfo.flag);
		printf("1.on music		\n");
		printf("2.stop music	\n");
		printf("3.off music		\n");
		printf("4.next music	\n");
		printf("5.front music	\n");
		printf("6.poweron music \n");
		printf("7.poweoff music	\n");
		printf("8.singal cycle	\n");
		printf("9.play in order	\n");
		printf("10.ramdom play	\n");
		
		
		scanf("%s", choise);
		switch(atoi(choise))
		{
			case 1:
				memset(SendInfo.flag, 0, sizeof(SendInfo.flag));
				strcpy(SendInfo.flag,"on");

				ret = send(sockfd, &SendInfo, sizeof(SendInfo), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("sned on success \n");
				}
				break;

			case 2:
				memset(SendInfo.flag, 0, sizeof(SendInfo.flag));
				strcpy(SendInfo.flag,"stop");

				ret = send(sockfd, &SendInfo, sizeof(SendInfo), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("sned stop success \n");
				}
				break;

			case 3:
				memset(SendInfo.flag, 0, sizeof(SendInfo.flag));
				strcpy(SendInfo.flag,"off");

				ret = send(sockfd, &SendInfo, sizeof(SendInfo), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("sned stop success \n");
				}
				break;

			case 4:
				memset(SendInfo.flag, 0, sizeof(SendInfo.flag));
				strcpy(SendInfo.flag,"next");

				ret = send(sockfd, &SendInfo, sizeof(SendInfo), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("sned stop success \n");
				}
				break;


			case 5:
				memset(SendInfo.flag, 0, sizeof(SendInfo.flag));
				strcpy(SendInfo.flag,"front");

				ret = send(sockfd, &SendInfo, sizeof(SendInfo), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("sned stop success \n");
				}
				break;

			case 6:
				memset(SendInfo.flag, 0, sizeof(SendInfo.flag));
				strcpy(SendInfo.flag,"poweron");

				ret = send(sockfd, &SendInfo, sizeof(SendInfo), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("sned power on success \n");
				}
				break;

			case 7:
				memset(SendInfo.flag, 0, sizeof(SendInfo.flag));
				strcpy(SendInfo.flag,"poweroff");

				ret = send(sockfd, &SendInfo, sizeof(SendInfo), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("sned power off success \n");
				}
				break;
			case 8:
				memset(SendInfo.flag, 0, sizeof(SendInfo.flag));
				strcpy(SendInfo.flag,"sigal");

				ret = send(sockfd, &SendInfo, sizeof(SendInfo), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("sned power off success \n");
				}
				break;
			case 9:
				memset(SendInfo.flag, 0, sizeof(SendInfo.flag));
				strcpy(SendInfo.flag,"order");

				ret = send(sockfd, &SendInfo, sizeof(SendInfo), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("sned power off success \n");
				}
				break;
			case 10:
				memset(SendInfo.flag, 0, sizeof(SendInfo.flag));
				strcpy(SendInfo.flag,"random");

				ret = send(sockfd, &SendInfo, sizeof(SendInfo), 0);
				if(ret == -1)
				{
					perror("send");
				}
				else
				{
					printf("sned power off success \n");
				}
				break;

		}
	}
}
