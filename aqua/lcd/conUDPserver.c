#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
void err_quit(const char *message);
void err_sys(const char *message);
#define MAXLINE 4096    /* max text line length */

int main(int argc,char *argv[])
{
	int listenfd;
	if ((listenfd = socket(AF_INET,SOCK_DGRAM,0))<0) // 비연결지향형 udp 소켓 생성 (socket)
		err_sys("socket error");

	struct sockaddr_in servaddr; // 서버주소와 소켓을 연결 (bind)
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
		err_sys("bind error");

	int n;
	char buff[MAXLINE + 1];
	char buff2[MAXLINE + 1];
	struct sockaddr_in fromaddr; // 클라이언트의 dummy 메시지 수신 (recvfrom)
	socklen_t fromaddrlen = sizeof(fromaddr);
	if ((n = recvfrom(listenfd,buff,MAXLINE,0,(struct sockaddr*)&fromaddr,&fromaddrlen))<=0)
		err_sys("recvfrom error");

	//connected UDP
	if(connect(listenfd,(struct sockaddr*)&fromaddr, sizeof(fromaddr))<0)
		err_sys("connect error");


	for ( ; ; ) {
		//	int n;
		//	char buff[MAXLINE + 1];
		//	struct sockaddr_in fromaddr; // 클라이언트의 dummy 메시지 수신 (recvfrom)
		//	socklen_t fromaddrlen = sizeof(fromaddr);
		//	if ((n = recvfrom(listenfd,buff,MAXLINE,0,(struct sockaddr*)&fromaddr,&fromaddrlen))<=0)
		//			err_sys("recvfrom error");

		//		char buff[MAXLINE + 1] = "udp dummy message";


		if((n = read(listenfd, buff2, MAXLINE))<=0) //수신
			err_sys("read error");


		if (write(listenfd, buff2, strlen(buff2))<0) //dummy 메시지 전송
			err_sys("write error");

		buff2[n]=0;
		fputs(buff2,stdout);
		fputc('\n',stdout);



		/*		char clntname[INET_ADDRSTRLEN]; // 클라이언트의 정보를 화면에 출력
				if (inet_ntop(AF_INET,&fromaddr.sin_addr.s_addr,clntname,sizeof(clntname))!=NULL)
				printf("Handling client %s/%d\n",clntname,ntohs(fromaddr.sin_port));
				else
				puts("Unable to get client address");

				time_t ticks = time(NULL); // daytime 문자열을 송신 (sendto)
				snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&ticks));
		//	if (sendto(listenfd,buff,strlen(buff),0,(struct sockaddr*)&fromaddr,fromaddrlen)<0)
		//		err_sys("sendto error");


		 */
	}
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


