#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <signal.h>

void run_script() {
    // Replace "your_script.sh" with the path to your script
    system("/bin/sh /path/to/your_script.sh");
}

void daemonize() {
    pid_t pid, sid;
    // Fork off the parent process
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    // If we got a good PID, then we can exit the parent process
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Change the file mode mask
    umask(0);

    // Create a new SID for the child process
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    // Change the current working directory to a safe place
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    // Close the standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main() {
    daemonize();

    // Replace these values with your desired time
    int target_hour = 12;
    int target_minute = 0;

    while (1) {
        time_t now;
        struct tm *tm_info;
        time(&now);
        tm_info = localtime(&now);

        if (tm_info->tm_hour == target_hour && tm_info->tm_min == target_minute) {
            run_script();
        }

        // Sleep for a while to avoid busy-waiting
        sleep(60);
    }

    return 0;
}
