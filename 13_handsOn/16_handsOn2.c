#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>


int acquireWriteLock(const char *filename) {
    int fd = open(filename, O_RDWR);
    if (fd == -1) {
        perror("Failed to open file");
        return -1;
    }

    struct flock lock;
    lock.l_type = F_WRLCK; 
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Failed to acquire write lock");
        close(fd);
        return -1;
    }

    printf("Write lock acquired. Press Enter to release the lock.\n");
    getchar();
    
    char ch;
    do {
        ch = getchar();
    } while (ch != '\n');

    lock.l_type = F_UNLCK; 
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Failed to release write lock");
        close(fd);
        return -1;
    }

    printf("Write lock released.\n");
    close(fd);
    return 0;
}


int acquireReadLock(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return -1;
    }

    struct flock lock;
    lock.l_type = F_RDLCK; 
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Failed to acquire read lock");
        close(fd);
        return -1;
    }

    printf("Read lock acquired. Press Enter to release the lock.\n");
    getchar();
    char ch;
    do {
        ch = getchar();
    } while (ch != '\n');

    lock.l_type = F_UNLCK;  // Unlock
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Failed to release read lock");
        close(fd);
        return -1;
    }

    printf("Read lock released.\n");
    close(fd);
    return 0;
}

int main() {
    char filename[256];
    int choice;

    while (1) {
        printf("Enter the filename: ");
        scanf("%255s", filename);
        getchar();

        printf("Choose an option:\n");
        printf("1. Acquire Write Lock\n");
        printf("2. Acquire Read Lock\n");
        printf("3. Quit\n");
        scanf("%d", &choice);

        if (choice == 1) {
            acquireWriteLock(filename);
        } else if (choice == 2) {
            acquireReadLock(filename);
        } else if (choice == 3) {
            break; 
        } else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}

