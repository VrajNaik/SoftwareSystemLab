/*
============================================================================
Name : 14.c
Author : Vraj Jatin Naik
Description : Write a program to find the type of a file.
              a. Input should be taken from command line.
              b. program should be able to identify any type of a file.
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Hey I think you forgot to write filename along with command...Please Provide Filename...\n\n");
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    struct stat file_info;

    if (stat(filename, &file_info) == -1) {
        perror("Oops !!!!!Error");
        return 1;
    }

    if (S_ISREG(file_info.st_mode)) {
        printf("%s is a regular file.\n", filename);
    } 
    else if (S_ISDIR(file_info.st_mode)) {
        printf("%s is a directory.\n", filename);
    } 
    else if (S_ISCHR(file_info.st_mode)) {
        printf("%s is a character special file (e.g., a device file).\n", filename);
    } 
    else if (S_ISBLK(file_info.st_mode)) {
        printf("%s is a block special file (e.g., a disk device).\n", filename);
    } 
    else if (S_ISFIFO(file_info.st_mode)) {
        printf("%s is a FIFO (named pipe) file.\n", filename);
    } 
    else if (S_ISLNK(file_info.st_mode)) {
        printf("%s is a symbolic link.\n", filename);
    } 
    else if (S_ISSOCK(file_info.st_mode)) {
        printf("%s is a socket.\n", filename);
    } 
    else {
        printf("%s is of an unknown type.\n", filename);
    }

    return 0;
}
