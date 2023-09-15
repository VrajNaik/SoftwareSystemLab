#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t child_pid;
    char *const cmd[] = { "ls", "-l", NULL };

    // Using execvp to run "ls" without specifying the full path
    child_pid = fork();
    if (child_pid == 0) {
        // Child process
        execvp("ls", cmd);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (child_pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // Wait for the child process to finish
    wait(NULL);

    return 0;
}

