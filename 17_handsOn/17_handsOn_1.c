#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

struct TicketDB {
    int train_num;
    int ticket_count;
};

int main() {
    int fd;
    struct TicketDB db;
    struct flock lock;

    // Open the file for reading and writing
    fd = open("record.txt", O_RDWR);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }

    // Lock the file for writing
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error locking file");
        close(fd);
        exit(1);
    }

    // Read the current ticket count
    if (read(fd, &db, sizeof(db)) == -1) {
        perror("Error reading file");
        close(fd);
        exit(1);
    }

    // Increment the ticket count
    db.ticket_count++;
    
    // Print the new ticket count
    printf("New ticket count: %d\n", db.ticket_count);

    // Write the updated database back to the file
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("Error seeking file");
        close(fd);
        exit(1);
    }

    if (write(fd, &db, sizeof(db)) == -1) {
        perror("Error writing file");
        close(fd);
        exit(1);
    }

    // Unlock the file
    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error unlocking file");
        close(fd);
        exit(1);
    }

    close(fd);

    return 0;
}
