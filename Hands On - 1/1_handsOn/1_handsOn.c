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
    int hardLink = link("target_file", "hardLink111");
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
