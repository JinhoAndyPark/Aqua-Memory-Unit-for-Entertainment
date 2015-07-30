#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/mman.h>


#include <pthread.h>


//for opencv
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


#define FBDEVFILE "/dev/fb1"

//bufferframe sturcture
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;


//screen value
char *fbp = 0;
int fbfd; // lcd file descriptor
int ret, screensize, flag;

void init_lcd_info();


using namespace cv;
using namespace std;


int main(void)
{
	//init
	init_lcd_info();

	int flag=0;
	

	IplImage* frame = cvLoadImage("swimming2.jpg");
	IplImage *frame_out2 = cvCreateImage(cvSize(480,320),IPL_DEPTH_8U,3);
	cvResize(frame,frame_out2);
	IplImage *frame_out = cvCreateImage(cvGetSize(frame_out2),IPL_DEPTH_8U,2);
	cvCvtColor(frame_out2,frame_out,CV_BGR2BGR565);
	memcpy(fbp,frame_out->imageData, 480*320*2);

//	pthread_t thread_spi;
//	pthread_create(&thread_spi,0,getADC,0);
	while(1)
	{

		if(flag == 0)
		{
			frame = cvLoadImage("swimming.jpg");
			frame_out2 = cvCreateImage(cvSize(480,320),IPL_DEPTH_8U,3);
			cvResize(frame,frame_out2);
			frame_out = cvCreateImage(cvGetSize(frame_out2),IPL_DEPTH_8U,2);
			cvCvtColor(frame_out2,frame_out,CV_BGR2BGR565);
			memcpy(fbp,frame_out->imageData, 480*320*2);
			flag = 1;
		}
		else
		{
			frame = cvLoadImage("swimming2.jpg");
			frame_out2 = cvCreateImage(cvSize(480,320),IPL_DEPTH_8U,3);
			cvResize(frame,frame_out2);
			frame_out = cvCreateImage(cvGetSize(frame_out2),IPL_DEPTH_8U,2);
			cvCvtColor(frame_out2,frame_out,CV_BGR2BGR565);
			memcpy(fbp,frame_out->imageData, 480*320*2);
			flag = 0;
		}

		delay(1000);



	}


	cvReleaseImage(&frame_out);
	cvReleaseImage(&frame_out2);
	
	
	return 0;
}

// init lcd
void init_lcd_info()
{

	int i = 0;
	fbfd = open(FBDEVFILE, O_RDWR);
	ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);
	ret = ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo);
	screensize = vinfo.xres * vinfo.yres * 2;
	fbp = (char *)mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd,0);

}


