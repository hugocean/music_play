#include "server.h"

struct info* shareInfo;
int total = 0;
Node *first = NULL;
pid_t pid;
//extern OnMusic[20];

int main()
{
	int shmid;
	int ret;
	int status;
	
	//初始化链表
	ret = LinkInit(&first);
	if(ret == SUCCESS)
	{
		printf("init success \n");
	}
	else
	{
		printf("init failure \n");
	}
	
	//获取歌曲名，顺便把歌曲名插入到链表中
	GetName();
	printf("total is %d \n",total);

	//显示链表
	ret = LinkShow(first);
	if(ret == SUCCESS)
	{
		printf("show success \n");
	}
	else
	{
		printf("show failure \n");
	}

	//开辟共享内存
	shmid = shmget(SHMKEY, sizeof(struct info), IPC_CREAT | IPC_EXCL);
	if(-1 == shmid)
	{
		perror("shmget");
		exit(1);
	}

	//映射到进程的虚拟内存
	shareInfo = (struct info*)shmat(shmid, NULL, 0);
	if(NULL == shareInfo)
	{
		perror("shmat");
		exit(1);
	}

	//初始化共享内存的一些参数
	strcpy(shareInfo->order, "off");
	strcpy(shareInfo->mode, "order");
	shareInfo->num = 1;
	
	//创建进程 先创建父子两个进程，再在子进程里面再次调用fork就有了孙进程
	pid = fork();
	if(pid < 0)
	{
		perror("fork");
	}
	else if(pid == 0)
	{
		//子进程代码段
		printf("clid process id: %d \n", getpid());
		//不停的创建孙进程
		while(1)
		{
			pid = fork();
			if(pid < 0)
			{
				perror("fork");
			}
			else if(pid == 0)
			{
				//孙进程代码段
				printf("grandson process id: %d \n", getpid() );
				shareInfo->ppid = getpid();
				//音乐播放函数
				MusicPlay();
			}
			else
			{
				printf("clild process id: %d \n", getpid() );
				printf("child in wait \n");
				//等待孙进程结束
				waitpid(pid, &status, 0);
				//判断孙进程结束状态
				//正常退出状态
				if(WIFEXITED(status))
				{
					printf("child exit \n");
					//查看循环模式，如果是顺序播放就把下一首音乐的歌名存到共享内存中
					ret = CheckMode();
					if(ret == SUCCESS)
					{
						printf("on music is %s\n",shareInfo->name);
					}
					else
					{
						printf("on music is %s\n",shareInfo->name);
					}
				}
				//被信号杀死而退出，什么都不做，因为再父进程那接收到socket请求的同时以及把歌曲写到共享内存中了
				else if(WIFSIGNALED(status))
				{
					printf("siganl kill \n");
				}
				else
				{
					printf("......\n");
				}
		
			}
			
		}
	}
	else
	{
		//父进程代码段
		printf("parent process id: %d \n", getpid() );
		//父进程的主函数，一直阻塞接收来自客户端socket的请求
		Parent_Recv();
	}

}




