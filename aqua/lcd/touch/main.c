/*
   A simple program that demonstrates how to program for a
   touch screen. Specifically on the Raspberry Pi.
   This prgram can be used for most Linux based systems.
   For more details: ozzmaker.com
   Copyright (C) 2013  Mark Williams
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   Library General Public License for more details.
   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
   MA 02111-1307, USA
 */

#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
//#include "touch.h"
//#include "touch.c"
//#include "framebuffer.c"
#include <signal.h>


#define SAMPLE_AMOUNT 2
#define KWHT  "\x1B[37m"
#define KYEL  "\x1B[33m"

int fd;

using namespace std;
/*
char *events[EV_MAX + 1] = {
	[0 ... EV_MAX] = NULL,
	[EV_SYN] = "Sync",			[EV_KEY] = "Key",
	[EV_REL] = "Relative",			[EV_ABS] = "Absolute",
	[EV_MSC] = "Misc",			[EV_LED] = "LED",
	[EV_SND] = "Sound",			[EV_REP] = "Repeat",
	[EV_FF] = "ForceFeedback",		[EV_PWR] = "Power",
	[EV_FF_STATUS] = "ForceFeedbackStatus",
};
*/

#define EV_SYN                  0x00
#define EV_KEY                  0x01
#define EV_REL                  0x02
#define EV_ABS                  0x03
#define EV_MSC                  0x04
#define EV_LED                  0x11
#define EV_SND                  0x12
#define EV_REP                  0x14
#define EV_FF                   0x15
#define EV_PWR                  0x16
#define EV_FF_STATUS            0x17
int openTouchScreen()
{
        if ((fd = open("/dev/input/event3", O_RDONLY)) < 0) {
                return 1;
        }
}


void getTouchSample(int *rawX, int *rawY, int *rawPressure)
{
	int i;
        /* how many bytes were read */
        size_t rb;
        /* the events (up to 64 at once) */
        struct input_event ev[64];

	rb=read(fd,ev,sizeof(struct input_event)*64);
        for (i = 0;  i <  (rb / sizeof(struct input_event)); i++){
  
            if (ev[i].type ==  EV_SYN)
{}
                else if (ev[i].type == EV_KEY && ev[i].code == 330 && ev[i].value == 1)
{}
                else if (ev[i].type == EV_KEY && ev[i].code == 330 && ev[i].value == 0)
{}
                else if (ev[i].type == EV_ABS && ev[i].code == 0 && ev[i].value > 0){
			*rawX = ev[i].value;
		}
                else if (ev[i].type == EV_ABS  && ev[i].code == 1 && ev[i].value > 0){
			*rawY = ev[i].value;
		}
                else if (ev[i].type == EV_ABS  && ev[i].code == 24 && ev[i].value > 0){
			*rawPressure = ev[i].value;
		}




	}


}
int main()
{

	int  xres,yres,x;

	int Xsamples[20];
	int Ysamples[20];

	int screenXmax, screenXmin;
	int screenYmax, screenYmin;

	float scaleXvalue, scaleYvalue;

	int rawX, rawY, rawPressure, scaledX, scaledY;

	int Xaverage = 0;
	int Yaverage = 0;


	if (openTouchScreen() == 1)
		perror("error opening touch screen");


//	getTouchScreenDetails(&screenXmin,&screenXmax,&screenYmin,&screenYmax);

//	framebufferInitialize(&xres,&yres);

//	scaleXvalue = ((float)screenXmax-screenXmin) / xres;
	scaleXvalue = 7.708;
	printf ("X Scale Factor = %f\n", scaleXvalue);
//	scaleYvalue = ((float)screenYmax-screenYmin) / yres;
	scaleYvalue = 11.562;
//	printf ("Y Scale Factor = %f\n", scaleYvalue);




	int h;


	int sample;

	while(1){
		for (sample = 0; sample < SAMPLE_AMOUNT; sample++){
			getTouchSample(&rawX, &rawY, &rawPressure);
			Xsamples[sample] = rawX;
			Ysamples[sample] = rawY;
		}

		Xaverage  = 0;
		Yaverage  = 0;

		for ( x = 0; x < SAMPLE_AMOUNT; x++ ){
			Xaverage += Xsamples[x];
			Yaverage += Ysamples[x];
		}

		Xaverage = Xaverage/SAMPLE_AMOUNT;
		Yaverage = Yaverage/SAMPLE_AMOUNT;

		scaledX = 	Xaverage / scaleXvalue;
		scaledY = 	Yaverage / scaleYvalue;
		
		printf("Xaverage : %d, Yaverage : %d \n", Xaverage, Yaverage);
		printf("scaledX  : %d, scaledY  : %d \n", scaledX, scaledY);
		
	}
}
