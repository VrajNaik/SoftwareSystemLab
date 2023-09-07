#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int is_even(int number) {
    return number % 2 == 0;
}

void display_file_contents(const char *filename, const char *message) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file for reading");
        exit(1);
    }

    printf("File Contents %s:\n", message);
    char ch;
    while (read(fd, &ch, sizeof(char)) > 0) {
        putchar(ch);
    }
    close(fd);
}

int main() {
    // Open a file for writing (or create it if it doesn't exist)
    int fd = open("numbers.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1) {
        perror("Failed to open file");
        exit(1);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        for (int i = 1; i <= 10; i += 2) {
            dprintf(fd, "%d\n", i); // Write odd numbers
        }
        close(fd);

        // Display file contents after the child process writes
        display_file_contents("numbers.txt", "after written by Child Process");
    } else {
        // Parent process
        wait(NULL); // Wait for the child to finish

        for (int i = 2; i <= 10; i += 2) {
            dprintf(fd, "%d\n", i); // Write even numbers
        }
        close(fd); // Close the file

        // Display file contents after both processes complete
        display_file_contents("numbers.txt", "after written by Parent Process");

        // Now, let's check if the file contains only even numbers
        int number;
        int all_even = 1; // Assume all numbers are even

        fd = open("numbers.txt", O_RDONLY);
        if (fd == -1) {
            perror("Failed to open file for reading");
            exit(1);
        }

        while (read(fd, &number, sizeof(int)) > 0) {
            if (!is_even(number)) {
                all_even = 0;
                break; // Found an odd number, no need to check further
            }
        }

        close(fd);

        if (all_even) {
            printf("The file contains only even numbers.\n");
        } else {
            printf("The file contains at least one odd number.\n");
        }
    }

    return 0;
}

