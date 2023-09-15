#include <sys/types.h>
#include <sys/stat.h>

int main() {
    const char *fifo_name = "my_fifo";
    mode_t fifo_mode = 0666;
    
    mkfifo(fifo_name, fifo_mode);
    return 0;
}

//myfifo <fifo_name>
//mknod <fifo_name> p
