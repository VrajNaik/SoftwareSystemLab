#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include<time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>

// Function to execute the script
void executeScript() {
    system("./temp.sh"); // Replace with the actual path to your script
}

// Function to daemonize the process
void daemonize() {
    pid_t pid, sid;

    // Fork off the parent process
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Create a new session ID for the child process
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    // Change the working directory to root
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    // Close standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Redirect standard input, output, and error to /dev/null or log files
    int fd = open("/dev/null", O_RDWR);
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    close(fd);
}

// Function to schedule the script execution
void scheduleScriptExecution(int hour, int minute) {
    struct itimerval timer;
    struct tm now;
    time_t current_time, target_time;

    // Get the current time
    current_time = time(NULL);
    localtime_r(&current_time, &now);

    // Calculate the target time (today's date at the specified time)
    now.tm_hour = hour;
    now.tm_min = minute;
    now.tm_sec = 0;
    target_time = mktime(&now);

    // Calculate the initial delay
    time_t initial_delay = target_time - current_time;

    if (initial_delay < 0) {
        // If the specified time has already passed today, schedule it for tomorrow
        initial_delay += 86400; // 24 hours
    }

    timer.it_interval.tv_sec = 86400; // 24 hours
    timer.it_interval.tv_usec = 0;
    timer.it_value.tv_sec = initial_delay;
    timer.it_value.tv_usec = 0;

    setitimer(ITIMER_REAL, &timer, NULL);

    // Register a signal handler to execute the script when the timer expires
    signal(SIGALRM, executeScript);

    // Sleep indefinitely to keep the daemon running
    while (1) {
        sleep(1);
    }
}

int main() {
    // Create a daemon process
    daemonize();

    // Schedule the script execution at 10:20 AM today
    scheduleScriptExecution(10, 24);

    return 0;
}
