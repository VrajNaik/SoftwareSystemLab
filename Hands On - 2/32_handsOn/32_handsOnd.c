/*
============================================================================
Name : 32.c
Author : Vraj Jatin Naik
Description : Write a program to implement semaphore to protect any critical section.
              a. rewrite the ticket number creation program using semaphore
              b. protect shared memory from concurrent write access
              c. protect multiple pseudo resources ( may be two) using counting semaphore
              d. remove the created semaphore
Date : 6th Oct, 2023.
============================================================================
*/


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>

void main()
{
    key_t semKey;
    int semIndentifier;
    int semctlStatus;

    semKey = ftok(".", 1);
    if (semKey == -1)
    {
        perror("Error while computing key!");
        _exit(1);
    }

    semIndentifier = semget(semKey, 1, IPC_CREAT | 0777);

    if (semIndentifier == -1)
    {
        perror("Error while creating semaphore!");
        _exit(1);
    }

    printf("Press enter to delete the semaphore!\n");
    getchar();

    semctlStatus = semctl(semIndentifier, 0, IPC_RMID);

    if (semctlStatus == -1)
    {
        perror("Error while deleting semaphore!");
        _exit(0);
    }
}

