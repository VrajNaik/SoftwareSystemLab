#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int fileDescriptor = open(argv[1], O_RDONLY);

    if (fileDescriptor == -1) {
        perror("Error opening file");
        exit(1);
    }
    char buffer;
    int bytesRead;
    char line[1024];
    int lineIndex = 0;

    while ((bytesRead = read(fileDescriptor, &buffer, 1)) > 0) {
        if (buffer == '\n') {
           
            write(STDOUT_FILENO, line, lineIndex);
            write(STDOUT_FILENO, "\n", 1);
            lineIndex = 0;
            sleep(5);
        } 
        else {
            line[lineIndex] = buffer;
            lineIndex++;
        }
    }

    close(fileDescriptor);

    return 0;
}

