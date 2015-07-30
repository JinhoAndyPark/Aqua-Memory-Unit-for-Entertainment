#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <wiringPi.h>

void error_handling(char *message);

int main(int argc, char * argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
	
	char message[] = "hello world!";

	if(argc != 2)
	{
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}
	
	printf("socket()\n");
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		printf("error in socket()\n");
	//	error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htonl(atoi(argv[1]));

	printf("bind()\n");
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))== -1)
		printf("error in bind()\n");
	//	error_handling("bind() error");
	printf("listen()\n");
	if(listen(serv_sock, 5) == -1)
		printf("error in listen()\n");
	//	error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);
	printf("accept()\n");
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if(clnt_sock == -1)
		printf("error in accept()\n");
	//	error_handling("accept() error");

	while(1)
	{
		printf("write..\n");
		write(clnt_sock, message, sizeof(message));
		delay(1000);
	}
	close(clnt_sock);
	close(serv_sock);
	return 0;

}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
