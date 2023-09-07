#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

void run_script() {
    // Replace "temp_script.sh" with the name of your script file
    execl("/usr/bin/bash", "/usr/bin/bash", "/temp.sh", NULL);
    perror("execl");
    exit(EXIT_FAILURE);
}

int main() {
    pid_t pid, sid;

    // Delay in seconds before running the script (e.g., 20 seconds)
    int delay_seconds = 5;

    // Fork off the parent process
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // If we got a good PID, then we can exit the parent process
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Create a new session
    sid = setsid();
    if (sid < 0) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }

    // Change the working directory to root or another safe location
    if (chdir("/") < 0) {
        perror("chdir");
        exit(EXIT_FAILURE);
    }

    // Close standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Sleep for the specified delay before running the script
    sleep(delay_seconds);

    execl("/usr/bin/bash", "/usr/bin/bash", "/temp.sh", NULL);
    perror("execl");
    exit(EXIT_FAILURE);

    return 0;
}
