#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1

int main(int argc, char *argv[]) {
    printf("HandsOn 7 Write a program to copy file1 into file2 ($cp file1 file2).\n");

    // Check if the correct number of command-line arguments is provided
    if (argc != 3) {
        fprintf(stderr, "Usage: %s source_file destination_file\n", argv[0]);
        return 1; // Exit with an error status
    }

    // Open the source file for reading
    int source_fd = open(argv[1], O_RDONLY);
    if (source_fd == -1) {
        perror("Oopas...Something went WRONG !!! Error opening source file");
        return 1; // Exit with an error status
    }

    // Open or create the destination file for writing
    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("Oopas...Something went WRONG !!! Error opening destination file");
        close(source_fd);
        return 1; // Exit with an error status
    }

    char buffer[BUFFER_SIZE];
    int bytes_read, bytes_written;

    // Copy data from source to destination file
    while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written == -1) {
            perror("Oopas...Something went WRONG !!! Error writing to destination file");
            close(source_fd);
            close(dest_fd);
            return 1; // Exit with an error status
        }
    }

    // Check for read errors
    if (bytes_read == -1) {
        perror("Oopas...Something went WRONG !!! Error reading from source file");
        close(source_fd);
        close(dest_fd);
        return 1; // Exit with an error status
    }

    // Close both source and destination files
    close(source_fd);
    close(dest_fd);

    // Display a success message
    printf("Congratulations !!! File copied successfully.\n");
    return 0; // Exit successfully
}

