/*
============================================================================
Name : 23.c
Author : Vraj Jatin Naik
Description : Write a program to print the maximum number of files can be opened within a process and
              size of a pipe (circular buffer).
Date : 2nd Oct, 2023.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>

#define PIPE_SIZE 5412  // Set your desired pipe size here

int main() {
    // Get the maximum number of files that can be opened within a process
    struct rlimit rlim;
    if (getrlimit(RLIMIT_NOFILE, &rlim) == 0) {
        printf("Maximum number of files that can be opened within a process: %lld\n", (long long)rlim.rlim_cur);
    } else {
        perror("getrlimit");
    }

    // Create a pipe and measure its size
    int pipe_fds[2];
    if (pipe(pipe_fds) == 0) {
        char buffer[PIPE_SIZE];
        ssize_t bytes_written = 0;
        ssize_t total_written = 0;

        while (total_written < PIPE_SIZE) {
            bytes_written = write(pipe_fds[1], buffer, sizeof(buffer));
            if (bytes_written < 0) {
                perror("write");
                break;
            }
            total_written += bytes_written;
        }

        printf("Size of a pipe (circular buffer): %zd bytes\n", total_written);

        close(pipe_fds[0]);
        close(pipe_fds[1]);
    } else {
        perror("pipe");
    }

    return 0;
}

