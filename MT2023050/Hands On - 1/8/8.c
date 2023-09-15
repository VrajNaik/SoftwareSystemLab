/*
============================================================================
Name : 8.c
Author : Vraj Jatin Naik
Description : Write a program to open a file in read only mode, read line by line and display each line as it is read.
              Close the file when end of file is reached.
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments (filename) is provided
    if (argc != 2) {
        printf("You have to give exactly one file name after command!!!\n");
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1; // Exit with an error status
    }

    // Open the file in read-only mode
    int fileDescriptor = open(argv[1], O_RDONLY);

    // Check for errors in opening the file
    if (fileDescriptor == -1) {
        perror("Oops...!!!Error opening file\nFile Doesn't Exists or Something went WRONG...Sorry Buddy");
        exit(1); // Exit with an error status
    }

    char buffer;
    int bytesRead;
    char line[1024];
    int lineIndex = 0;

    // Read and display the file line by line
    while ((bytesRead = read(fileDescriptor, &buffer, 1)) > 0) {
        if (buffer == '\n') {
            // Display the line when a newline character is encountered
            write(STDOUT_FILENO, line, lineIndex);
            write(STDOUT_FILENO, "\n", 1);
            lineIndex = 0;
            sleep(2); // Add a delay for readability
        } else {
            // Store characters in the line buffer until a newline is found
            line[lineIndex] = buffer;
            lineIndex++;
        }
    }

    // Close the file
    close(fileDescriptor);

    return 0; // Exit successfully
}

