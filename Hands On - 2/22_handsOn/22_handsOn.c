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
    char buff[256];
    int fd;
    char fifo_name[256];

    // Ask the user for the FIFO name
    printf("Enter the name of the FIFO: ");
    fgets(fifo_name, sizeof(fifo_name), stdin);
    fifo_name[strcspn(fifo_name, "\n")] = '\0'; // Remove the newline character

    // Check if the FIFO already exists, and create it if not
    if (access(fifo_name, F_OK) == -1) {
        if (mkfifo(fifo_name, 0666) == -1) {
            perror("mkfifo");
            exit(1);
        }
    }

    // Open the FIFO for reading
    fd = open(fifo_name, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    // Set the timeout to 10 seconds
    tv.tv_sec = 10;
    tv.tv_usec = 0;

    // Initialize a timer
    struct timeval start_time, current_time;
    gettimeofday(&start_time, NULL);

    // Display a "waiting" message
    printf("Waiting for data from the FIFO...\n");

    while (1) {
        // Calculate elapsed time
        gettimeofday(&current_time, NULL);
        double elapsed_time = difftime(current_time.tv_sec, start_time.tv_sec);

        // Use select to wait for data or timeout
        int result = select(fd + 1, &rfds, NULL, NULL, &tv);

        if (result == -1) {
            perror("select");
            exit(1);
        } else if (result > 0) {
            printf("Data is available now\n");
            ssize_t bytesRead = read(fd, buff, sizeof(buff));
            if (bytesRead == -1) {
                perror("read");
                exit(1);
            }
            buff[bytesRead] = '\0';  // Null-terminate the received data
            printf("Data from FIFO: %s\n", buff);
            break; // Exit the loop when data is available
        } else if (elapsed_time >= 10) {
            printf("No data is available for reading within 10 seconds\n");
            break; // Exit the loop after 10 seconds
        }
    }

    // Close the FIFO and do other tasks
    close(fd);

    return 0;
}

