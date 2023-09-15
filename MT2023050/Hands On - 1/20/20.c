/*
============================================================================
Name : 20.c
Author : Vraj Jatin Naik
Description : Find out the priority of your running program. Modify the priority with nice command.
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>

int main() {
    int priority;

    // Get priority value from the user
    printf("Enter the priority value (-20 to 19): ");
    scanf("%d", &priority);

    // Set the priority using nice
    if (nice(priority) == -1) {
        perror("nice");
        return 1;
    }

    // Check the priority after modifying
    int current_priority = nice(0);
    printf("Current priority: %d\n", current_priority);

    // Keep the process running so we can check it in another command line
    while (1) {
  
    }

    return 0;
}
