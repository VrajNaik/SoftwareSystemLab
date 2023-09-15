/*
============================================================================
Name : 17.c
Author : Vraj Jatin Naik
Description : Write a program to execute ls -l | wc.
              a. use dup
              b. use dup2
              c. use fcntl
        Date: 31th Aug, 2023.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int choice;

    while (1) {
        printf("\nCatalog Menu:\n");
        printf("1. Using dup\n");
        printf("2. Using dup2\n");
        printf("3. Using fcntl\n");
        printf("4. Using all\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int original_fd = STDOUT_FILENO;
                int duplicate_fd = dup(original_fd);

                if (duplicate_fd == -1) {
                    perror("dup");
                    exit(EXIT_FAILURE);
                }

                printf("Using dup: This is a catalog entry.\n");

                // Close the duplicated file descriptor
                close(duplicate_fd);
                break;
            }

            case 2: {
                int original_fd = STDOUT_FILENO;
                int new_fd = open("/dev/null", O_WRONLY);

                if (new_fd == -1) {
                    perror("open");
                    exit(EXIT_FAILURE);
                }

                if (dup2(new_fd, original_fd) == -1) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }

                printf("Using dup2: This is a catalog entry.\n");

                // Close the new file descriptor
                close(new_fd);
                break;
            }

            case 3: {
                int original_fd = STDOUT_FILENO;
                int new_fd = fcntl(original_fd, F_DUPFD, 0);

                if (new_fd == -1) {
                    perror("fcntl");
                    exit(EXIT_FAILURE);
                }

                printf("Using fcntl: This is a catalog entry.\n");

                // Close the new file descriptor
                close(new_fd);
                break;
            }

            case 4: {
                int original_fd = STDOUT_FILENO;
                int duplicate_fd1 = dup(original_fd);

                if (duplicate_fd1 == -1) {
                    perror("dup");
                    exit(EXIT_FAILURE);
                }

                int duplicate_fd2 = fcntl(original_fd, F_DUPFD, 0);

                if (duplicate_fd2 == -1) {
                    perror("fcntl");
                    exit(EXIT_FAILURE);
                }

                int new_fd = open("/dev/null", O_WRONLY);

                if (new_fd == -1) {
                    perror("open");
                    exit(EXIT_FAILURE);
                }

                if (dup2(new_fd, original_fd) == -1) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }

                printf("Using all: This is a catalog entry.\n");

                // Close the duplicated file descriptors and the new file descriptor
                close(duplicate_fd1);
                close(duplicate_fd2);
                close(new_fd);
                break;
            }

            case 5:
                printf("Exiting the catalog program.\n");
                exit(EXIT_SUCCESS);

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
