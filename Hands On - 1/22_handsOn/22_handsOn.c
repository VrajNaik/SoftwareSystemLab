#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

int main() {
    // Open a file for writing
    int file_descriptor = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (file_descriptor == -1) {
        perror("Error opening file");
        return 1;
    }

    // Use fork to create a child process
    pid_t child_pid = fork();

    if (child_pid < 0) {
        perror("Fork failed");
        return 1;
    }

    // In the parent process
    if (child_pid > 0) {
        // Parent process writes to the file
        const char *parent_message = "Parent process writing to the file.\n";
        write(file_descriptor, parent_message, strlen(parent_message));

        // Wait for the child to finish
        wait(NULL);
    }
    // In the child process
    else {
        // Child process writes to the file
        const char *child_message = "Child process writing to the file.\n";
        write(file_descriptor, child_message, strlen(child_message));

        // Child process exits
        exit(0);
    }

    // Close the file
    close(file_descriptor);

    return 0;
}

