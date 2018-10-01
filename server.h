#ifndef MUSIC_H
#define MUSIC_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <linux/soundcard.h>



#define SHMKEY		1234
#define PORT 		7777
#define FAILURE		10000
#define SUCCESS		10001
#define TRUE		10002
#define FALSE		10003

struct sockaddr_in server_addr;
struct sockaddr_in client_addr;


struct info
{
	int ppid;
	int num;
	char name[20];
	char order[10];
	char mode[10];
};
typedef struct info Info;

struct info1
{
	char flag[10];
	char music[20];
};
typedef struct info1 Info1;

struct node
{
	char music_name[20];
	struct node *next;

};
typedef struct node Node;

int LinkShow(Node *l);
void Parent_Recv();
int SocketInit();
void Child_Pro();
void GetName();
int LinkInit(Node **l);
int LinkInsert(Node *l, char *name);
void MusicPlay();




#endif



