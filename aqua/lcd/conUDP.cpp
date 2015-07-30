#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void err_quit(const char *message);
void err_sys(const char *message);
#define MAXLINE 4096    /* max text line length */

int main(int argc,char *argv[])
{
	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

	int sockfd; //Connected UDP 소켓 생성
	if ((sockfd = socket(AF_INET,SOCK_DGRAM,0))<0)
		err_sys("socket error");

	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);
	if (inet_pton(AF_INET,argv[1],&servaddr.sin_addr)<=0)
		err_quit("inet_pton error");

	// Connected UDP로 구성     
	if (connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
		err_sys("connect error");

	char buff[MAXLINE + 1] = "udp dummy message";
	if (write(sockfd,buff,strlen(buff))<0) //dummy 메시지 전송
		err_sys("write error");

	int n;
	if ((n = read(sockfd,buff,MAXLINE))<=0) //Data 수신
		err_sys("read error");    

	buff[n]=0;
	fputs(buff,stdout);

	close(sockfd);
	exit(0);
}

void err_quit(const char *message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
void err_sys(const char *message)
{
	perror(message);
	exit(1);
}

