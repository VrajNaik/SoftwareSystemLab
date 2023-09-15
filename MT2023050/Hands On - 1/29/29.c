/*
============================================================================
Name : 29.c
Author : Vraj Jatin Naik
Description : Write a program to get scheduling policy and modify the scheduling policy (SCHED_FIFO, SCHED_RR).
Date: 28th Aug, 2023.
============================================================================
*/


#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// ANSI color codes for formatting
#define ANSI_COLOR_CYAN  "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

int main() {
    int pid = getpid();
    struct sched_param param;
    char policy_str[20];

    while (1) {
        // Get the current scheduling policy and priority of the process
        int get_policy = sched_getscheduler(0); // 0 indicates the calling process
        if (get_policy == -1) {
            perror("sched_getscheduler");
            exit(EXIT_FAILURE);
        }

        // Get the current scheduling parameters
        if (sched_getparam(pid, &param) == -1) {
            perror("sched_getparam");
            exit(EXIT_FAILURE);
        }

        switch (get_policy) {
            case SCHED_FIFO:
                strcpy(policy_str, "SCHED_FIFO");
                break;
            case SCHED_RR:
                strcpy(policy_str, "SCHED_RR");
                break;
            case SCHED_OTHER:
                strcpy(policy_str, "SCHED_OTHER");
                break;
            default:
                strcpy(policy_str, "Unknown");
                break;
        }

        printf(ANSI_COLOR_CYAN "Current Scheduling Policy: %s\n" ANSI_COLOR_RESET, policy_str);
        printf("Current Priority: %d\n", param.sched_priority);

        // Display menu options
        printf("\nMenu:\n");
        printf("1. Change Scheduling Policy\n");
        printf("2. Check Current Priority\n");
        printf("3. Exit\n");
        printf("Enter your choice (1/2/3): ");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int new_policy;
                // Prompt the user for a new scheduling policy
                printf("\nSelect a new Scheduling Policy:\n");
                printf("1. SCHED_FIFO\n");
                printf("2. SCHED_RR\n");
                printf("3. SCHED_OTHER\n");
                printf("Enter your choice (1/2/3): ");
                
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        new_policy = SCHED_FIFO;
                        break;
                    case 2:
                        new_policy = SCHED_RR;
                        break;
                    case 3:
                        new_policy = SCHED_OTHER;
                        break;
                    default:
                        printf("Invalid choice. Using the current policy.\n");
                        new_policy = get_policy;
                        break;
                }

                // Change the scheduling policy to the selected option
                struct sched_param new_param;
                new_param.sched_priority = 50; // Set the priority for SCHED_FIFO and SCHED_RR

                if (sched_setscheduler(pid, new_policy, &new_param) == -1) {
                    perror("sched_setscheduler");
                    exit(EXIT_FAILURE);
                }

                // Get and display the new scheduling policy
                get_policy = sched_getscheduler(0);
                if (get_policy == -1) {
                    perror("sched_getscheduler");
                    exit(EXIT_FAILURE);
                }

                
                switch (get_policy) {
                    case SCHED_FIFO:
                        strcpy(policy_str, "SCHED_FIFO");
                        break;
                    case SCHED_RR:
                        strcpy(policy_str, "SCHED_RR");
                        break;
                    case SCHED_OTHER:
                        strcpy(policy_str, "SCHED_OTHER");
                        break;
                    default:
                        strcpy(policy_str, "Unknown");
                        break;
                }

                printf("\nScheduling policy changed to: %s\n", policy_str);
                break;
            }
            case 2:
                // Check the current priority again
                printf("Current Priority: %d\n", param.sched_priority);
                break;
            case 3:
                // Exit the program
                printf("Exiting...\n");
                exit(EXIT_SUCCESS);
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    return 0;
}

