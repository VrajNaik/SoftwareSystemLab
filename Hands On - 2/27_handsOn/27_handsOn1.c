#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf {
    long mtype;  // Message type
    char mtext[256];  // Message data
};

int main() {
    key_t key;
    int msgqid;
    struct msgbuf message;

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

    // Ask the user for a message
    printf("Enter a message to send to the queue: ");
    fgets(message.mtext, sizeof(message.mtext), stdin);

    // Remove the trailing newline character if present
    size_t length = strlen(message.mtext);
    if (length > 0 && message.mtext[length - 1] == '\n') {
        message.mtext[length - 1] = '\0';
    }

    // Set the message type (can be any positive integer)
    message.mtype = 1;

    // Send the message to the queue
    if (msgsnd(msgqid, &message, sizeof(message.mtext), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    printf("Message sent to the queue: %s\n", message.mtext);

    return 0;
}

