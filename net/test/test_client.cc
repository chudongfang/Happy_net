#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <termios.h>  

typedef struct sockaddr SA;

int main(int argc, char const *argv[])
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);//启动socket
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(connect(sockfd,(SA*)&addr,sizeof(addr))==-1){
	    perror("无法连接到服务器");
	    printf("客户端启动失败\n");
	    exit(-1);
	}
    
    
    //sleep(20);
	printf("客户端启动成功\n");
    send(sockfd,"lala",5,0) ;
	return 0;
}

