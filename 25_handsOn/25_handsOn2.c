#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    // In the parent process
    int chosen_child;
    printf("Enter the child process number to wait for (1, 2, or 3): ");
    scanf("%d", &chosen_child);

    if (chosen_child < 1 || chosen_child > 3) {
        printf("Invalid input. Please enter a valid child process number (1, 2, or 3).\n");
        return 1;
    }

    // Create three child processes
    pid_t child_pids[3];

    for (int i = 0; i < 3; i++) {
        child_pids[i] = fork();

        // Check for fork() failure
        if (child_pids[i] < 0) {
            perror("Fork failed");
            return 1;
        }

        // In the child process
        if (child_pids[i] == 0) {
            printf("Child process %d with PID %d\n", i + 1, getpid());
            sleep(i + 1); // Simulate some work
            exit(0);
        }
    }

    // Add a delay to show that the parent is waiting
    printf("Parent process: Waiting for child %d...\n", chosen_child);
    sleep(3); // Wait for 3 seconds

    int status;
    pid_t waited_pid = waitpid(child_pids[chosen_child - 1], &status, 0);

    if (waited_pid == -1) {
        perror("Waitpid failed");
        return 1;
    }

    printf("Parent process: Waited for child %d with PID %d\n", chosen_child, waited_pid);

    // Now the parent can continue its execution
    return 0;
}

