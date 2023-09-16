/*
============================================================================
Name : 20.c
Author : Vraj Jatin Naik
Description : Write two programs so that both can communicate by FIFO -Use one way communication.
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
    const char *fifoPath = "/home/vraj-naik/sslab/Hands On - 2/20_handsOn/vrajFIFO"; // Use the same path for both programs

    int fd = open(fifoPath, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    ssize_t bytesRead;

    while (1) {
        bytesRead = read(fd, buffer, sizeof(buffer));
        if (bytesRead <= 0) {
            break; // End the loop when communication is terminated
        }

        printf("Message received from the other end is: %.*s\n", (int)bytesRead, buffer);
        fflush(stdout); // Flush the output buffer to ensure the message is displayed
    }

    close(fd);
    return 0;
}



