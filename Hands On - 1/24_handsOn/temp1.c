#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
int main()
{
  if(!fork()) {
    printf("In child %d", getppid());
    sleep(1);
    printf("In child after orphan%d", getppid());
  }
  else {
    printf("Parent Id: %d", getpid());
    exit(0);
  }
  
}

