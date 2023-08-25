#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>

int main() {
    // Open the file for reading and writing
    int file_descriptor = open("ticket_file.txt", O_RDWR);
    
    if (file_descriptor == -1) {
        perror("Failed to open the file");
        exit(1);
    }

    struct flock lock;
    lock.l_type = F_WRLCK;  // Write lock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    // Acquire the write lock
    if (fcntl(file_descriptor, F_SETLKW, &lock) == -1) {
        perror("Failed to acquire write lock");
        close(file_descriptor);
        exit(1);
    }

    // Read the ticket number from the file
    int ticket_number;
    if (read(file_descriptor, &ticket_number, sizeof(int)) == -1) {
        perror("Failed to read ticket number");
        lock.l_type = F_UNLCK;
        if (fcntl(file_descriptor, F_SETLK, &lock) == -1) {
            perror("Failed to release write lock");
        }
        close(file_descriptor);
        exit(1);
    }

    // Increment the ticket number
    ticket_number++;

    // Rewind the file and write the updated ticket number
    if (lseek(file_descriptor, 0, SEEK_SET) == -1) {
        perror("Failed to rewind file");
        lock.l_type = F_UNLCK;
        if (fcntl(file_descriptor, F_SETLK, &lock) == -1) {
            perror("Failed to release write lock");
        }
        close(file_descriptor);
        exit(1);
    }
    
    if (write(file_descriptor, &ticket_number, sizeof(int)) == -1) {
        perror("Failed to write updated ticket number");
        lock.l_type = F_UNLCK;
        if (fcntl(file_descriptor, F_SETLK, &lock) == -1) {
            perror("Failed to release write lock");
        }
        close(file_descriptor);
        exit(1);
    }

    // Release the write lock and close the file
    lock.l_type = F_UNLCK;
    if (fcntl(file_descriptor, F_SETLK, &lock) == -1) {
        perror("Failed to release write lock");
    }
    close(file_descriptor);

    printf("New ticket number: %d\n", ticket_number);
    return 0;
}
