#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

// ANSI escape codes for text color
#define RESET "\x1B[0m"
#define BOLD "\x1B[1m"
#define CYAN "\x1B[36m"
#define MAGENTA "\x1B[35m"
#define GREEN "\x1B[32m"
#define BRIGHT_BLUE "\x1B[94m"

void printMessage(const char *message, const char *color, int pid) {
    printf("%s%s (PID: %d)%s\n", color, message, pid, RESET);
}

void printError(const char *message) {
    fprintf(stderr, BOLD BRIGHT_BLUE "Error: %s" RESET "\n", message);
}

int main() {
    pid_t child1, child2, child3;

    int parentPID = getpid(); // Get the parent process ID

    printMessage("Before forking processes...", CYAN, parentPID);

    child1 = fork(); // Create child process 1

    if (child1 == 0) {
        // Child process 1
        int child1PID = getpid();
        printMessage("Child 1 has been created.", MAGENTA, child1PID);
        printf("Child 1's parent PID is: %d\n", parentPID);
    } else if (child1 > 0) {
        // Parent process
        printMessage("Inside the first parent process.", CYAN, parentPID);

        child2 = fork(); // Create child process 2

        if (child2 == 0) {
            // Child process 2
            int child2PID = getpid();
            printMessage("Child 2 has been created.", GREEN, child2PID);
            printf("Child 2's parent PID is: %d\n", parentPID);
        } else if (child2 > 0) {
            // Parent process
            printMessage("Inside the second parent process.", CYAN, parentPID);

            child3 = fork(); // Create child process 3

            if (child3 == 0) {
                // Child process 3
                int child3PID = getpid();
                sleep(10);
                printMessage("Child 3 has been created.", MAGENTA, child3PID);
                printf("Child 3's parent PID is: %d\n", parentPID);
                printf("Child 3 has completed its task.\n");
            } else if (child3 > 0) {
                // Parent process
                int status;
                waitpid(child3, &status, 0);
                printMessage("Parent of all processes.", CYAN, parentPID);
            } else {
                printError("Fork error for child 3");
                exit(EXIT_FAILURE);
            }
        } else {
            printError("Fork error for child 2");
            exit(EXIT_FAILURE);
        }
    } else {
        printError("Fork error for child 1");
        exit(EXIT_FAILURE);
    }

    return 0;
}

