#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t p;
    
    printf("pid of main program: %d\n", getpid());

    p = fork();

    if (p < 0) {
        perror("Fork failed");
        exit(1);
    } 
    else if (p == 0) {
        printf("In child process: Hey....!\npid: %d \nParents Process ID (ppid): %d\n", getpid(), getppid());
        // Add child process logic here
        exit(0); // Exit the child process
    } 
    else {
        printf("In parent: Hello Guys....!\npid: %d\nChild Process ID (fork returned): %d\n", getpid(), p);

        // Wait for the child process to complete
        int status;
        waitpid(p, &status, 0);

        if (WIFEXITED(status)) {
            printf("Child process exited with status: %d\n", WEXITSTATUS(status));
        } 
        else {
            perror("Child process did not exit normally");
        }
    }

    return 0;
}

