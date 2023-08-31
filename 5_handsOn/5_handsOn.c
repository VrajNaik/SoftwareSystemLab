#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int numFiles;
    mode_t file_permissions = 0644;
    printf("Enter the number of files you want to create: \n");
    scanf("%d", &numFiles);

    // An array to store file descriptors
    int file_descriptors[numFiles];

    // Create a specified number of files
    for (int i = 0; i < numFiles; ++i) {
        char filename[20];
        snprintf(filename, sizeof(filename), "file%d.txt", i + 1);

        int file_descriptor = open(filename, O_CREAT | O_WRONLY, file_permissions);

        if (file_descriptor == -1) {
            perror("Error creating file");
            return 1;
        }

        printf("File %s created with descriptor %d\n", filename, file_descriptor);

        // Store the file descriptor in the array
        file_descriptors[i] = file_descriptor;
    }

    // Close all the file descriptors
    for (int i = 0; i < numFiles; ++i) {
        close(file_descriptors[i]);
    }

    // Enter an infinite loop to keep the program running
    while (1) {
        sleep(1);
    }

    return 0;
}

