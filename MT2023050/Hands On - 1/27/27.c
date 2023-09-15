/*
============================================================================
Name : 27.c
Author : Vraj Jatin Naik
Description : Write a program to execute ls -Rl by the following system calls
              a. execl
              b. execlp
              c. execled. execv
              e. execvp
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int choice;
    pid_t child_pid;
    int exit_program = 0; // Flag to exit the program

    while (!exit_program) {
        printf("Options:\n");
        printf("1) Execute using execl\n");
        printf("2) Execute using execlp\n");
        printf("3) Execute using execle\n");
        printf("4) Execute using execv\n");
        printf("5) Execute using execvp\n");
        printf("6) Execute using all exec calls\n");
        printf("7) Exit\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Using execl:\n");
                if ((child_pid = fork()) == 0) {
                    execl("/bin/ls", "ls", "-Rl", (char *)NULL);
                    perror("execl");
                    exit(1);
                }
                waitpid(child_pid, NULL, 0);
                break;
            case 2:
                printf("Using execlp:\n");
                if ((child_pid = fork()) == 0) {
                    execlp("ls", "ls", "-Rl", (char *)NULL);
                    perror("execlp");
                    exit(1);
                }
                waitpid(child_pid, NULL, 0);
                break;
            case 3:
                printf("Using execle:\n");
                if ((child_pid = fork()) == 0) {
                    char *envp[] = { NULL };
                    execle("/bin/ls", "ls", "-Rl", (char *)NULL, envp);
                    perror("execle");
                    exit(1);
                }
                waitpid(child_pid, NULL, 0);
                break;
            case 4:
                printf("Using execv:\n");
                if ((child_pid = fork()) == 0) {
                    char *args[] = { "/bin/ls", "ls", "-Rl", NULL };
                    execv("/bin/ls", args);
                    perror("execv");
                    exit(1);
                }
                waitpid(child_pid, NULL, 0);
                break;
            case 5:
                printf("Using execvp:\n");
                if ((child_pid = fork()) == 0) {
                    char *args[] = { "/usr/bin/ls","-Rl", NULL };
                    execvp("/usr/bin/ls", args);
                    perror("execvp");
                    exit(1);
                }
                waitpid(child_pid, NULL, 0);
                break;
            case 6:
                printf("Using all exec calls:\n");
                
                // execl
                if ((child_pid = fork()) == 0) {
                    execl("/bin/ls", "ls", "-Rl", (char *)NULL);
                    perror("execl");
                    exit(1);
                }
                waitpid(child_pid, NULL, 0);
                
                // execlp
                if ((child_pid = fork()) == 0) {
                    execlp("ls", "ls", "-Rl", (char *)NULL);
                    perror("execlp");
                    exit(1);
                }
                waitpid(child_pid, NULL, 0);

                // execle
                if ((child_pid = fork()) == 0) {
                    char *envp[] = { NULL };
                    execle("/bin/ls", "ls", "-Rl", (char *)NULL, envp);
                    perror("execle");
                    exit(1);
                }
                waitpid(child_pid, NULL, 0);

                // execv
                if ((child_pid = fork()) == 0) {
                    char *args[] = { "/usr/bin/ls", "-Rl", NULL };
                    execv("/usr/bin/ls", args);
                    perror("execv");
                    exit(1);
                }
                waitpid(child_pid, NULL, 0);

                // execvp
                if ((child_pid = fork()) == 0) {
                    char *args[] = { "/usr/bin/ls", "-Rl", NULL };
                    execvp("/usr/bin/ls", args);
                    perror("execvp");
                    exit(1);
                }
                waitpid(child_pid, NULL, 0);

                break;
            case 7:
                exit_program = 1; // Set the exit flag to exit the loop
                break;
            default:
                printf("Invalid choice. Please enter a number from 1 to 7.\n");
        }
    }

    return 0;
}
