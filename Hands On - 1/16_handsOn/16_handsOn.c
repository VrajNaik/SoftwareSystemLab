#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>

int main() {
    int fd;
    char filename[256];
    printf("Write a program to perform mandatory locking.\na. Implement write lock\nb. Implement read lock.\n\n");

    printf("Enter the filename: ");
    scanf("%255s", filename);
    getchar(); // Consume the newline character

    fd = open(filename, O_RDWR | O_CREAT, 0666);

    if (fd == -1) {
        perror("Unable to open file");
        exit(1);
    }

    char choice;
    struct flock lock;

    while (1) {
        printf("Yeh Raha Menu aapke computer screen par:\n");
        printf("1. Acquire Read Lock\n");
        printf("2. Acquire Write Lock\n");
        printf("3. Quit\n");
        printf("Enter your choice (1/2/3): ");
        scanf(" %c", &choice); // Notice the space before %c to consume any whitespace
        getchar(); 

        switch (choice) {
            case '1':
                lock.l_type = F_RDLCK;
                lock.l_whence = SEEK_SET;
                lock.l_start = 0;
                lock.l_len = 0;

                while (fcntl(fd, F_SETLK, &lock) == -1) {
                    perror("Areeey !!! Unable to acquire read lock. Waiting...");
                    sleep(1); // Wait for 1 second before retrying
                }

                printf("Read lock acquired.\n");
                printf("Press Enter to release the lock...\n");
                getchar();
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                printf("Read lock released.\n");
                break;
            case '2':
                lock.l_type = F_WRLCK;
                lock.l_whence = SEEK_SET;
                lock.l_start = 0;
                lock.l_len = 0;

                while (fcntl(fd, F_SETLK, &lock) == -1) {
                    perror("Areeey !!! Unable to acquire write lock. Waiting...");
                    sleep(1); // Wait for 1 second before retrying
                }

                printf("Write lock acquired.\n");
                printf("Press Enter to release the lock...\n");
                getchar();
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                printf("Write lock released.\n");
                break;
            case '3':
                close(fd);
                exit(0);
            default:
                printf("What are you doing man !!! It's an Invalid choice. Please enter 1, 2, or 3.\n");
        }
    }

    return 0;
}

