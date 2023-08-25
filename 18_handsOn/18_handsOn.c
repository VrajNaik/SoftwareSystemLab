#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// Function to acquire a write lock on a record
void acquireWriteLock(int fd, int recordNumber) {
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = recordNumber * sizeof(char) * 100; // Assuming each record is 100 bytes
    lock.l_len = sizeof(char) * 100;

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error acquiring write lock");
        exit(1);
    }
}

// Function to release a lock
void releaseLock(int fd) {
    struct flock lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error releasing lock");
        exit(1);
    }
}

int main() {
    int fd = open("records.txt", O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }

    // Create three records
    char record1[100] = "Record 1:This is the first record\n";
    char record2[100] = "Record 2:This is the second record\n";
    char record3[100] = "Record 3:This is the third record\n";
    

    write(fd, record1, sizeof(char) * sizeof(record1)/sizeof(char));
    write(fd, record2, sizeof(char) * sizeof(record2)/sizeof(char));
    write(fd, record3, sizeof(char) * sizeof(record3)/sizeof(char));

    int choice, recordNumber;

    while (1) {
        printf("1. Read Record\n");
        printf("2. Write Record\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear the newline character from the buffer

        switch (choice) {
            case 1:
                printf("Enter the record number to read (1, 2, or 3): ");
                scanf("%d", &recordNumber);
                getchar(); // Clear the newline character from the buffer

                if (recordNumber >= 1 && recordNumber <= 3) {
                    // Acquire a read lock
                    struct flock lock;
                    lock.l_type = F_RDLCK;
                    lock.l_whence = SEEK_SET;
                    lock.l_start = recordNumber * sizeof(char) * 100;
                    lock.l_len = sizeof(char) * 100;

                    if (fcntl(fd, F_SETLKW, &lock) == -1) {
                        perror("Error acquiring read lock");
                        exit(1);
                    }

                    // Read and print the record
                    char buffer[100];
                    lseek(fd, (recordNumber - 1) * sizeof(char) * 100, SEEK_SET);
                    read(fd, buffer, sizeof(char) * 100);
                    printf("Record %d: %s\n", recordNumber, buffer);

                    // Release the read lock
                    lock.l_type = F_UNLCK;
                    if (fcntl(fd, F_SETLK, &lock) == -1) {
                        perror("Error releasing read lock");
                        exit(1);
                    }
                } else {
                    printf("Invalid record number.\n");
                }
                break;
            case 2:
                printf("Enter the record number to write (1, 2, or 3): ");
                scanf("%d", &recordNumber);
                getchar(); // Clear the newline character from the buffer

                if (recordNumber >= 1 && recordNumber <= 3) {
                    // Acquire a write lock
                    acquireWriteLock(fd, recordNumber);

                    // Read the record before modification
                    char buffer[100];
                    lseek(fd, (recordNumber - 1) * sizeof(char) * 100, SEEK_SET);
                    read(fd, buffer, sizeof(char) * 100);
                    printf("Current content of Record %d: %s\n", recordNumber, buffer);

                    // Modify the record
                    char newContent[100];
                    printf("Enter new content for Record %d: ", recordNumber);
                    scanf(" %[^\n]", newContent);
                    lseek(fd, (recordNumber - 1) * sizeof(char) * 100, SEEK_SET);
                    write(fd, newContent, sizeof(char) * 100);

                    printf("Record %d updated.\n", recordNumber);

                    // Release the write lock
                    releaseLock(fd);
                } else {
                    printf("Invalid record number.\n");
                }
                break;
            case 3:
                close(fd);
                printf("Exiting the program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
