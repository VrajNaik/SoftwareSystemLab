#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

struct Record {
    int record_num;
    char a1[100];
};

int main() {
    int fd = open("records.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }

    struct Record record;

    for (int i = 0; i < 3; i++) {
        record.record_num = 1 + i; // Example train numbers
        record.a1 = "hiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii";
        write(fd, &record, sizeof(struct Train));
    }

    close(fd);

    printf("Record data initialized.\n");

    return 0;
}
