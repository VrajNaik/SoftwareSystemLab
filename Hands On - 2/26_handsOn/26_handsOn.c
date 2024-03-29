/*
============================================================================
Name : 26.c
Author : Vraj Jatin Naik
Description : Write a program to send messages to the message queue. Check $ipcs -q
Date : 5th Oct, 2023.
============================================================================
*/
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

    // Generate a unique key for the message queue (use the same key as when creating it)
    if ((key = ftok(".", 'A')) == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Try to get the message queue ID, create it if it doesn't exist
    if ((msgqid = msgget(key, IPC_CREAT | 0666)) == -1) {
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

