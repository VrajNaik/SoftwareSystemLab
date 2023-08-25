#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define TICKET_FILE "ticket_data.txt"
#define ADMIN_ID "admin"
#define ADMIN_PASSWORD "password"

int checkAvailability() {
    int fd = open(TICKET_FILE, O_RDONLY);
    if (fd == -1) {
        perror("Error opening ticket data file");
        return -1;
    }

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = sizeof(int);

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error acquiring read lock");
        close(fd);
        return -1;
    }

    int availableTickets;
    if (read(fd, &availableTickets, sizeof(int)) == -1) {
        perror("Error reading ticket data");
        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1) {
            perror("Error releasing read lock");
        }
        close(fd);
        return -1;
    }

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error releasing read lock");
    }

    close(fd);
    return availableTickets;
}


int reserveTickets(int numTickets) {
    int fd = open(TICKET_FILE, O_RDWR);
    if (fd == -1) {
        perror("Error opening ticket data file");
        return -1;
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = sizeof(int);

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error acquiring write lock");
        close(fd);
        return -1;
    }

    int availableTickets;
    if (read(fd, &availableTickets, sizeof(int)) == -1) {
        perror("Error reading ticket data");
        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1) {
            perror("Error releasing write lock");
        }
        close(fd);
        return -1;
    }

    if (numTickets > availableTickets) {
        printf("Not enough tickets available.\n");
        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1) {
            perror("Error releasing write lock");
        }
        close(fd);
        return -1;
    }

    availableTickets -= numTickets;

    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("Error seeking to the beginning of the file");
        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1) {
            perror("Error releasing write lock");
        }
        close(fd);
        return -1;
    }

    if (write(fd, &availableTickets, sizeof(int)) == -1) {
        perror("Error writing updated ticket data");
        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1) {
            perror("Error releasing write lock");
        }
        close(fd);
        return -1;
    }

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error releasing write lock");
    }

    close(fd);

    int reservation_fd = open("reservations.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (reservation_fd == -1) {
        perror("Error opening reservation file");
        return -1;
    }

    char reservationMsg[100];
    snprintf(reservationMsg, sizeof(reservationMsg), "Reserved %d tickets\n", numTickets);
    if (write(reservation_fd, reservationMsg, strlen(reservationMsg)) == -1) {
        perror("Error writing reservation");
    }

    close(reservation_fd);

    return 0;
}

void viewReservations() {
    int reservation_fd = open("reservations.txt", O_RDONLY);
    if (reservation_fd == -1) {
        perror("Error opening reservation file");
        return;
    }

    char buffer[100];
    ssize_t bytes_read;

    printf("Reservations:\n");

    while ((bytes_read = read(reservation_fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    close(reservation_fd);
}


void setAvailableTickets() {
    int newTickets;
    printf("Enter the new available ticket count: ");
    scanf("%d", &newTickets);

    int fd = open(TICKET_FILE, O_RDWR);
    if (fd == -1) {
        perror("Error opening ticket data file");
        return;
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = sizeof(int);

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error acquiring write lock");
        close(fd);
        return;
    }

    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("Error seeking to the beginning of the file");
        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1) {
            perror("Error releasing write lock");
        }
        close(fd);
        return;
    }

    if (write(fd, &newTickets, sizeof(int)) == -1) {
        perror("Error writing updated ticket data");
        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1) {
            perror("Error releasing write lock");
        }
        close(fd);
        return;
    }

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error releasing write lock");
    }

    close(fd);
}

int main() {
    int choice, numTickets;
    char userType[10];

    while (1) {
        printf("\nTicket Reservation System\n");
        printf("1. User Login\n");
        printf("2. Admin Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("User Login\n");
                printf("Enter 'user' as the user type.\n");
                printf("User: ");
                scanf("%s", userType);

                if (strcmp(userType, "user") != 0) {
                    printf("Invalid user type.\n");
                    break;
                }

                while (1) {
                    printf("1. Check Availability\n");
                    printf("2. Reserve Tickets\n");
                    printf("3. Exit\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice);

                    switch (choice) {
                        case 1:
                            printf("Available tickets: %d\n", checkAvailability());
                            break;
                        case 2:
                            printf("Enter the number of tickets to reserve: ");
                            scanf("%d", &numTickets);
                            if (reserveTickets(numTickets) == 0) {
                                printf("Reservation successful.\n");
                            }
                            break;
                        case 3:
                            printf("Exiting user mode.\n");
                            break;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }

                    if (choice == 3) {
                        break; 
                    }
                }
                break;
            case 2:
                printf("Admin Login\n");
                char adminId[50];
                char adminPassword[50];
                
                printf("Enter Admin ID: ");
                scanf("%s", adminId);
                printf("Enter Password: ");
                scanf("%s", adminPassword);

                if (strcmp(adminId, ADMIN_ID) == 0 && strcmp(adminPassword, ADMIN_PASSWORD) == 0) {
                    printf("Admin Login Successful.\n");

                    while (1) {
                        printf("1. View Reservations\n");
                        printf("2. Set Available Tickets\n");
                        printf("3. Exit\n");
                        printf("Enter your choice: ");
                        scanf("%d", &choice);

                        switch (choice) {
                            case 1:
                                viewReservations();
                                break;
                            case 2:
                                setAvailableTickets();
                                break;
                            case 3:
                                printf("Exiting admin mode.\n");
                                break;                        
                            default:
                                printf("Invalid choice. Please try again.\n");
                        }

                        if (choice == 3) {
                            break; 
                        }
                    }
                } else {
                    printf("Admin Login Failed. Please try again.\n");
                }
                break;
            case 3:
                printf("Exiting the program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
