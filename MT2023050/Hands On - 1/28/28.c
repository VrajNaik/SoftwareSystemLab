/*
============================================================================
Name : 28.c
Author : Vraj Jatin Naik
Description : Write a program to get maximum and minimum real time priority.
Date: 28th Aug, 2023.
============================================================================
*/


#include <stdio.h>
#include <sched.h>

// ANSI color codes for formatting
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int main() {
    int max_priority, min_priority;

    // Get the maximum real-time priority
    max_priority = sched_get_priority_max(SCHED_FIFO);
    if (max_priority == -1) {
        perror("Error getting maximum real-time priority");
        return 1;
    }

    // Get the minimum real-time priority
    min_priority = sched_get_priority_min(SCHED_FIFO);
    if (min_priority == -1) {
        perror("Error getting minimum real-time priority");
        return 1;
    }

    // Print with colorful formatting
    printf(ANSI_COLOR_GREEN "Real-Time Priority Information:\n" ANSI_COLOR_RESET);
    printf("Maximum real-time priority: " ANSI_COLOR_RED "%d\n" ANSI_COLOR_RESET, max_priority);
    printf("Minimum real-time priority: " ANSI_COLOR_GREEN "%d\n" ANSI_COLOR_RESET, min_priority);

    return 0;
}
