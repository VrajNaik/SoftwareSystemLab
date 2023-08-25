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

int userLogin() {
    char username[20];
    char password[20];
    printf("Enter user username: ");
    scanf("%s", username);
    printf("Enter user password: ");
    scanf("%s", password);

    // Hardcoded user credentials (replace with a secure authentication method)
    if (strcmp(username, "user") == 0 && strcmp(password, "userpass") == 0) {
        return 1; // Successful login
    } else {
        return 0; // Login failed
    }
}

int main() {
    if (!userLogin()) {
        printf("User login failed. Exiting...\n");
        return 1;
    }

    printf("User logged in.\n");

    int userChoice;

    int fd = open("train_data1.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }

    do {
        printf("\nUser Menu:\n");
        printf("1. View available tickets\n");
        printf("2. Reserve a ticket\n");
        printf("3. View reservations\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &userChoice);

        switch (userChoice) {
            case 1:
                // View available tickets
                int trainNum;
                printf("Enter train number: ");
                scanf("%d", &trainNum);

                lockFile(fd); // Lock the file before reading

                struct Train train;
                lseek(fd, (trainNum - 101) * sizeof(struct Train), SEEK_SET);
                read(fd, &train, sizeof(struct Train));

                printf("Train Number: %d\n", train.train_num);
                printf("Available Tickets: %d\n", train.total_tickets);

                unlockFile(fd); // Unlock the file after reading
                break;

            case 2:
                // Reserve a ticket
                int trainNumReserve, numTickets;
                printf("Enter train number: ");
                scanf("%d", &trainNumReserve);
                printf("Enter number of tickets to reserve: ");
                scanf("%d", &numTickets);
                getchar();
                getchar(); 

                lockFile(fd); // Lock the file before updating

                struct Train trainReserve;
                lseek(fd, (trainNumReserve - 101) * sizeof(struct Train), SEEK_SET);
                read(fd, &trainReserve, sizeof(struct Train));

                if (numTickets <= (trainReserve.total_tickets - trainReserve.reserved_tickets)) {
                    trainReserve.total_tickets -= numTickets;
                    trainReserve.reserved_tickets += numTickets;
                    printf("Reserved %d tickets for Train %d.\n", numTickets, trainNumReserve);
                } 
                else {
                    printf("Not enough available tickets for Train %d.\n", trainNumReserve);
                }

                lseek(fd, (trainNumReserve - 101) * sizeof(struct Train), SEEK_SET);
                write(fd, &trainReserve, sizeof(struct Train));

                unlockFile(fd); // Unlock the file after updating
                break;

            case 3:
                // View reservations
                printf("Train Number\tReserved Tickets\n");
                for (int i = 0; i < 3; i++) {
                    struct Train train;
                    lseek(fd, i * sizeof(struct Train), SEEK_SET);
                    read(fd, &train, sizeof(struct Train));

                    printf("%d\t\t%d\t\t%d\n", train.train_num, train.reserved_tickets, train.total_tickets);
                }
                break;

            case 4:
                printf("User logged out.\n");
                break;

            default:
                printf("Invalid choice.\n");
        }
    } while (userChoice != 4);

    close(fd);

    return 0;
}
