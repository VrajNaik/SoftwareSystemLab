/*
============================================================================
Name : 16.c
Author : Vraj Jatin Naik
Description : Write a program to send and receive data from parent to child vice versa. Use two way
              communication.
        Date: 31th Aug, 2023.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h> // for memset

int main() {
    int fd1[2];
    int fd2[2];
    int pipe1 = pipe(fd1);
    int pipe2 = pipe(fd2);

    if (pipe1 == -1 || pipe2 == -1) {
        perror("Error: Unable to create pipes");
        return -1;
    }

    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("Error: Unable to fork");
        return -1;
    }

    if (child_pid == 0) { // Child process
        close(fd1[1]);
        close(fd2[0]);
        char child_buffer[100];

        int read_result = read(fd1[0], child_buffer, sizeof(child_buffer));
        if (read_result == -1) {
            perror("Error: Unable to read data from parent");
            return -1;
        }

        printf("Child received from parent: \"%s\"\n", child_buffer);

        printf("Child: Enter the message to send to parent:\n");
        memset(child_buffer, 0, sizeof(child_buffer)); // Clear the buffer
        scanf(" %[^\n]", child_buffer);
        int write_result = write(fd2[1], child_buffer, sizeof(child_buffer));
        if (write_result == -1) {
            perror("Error: Unable to write data to parent");
            return -1;
        }
    } else { // Parent process
        close(fd1[0]);
        close(fd2[1]);
        char parent_buffer[100];

        printf("Parent: Enter the message to send to child:\n");
        memset(parent_buffer, 0, sizeof(parent_buffer)); // Clear the buffer
        scanf(" %[^\n]", parent_buffer);
        int write_result = write(fd1[1], parent_buffer, sizeof(parent_buffer));
        if (write_result == -1) {
            perror("Error: Unable to write data to child");
            return -1;
        }

        int read_result = read(fd2[0], parent_buffer, sizeof(parent_buffer));
        if (read_result == -1) {
            perror("Error: Unable to read data from child");
            return -1;
        }

        printf("Parent received from child: \"%s\"\n", parent_buffer);
    }
    return 0;
}

