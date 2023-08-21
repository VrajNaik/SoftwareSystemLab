#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]) {
    printf("Hands On 12 Write a program to find out the opening mode of a file. Use fcntl.\n");
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <mode>\n", argv[0]);
        fprintf(stderr, "  <mode> should be 'r' for read, 'w' for write, or 'rw' for read-write\n");
        return 1;
    }

    const char *filename = argv[1];
    const char *mode = argv[2];
    int mode_flags;

    if (strcmp(mode, "r") == 0) {
        mode_flags = O_RDONLY;
    } else if (strcmp(mode, "w") == 0) {
        mode_flags = O_WRONLY;
    } else if (strcmp(mode, "rw") == 0) {
        mode_flags = O_RDWR;
    } else {
        fprintf(stderr, "Invalid mode argument. Use 'r', 'w', or 'rw'.\n");
        return 1;
    }

    int fd = open(filename, mode_flags);
    printf("%d", fd);

    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    if (mode_flags == O_RDONLY) {
        printf("%s is open in READ-ONLY mode.\n", filename);
    } else if (mode_flags == O_WRONLY) {
        printf("%s is open in WRITE-ONLY mode.\n", filename);
    } else if (mode_flags == O_RDWR) {
        printf("%s is open in READ-WRITE mode.\n", filename);
    } else {
        printf("%s is open with an unknown mode.\n", filename);
    }

    close(fd);

    return 0;
}
