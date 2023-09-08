#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    printf("Write a program to simulate online ticket reservation. Implement write lock Write a program to open a file, store a ticket number and exit. \nWrite a separate program, to open the file, implement write lock, read the ticket number, increment the number and print the new ticket number then close the file.\n\n\n");
    int choice;

    printf("Welcome to the Ticket Reservation System\n");
    printf("1. Admin\n");
    printf("2. User\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            system("./admin"); // Execute the admin program
            break;
        case 2:
            system("./user"); // Execute the user program
            break;
        default:
            printf("Invalid choice.\n");
    }

    return 0;
}
