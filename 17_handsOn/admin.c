#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

// Define a structure for train details
struct Train {
    int train_num;
    int total_tickets;
    int reserved_tickets;
};

void lockFile(int fd) {
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    fcntl(fd, F_SETLKW, &lock);
}

void unlockFile(int fd) {
    struct flock lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    fcntl(fd, F_SETLK, &lock);
}

int adminLogin() {
    char username[20];
    char password[20];
    printf("Enter admin username: ");
    scanf("%s", username);
    printf("Enter admin password: ");
    scanf("%s", password);

    // Hardcoded admin credentials (replace with a secure authentication method)
    if (strcmp(username, "admin") == 0 && strcmp(password, "adminpass") == 0) {
        return 1; // Successful login
    } else {
        return 0; // Login failed
    }
}

int main() {
    if (!adminLogin()) {
        printf("Admin login failed. Exiting...\n");
        return 1;
    }

    printf("Admin logged in.\n");

    int adminChoice;

    int fd = open("train_data1.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }

    do {
        printf("\nAdmin Menu:\n");
        printf("1. Set number of tickets for a train\n");
        printf("2. View reservations\n");
        printf("3. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &adminChoice);

        switch (adminChoice) {
            case 1:
                // Set the number of tickets for a train
                int trainNum, newTotal;
                printf("Enter train number: ");
                scanf("%d", &trainNum);
                printf("Enter new total tickets: ");
                scanf("%d", &newTotal);

                lockFile(fd); // Lock the file before updating

                struct Train train;
                lseek(fd, (trainNum - 101) * sizeof(struct Train), SEEK_SET);
                read(fd, &train, sizeof(struct Train));

                train.total_tickets = newTotal;

                lseek(fd, (trainNum - 101) * sizeof(struct Train), SEEK_SET);
                if(write(fd, &train, sizeof(struct Train)) != sizeof(struct Train)) {
                    printf("Error\n");
                }
                else{
                    printf("Total tickets for Train %d updated to %d.\n", trainNum, newTotal);
                }
                unlockFile(fd); // Unlock the file after updating
                break;

            case 2:
                // View reservations
                printf("Train Number\tReserved Tickets\n");
                for (int i = 0; i < 3; i++) {
                    struct Train train;
                    lseek(fd, i * sizeof(struct Train), SEEK_SET);
                    read(fd, &train, sizeof(struct Train));

                    printf("%d\t\t%d\n", train.train_num, train.reserved_tickets);
                }
                break;

            case 3:
                printf("Admin logged out.\n");
                system("./catalog");
                break;

            default:
                printf("Invalid choice.\n");
                continue;
        }
    } while (adminChoice != 3);

    close(fd);

    return 0;
}
