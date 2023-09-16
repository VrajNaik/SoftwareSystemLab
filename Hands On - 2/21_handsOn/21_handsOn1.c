/*
============================================================================
Name : 21.c
Author : Vraj Jatin Naik
Description : Write two programs so that both can communicate by FIFO -Use two way communication.
Date : 31th Aug, 2023.

============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

int main() {
    // Create the FIFOs (if they don't already exist)
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    // Open FIFO1 for writing and FIFO2 for reading
    int fd1 = open(FIFO1, O_WRONLY);
    int fd2 = open(FIFO2, O_RDONLY);

    if (fd1 == -1 || fd2 == -1) {
        perror("Error opening FIFOs");
        exit(EXIT_FAILURE);
    }

    char message[100];

    while (1) {
        printf("Enter a message for program 2 (or 'exit' to quit): ");
        fgets(message, sizeof(message), stdin);

        // Send the message to program 2
        write(fd1, message, strlen(message) + 1);

        if (strcmp(message, "exit\n") == 0) {
            break;
        }

        // Receive and display the response from program 2
        read(fd2, message, sizeof(message));
        printf("Program 2 says: %s", message);
    }

    // Close and remove the FIFOs
    close(fd1);
    close(fd2);
    unlink(FIFO1);
    unlink(FIFO2);

    return 0;
}










