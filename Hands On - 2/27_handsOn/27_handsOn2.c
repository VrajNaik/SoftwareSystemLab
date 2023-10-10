#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

struct msgbuf {
    long mtype;  // Message type
    char mtext[256];  // Message data
};

int main() {
    key_t key;
    int msgqid;
    struct msgbuf message;
    int option;

    // Generate the same key as when creating the message queue
    if ((key = ftok(".", 'A')) == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Get the message queue ID
    if ((msgqid = msgget(key, 0666)) == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Display the menu
        printf("Menu:\n");
        printf("1. Receive with blocking (0 as flag)\n");
        printf("2. Receive with non-blocking (IPC_NOWAIT as flag)\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        if (option == 1) {
            // Receive a message with 0 as a flag (blocking)
            if (msgrcv(msgqid, &message, sizeof(message.mtext), 1, 0) == -1) {
                perror("msgrcv");
                exit(EXIT_FAILURE);
            }
            printf("Received message with 0 as a flag: %s\n", message.mtext);
        } else if (option == 2) {
            // Receive a message with IPC_NOWAIT as a flag (non-blocking)
            if (msgrcv(msgqid, &message, sizeof(message.mtext), 1, IPC_NOWAIT) == -1) {
                if (errno == ENOMSG) {
                    printf("No message available (IPC_NOWAIT flag).\n");
                } else {
                    perror("msgrcv");
                    exit(EXIT_FAILURE);
                }
            } else {
                printf("Received message with IPC_NOWAIT as a flag: %s\n", message.mtext);
            }
        } else if (option == 3) {
            // Exit the program
            printf("Exiting the program.\n");
            break;
        } else {
            printf("Invalid option. Please enter a valid choice.\n");
        }
    }

    return 0;
}

