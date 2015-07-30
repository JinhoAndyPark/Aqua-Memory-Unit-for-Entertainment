#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/* 스레드 기본예제.. 컴파일시 gcc -o main main.c -lpthread 해야암 */
void * smart1(void * );
void * smart2(void * );
void * smart3(void * );

int main(void)
{
  pthread_t thread1;
  pthread_t thread2;
  pthread_t thread3;

  pthread_create(&thread1,0 , smart1, 0);
  pthread_create(&thread2,0 , smart2, 0);
  pthread_create(&thread3,0 , smart3, 0);

  getchar();
  
  return 0;
}

void * smart1(void * vpData)
{  
  while(1)
  {
    printf("1\n");
    sleep(1);
  }  
  
  return 0;
}


void * smart2(void * vpData)
{  
  while(1)
  {
    printf("2\n");
    sleep(1);
  }  
}


void * smart3(void * vpData)
{
  while(1)
  {
    printf("3\n");
    sleep(1);
  }  
  return 0;
}
