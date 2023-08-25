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
