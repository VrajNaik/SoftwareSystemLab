#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    const char *fifo_name = "my_fifo";
    mode_t fifo_mode = S_IFIFO | 0666; // FIFO file type and permissions
    
    mknod(fifo_name, fifo_mode, 0);
    return 0;
}
//mknode <fifo_name> p
