/*
============================================================================
Name : 1.c
Author : Vraj Jatin Naik
Description : Write a simple program to create a pipe, write to the pipe, read from pipe and display on
              the monitor.
            Date: 31th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int pipe_fd[2];
    char message[100];
    ssize_t bytes_read;

    // Create the pipe
    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // Prompt the user for input
    write(STDOUT_FILENO, "Enter a message: ", 18);

    // Read input from the user using read()
    bytes_read = read(STDIN_FILENO, message, sizeof(message));

    if (bytes_read > 0) {
        // Write data to the pipe
        if (write(pipe_fd[1], message, bytes_read) == -1) {
            perror("Write to pipe failed");
            exit(EXIT_FAILURE);
        }

        // Close the write end of the pipe
        close(pipe_fd[1]);

        // Read data from the pipe
        if ((bytes_read = read(pipe_fd[0], message, sizeof(message))) == -1) {
            perror("Read from pipe failed");
            exit(EXIT_FAILURE);
        }

        // Display the data read from the pipe
        if (write(STDOUT_FILENO, "Message received from the pipe: ", 32) == -1) {
            perror("Write to STDOUT failed");
            exit(EXIT_FAILURE);
        }
        if (write(STDOUT_FILENO, message, bytes_read) == -1) {
            perror("Write to STDOUT failed");
            exit(EXIT_FAILURE);
        }
    } else {
        write(STDOUT_FILENO, "Error reading input\n", 20);
    }

    return 0;
}
