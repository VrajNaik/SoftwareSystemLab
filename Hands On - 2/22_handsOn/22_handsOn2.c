/*
============================================================================
Name : 22.c
Author : Vraj Jatin Naik
Description : Write a program to wait for data to be written into FIFO within 10 seconds, use select
              system call with FIFO.
Date : 2nd Oct, 2023.
============================================================================
*/

#include <sys/types.h> 
#include <sys/stat.h>  
#include <fcntl.h>     
#include <sys/time.h>  
#include <unistd.h>   
#include <stdio.h>    

#include "./22_handsOn.h"

void main()
{
    int fileDescriptor, writeBytes;
    char data[100];

    fileDescriptor = open(fifoName, O_NONBLOCK | O_WRONLY);

    if (fileDescriptor == -1)
    {
        perror("Error while opening FIFO file!");
        _exit(0);
    }

    writeBytes = write(fileDescriptor, "Hello there.......! Hi MT2023050 - Vraj Jatin Naik", sizeof("Hello there.......! Hi MT2023050 - Vraj Jatin Naik"));

    if (writeBytes == -1)
        perror("Error while writing to FIFO!");

    close(fileDescriptor);
}
