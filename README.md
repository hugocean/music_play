# music_play
嵌入式音乐播放器

通过madplay命令进行歌曲播放，功能有：
1、实现音乐的开始播放、结束播放、继续播放、暂停播放；
2、实现歌曲上一首、下一首切换；
3、实现歌曲播放模式的选择（顺序播放、随机播放、单曲循环）；
4、实现音量调节（有上限和下限）；

通过两个程序控制这个音乐播放器
客户端用来发一些请求就可以了。
服务器端：至少创建三个进程，父进程，子进程阻塞，等待回收孙进程，孙进程负责播放音乐。
父进程：用于监听按键和socket请求，与孙进程进行通信（通过共享内存），告诉孙进程需要播放的歌曲名，以及发送信号给孙进程。
子进程：一直不停的创建孙进程，然后wait等待孙进程结束，再创建新的孙进程。
孙进程：读共享内存中的歌曲名，execl执行madplay，接收父进程发来的信号（暂停，杀死等信号）。
共享内存：三个进程之间的通信用共享内存实现，歌曲列表通过系统调用的函数写道链表中，再fork父子孙三进程。
音量调节：https://blog.csdn.net/zqixiao_09/article/details/50859302

用到的系统调用函数：
pid_t fork(void);
pid_t waitpid(pid_t pid,int *status, int options);
int execl(const char *pathname, const char *arg, ..., (char *)NULL);
shmid = shmget(SHMKEY, SHMSIZE, IPC_CREAT | IPC_EXCL);   //创建共享内存
shmaddr = shmat(shmid, NULL, 0);    //映射到虚拟地址空间
int ioctl(int handle, int cmd,[int *argdx, int argcx]); //控制I/O设备 /dev/mixer


问题：
客户端退出时服务器端父进程接收quit，然后kill孙进程子进程，会出现tty：input/ouput error
是因为没有close 文件描述符 open("/dev/mixer")的文件描述符


