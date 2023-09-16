/*
============================================================================
Name : 19.c
Author : Vraj Jatin Naik
Description : Create a FIFO file by
              a. mknod command
              b. mkfifo command
              c. use strace command to find out, which command (mknod or mkfifo) is better.
              c. mknod system call
              d. mkfifo library function
      Date : 31th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *fifoPath = "/home/vraj-naik/sslab/Hands On - 2/19_handsOn/vrajFIFO"; // Change the path as needed

    if (mkfifo(fifoPath, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    printf("FIFO created successfully at %s\n", fifoPath);

    return 0;
}

