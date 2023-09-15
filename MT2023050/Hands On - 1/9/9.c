/*
============================================================================
Name : 9.c
Author : Vraj Jatin Naik
Description : Write a program to print the following information about a given file.
            a. inode
            b. number of hard links
            c. uid
            d. gid
            e. size
            f. block size
            g. number of blocks
            h. time of last access
            i. time of last modification
            j. time of last change
Date: 28th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int main() {

    char fileName[100];
    struct stat fileStats;
    struct passwd *pwd;
    struct group *grp;

    // Prompt the user to enter the file name 
    printf("Enter the file name: ");
    fgets(fileName, sizeof(fileName), stdin);
    fileName[strcspn(fileName, "\n")] = '\0';
    int check_fd = open(fileName, O_RDONLY);
    
    //check if file exists or not
    if (check_fd == -1) {
        perror("Oops...!!!File Doesn't Exists or Something went WRONG...Sorry Buddy");
    }
    
    else {
        // Get file statistics using stat() function
        if (stat(fileName, &fileStats) == -1) {
            perror("Sorry Buddy !!! Error getting file stats\n");
            return 1; // Exit with an error status
        }

        // Print the requested information
        printf("Inode: %ld\n", (long)fileStats.st_ino);
        printf("Number of hard links: %ld\n", (long)fileStats.st_nlink);
  
        // Get and print the user (UID) and group (GID) names
        pwd = getpwuid(fileStats.st_uid);
        if (pwd != NULL) {  
            printf("UID: %s\n", pwd->pw_name);
        } 
        else {
            printf("UID: %d\n", (int)fileStats.st_uid);
        }

        grp = getgrgid(fileStats.st_gid);
        if (grp != NULL) {
            printf("GID: %s\n", grp->gr_name);
        } 
        else {
            printf("GID: %d\n", (int)fileStats.st_gid);
        }

        // Print file size, block size, number of blocks, and time-related information
        printf("Size: %ld bytes\n", (long)fileStats.st_size);
        printf("Block size: %ld bytes\n", (long)fileStats.st_blksize);
        printf("Number of blocks: %ld\n", (long)fileStats.st_blocks);
        printf("Time of last access: %s", ctime(&fileStats.st_atime));
        printf("Time of last modification: %s", ctime(&fileStats.st_mtime));
        printf("Time of last change: %s", ctime(&fileStats.st_ctime));

        return 0; // Exit successfully
    }
}
