#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    key_t key;
    int shmid;
    char *shm_ptr;

    // Generate a unique key for the shared memory segment
    key = ftok("/tmp", 'A');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // Create a shared memory segment with read and write permissions
    shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    printf("Shared memory segment created successfully.\n");

    // Attach the shared memory segment for writing
    shm_ptr = (char *)shmat(shmid, NULL, 0);
    if (shm_ptr == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

    // Write data to the shared memory
    strcpy(shm_ptr, "Hello, Shared Memory!");

    printf("Data written to shared memory successfully.\n");

    // Attach the shared memory segment with O_RDONLY
    char *shm_readonly = (char *)shmat(shmid, NULL, SHM_RDONLY);
    if (shm_readonly == (char *)(-1)) {
        perror("shmat O_RDONLY");
        exit(1);
    }

    printf("Attached shared memory with O_RDONLY successfully.\n");

    // Attempt to overwrite data in the read-only attachment
    // This should result in a segmentation fault
    // strcpy(shm_readonly, "This should not work!"); // Uncomment this line to trigger segmentation fault

    // Detach the shared memory segments
    shmdt(shm_ptr);
    shmdt(shm_readonly);

    printf("Shared memory detached successfully.\n");

    // Remove the shared memory segment
    shmctl(shmid, IPC_RMID, NULL);

    printf("Shared memory segment removed successfully.\n");

    return 0;
}

