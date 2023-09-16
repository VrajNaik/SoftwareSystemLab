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
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *fifoPath = "/home/vraj-naik/sslab/Hands On - 2/20_handsOn/vrajFIFO"; // Use the same path for both programs

    // Create the FIFO if it doesn't exist
    if (mkfifo(fifoPath, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    int fd = open(fifoPath, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char message[256];
    ssize_t bytesRead;

    

    while (1) {
        printf("Please enter a message (type 'exit' to terminate): ");
        fflush(stdout); // Flush the output buffer to ensure the prompt is displayed
        bytesRead = read(STDIN_FILENO, message, sizeof(message));
        if (bytesRead <= 0) {
            break; // End the loop when Ctrl+D is pressed or input is closed
        }
        // Remove trailing newline character if present
        if (bytesRead > 0 && message[bytesRead - 1] == '\n') {
            message[bytesRead - 1] = '\0';
        }

        if (strcmp(message, "exit") == 0) {
            printf("Exiting the conversation...\n");
            break; // Terminate the loop and communication
        }

        if (write(fd, message, bytesRead) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    close(fd);
    return 0;
}
