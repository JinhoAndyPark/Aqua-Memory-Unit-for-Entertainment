#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/mman.h>

//thread
#include <pthread.h>
//opencv
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
//serial
#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>
//udp
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>


#define FBDEVFILE "/dev/fb1"
#define MAXLINE 4096    /* max text line length */


using namespace cv;
using namespace std;



struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

//screen value
char *fbp = 0;
int fbfd; // lcd file descriptor
int ret, screensize, flag;
IplImage * frame;
IplImage * frame_out;
IplImage * frame_out2;

//serial value
int serialfd;
char rcvdata;

//udpserver value
int listenfd;
int n;
char buff[MAXLINE + 1];
char buff2[MAXLINE + 1];

void * serial(void*);
void * udpserver(void*);

void lcdShow(const char* filename);
void init_lcd_info();
void init_serial();
void init_udpserver();
void err_quit(const char* message);
void err_sys(const char* message);

int main(void)
{
	init_lcd_info();
	init_serial();
	init_udpserver();
	/*
	   IplImage* frame = cvLoadImage("swimming2.jpg");

	   IplImage *frame_out2 = cvCreateImage(cvSize(480,320),IPL_DEPTH_8U,3);
	   cvResize(frame,frame_out2);
	   IplImage *frame_out = cvCreateImage(cvGetSize(frame_out2),IPL_DEPTH_8U,2);
	   cvCvtColor(frame_out2,frame_out,CV_BGR2BGR565);
	   memcpy(fbp,frame_out->imageData, 480*320*2);
	 */

	pthread_t thread_serial;
	pthread_t thread_server;
	pthread_create(&thread_serial,0,serial,0);
	pthread_create(&thread_server,0,udpserver,0);


	while(1)
	{

/*
		if(flag == 0)
		{
			lcdShow("swimming.jpg");
			flag = 1;
		}
		else
		{
			lcdShow("swimming2.jpg");
			flag = 0;
		}

		delay(1000);
*/

	}

	return 0;
}


void * serial(void*)
{
	while(1)
	{
		//              serialPutchar (fd, '7') ;
		//printf("im in serial\n");

		//            delay(1000);

		while (serialDataAvail (serialfd))
		{
			rcvdata = serialGetchar(serialfd);
			printf (" -> %c\n", rcvdata) ;
			fflush (stdout) ;
			if(rcvdata == '7')
			{
				lcdShow("swimming.jpg");
				serialPutchar(serialfd, '7');
				printf("send 7\n");
			}
			else if(rcvdata == '8')
			{
				lcdShow("swimming2.jpg");
				serialPutchar(serialfd, '8');
				printf("send 8\n");
			}
			else if(rcvdata == '9')
			{
				serialPutchar(serialfd, '9');
				printf("send 9\n");
			}
		}
	}

	return 0;


}
void * udpserver(void*)
{
	init_lcd_info();
	while(1)
	{
		if((n = read(listenfd, buff2, MAXLINE))<=0) //수신
			err_sys("read error");

		if (write(listenfd, buff2, strlen(buff2))<0) //dummy 메시지 전송
			err_sys("write error");

		buff2[n]=0;
		fputs(buff2,stdout);
		fputc('\n',stdout);
	}
}


void lcdShow(const char* filename)
{
	frame = cvLoadImage(filename);
	frame_out2 = cvCreateImage(cvSize(480,320),IPL_DEPTH_8U,3);
	cvResize(frame,frame_out2);
	frame_out = cvCreateImage(cvGetSize(frame_out2),IPL_DEPTH_8U,2);
	cvCvtColor(frame_out2,frame_out,CV_BGR2BGR565);
	memcpy(fbp,frame_out->imageData, 480*320*2);
}

void init_lcd_info()
{

	int i = 0;
	fbfd = open(FBDEVFILE, O_RDWR);
	ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);
	ret = ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo);
	screensize = vinfo.xres * vinfo.yres * 2;
	fbp = (char *)mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd,0);
}

void init_serial()
{

	if ((serialfd = serialOpen ("/dev/ttyAMA0",9600 )) < 0)
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return  ;
	}

	if (wiringPiSetup () == -1)
	{
		fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
		return  ;
	}
}

void init_udpserver()
{
	if ((listenfd = socket(AF_INET,SOCK_DGRAM,0))<0) // 비연결지향형 udp 소켓 생성 (socket)
		err_sys("socket error");

	struct sockaddr_in servaddr; // 서버주소와 소켓을 연결 (bind)
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
		err_sys("bind error");

	struct sockaddr_in fromaddr; // 클라이언트의 dummy 메시지 수신 (recvfrom)
	socklen_t fromaddrlen = sizeof(fromaddr);

	if ((n = recvfrom(listenfd,buff,MAXLINE,0,(struct sockaddr*)&fromaddr,&fromaddrlen))<=0)
		err_sys("recvfrom error");

	//connected UDP
	if(connect(listenfd,(struct sockaddr*)&fromaddr, sizeof(fromaddr))<0)
		err_sys("connect error");

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
