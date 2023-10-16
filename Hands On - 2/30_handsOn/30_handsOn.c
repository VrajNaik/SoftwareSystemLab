/*
============================================================================
Name : 30.c
Author : Vraj Jatin Naik
Description : Write a program to create a shared memory.
              a. write some data to the shared memory
              b. attach with O_RDONLY and check whether you are able to overwrite.
              c. detach the shared memory
              d. remove the shared memory
Date : 4th Oct, 2023.
============================================================================
*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void main() {
    key_t shmKey;
    int shmIdentifier;
    ssize_t shmSize = 30;
    char *shmPointer, *rdOnlyShmPointer;

    shmKey = ftok(".", 1);

    if (shmKey == -1) {
        perror("Error while computing key!");
        _exit(0);
    }

    shmIdentifier = shmget(shmKey, shmSize, IPC_CREAT | 0700);

    if (shmIdentifier == -1) {
        perror("Error while getting Shared Memory!");
        _exit(0);
    }

    shmPointer = shmat(shmIdentifier, (void *)0, 0);

    if (shmPointer == (void *)-1) {
        perror("Error while attaching address space!");
        _exit(0);
    }

    printf("Enter data to write to the shared memory: ");
    fgets(shmPointer, shmSize, stdin);

    printf("Data written to shared memory: %s\n", shmPointer);

    rdOnlyShmPointer = shmat(shmIdentifier, (void *)0, SHM_RDONLY);
    if (rdOnlyShmPointer == (void *)-1) {
        perror("Error while attaching read-only address space!");
        _exit(0);
    }

    printf("Trying to overwrite read-only shared memory...\n");

    // The following line will not result in a segmentation fault
    // as it's writing to regular (read-write) shared memory
    strncpy(shmPointer, "Overwritten", shmSize);

    printf("Data in read-only shared memory: %s\n", rdOnlyShmPointer);

    printf("Detaching pointers from shared memory!\n");
    shmdt(shmPointer);
    shmdt(rdOnlyShmPointer);

    printf("Press enter to delete the shared memory!\n");
    getchar();

    // Delete Shared Memory
    shmctl(shmIdentifier, IPC_RMID, NULL);
}

