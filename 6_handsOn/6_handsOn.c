#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main() {
    printf("HandsOn 6 Write a program to take input from STDIN and display on STDOUT. Use only read/write system calls.\n");
    char buffer[BUFFER_SIZE];
    int bytes_read;

    while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    if (bytes_read == -1) {
        perror("Error reading from STDIN");
        return 1;
    }

    return 0;
}
