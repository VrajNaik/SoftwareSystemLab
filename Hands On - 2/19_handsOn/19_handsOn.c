/*
============================================================================
Name : 19.c
Author : Vraj Jatin Naik
Description : Create a FIFO file by
              a. mknod command
              b. mkfifo command
              c. use strace command to find out, which command (mknod or mkfifo) is better.
              c. mknod system call
              d. mkfifo library function
      Date : 31th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void createFIFOWithMknod();
void createFIFOWithMkfifo();
void checkSystemCalls(char* command);

int main() {
    char choice;
    
    do {
        printf("\nFIFO Creation Menu:\n");
        printf("a. Create FIFO using mknod\n");
        printf("b. Create FIFO using mkfifo\n");
        printf("x. Exit\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice);
        
        switch (choice) {
            case 'a':
                createFIFOWithMknod();
                break;
            case 'b':
                createFIFOWithMkfifo();
                break;
            case 'x':
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 'x');
    
    return 0;
}

void createFIFOWithMknod() {
    if (mknod("myfifo_mknod", S_IFIFO | 0666, 0) == 0) {
        printf("FIFO created successfully using mknod.\n");
    } else {
        perror("Error creating FIFO with mknod");
    }
}

void createFIFOWithMkfifo() {
    if (mkfifo("myfifo_mkfifo", 0666) == 0) {
        printf("FIFO created successfully using mkfifo.\n");
    } else {
        perror("Error creating FIFO with mkfifo");
    }
}

/*1. **System Call Arguments**:
   - The main difference between the two methods lies in the arguments passed to the system calls:
     - For `mknod` (option 'a'), the system call `mknodat` is used to create the FIFO with the arguments `("myfifo_mknod", S_IFIFO|0666)`. This specifies the name of the FIFO to be created and sets its permissions.
     - For `mkfifo` (option 'b'), the system call `mknodat` is also used to create the FIFO with similar arguments, `("myfifo_mkfifo", S_IFIFO|0666)`.

2. **Result**:
   - Both options successfully create the FIFO with a return value of `0`.
   - Both options print a success message indicating that the FIFO was created successfully.

Based on the `diff` output and the specific use of `strace`, there isn't a significant difference between the two methods in terms of system calls or functionality. Both methods use the `mknodat` system call with similar arguments to create FIFOs, and both succeed in creating the FIFO.

In this specific case, neither method is inherently better or worse than the other in terms of system calls. Both options appear to have equivalent efficiency and functionality for creating FIFOs. The choice between `mknod` and `mkfifo` may depend on other factors, such as personal preference or compatibility with specific use cases./*
