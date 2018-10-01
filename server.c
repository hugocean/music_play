#include "server.h"

int ret;
int sound = 1;  //音量参数
//int number = 1;
extern struct info* shareInfo; //共享内存地址
extern Node *first; //链表首地址
//char OnMusic[20] = "../music/111.mp3";
int devmask, stereodevs;  //调节音量的
const char *sound_device_names[] = SOUND_DEVICE_NAMES;  //存放可用的音频设备
int devfd; //打开的音频设备的文件描述符
extern int total; //总共的歌曲数目
extern pid; 

//音频设备的初始化
int DeviceInit(char *dev)
{
	int i;
	int status;
	devfd = open("/dev/mixer", O_RDONLY);
	if (devfd == -1) 
	{
		perror("unable to open /dev/mixer");
		return FAILURE;
	}

	status = ioctl(devfd, SOUND_MIXER_READ_DEVMASK, &devmask);
	if (status == -1)
	{
		perror("SOUND_MIXER_READ_DEVMASK ioctl failed");
		return FAILURE;
	}
	status = ioctl(devfd, SOUND_MIXER_READ_STEREODEVS, &stereodevs);
	if (status == -1)
	{
		perror("SOUND_MIXER_READ_STEREODEVS ioctl failed");
  		return FAILURE;
	}

	for (i = 0 ; i < SOUND_MIXER_NRDEVICES ; i++)
	{
		if (((1 << i) & devmask) && !strcmp(dev, sound_device_names[i]))
		{
			break;
		}
	}
	if(i == SOUND_MIXER_NRDEVICES) 
	{ 
		fprintf(stderr, "%s is not a valid mixer device\n", dev);
		return FAILURE;
	}

	return i;
}

//父进程的函数
void Parent_Recv()
{
	int fd, sockfd;
	int length = sizeof(client_addr);
	int ppid;
	int device;
	char choise[10];
	Info1 RecvInfo;
	char vol[10] = "vol";

	
	sockfd = SocketInit();

	fd = accept(sockfd, (struct sockaddr *)&client_addr, &length);	
	if(fd == -1)
	{
		perror("accept");
	}
	else
	{
		printf("client connect success \n");
		printf("port is %d fd is %d \n", client_addr.sin_port, fd);	
	}

	ret = MusicCopy(first, 1);
	if(ret == SUCCESS)
	{
		printf("init music copy success \n");
		printf("success on music is %s \n",shareInfo->name);
	}
	else
	{
		printf("init music copy fialure \n");
		printf("failure on music is %s \n",shareInfo->name);
	}

	device = DeviceInit(vol);
	if(device == FAILURE)
	{
		printf("device init failure \n");
	}
	else
	{
		printf("device init success \n");
	}

		
	while(1)
	{
		printf("in parent while\n");
		
		ret = recv(fd ,&RecvInfo, sizeof(RecvInfo), 0);
		if(ret == -1)
		{
			perror("recv");
		}
		if(strcmp(RecvInfo.flag, "on") == 0) 
		{
			printf("recv flag : %s \n", RecvInfo.flag);
			strcpy(shareInfo->order, "on");
			printf("chareinfo order : %s \n", shareInfo->order);
			kill(shareInfo->ppid, SIGCONT);
		}
		else if(strcmp(RecvInfo.flag, "off") == 0) 
		{
			printf("recv flag : %s \n", RecvInfo.flag);
			strcpy(shareInfo->order, "off");
			printf("chareinfo order : %s \n", shareInfo->order);
			kill(shareInfo->ppid, SIGKILL);
		}
		else if(strcmp(RecvInfo.flag, "stop") == 0) 
		{
			printf("recv flag : %s \n", RecvInfo.flag);
			strcpy(shareInfo->order, "stop");
			printf("chareinfo order : %s \n", shareInfo->order);
			kill(shareInfo->ppid, SIGSTOP);
		}
		else if(strcmp(RecvInfo.flag, "next") == 0) 
		{
			printf("recv flag : %s \n", RecvInfo.flag);
			kill(shareInfo->ppid, SIGKILL);
			strcpy(shareInfo->order, "next");
			printf("chareinfo order : %s \n", shareInfo->order);
			(shareInfo->num)++;
			if((shareInfo->num) > total)
			{
				shareInfo->num = 1;
			}
			printf("num is %d \n", shareInfo->num);
			ret = MusicCopy(first, shareInfo->num);
			if(ret == SUCCESS)
			{
				printf("music copy success \n");
				printf("success on music is %s \n",shareInfo->name);
			}
			else
			{
				printf("music copy fialure \n");
				printf(" fial on music is %s \n",shareInfo->name);
			}

		}
		else if(strcmp(RecvInfo.flag, "front") == 0) 
		{
			printf("recv flag : %s \n", RecvInfo.flag);
			kill(shareInfo->ppid, SIGKILL);
			strcpy(shareInfo->order, "front");
			printf("chareinfo order : %s \n", shareInfo->order);
			(shareInfo->num)--;
			if((shareInfo->num) <= 0)
			{
				shareInfo->num = total;
			}
			printf("num is %d \n", shareInfo->num);
			ret = MusicCopy(first, shareInfo->num);
			if(ret == SUCCESS)
			{
				printf("music copy success \n");
				printf("success on music is %s \n",shareInfo->name);
			}
			else
			{
				printf("music copy fialure \n");
				printf("failure on music is %s \n",shareInfo->name);
			}

		}
		else if(strcmp(RecvInfo.flag, "poweron") == 0) 
		{
			printf("recv flag : %s \n", RecvInfo.flag);
			sound++;
			printf("soud is %d \n", sound);
			ret = Sound(device, sound);
			if(ret == SUCCESS)
			{
				printf("soud is %d\n", sound);
			}
			else
			{
				printf("sound failure \n");
			}
		}
		else if(strcmp(RecvInfo.flag, "poweroff") == 0) 
		{
			printf("recv flag : %s \n", RecvInfo.flag);
			sound--;
			printf("soud is %d \n", sound);
			ret = Sound(device, sound);
			if(ret == SUCCESS)
			{
				printf("soud is %d\n", sound);
			}
			else
			{
				printf("sound failure \n");
			}
		}
		else if(strcmp(RecvInfo.flag, "sigal") == 0) 
		{
			printf("recv flag : %s \n", RecvInfo.flag);
			strcpy(shareInfo->mode, "sigal");
		}
		else if(strcmp(RecvInfo.flag, "order") == 0) 
		{
			printf("recv flag : %s \n", RecvInfo.flag);
			strcpy(shareInfo->mode, "order");
		}
		else if(strcmp(RecvInfo.flag, "random") == 0) 
		{
			printf("recv flag : %s \n", RecvInfo.flag);
			strcpy(shareInfo->mode, "random");
		}
		else if(strcmp(RecvInfo.flag, "quit") == 0) 
		{
			printf("recv flag : %s \n", RecvInfo.flag);
			kill(shareInfo->ppid, SIGKILL);
			kill(pid,SIGKILL);
			exit(1);
		}
		else
		{
			sleep(3);
			printf("wait client \n");
		}
	}

}


//调节音量 dev：设备名 num：音量参数
int Sound(int dev, int num)
{
	int left, right, level;
	int status;
	if(num > 5 || num < 0)
	{
		return FAILURE;
	}
	switch(num)
	{
		case 1:
		left = 50;
		right = 50;
		level = (right << 8) + left;
		status = ioctl(devfd, MIXER_WRITE(dev), &level);
		if (status == -1) 
		{
			perror("MIXER_WRITE ioctl failed");
			return FAILURE;
		}
		left = level & 0xff;
		right = (level & 0xff00) >> 8;
		fprintf(stderr, "%s gain set to %d%% / %d%%\n", dev, left, right);
			break;

		case 2:
		left = 60;
		right = 60;
		level = (right << 8) + left;
		status = ioctl(devfd, MIXER_WRITE(dev), &level);
		if (status == -1) 
		{
			perror("MIXER_WRITE ioctl failed");
			return FAILURE;
		}
		left = level & 0xff;
		right = (level & 0xff00) >> 8;
		fprintf(stderr, "%s gain set to %d%% / %d%%\n", dev, left, right);
			break;

		case 3:
		left = 70;
		right = 70;
		level = (right << 8) + left;
		status = ioctl(devfd, MIXER_WRITE(dev), &level);
		if (status == -1) 
		{
			perror("MIXER_WRITE ioctl failed");
			return FAILURE;
		}
		left = level & 0xff;
		right = (level & 0xff00) >> 8;
		fprintf(stderr, "%s gain set to %d%% / %d%%\n", dev, left, right);
			break;

		case 4:
		left = 80;
		right = 80;
		level = (right << 8) + left;
		status = ioctl(devfd, MIXER_WRITE(dev), &level);
		if (status == -1) 
		{
			perror("MIXER_WRITE ioctl failed");
			return FAILURE;
		}
		left = level & 0xff;
		right = (level & 0xff00) >> 8;
		fprintf(stderr, "%s gain set to %d%% / %d%%\n", dev, left, right);
			break;

		case 5:
		left = 90;
		right = 90;
		level = (right << 8) + left;
		status = ioctl(devfd, MIXER_WRITE(dev), &level);
		if (status == -1) 
		{
			perror("MIXER_WRITE ioctl failed");
			return FAILURE;
		}
		left = level & 0xff;
		right = (level & 0xff00) >> 8;
		fprintf(stderr, "%s gain set to %d%% / %d%%\n", dev, left, right);
			break;

		case 6:
		left = 100;
		right = 100;
		level = (right << 8) + left;
		status = ioctl(devfd, MIXER_WRITE(dev), &level);
		if (status == -1) 
		{
			perror("MIXER_WRITE ioctl failed");
			return FAILURE;
		}
		left = level & 0xff;
		right = (level & 0xff00) >> 8;
		fprintf(stderr, "%s gain set to %d%% / %d%%\n", dev, left, right);
			break;

	}
	return SUCCESS;
}


//把链表中的第num首歌写道共享内存中
int MusicCopy(Node *l, int num)
{
	int k = 1;
	printf("node is %s \n", l);
	if(NULL == l)
	{
		printf("l is null\n");
		return FAILURE;
	}

	Node *p = l;
	
	while(k <= num && p != NULL)
	{
		printf("k < num failure \n");
		p = p->next;
		k++;
	}

	if((k > (num + 1)) || p == NULL)
	{
		printf("k > num failure \n");
		return FAILURE;
	}

	strcpy(shareInfo->name, p->music_name);
	printf("share music name is:%s \n",shareInfo->name);

	return SUCCESS;
}



//socket初始化
int SocketInit()
{
	int sockfd;

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == sockfd)
	{
		perror("socket");
		//exit(1);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr("192.168.162.128");
	ret = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(ret < 0)
	{
		perror("bind");
	//	exit(1);
	}

	ret = listen(sockfd, 5);
	if(ret < 0)
	{
		perror("listen");
	//	exit(1);
	}

	return sockfd;
}


//获取歌曲的名字
void GetName()
{
	char name[20];
	char flag[10];
    DIR *dirp;
    struct dirent *dp;
	const char *dirpath = "./music";

    dirp = opendir(dirpath);
    if (dirp  == NULL) 
	{
        printf("opendir failed on '%s'", dirpath);
        return;
    }

	while(1)
	{
        dp = readdir(dirp);
        if (dp == NULL)
		{
			break;
		}
		else if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
		{
			continue;          
		}
        else
		{
			strcpy(name, dirpath);
			strcat(name, "/");
			strcat(name, dp->d_name);
		}

		ret = LinkInsert(first, name);
		if(ret == SUCCESS)
		{
			total++;
			printf("insert success \n");
		}
		else
		{
			printf("insert failure \n");
		}
		printf("total is %d \n",total);
	}
}

//链表初始化
int LinkInit(Node **l)
{
	*l = (Node *)malloc(sizeof(Node));
	if(NULL == *l)
	{
		return FAILURE;
	}

	(*l)->next = NULL;
	
	return SUCCESS;
}


int LinkInsert(Node *l, char *name)
{
	if(l == NULL)
	{
		return FAILURE;
	}
	
	if(name == NULL)
	{
		return FAILURE;
	}

	Node *p = l;
	while(p->next != NULL)
	{
		p = p->next;
	}
	Node *n = (Node *)malloc(sizeof(Node));

	p->next = n;

	strcpy(n->music_name, name);
	
	n->next = NULL;

	return SUCCESS;
}


int LinkShow(Node *l)
{
	if(NULL == l)
	{
		return FAILURE;
	}
	
	Node *p = l;

	while(p->next != NULL)
	{
		p = p->next;
		printf("link info :%s \n", p->music_name);
	}

	return SUCCESS;
}
/*
char* GetLink(Node *l, int num)
{
	char buf[10] = "false";
	char *name;
	name = (char *)malloc(10);
	int k = 1;
	if(NULL == l)
	{
		return buf;
	}

	Node *p = l;
	
	while(k <= num && p != NULL)
	{
		p = p->next;
		k++;
	}

	if(k > num || p == NULL)
	{
		return buf;
	}

	strcpy(name, p->music_name);
	return name;
}
*/


//**********************************

//检查播放模式
int CheckMode()
{
	printf("this mode is %s \n", shareInfo->mode);
	if(strcmp(shareInfo->mode, "sigal") == 0)
	{
		ret = MusicCopy(first, shareInfo->num);
		if(ret == SUCCESS)
		{
			printf("music copy success \n");
			printf("success on music is %s \n",shareInfo->name);
		}
		else
		{
			printf("music copy fialure \n");
			printf("fail on music is %s \n",shareInfo->name);
		}
	}
	else if(strcmp(shareInfo->mode, "order") == 0)
	{
		(shareInfo->num)++;
		if((shareInfo->num) > total)
		{
			shareInfo->num = 1;
		}
		printf("set mode num is %d \n", shareInfo->num);
		ret = MusicCopy(first, shareInfo->num);
		if(ret == SUCCESS)
		{
			printf("music copy success \n");
			printf("success on music is %s \n",shareInfo->name);
		}
		else
		{
			printf("music copy fialure \n");
			printf("fail on music is %s \n",shareInfo->name);
		}
	}
	else if(strcmp(shareInfo->mode, "random") == 0)
	{
		shareInfo->num = rand()%total;
		ret = MusicCopy(first, shareInfo->num);
		if(ret == SUCCESS)
		{
			printf("music copy success \n");
			printf("success on music is %s \n",shareInfo->name);
		}
		else
		{
			printf("music copy fialure \n");
			printf("fail on music is %s \n",shareInfo->name);
		}
	}
	else
	{
		return FAILURE;
	}

	return SUCCESS;
}

void KillPro()
{
	printf("kill signal \n");
}

void StopPro()
{
//	printf("stop signal \n");
}

void ContPro()
{
//	printf("cont signal \n");
}

//孙进程的音乐播放函数
void MusicPlay()
{
	char opt[10] = " ";
	signal(SIGKILL, KillPro);
	signal(SIGSTOP, StopPro);
	signal(SIGCONT, ContPro);
/*	ret = CheckMode();
	if(ret == SUCCESS)
	{
		printf("on music is %s\n",shareInfo->name);
	}
	else
	{
		printf("on music is %s\n",shareInfo->name);
	}
*/
	while(1)
	{

		printf("madplay share info music is : %s \n", shareInfo->name);
		printf("madplay share info order is : %s \n", shareInfo->order);
		printf("madplay No%d \n",shareInfo->num);
		if(strcmp(shareInfo->order, "off") != 0)
		{
			execl("/usr/local/bin/madplay" , opt , shareInfo->name , NULL);
		}
		{
			sleep(3);
			printf("wait client \n");
		}
		
	}
}




















