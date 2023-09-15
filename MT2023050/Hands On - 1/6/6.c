/*
============================================================================
Name : 6.c
Author : Vraj Jatin Naik
Description : Write a program to take input from STDIN and display on STDOUT. Use only read/write system calls
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main() {
    printf("HandsOn 6 Write a program to take input from STDIN and display on STDOUT. Use only read/write system calls.\n");
    
    char buffer[BUFFER_SIZE];
    int bytes_read;

    char enter_message[] = "You Entered: ";
    char prompt_message[] = "Please Enter Something: ";

    while (1) {
        // Write a prompt for the user to enter data
        write(STDOUT_FILENO, prompt_message, sizeof(prompt_message) - 1); // -1 to exclude the null terminator

        // Read data from STDIN
        bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));

        if (bytes_read > 0) {
            write(STDOUT_FILENO, enter_message, sizeof(enter_message) - 1); 

            // Write the data read from STDIN to STDOUT
            write(STDOUT_FILENO, buffer, bytes_read);
        }

        // Check for read errors
        if (bytes_read == -1) {
            perror("OOps...Something went wrong!!! Error reading from STDIN");
            return 1; // Exit with an error status
        }
    }

    return 0;
}
