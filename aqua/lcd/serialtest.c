#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
int main ()
{
  int fd ;
  int count ;
  unsigned int nextTime ;
  if ((fd = serialOpen ("/dev/ttyAMA0", 115200)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }
  if (wiringPiSetup () == -1)
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }

  for (count = 0 ; count < 256 ; )
  {
  
    
      printf ("\nOut: %3d: ", count) ;
      fflush (stdout) ;
      serialPutchar (fd, count) ;

      ++count ;
    
    delay (1000) ;
    while (serialDataAvail (fd))
    {
      printf (" -> %3d", serialGetchar (fd)) ;
      fflush (stdout) ;
    }
  }
  printf ("\n") ;
  return 0 ;
} 
