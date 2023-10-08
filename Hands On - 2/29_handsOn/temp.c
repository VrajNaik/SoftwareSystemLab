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

    // Create the message queue
    msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Message queue created with ID: %d\n", msgid);

    return 0;
}

