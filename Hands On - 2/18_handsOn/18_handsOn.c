/*
============================================================================
Name : 18.c
Author : Vraj Jatin Naik
Description : Write a program to find out total number of directories on the pwd.
              execute ls -l | grep ^d | wc ? Use only dup2.
        Date: 31th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipe_fd[2];
    pid_t child_pid;

    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    child_pid = fork();

    if (child_pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) { // Child process
        close(pipe_fd[0]); // Close read end of the pipe
        dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipe_fd[1]); // Close write end of the pipe

        execlp("ls", "ls", "-l", NULL); // Execute ls -l

        perror("execlp"); // This line will only be reached if execlp fails
        exit(EXIT_FAILURE);
    } else { // Parent process
        close(pipe_fd[1]); // Close write end of the pipe
        dup2(pipe_fd[0], STDIN_FILENO); // Redirect stdin to pipe
        close(pipe_fd[0]); // Close read end of the pipe

        execlp("grep", "grep", "^d", NULL); // Execute grep ^d
        perror("execlp"); // This line will only be reached if execlp fails
        exit(EXIT_FAILURE);
    }

    return 0;
}

