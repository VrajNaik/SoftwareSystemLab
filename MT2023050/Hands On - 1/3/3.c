/*
============================================================================
Name : 1.c
Author : Vraj Jatin Naik
Description : Write a program to create a file and print the file descriptor value. Use creat ( ) system call.
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main() {
    char filename[100];

    mode_t mode = 0644; // Sets the file permission to read and write for the owner and read-only for others.
    
    // Prompt the user to enter a file path-name
    
    printf("Enter the name of file you want to create: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0'; // Remove newline
    
    // Check if the file exists by trying to open it
    int check_fd = open(filename, O_RDONLY);
    if (check_fd == -1) {
        if (errno == ENOENT) {
            // File doesn't exist, so create it
            int fd = creat(filename, mode);

            // Check for errors in creating the file
            if (fd == -1) {
                perror("Sorry buddy !!! Error creating file");
                exit(EXIT_FAILURE);
            }
            
            printf("Wohhooo !!!!! Congratulations !!! ;) \n File created successfully.\n");
            printf("File descriptor value: %d\n", fd);

            // Close the file
            close(fd);
        } 
        else {
            perror("Sorry Buddy!!! Something went wrong...Error opening file");
            exit(EXIT_FAILURE);
        }
    } else {
        // File exists
        close(check_fd);
        fprintf(stderr, "Ooops.......!!! It'seems like File already exists. Give some other name man.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

