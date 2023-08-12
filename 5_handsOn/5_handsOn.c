#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int numFiles;
    mode_t file_permissions = 0644;
    printf("Enter number of files you want to create: \n");
    scanf("%d", &numFiles);
    for (int i = 1; i <= numFiles; ++i) {
        char filename[20];
        snprintf(filename, sizeof(filename), "file%d.txt", i);

        int file_descriptor = creat(filename, file_permissions);

        if (file_descriptor == -1) {
            perror("Error creating file");
            return 1;
        }

        printf("File %s created with descriptor %d\n", filename, file_descriptor);

        close(file_descriptor);
    }

    while (1) {
        sleep(1);
    }

    return 0;
}
