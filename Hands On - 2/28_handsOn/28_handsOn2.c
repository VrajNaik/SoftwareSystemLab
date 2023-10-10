/*
============================================================================
Name : 28_2.c
Author : Vraj Jatin Naik
Description : Write a program to change the exiting message queue permission. (use msqid_ds structure). Program to modify queue permissions.
Date : 5th Oct, 2023.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

int main() {
    key_t key;
    int msgid;
    struct msqid_ds msq_ds;

    // Generate the same key used to create the message queue
    key = ftok("/tmp", 'A');
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

    // Get the current permissions
    if (msgctl(msgid, IPC_STAT, &msq_ds) == -1) {
        perror("msgctl");
        exit(1);
    }

    // Modify the permissions (e.g., give read permission to others)
    msq_ds.msg_perm.mode |= 0444; // Add read permission for others

    // Update the message queue permissions
    if (msgctl(msgid, IPC_SET, &msq_ds) == -1) {
        perror("msgctl");
        exit(1);
    }

    printf("Message queue permissions updated.\n");

    return 0;
}

