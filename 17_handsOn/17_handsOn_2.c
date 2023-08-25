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

    // Initialize the database
    db.train_num = 123; // Example train number
    db.ticket_count = 0;

    // Open the file for writing
    fd = open("record.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }

    // Write the initial database to the file
    if (write(fd, &db, sizeof(db)) == -1) {
        perror("Error writing file");
        close(fd);
        exit(1);
    }

    close(fd);
    
    printf("Initial ticket count: %d\n", db.ticket_count);

    return 0;
}
