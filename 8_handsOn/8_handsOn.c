#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int file_fd = open(argv[1], O_RDONLY);
    if (file_fd == -1) {
        perror("Error opening file");
        return 1;
    }

    char buffer[1024];
    int bytes_read;

    while ((bytes_read = read(file_fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read); 
    }

    if (bytes_read == -1) {
        perror("Error reading from file");
        close(file_fd);
        return 1;
    }

    close(file_fd);

    return 0;
}
