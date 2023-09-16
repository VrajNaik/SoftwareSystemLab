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
    // Open FIFO1 for reading and FIFO2 for writing
    int fd1 = open(FIFO1, O_RDONLY);
    int fd2 = open(FIFO2, O_WRONLY);

    if (fd1 == -1 || fd2 == -1) {
        perror("Error opening FIFOs");
        exit(EXIT_FAILURE);
    }

    char message[100];

    while (1) {
        // Receive and display the message from program 1
        read(fd1, message, sizeof(message));
        printf("Program 1 says: %s", message);

        if (strcmp(message, "exit\n") == 0) {
            break;
        }

        printf("Enter a response for program 1 (or 'exit' to quit): ");
        fgets(message, sizeof(message), stdin);

        // Send the response to program 1
        write(fd2, message, strlen(message) + 1);
    }

    // Close the FIFOs
    close(fd1);
    close(fd2);

    return 0;
}











