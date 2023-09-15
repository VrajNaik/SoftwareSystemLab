/*
============================================================================
Name : 11.c
Author : Vraj Jatin Naik
Description : Write a program to open a file, duplicate the file descriptor and append the file with both the
              descriptors and check whether the file is updated properly or not.
              a. use dup
              b. use dup2
              c. use fcntl
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// Function to display the content of a file
void displayFileContent(const char *filename) {
    char buffer[256];
    int file_descriptor = open(filename, O_RDONLY);

    if (file_descriptor == -1) {
        perror("Error opening file for reading");
        exit(1);
    }

    ssize_t bytesRead;
    while ((bytesRead = read(file_descriptor, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytesRead);
    }

    close(file_descriptor);
}

int main() {
    printf("HandsOn 11 Write a program to open a file, duplicate the file descriptor and append the file with both the descriptors and check whether the file is updated properly or not.\na. use dup\nb. use dup2\nc. use fcntl\n");

    char filename[256];

    // Ask the user for the filename or filepath
    printf("Enter the filename or filepath: ");
    fgets(filename, sizeof(filename), stdin);

    // Remove the newline character from the input
    filename[strcspn(filename, "\n")] = '\0';

    int fd1, fd2, fd3, fd4;

    // Open the file for writing (creating if it doesn't exist)
    fd1 = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd1 == -1) {
        perror("Error opening file for writing");
        return 1;
    }

    const char *data = "This is some data written using fd1.\n";
    write(fd1, data, strlen(data));

    // Duplicate fd1 with fd2
    fd2 = dup(fd1);

    const char *data2 = "This is some data written using fd2. Which is created using dup.\n";
    write(fd2, data2, strlen(data2));

    // Open the file for writing in append mode
    fd3 = open(filename, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (fd3 == -1) {
        perror("Error opening file for writing (fd3)");
        return 1;
    }

    // Duplicate fd1 to fd3 using dup2
    if (dup2(fd1, fd3) == -1) {
        perror("Error duplicating file descriptor (dup2)");
        return 1;
    }

    const char *data3 = "This is some data written using fd3. Which is created using dup2\n";
    write(fd3, data3, strlen(data3));

    // Duplicate fd1 with a new file descriptor fd4 using fcntl
    fd4 = fcntl(fd1, F_DUPFD, 0);

    const char *data4 = "This is some data written using fd4. Which is created using fcntl.\n";
    write(fd4, data4, strlen(data4));

    // Display the content of the file
    displayFileContent(filename);

    // Close the file descriptors
    close(fd1);
    close(fd2);
    close(fd3);

    return 0;
}

