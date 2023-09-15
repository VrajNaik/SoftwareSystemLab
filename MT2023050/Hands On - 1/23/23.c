/*
============================================================================
Name : 23.c
Author : Vraj Jatin Naik
Description : Write a program to create a Zombie state of the running program.
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // Use fork to create a child process
    pid_t child_pid = fork();

    // Check if fork() failed
    if (child_pid < 0) {
        perror("Fork failed");
        return 1;
    }

    // In the parent process
    if (child_pid > 0) {
        printf("Parent process: Created a child process with PID %d\n", child_pid);
        printf("Parent process is not waiting, creating a zombie.\n");
        // Do not wait for the child to exit
        sleep(30); // Sleep for a while to keep the parent alive
    }
    // In the child process
    else {
        printf("Child process: My PID is %d\n", getpid());
        printf("Child process is exiting.\n");
        exit(0); // Child exits immediately
    }

    return 0;
}

