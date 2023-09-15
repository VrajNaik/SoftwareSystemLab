/*
============================================================================
Name : 19.c
Author : Vraj Jatin Naik
Description : Write a program to find out time taken to execute getpid system call. Use time stamp counter.
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

// Function to read the Time Stamp Counter (TSC)
static inline uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

int main() {
    uint64_t start, end;
    uint64_t tsc_frequency;  // TSC frequency in Hz (cycles per second)
    
    // Get the TSC frequency
    __asm__ __volatile__("rdtsc" : "=a" (start), "=d" (end));
    __asm__ __volatile__("rdtsc" : "=a" (end), "=d" (start));
    tsc_frequency = end - start;
    
    // Get the start time
    start = rdtsc();
    
    // Execute the getpid system call
    getpid();
    
    // Get the end time
    end = rdtsc();
    
    // Calculate the elapsed time in seconds
    double elapsed_time = (double)(end - start) / tsc_frequency;
    
    // Print the result
    printf("Time taken to execute getpid() system call: %.6f seconds\n", elapsed_time);
    
    return 0;
}

