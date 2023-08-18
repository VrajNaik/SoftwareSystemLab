#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd; 
    fd = open("test_file.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }
    const char data1[] = "abcdefghij";
    int bytesWritten = write(fd, data1, sizeof(data1) - 1);
    if (bytesWritten == -1) {
        perror("Error writing data");
        close(fd);
        return 1;
    }

    int newPosition = lseek(fd, 10, SEEK_CUR);
    if (newPosition == -1) {
        perror("Error seeking");
        close(fd);
        return 1;
    }
    const char data2[] = "ABCDEFGHIJ";
    bytesWritten = write(fd, data2, sizeof(data2) - 1);
    if (bytesWritten == -1) {
        perror("Error writing data");
        close(fd);
        return 1;
    }
    printf("lseek return value: %ld\n", newPosition);
    close(fd);
    return 0;
}
