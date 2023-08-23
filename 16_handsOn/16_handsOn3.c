#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <signal.h>

// Function to handle the alarm signal (remind user to release the lock)
void alarm_handler(int signum) {
    printf("Ab release bhi kardo Please!!!! Press Enter to release the lock...\n");
    alarm(10);  // Set another alarm in 10 seconds
}

int main() {
    int fd;
    char filename[256];

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

    signal(SIGALRM, alarm_handler); 
    
    while (1) {
        printf("Yeh Raha Menu aapke computer screen par:\n");
        printf("1. Acquire Read Lock\n");
        printf("2. Acquire Write Lock\n");
        printf("3. Quit\n");
        printf("Enter your choice (1/2/3): ");
        scanf(" %c", &choice); 
        getchar(); 

        switch (choice) {
            case '1':
                lock.l_type = F_RDLCK;
                lock.l_whence = SEEK_SET;
                lock.l_start = 0;
                lock.l_len = 0;

                if (fcntl(fd, F_SETLK, &lock) == 0) {
                    printf("Read lock acquired.\n");
                    printf("Press Enter to release the lock...\n");
                    alarm(10);
                    getchar();
                    alarm(0); 
                    lock.l_type = F_UNLCK;
                    fcntl(fd, F_SETLK, &lock);
                    printf("Read lock released.\n");
                } else {
                    perror("Areeey !!! Unable to acquire read lock");
                }
                break;
            case '2':
                lock.l_type = F_WRLCK;
                lock.l_whence = SEEK_SET;
                lock.l_start = 0;
                lock.l_len = 0;

                if (fcntl(fd, F_SETLK, &lock) == 0) {
                    printf("Write lock acquired.\n");
                    printf("Press Enter to release the lock...\n");
                    alarm(10); 
                    getchar();
                    alarm(0); 
                    lock.l_type = F_UNLCK;
                    fcntl(fd, F_SETLK, &lock);
                    printf("Write lock released.\n");
                } else {
                    perror("Areeey !!! Unable to acquire write lock");
                }
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

