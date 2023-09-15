/*
============================================================================
Name : 1.c
Author : Vraj Jatin Naik
Description : Create the following types of a files using (i) shell command (ii) system call
            a. soft link (symlink system call)
            b. hard link (link system call)
            c. FIFO (mkfifo Library Function or mknod system call)
            Date: 28th Aug, 2023.
============================================================================
*/
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    int softLink = symlink("target_file", "softLink050");
    if(softLink < 0){
      perror("Failure in creating Softlink....Failed !!! Try Again MAN!!!!!!!");
      return 1;
    }
    int hardLink = link("target_file", "hardLink3050");
    if(hardLink < 0){
      perror("Failure in creating Hardlink....Failed !!! Try Again MAN!!!!!!!");
      return 1;
    }
    int FIFO = mknod("FIFO050", S_IFIFO, 0);
    if(FIFO < 0){
      perror("Failure in creating PIPE....Failed !!! Try Again MAN!!!!!!!");
    }
    return 0;
}
//ln -s <target> <link_name>
//ln <target> <link_name>
//mknod <fifo_name> p
