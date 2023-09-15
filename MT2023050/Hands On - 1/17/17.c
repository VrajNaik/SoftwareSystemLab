/*
============================================================================
Name : 17.c
Author : Vraj Jatin Naik
Description : Write a program to simulate online ticket reservation. Implement write lock.
              Write a program to open a file, store a ticket number and exit. Write a separate program, to
              open the file, implement write lock, read the ticket number, increment the number and print
              the new ticket number then close the file.
              a. intialize.c for initialization of ticket.
              b. admin.c for admin that can update available tickets and see reservation.
              c. user.c for users who can see available and tickets book them if available. 
Date: 28th Aug, 2023.
============================================================================
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
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
