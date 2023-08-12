#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    int softLink = symlink("target_file", "softLink3");
    if(softLink < 0){
      perror("Failed !!! Try Again MAN!!!!!!!");
      return 1;
    }
    int hardLink = link("target_file", "hardLink3");
    if(hardLink < 0){
      perror("Failed !!! Try Again MAN!!!!!!!");
      return 1;
    }
    int FIFO = mknod("FIFO1", S_IFIFO, 0);
    if(FIFO < 0){
      perror("Failed !!! Try Again MAN!!!!!!!");
    }
    return 0;
}
//ln -s <target> <link_name>
//ln <target> <link_name>
//mknod <fifo_name> p
