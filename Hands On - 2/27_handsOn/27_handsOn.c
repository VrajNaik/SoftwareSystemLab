/*
============================================================================
Name : 27.c
Author : Vraj Jatin Naik
Description : Write a program to receive messages from the message queue.
              a. with 0 as a flag
              b. with IPC_NOWAIT as a flag
Date : 4th Oct, 2023.
============================================================================
*/
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

    // Generate a unique key for the message queue (use the same key as when creating it)
    if ((key = ftok(".", 'A')) == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Get the message queue ID
    if ((msgqid = msgget(key, 0666)) == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Receive a message with 0 as a flag (blocking)
    if (msgrcv(msgqid, &message, sizeof(message.mtext), 1, 0) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    printf("Received message with 0 as a flag: %s\n", message.mtext);

    // Receive a message with IPC_NOWAIT as a flag (non-blocking)
    if (msgrcv(msgqid, &message, sizeof(message.mtext), 1, IPC_NOWAIT) == -1) {
        perror("msgrcv");
        if (errno == ENOMSG) {
            printf("No message available (IPC_NOWAIT flag).\n");
        } else {
            exit(EXIT_FAILURE);
        }
    } else {
        printf("Received message with IPC_NOWAIT as a flag: %s\n", message.mtext);
    }

    return 0;
}

