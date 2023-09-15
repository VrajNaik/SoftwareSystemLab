/*
============================================================================
Name : 15.c
Author : Vraj Jatin Naik
Description : Write a program to display the environmental variable of the user (use environ).
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


extern char **environ; 

// Comparison function for sorting environment variables
int compare_env_variables(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}
    
int main() {
    printf("Here it come.\nTo chaliye shuru karte hein\n");
    
    // Count the number of environment variables
    int num_env_vars = 0;
    for (int i = 0; environ[i] != NULL; i++) {
        num_env_vars++;
    }

    // Copy environment variables to a separate array for sorting
    char* env_vars_copy[num_env_vars];
    for (int i = 0; i < num_env_vars; i++) {
        env_vars_copy[i] = environ[i];
    }

    // Sort environment variables alphabetically
    qsort(env_vars_copy, num_env_vars, sizeof(char*), compare_env_variables);

    // Display sorted environment variables
    for (int i = 0; i < num_env_vars; i++) {
        printf("%s\n", env_vars_copy[i]);
    }

    return 0;
}

