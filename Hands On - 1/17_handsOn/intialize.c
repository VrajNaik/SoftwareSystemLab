#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

struct Train {
    int train_num;
    int total_tickets;
    int reserved_tickets;
};

int main() {
    int fd = open("train_data1.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }

    struct Train train;

    for (int i = 0; i < 3; i++) {
        train.train_num = 101 + i; // Example train numbers
        train.total_tickets = 100; // Initial ticket count
        train.reserved_tickets = 0; // No reserved tickets initially

        write(fd, &train, sizeof(struct Train));
    }

    close(fd);

    printf("Train data initialized.\n");

    return 0;
}
