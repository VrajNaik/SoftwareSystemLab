#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

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
    const char *filename = "temp.txt";
    int fd1, fd2, fd3, fd4;

    fd1 = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd1 == -1) {
        perror("Error opening file for writing");
        return 1;
    }

    const char *data = "This is some data written using fd1.\n";
    write(fd1, data, strlen(data));

    fd2 = dup(fd1);

    const char *data2 = "This is some data written using fd2. Which is created using dup.\n";
    write(fd2, data2, strlen(data2));

    fd3 = open(filename, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (fd3 == -1) {
        perror("Error opening file for writing (fd3)");
        return 1;
    }

    if (dup2(fd1, fd3) == -1) {
        perror("Error duplicating file descriptor (dup2)");
        return 1;
    }

    const char *data3 = "This is some data written using fd3. Which is created using dup2\n";
    write(fd3, data3, strlen(data3));

    fd4 = fcntl(fd1, F_DUPFD, 0);

    const char *data4 = "This is some data written using fd4. Which is created using fcntl.\n";
    write(fd4, data4, strlen(data4));
    displayFileContent(filename);

    close(fd1);
    close(fd2);
    close(fd3);

    return 0;
}
