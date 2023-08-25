#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    // Open the file for writing
    int file_descriptor = open("ticket_file.txt", O_WRONLY | O_CREAT, 0666);
    
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

    // Write the ticket number to the file
    int ticket_number = 1001; 
    dprintf(file_descriptor, "%d\n", ticket_number);

    // Release the write lock and close the file
    lock.l_type = F_UNLCK;
    if (fcntl(file_descriptor, F_SETLK, &lock) == -1) {
        perror("Failed to release write lock");
    }
    close(file_descriptor);

    printf("Ticket number %d written to file.\n", ticket_number);
    return 0;
}
