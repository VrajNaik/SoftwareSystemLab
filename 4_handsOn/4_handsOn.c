#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main() {
    char file_path[100];
    int fd;

    printf("Enter the file path: ");
    fgets(file_path, sizeof(file_path), stdin);
    file_path[strcspn(file_path, "\n")] = '\0'; // Remove newline

    fd = open(file_path, O_RDWR);
    if (fd == -1) {
        perror("Error opening file with read-write mode");
    } else {
        printf("File opened successfully with read-write mode.\n");
        close(fd);
    }

    fd = open(file_path, O_RDWR | O_EXCL);
    if (fd == -1) {
        if (errno == EEXIST) {
            printf("File already exists. O_EXCL flag test failed.\n");
        } else {
            perror("Error opening file with O_EXCL flag");
        }
    } else {
        printf("File opened successfully with O_EXCL flag.\n");
        close(fd);
    }

    return 0;
}
