/*
============================================================================
Name : 13.c
Author : Vraj Jatin Naik
Description : Write a program to wait for a STDIN for 10 seconds using select. Write a proper print statement to
              verify whether the data is available within 10 seconds or not (check in $man 2 select).
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
   
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    printf("Waiting for data.......\n");

    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    int ready = select(STDIN_FILENO + 1, &fds, NULL, NULL, &timeout);

    if (ready == -1) {
        perror("Error in select");
        exit(1);
    } 
    else if (ready == 0) {
        printf("Oh Nooooo !!!! Sorry Buddy... No data available within 10 seconds.\n");
    } 
    else {
        printf("Hurrey....!!!! Data is available within 10 seconds.\n");
        getchar();
    }

    return 0;
}
