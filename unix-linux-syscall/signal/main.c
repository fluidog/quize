#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>


#include <signal.h>

void handler(int sig)
{
	printf("handler:%d\n",sig);
}


void send_signal(int sig)
{
	printf("send_signal:%d\n",sig);

	// 向自己发送信号
	// kill(getpid(),sig);
	// raise(sig);

	// 向自己所在组 群发信号
	// kill(0,sig);
	// kill(-getpid(),sig);

	// 向其他组 群发信号
	// pid_t group = ;
	// kill(-group,sig);
}

void setup_signals(int sig)
{
	struct sigaction act;
	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(sig,&act,NULL);
}

void other_signals()
{
	// 定时器
	// alarm(2);

	// 睡眠直到超时，或接收到任意信号，返回剩余时间
	// int seconds = 3;
	// seconds = sleep(seconds);

	// 挂起自身，直到接收到任意信号
	// pause();

	// 异常终止自身
	// abort();
}

int main()
{
	int i, j;

	printf("start\n");
	// signal(SIGINT,handler);
	signal(SIGTSTP,handler);
	signal(SIGQUIT,handler);
	signal(SIGABRT,handler);



	pid_t pid = fork();

	if (pid == 0)
	{
		printf("child\n");
		kill(0,SIGQUIT);
		while (1)
		{

		}

	}
	else
	{
		printf("parent\n");
		while (1)
		{

		}

	}
	printf("end\n");
	return 0;
}
