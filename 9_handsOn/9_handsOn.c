#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int main() {
    printf("HandsOn 9 Write a program to print the following information about a given file.\na. inode\nb. number of hard links\nc. uid\nd. gid\ne. size\nf. block size\ng. number of blocks\nh. time of last access\ni. time of last modification\nj. time of last change.\n");
    char fileName[100];
    struct stat fileStats;
    struct passwd *pwd;
    struct group *grp;

    printf("Enter the file name: ");
    scanf("%s", fileName);

    if (stat(fileName, &fileStats) == -1) {
        perror("Error getting file stats");
        return 1;
    }

    printf("Inode: %ld\n", (long)fileStats.st_ino);
    printf("Number of hard links: %ld\n", (long)fileStats.st_nlink);

    pwd = getpwuid(fileStats.st_uid);
    if (pwd != NULL) {  
        printf("UID: %s\n", pwd->pw_name);
    } else {
        printf("UID: %d\n", (int)fileStats.st_uid);
    }

    grp = getgrgid(fileStats.st_gid);
    if (grp != NULL) {
        printf("GID: %s\n", grp->gr_name);
    } else {
        printf("GID: %d\n", (int)fileStats.st_gid);
    }

    printf("Size: %ld bytes\n", (long)fileStats.st_size);
    printf("Block size: %ld bytes\n", (long)fileStats.st_blksize);
    printf("Number of blocks: %ld\n", (long)fileStats.st_blocks);
    printf("Time of last access: %s", ctime(&fileStats.st_atime));
    printf("Time of last modification: %s", ctime(&fileStats.st_mtime));
    printf("Time of last change: %s", ctime(&fileStats.st_ctime));

    return 0;
}
