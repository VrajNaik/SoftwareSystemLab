/*
============================================================================
Name : 10.c
Author : Vraj Jatin Naik
Description : Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10
              bytes (use lseek) and write again 10 bytes.
              a. check the return value of lseek
              b. open the file with od and check the empty spaces in between the data.
Date: 28th Aug, 2023.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd; // File descriptor
    char fileName[256]; // Buffer to store the file name

    // Prompt the user to enter the file name
    printf("Enter the file name: ");
    fgets(fileName, sizeof(fileName), stdin);

    // Remove the newline character from the input
    fileName[strcspn(fileName, "\n")] = '\0';

    // Open or create the specified file with read-write permissions
    //file will be created if not exists
    fd = open(fileName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening file");
        return 1; // Exit with an error status if the file cannot be opened
    }

    const char data1[] = "MT2023050-";
    int bytesWritten = write(fd, data1, sizeof(data1) - 1);

    if (bytesWritten == -1) {
        perror("Error writing data");
        close(fd);
        return 1; // Exit with an error status if writing fails
    }

    // Move the file pointer by 10 bytes from the current position
    int newPosition = lseek(fd, 10, SEEK_CUR);
    if (newPosition == -1) {
        perror("Error seeking");
        close(fd);
        return 1; // Exit with an error status if seeking fails
    }

    const char data2[] = " VrajJNaik";
    bytesWritten = write(fd, data2, sizeof(data2) - 1);
    if (bytesWritten == -1) {
        perror("Error writing data");
        close(fd);
        return 1; // Exit with an error status if writing fails
    }

    // Print the return value of lseek
    printf("lseek return value: %ld\n", (long)newPosition);

    // Close the file
    close(fd);

    // Use the 'od' command to view the file in octal dump format
    char odCommand[512]; // Increase buffer size
    snprintf(odCommand, sizeof(odCommand), "od -c %s", fileName);
    system(odCommand);

    return 0; // Exit successfully
}

