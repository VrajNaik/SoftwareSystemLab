/*
============================================================================
Name : 25.c
Author : Vraj Jatin Naik
Description : Write a program to print a message queue's (use msqid_ds and ipc_perm structures)
              a. access permission
              b. uid, gid
              c. time of last message sent and received
              d. time of last change in the message queue
              e. size of the queue
              f. number of messages in the queue
              g. maximum number of bytes allowed
              h. pid of the msgsnd and msgrcv
Date : 5th Oct, 2023.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <time.h>

int main() {
    int msqid;
    key_t key;
    struct msqid_ds buf;

    // Create a key for the message queue (replace with your key if necessary)
    key = ftok("/tmp", 'A');

    // Get the message queue ID
    msqid = msgget(key, 0666 | IPC_CREAT);

    if (msqid == -1) {
        perror("msgget");
        exit(1);
    }

    // Get the message queue information
    if (msgctl(msqid, IPC_STAT, &buf) == -1) {
        perror("msgctl");
        exit(1);
    }

    // Print access permissions
    printf("Access Permissions: %o\n", buf.msg_perm.mode);

    // Print UID and GID
    printf("UID: %d\n", buf.msg_perm.uid);
    printf("GID: %d\n", buf.msg_perm.gid);

    // Print times if they are set
    if (buf.msg_stime != 0) {
        printf("Time of last message sent: %s", ctime(&buf.msg_stime));
    }
    if (buf.msg_rtime != 0) {
        printf("Time of last message received: %s", ctime(&buf.msg_rtime));
    }
    if (buf.msg_ctime != 0) {
        printf("Time of last change: %s", ctime(&buf.msg_ctime));
    }

    // Print queue size and message count
    printf("Size of queue: %ld\n", buf.msg_qbytes);
    printf("Number of messages in queue: %ld\n", buf.msg_qnum);

    // Print maximum number of bytes allowed
    printf("Maximum number of bytes allowed: %ld\n", buf.msg_qbytes);

    // Print PIDs of msgsnd and msgrcv
    printf("PID of msgsnd: %d\n", buf.msg_lspid);
    printf("PID of msgrcv: %d\n", buf.msg_lrpid);

    return 0;
}

