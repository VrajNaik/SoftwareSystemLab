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
#include <mqueue.h>

int main() {
    // Define the key for the message queue
    key_t queue_key = ftok("/tmp", 'A'); // You can use a different path and identifier as needed

    // Define the name of the message queue
    const char* queue_name = "/my_queue";

    // Define the message queue attributes
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10; // Maximum number of messages in the queue
    attr.mq_msgsize = 256; // Maximum message size
    attr.mq_curmsgs = 0; // Current number of messages in the queue

    // Create the message queue with the specified key
    mqd_t mq = mq_open(queue_name, O_CREAT | O_RDWR, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    printf("Message queue created.\n");

    // Close the message queue
    mq_close(mq);

    return 0;
}

