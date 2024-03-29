#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("Fork failed");
        return 1;
    }

    if (child_pid == 0) {
        // Child process
        printf("Child process (PID %d) created with parent PID %d.\n", getpid(), getppid());
        sleep(15);

        // After the parent exits, getppid() will return 1.
        printf("Child process (PID %d) now has PPID %d.\n", getpid(), getppid());
    } else {
        // Parent process
        printf("Parent process (PID %d) created.\n", getpid());
        printf("Parent process (PID %d) exiting, leaving the child as an orphan.\n", getpid());
        exit(0); // Exit the parent process, leaving the child as an orphan
    }

    return 0;
}

