/*
============================================================================
Name : 24.c
Author : Vraj Jatin Naik
Description : Write a program to create a message queue and print the key and message queue id.
Date : 2nd Oct, 2023.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    key_t key;
    int msgqid;

    // Generate a unique key for the message queue
    if ((key = ftok(".", 'A')) == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Create a message queue with read/write permissions for the owner
    if ((msgqid = msgget(key, IPC_CREAT | 0666)) == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("Message queue created with key: %d\n", key);
    printf("Message queue ID: %d\n", msgqid);

    return 0;
}


