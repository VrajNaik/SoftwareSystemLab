/*
============================================================================
Name : 17.c
Author : Vraj Jatin Naik
Description : Write a program to execute ls -l | wc.
              a. use dup
              b. use dup2
              c. use fcntl
        Date: 31th Aug, 2023.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void executeWithDup() {
    int pipefd[2];
    pipe(pipefd);

    pid_t ls_pid = fork();

    if (ls_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (ls_pid == 0) {
        // Child process (ls)
        close(pipefd[0]);  // Close the read end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);  // Redirect stdout to the pipe
        execlp("ls", "ls", "-l", NULL);
        perror("exec ls");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        wait(NULL);
        close(pipefd[1]);  // Close the write end of the pipe

        pid_t wc_pid = fork();

        if (wc_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (wc_pid == 0) {
            // Child process (wc)
            dup2(pipefd[0], STDIN_FILENO);  // Redirect stdin to the ls output pipe
            execlp("wc", "wc", NULL);
            perror("exec wc");
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            wait(NULL);
        }
    }
}

void executeWithDup2() {
    int pipefd[2];
    pipe(pipefd);

    pid_t ls_pid = fork();

    if (ls_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (ls_pid == 0) {
        // Child process (ls)
        close(pipefd[0]);  // Close the read end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);  // Redirect stdout to the pipe
        execlp("ls", "ls", "-l", NULL);
        perror("exec ls");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        wait(NULL);
        close(pipefd[1]);  // Close the write end of the pipe

        pid_t wc_pid = fork();

        if (wc_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (wc_pid == 0) {
            // Child process (wc)
            dup2(pipefd[0], STDIN_FILENO);  // Redirect stdin to the ls output pipe
            execlp("wc", "wc", NULL);
            perror("exec wc");
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            wait(NULL);
        }
    }
}

void executeWithFcntl() {
    int pipefd[2];
    pipe(pipefd);

    pid_t ls_pid = fork();

    if (ls_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (ls_pid == 0) {
        // Child process (ls)
        close(pipefd[0]);  // Close the read end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);  // Redirect stdout to the pipe
        execlp("ls", "ls", "-l", NULL);
        perror("exec ls");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        wait(NULL);
        close(pipefd[1]);  // Close the write end of the pipe

        pid_t wc_pid = fork();

        if (wc_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (wc_pid == 0) {
            // Child process (wc)
            int saved_stdin = dup(STDIN_FILENO);
            dup2(pipefd[0], STDIN_FILENO);  // Redirect stdin to the ls output pipe
            execlp("wc", "wc", NULL);
            perror("exec wc");
            dup2(saved_stdin, STDIN_FILENO);  // Restore stdin
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            wait(NULL);
        }
    }
}

int main() {
    int choice;

    do {
        printf("\nSelect an option:\n");
        printf("1. Execute ls -l | wc with dup\n");
        printf("2. Execute ls -l | wc with dup2\n");
        printf("3. Execute ls -l | wc with fcntl\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\n-------- ls -l --------\n");
                printf("Lines   Words   Numbers\n");
                executeWithDup();
                printf("\n-------- wc --------\n");
                break;
            case 2:
                printf("\n-------- ls -l --------\n");
                printf("Lines   Words   Numbers\n");
                executeWithDup2();
                printf("\n-------- wc --------\n");
                break;
            case 3:
                printf("\n-------- ls -l --------\n");
                printf("Lines   Words   Numbers\n");
                executeWithFcntl();
                printf("\n-------- wc --------\n");
                
                break;
            case 4:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid option. Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}


