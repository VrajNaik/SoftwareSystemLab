/*
============================================================================
Name : 22.c
Author : Vraj Jatin Naik
Description : Write a program to wait for data to be written into FIFO within 10 seconds, use select
              system call with FIFO.
Date : 2nd Oct, 2023.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/select.h>
#include <string.h>
#include <errno.h>

#define FIFO_PATH "myfifo1"

int main() {
    // Create the FIFO if it doesn't exist
    if (mkfifo(FIFO_PATH, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    // Open the FIFO for reading (non-blocking)
    int fd = open(FIFO_PATH, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Initialize the file descriptor set for select
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(fd, &read_fds);

    // Set a timeout of 10 seconds
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    printf("Waiting for data in FIFO within 10 seconds...\n");

    // Wait for data or timeout using select
    int result = select(fd + 1, &read_fds, NULL, NULL, &timeout);

    if (result == -1) {
        perror("select");
        exit(EXIT_FAILURE);
    } else if (result == 0) {
        printf("Timeout: No data written to FIFO within 10 seconds.\n");
    } else {
        printf("Data is available in FIFO.\n");

        // Read and process the data
        char buffer[1024];
        ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
        if (bytesRead == -1) {
            perror("read");
        } else {
            buffer[bytesRead] = '\0';
            printf("Received data: %s\n", buffer);
        }
    }

    // Close and remove the FIFO
    close(fd);
    unlink(FIFO_PATH);

    return 0;
}



