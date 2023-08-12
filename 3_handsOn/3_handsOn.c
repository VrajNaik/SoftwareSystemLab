#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *filename = "my_file1.txt";
    mode_t file_permissions = 0644;
    
    int file_descriptor = creat(filename, file_permissions);
    
    if (file_descriptor == -1) {
        perror("Error creating file\n");
        return 1;
    }
    
    printf("Wohhooo !!!!! Congratulations !!! lappu sa to hein hands on ;) \n File created successfully. File descriptor: %d\n", file_descriptor);
    
    close(file_descriptor); 
    return 0;
}
