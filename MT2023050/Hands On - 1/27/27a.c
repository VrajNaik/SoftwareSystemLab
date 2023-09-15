#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid;

    printf("Using execl:\n");
    if ((child_pid = fork()) == 0) {
        execl("/bin/ls", "ls", "-Rl", (char *)NULL);
        perror("execl");
        exit(1);
    }
    waitpid(child_pid, NULL, 0);

    printf("Using execlp:\n");
    if ((child_pid = fork()) == 0) {
        execlp("ls", "ls", "-Rl", (char *)NULL);
        perror("execlp");
        exit(1);
    }
    waitpid(child_pid, NULL, 0);

    printf("Using execle:\n");
    if ((child_pid = fork()) == 0) {
        char *envp[] = { NULL };
        execle("/bin/ls", "ls", "-Rl", (char *)NULL, envp);
        perror("execle");
        exit(1);
    }
    waitpid(child_pid, NULL, 0);

    printf("Using execv:\n");
    if ((child_pid = fork()) == 0) {
        char *args[] = { "/usr/bin/ls", "ls", "-Rl", NULL };
        execv("/usr/bin/ls", args);
        perror("execv");
        exit(1);
    }
    waitpid(child_pid, NULL, 0);

    printf("Using execvp:\n");
    if ((child_pid = fork()) == 0) {
        char *args[] = { "/usr/bin/ls", "ls", "-Rl", NULL };
        execvp("/usr/bin/ls", args);
        perror("execvp");
        exit(1);
    }
    waitpid(child_pid, NULL, 0);

    return 0;
}
