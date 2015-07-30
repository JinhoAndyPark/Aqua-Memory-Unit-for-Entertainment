
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{
	int fd ;
	char data ;

	if ((fd = serialOpen ("/dev/ttyAMA0",9600 )) < 0)
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}

	if (wiringPiSetup () == -1)
	{
		fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
		return 1 ;
	}


	while(1)
	{
//		serialPutchar (fd, '7') ;
//		delay(1000);

		while (serialDataAvail (fd))
		{	
			data = serialGetchar(fd);
			printf (" -> %c\n", data) ;
			fflush (stdout) ;
			if(data == '7')
			{
				serialPutchar(fd, '7');
				printf("send 7\n");
			}
			else if(data == '8')
			{
				serialPutchar(fd, '8');
				printf("send 8\n");
			}
			else if(data == '9')
			{
				serialPutchar(fd, '9');
				printf("send 9\n");
			}
		}
	}

	return 0 ;
}
