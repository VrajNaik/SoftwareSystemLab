#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf("Hands On 12 Write a program to find out the opening mode of a file. Use fcntl.\n");

    // Check for the correct number of command-line arguments
    if (argc != 3) {
        printf("Please Provide Proper command line arguments!!!\n2");
        fprintf(stderr, "Usage: %s <filename> <mode>\n", argv[0]);
        fprintf(stderr, "  <mode> should be 'r' for read, 'w' for write, or 'rw' for read-write\n");
        return 1;
    }

    const char *filename = argv[1];
    const char *mode = argv[2];
    int mode_flags;

    // Determine the file access mode based on the user input
    if (strcmp(mode, "r") == 0) {
        mode_flags = O_RDONLY;
    } 
    else if (strcmp(mode, "w") == 0) {
        mode_flags = O_WRONLY;
    } 
    else if (strcmp(mode, "rw") == 0) {
        mode_flags = O_RDWR;
    } 
    else {
        fprintf(stderr, "Invalid mode argument. Use 'r', 'w', or 'rw'.\n");
        return 1;
    }

    // Open the file using the determined access mode
    int fd = open(filename, mode_flags);

    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Get the file status flags using fcntl
    int status = fcntl(fd, F_GETFL);

    // Extract the access mode from the status flags
    int access_mode = (O_ACCMODE & status);

    // Print the opening mode based on the access mode
    switch (access_mode) {
        case O_RDONLY:
            printf("%s is open in READ-ONLY mode.\n", filename);
            break;
        case O_WRONLY:
            printf("%s is open in WRITE-ONLY mode.\n", filename);
            break;
        case O_RDWR:
            printf("%s is open in READ-WRITE mode.\n", filename);
            break;
        default:
            printf("%s is open with an unknown mode.\n", filename);
    }

    // Check for specific flags
    if (status & O_CREAT) {
        printf("%s is open with flags O_CREAT.\n", filename);
    }
    if (status & O_TRUNC) {
        printf("%s is open with flags O_TRUNC.\n", filename);
    }
    if (status & O_APPEND) {
        printf("%s is open in Append mode.\n", filename);
    }

    close(fd);

    return 0;
}

