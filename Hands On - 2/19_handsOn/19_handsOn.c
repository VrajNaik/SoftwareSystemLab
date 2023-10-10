/*
============================================================================
Name : 19.c
Author : Vraj Jatin Naik
Description : Create a FIFO file by
              a. mknod command
              b. mkfifo command
              c. use strace command to find out, which command (mknod or mkfifo) is better.
              c. mknod system call
              d. mkfifo library function
      Date : 31th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_FILENAME_LENGTH 255

int main() {
    char choice;
    char filename[MAX_FILENAME_LENGTH + 1];

    printf("Choose an option:\n");
    printf("1). - Create FIFO using mknod\n");
    printf("2). - Create FIFO using mkfifo\n");
    printf("3). - Quit\n");

    while (1) {
        printf("Enter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                printf("Enter the name for FIFO (max %d characters): ", MAX_FILENAME_LENGTH);
                scanf("%s", filename);
                if (mknod(filename, S_IFIFO | 0666, 0) == -1) {
                    perror("mknod");
                    return 1;
                }
                printf("FIFO file '%s' created using mknod.\n", filename);
                break;

            case '2':
                printf("Enter the name for FIFO (max %d characters): ", MAX_FILENAME_LENGTH);
                scanf("%s", filename);
                if (mkfifo(filename, 0666) == -1) {
                    perror("mkfifo");
                    return 1;
                }
                printf("FIFO file '%s' created using mkfifo.\n", filename);
                break;

            case '3':
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
    }

    return 0;
}

