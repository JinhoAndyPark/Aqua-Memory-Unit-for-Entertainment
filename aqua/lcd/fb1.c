#include<stdio.h>
#include<stdlib.h> /* for exit */
#include<string.h>
#include<unistd.h> /* for open/close .. */
#include<fcntl.h> /* for O_RDWR */
#include<sys/ioctl.h> /* for ioctl */
#include<sys/mman.h> /* for mmap */
#include<sys/types.h> //lseek ->using when move file pointer
#include<linux/fb.h> /* for fb_var_screeninfo, FBIOGET_VSCREENINFO */
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define KEY_NUM 9527
#define FBDEVFILE "/dev/fb1"
#define BUFF_SIZE 153600

typedef unsigned char ubyte;

unsigned short makepixel(ubyte r, ubyte g, ubyte b)
{
return (unsigned short)(((r>>3)<<11)|((g>>2)<<5)|(b>>3));
}

unsigned short bytepixel(ubyte rg, ubyte gb)
{
return (unsigned short)((rg<<8) | (gb));
}

int main(void)
{
int fbfd, fbsize, offset;
int i, fd, ret, shm_id;
struct fb_var_screeninfo fbvar;
unsigned short pixel;
char buff[BUFF_SIZE];
char fbuff[BUFF_SIZE];
char* pfbdata;
char* shm_addr;

fbfd = open(FBDEVFILE, O_RDWR); // /dev/fb1 RDed and WRite
if(fbfd < 0)
{
perror("fbdev open");
exit(1);
}
ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &fbvar); // get info from fb_var_screeninfo,thr var iable value
if(ret < 0)
{
perror("fbdev ioctl");
exit(1);
}
if(fbvar.bits_per_pixel != 16) //make sure the fb packet 54 
{
fprintf(stderr, "bpp is not 16n");
exit(1);
}
fbsize = fbvar.xres*fbvar.yres*2;//fb size 320*240*2 = 153600

pfbdata = (char *) //kernel fb memory -> user fb memory
mmap(0, fbvar.xres*fbvar.yres*2,
PROT_READ|PROT_WRITE, MAP_SHARED, fbfd, 0);

if((unsigned)pfbdata == (unsigned)-1)
{
perror("fbdev mmap");
exit(1);
}
// memset(buff, 0, BUFF_SIZE);

if( -1 == ( shm_id = shmget( (key_t)KEY_NUM, BUFF_SIZE, IPC_CREAT | 0666)))
{
printf("shared memory making failed \n");
return -1;
}
if( (void*)-1 == (shm_addr = shmat( shm_id, (void*)0, 0)))
{
printf("shared memory attached failed \n");
return -1;
}

while(1)
{
memcpy( pfbdata, shm_addr, BUFF_SIZE);
}

munmap(pfbdata, fbvar.xres*fbvar.yres*2);

close(fd);
close(fbfd);
exit(0);
return 0;
}


