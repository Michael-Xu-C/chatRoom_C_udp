/*************************************************************************
> File Name: serverInput.c
> Author: Michael Jay
> mail: xuchuanwork@foxmail.com
> Created Time: 2016年07月06日 星期三 13时33分12秒
> Function: 
	创建一个聊天室
	本文件作为服务器端输入程序
 ************************************************************************/

#include "myhead.h"

#define BUFF_SIZE 1024
#define FIFO_FILE "./fifo_file_ser"

int main(int argc, char* argv[])
{
	int fifo_fd;
	int real_write;
	char buf[BUFF_SIZE];

	system("clear");

	fifo_fd  = Open(FIFO_FILE, O_WRONLY);

	printf("----------------------------------------------------\n");
	printf("-              Server Input Application            -\n");
	printf("-              (Input \"%s\" to Quit)              -\n", "quit");
	printf("-                                                  -\n");
	printf("-                           Powered by Michael Jay -\n");
	printf("----------------------------------------------------\n");

	while(1)
	{
		printf("Input> ");

		fgets(buf, BUFF_SIZE, stdin);
		buf[strlen(buf)-1] = 0;
		if(strcmp("quit", buf) == 0)
		{
			printf("Quiting...\n");
			close(fifo_fd);
			exit(EXIT_SUCCESS);
		}

		real_write = Write(fifo_fd, buf, strlen(buf) );
	}
    return 0;
}
