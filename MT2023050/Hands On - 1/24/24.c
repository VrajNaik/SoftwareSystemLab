/*
============================================================================
Name : 24.c
Author : Vraj Jatin Naik
Description : Write a program to create an orphan process.
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("Fork failed");
        return 1;
    }

    if (child_pid == 0) {
        // Child process
        printf("Child process (PID %d) created with parent PID %d.\n", getpid(), getppid());
        sleep(2); // Simulate some work in the child process
        printf("Child process (PID %d) exiting. Parents ID: %d\n", getpid(), getppid());
    } else {
        // Parent process
        printf("Parent process (PID %d) created.\n", getpid());
        printf("Parent process (PID %d) exiting, leaving the child as an orphan.\n", getpid());
        exit(0); // Exit the parent process, leaving the child as an orphan
    }

    return 0;
}

