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
        fflush(stdout);
        sleep(5); // Simulate some work in the child process
        printf("Child process (PID %d) exiting.\n", getpid());
        fflush(stdout);
    } else {
        // Parent process
        printf("Parent process (PID %d) created.\n", getpid());
        fflush(stdout);
        sleep(1); // Let the child process start

        // Check if the child process has become an orphan
        if (getppid() == 1) {
            printf("Child process (PID %d) has become an orphan.\n", child_pid);
        } else {
            printf("Child process (PID %d) still has parent PID %d.\n", child_pid, getppid());
        }
        fflush(stdout);

        printf("Parent process (PID %d) exiting.\n", getpid());
        fflush(stdout);
    }

    return 0;
}

