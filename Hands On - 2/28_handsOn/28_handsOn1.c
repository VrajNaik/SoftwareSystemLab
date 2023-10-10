/*
============================================================================
Name : 28_1.c
Author : Vraj Jatin Naik
Description : Write a program to change the exiting message queue permission. (use msqid_ds structure). Program to create message queue.
Date : 5th Oct, 2023.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key;
    int msgid;
    struct msg_buffer message;

    // Generate a unique key for the message queue
    key = ftok("/tmp", 'A');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // Create a message queue with read and write permissions for the owner only
    msgid = msgget(key, 0600 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Message queue created with ID %d\n", msgid);

    // Send a message to the queue (optional)
    message.msg_type = 1;
    strcpy(message.msg_text, "Hello, Message Queue!");
    if (msgsnd(msgid, &message, sizeof(message), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }

    return 0;
}

