#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>

void run_script() {
    // Replace "/path/to/your/script.sh" with the actual script path
    char* script_path = "home/vraj-naik/sslab/30_handsOn/temp.sh";
    execl("/bin/bash", "/bin/bash", script_path, NULL);
    perror("execl");
    exit(EXIT_FAILURE);
}

int main() {
    pid_t pid, sid;

    // Set the desired time (in seconds since the epoch)
    time_t target_time = 20;

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

    while (1) {
        // Get the current time
        time_t current_time;
        time(&current_time);

        // Check if it's time to run the script
        if (current_time >= target_time) {
            run_script();
            break; // Exit the loop after running the script
        } else {
            // Sleep for a while before checking again
            sleep(60); // Sleep for 60 seconds (adjust as needed)
        }
    }

    return 0;
}
