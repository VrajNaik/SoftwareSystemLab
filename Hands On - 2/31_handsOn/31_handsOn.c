#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>  // For sleep

// Define union semun
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// Define global semaphores
int binary_semaphore;
int counting_semaphore;

void binary_semaphore_wait() {
    struct sembuf operation;
    operation.sem_num = 0;    // Index of the semaphore in the semaphore set
    operation.sem_op = -1;    // Decrement the semaphore
    operation.sem_flg = 0;    // No special flags

    semop(binary_semaphore, &operation, 1);
}

void binary_semaphore_post() {
    struct sembuf operation;
    operation.sem_num = 0;    // Index of the semaphore in the semaphore set
    operation.sem_op = 1;     // Increment the semaphore
    operation.sem_flg = 0;    // No special flags

    semop(binary_semaphore, &operation, 1);
}

void* binary_semaphore_example(void* arg) {
    printf("Binary Semaphore Example: Thread Started\n");

    // Wait on the binary semaphore
    binary_semaphore_wait();
    printf("Binary Semaphore Example: Semaphore Acquired\n");

    // Release the binary semaphore
    binary_semaphore_post();
    printf("Binary Semaphore Example: Semaphore Released\n");

    printf("Binary Semaphore Example: Thread Finished\n");
    return NULL;
}

void* counting_semaphore_example(void* arg) {
    printf("Counting Semaphore Example: Thread Started\n");

    // Wait on the counting semaphore
    binary_semaphore_wait();
    printf("Counting Semaphore Example: Semaphore Acquired\n");

    // Release the counting semaphore
    binary_semaphore_post();
    printf("Counting Semaphore Example: Semaphore Released\n");

    printf("Counting Semaphore Example: Thread Finished\n");
    return NULL;
}

int main() {
    union semun sem_init_val;

    // Create a binary semaphore
    binary_semaphore = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (binary_semaphore == -1) {
        perror("Failed to create binary semaphore");
        return 1;
    }

    // Initialize the binary semaphore with an initial value of 1
    sem_init_val.val = 1;
    if (semctl(binary_semaphore, 0, SETVAL, sem_init_val) == -1) {
        perror("Failed to initialize binary semaphore");
        return 1;
    }

    // Create a counting semaphore
    counting_semaphore = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (counting_semaphore == -1) {
        perror("Failed to create counting semaphore");
        return 1;
    }

    // Initialize the counting semaphore with an initial value of 3
    sem_init_val.val = 3;
    if (semctl(counting_semaphore, 0, SETVAL, sem_init_val) == -1) {
        perror("Failed to initialize counting semaphore");
        return 1;
    }

    printf("Semaphores created. You can check them using 'ipcs' in another terminal.\n");

    // Create threads to demonstrate semaphore usage
    pthread_t binary_thread, counting_thread;
    pthread_create(&binary_thread, NULL, binary_semaphore_example, NULL);
    pthread_create(&counting_thread, NULL, counting_semaphore_example, NULL);

    // Sleep for a while to allow checking semaphores using 'ipcs' in another terminal
    sleep(30);

    // Remove the semaphores
    semctl(binary_semaphore, 0, IPC_RMID);
    semctl(counting_semaphore, 0, IPC_RMID);

    return 0;
}

