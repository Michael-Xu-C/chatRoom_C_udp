/*************************************************************************
> File Name: server.c
> Author: Michael Jay
> mail: xuchuanwork@foxmail.com
> Created Time: 2016年07月06日 星期三 13时33分12秒
> Function: 
	创建一个聊天室
	本文件作为服务器端的收发显示

	消息的输入利用fifo从另一个终端读入

 ************************************************************************/

#include "myhead.h"

#define BUFFER_SIZE 1024
#define IP "127.0.0.1"
#define PORT 8888
#define FIFO_FILE "./fifo_file_ser"

void clearZombie(int sig)
{
	while(waitpid(-1, NULL, WNOHANG) > 0)
		;
}


int main(int argc, char* argv[])
{
	int fifo_fd;//用于管道通信
	int sock_fd;//用于与client通信
	char fifobuf[BUFFER_SIZE];
	char buf[BUFFER_SIZE];
	int real_read;
	
	SAIN servaddr, cliaddr;
	socklen_t peerlen; //32 ul 保存地址client长度

	if(argc < 3)
	{
		printf("Usage: %s IP PORT\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	system("clear");

	printf("*******************************************\n*");
	printf("     Chating Room under UDP Protocol      *\n*");
	printf("*******************************************\n*");

	if((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) <0 )
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	/*init servaddr/cliaddr */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = inet_addr(IP);

	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(atoi(argv[2]));
	cliaddr.sin_addr.s_addr = inet_addr(argv[1]);
	
	/*bind*/
	Bind(sock_fd, (SA *)&servaddr, sizeof(servaddr));
	printf("          bind successful.                *\n*");
	
	//ignore the SIGCHLD, avoid the Zombie
	signal(SIGCHLD, clearZombie);

	int pid = Fork();
	if(pid == 0) //子进程负责输入
	{
		if(access(FIFO_FILE, F_OK) < 0)	
		{
			printf("           FIFO Invalid!                  *\n*");
			printf("          FIFO Building...                *\n*");
			if(mkfifo(FIFO_FILE, 0666) < 0)
			{
				perror("fifo");
				exit(EXIT_FAILURE);
			}
			else
			{
				printf("         FIFO Built Succesful...          *\n*");
			}
		}

		//括号！！！！！！！！！！！！！
		if((fifo_fd = open(FIFO_FILE, O_RDWR)) < 0)
		{
			perror("fifo open file");
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("    FIFO for Inputing Opened Successful   *\n*");
			printf("       Now You can enjoy the chat...      *\n*");
			printf("                                          *\n*");
			printf("                   Powered by Michael Jay *\n*");
			printf("                  xuchuanwork@foxmail.com *\n*");
			printf("*******************************************\n");

			system("gnome-terminal -e ./sin");
		}
		peerlen = sizeof(cliaddr);// before calls, the addlen should be init to the size of the src_addr.			

		while(1)
		{
			memset(fifobuf, 0, BUFFER_SIZE);

			if((real_read = read(fifo_fd, fifobuf, BUFFER_SIZE))  > 0)
			{
				if(strcmp("quit", fifobuf) == 0)
				{
					printf("FIFO closed.\n");
					close(fifo_fd);
					close(sock_fd);
					//exit(EXIT_FAILURE);
					return 0;
				}

				printf("Me: %s\n", fifobuf);
				//将信息发送个client
				Sendto(sock_fd, fifobuf, sizeof(fifobuf), 0, (SA*)&cliaddr, peerlen);
			}

		}
	}
	else	//父进程负责通信
	{
		while(1)
		{
			/*  wait for client sending data*/
			//Recvfrom(sock_fd, buf, sizeof(buf), 0, (SA*)&cliaddr, &peerlen); 
			Recvfrom(sock_fd, buf, sizeof(buf), 0, NULL, NULL); 
			printf("Handsome: %s\n", buf);
			}
	}

	close(fifo_fd);
	close(sock_fd);
    return 0;
}
