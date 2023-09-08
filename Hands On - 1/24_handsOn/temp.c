#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t parent_pid = getppid();
    
    printf("Main parent process PID: %d\n", parent_pid);

    return 0;
}

