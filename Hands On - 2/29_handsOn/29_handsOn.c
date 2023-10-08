/*
============================================================================
Name : 29.c
Author : Vraj Jatin Naik
Description : Write a program to remove the message queue.
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
    int msgid;

    // Generate a unique key for the message queue
    key = ftok(".", 'M');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // Get the message queue ID
    msgid = msgget(key, 0);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    // Remove the message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }

    printf("Message queue removed successfully.\n");

    return 0;
}

