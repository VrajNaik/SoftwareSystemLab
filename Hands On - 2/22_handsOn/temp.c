#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main() {
    fd_set rfds;
    struct timeval tv;
    int fd;
    char buff[256];

    // Create the FIFO if it doesn't exist
    mkfifo("myfifo", 0666);

    // Open the FIFO for reading
    fd = open("myfifo", O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    tv.tv_sec = 10;  // Wait for 10 seconds
    tv.tv_usec = 0;

    int retval = select(fd + 1, &rfds, NULL, NULL, &tv);

    if (retval == -1) {
        perror("select");
        exit(1);
    } else if (retval == 0) {
        printf("No data is available for reading within 10 seconds\n");
    } else {
        if (FD_ISSET(fd, &rfds)) {
            printf("Data is available now\n");
            ssize_t bytesRead = read(fd, buff, sizeof(buff));
            if (bytesRead == -1) {
                perror("read");
                exit(1);
            }
            if (bytesRead > 0) {
                buff[bytesRead] = '\0';  // Null-terminate the string
                printf("Data from FIFO: %s\n", buff);
            } else {
                printf("No data read from FIFO\n");
            }
        }
    }

    // Use a second select call with a timeout of 0 to check if 10 seconds have passed
    FD_ZERO(&rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    retval = select(0, NULL, NULL, NULL, &tv);

    if (retval == -1) {
        perror("select");
        exit(1);
    } else if (retval == 0) {
        printf("10 seconds have passed\n");
    }

    close(fd);

    // Do other tasks

    return 0;
}

