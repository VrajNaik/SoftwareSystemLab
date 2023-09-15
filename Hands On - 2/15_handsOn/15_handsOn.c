/*
============================================================================
Name : 15.c
Author : Vraj Jatin Naik
Description : Write a simple program to send some data from parent to the child process.
        Date: 31th Aug, 2023.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int pipe_fd[2];
    pid_t child_pid;

    // Create a pipe
    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // Create a child process
    child_pid = fork();

    if (child_pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Child process
        close(pipe_fd[1]); // Close the write end of the pipe in the child
        char message[100];
        ssize_t bytes_read;

        // Read data from the pipe
        bytes_read = read(pipe_fd[0], message, sizeof(message));

        if (bytes_read > 0) {
            message[bytes_read] = '\0';
            printf("Child received: %s\n", message);
        } else {
            printf("Child: Error reading from pipe\n");
        }

        close(pipe_fd[0]); // Close the read end of the pipe in the child
    } else {
        // Parent process
        close(pipe_fd[0]); // Close the read end of the pipe in the parent
        char message[100];

        // Prompt the user for input
        printf("Parent: Enter a message: ");
        fflush(stdout); // Flush the output to ensure the prompt is displayed

        // Read input from the user using read()
        ssize_t bytes_read = read(STDIN_FILENO, message, sizeof(message));

        if (bytes_read > 0) {
            // Write data to the pipe
            if (write(pipe_fd[1], message, bytes_read) == -1) {
                perror("Parent: Write to pipe failed");
                exit(EXIT_FAILURE);
            }

            printf("Parent sent: %s\n", message);
        } else {
            printf("Parent: Error reading input\n");
        }

        close(pipe_fd[1]); // Close the write end of the pipe in the parent
    }

    return 0;
}
