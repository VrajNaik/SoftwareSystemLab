#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <termios.h>
#include <openssl/sha.h>
#include <openssl/evp.h>

#include"./Structs/Admin.h"

// Function to get the ID for the new admin
int getNewAdmin() {
    int fd = open("/home/vraj-naik/sslab/SS_Project/VidyaSynergy/data/admin.data", O_RDWR | O_CREAT | O_APPEND,0744);
    if (fd == -1 && errno == ENOENT) {
        return 1; // Start admin IDs from 1
    }
    struct Admin new_admin;
    lseek(fd, -sizeof(struct Admin), SEEK_END);
    read(fd, &new_admin, sizeof(struct Admin));
    close(fd);
    return new_admin.admin_id + 1;
}

// Function to check if the password meets the criteria
bool isValidPassword(const char *password) {
    bool hasUpperCase = false;
    bool hasLowerCase = false;
    bool hasSpecialChar = false;

    for (int i = 0; password[i] != '\0'; i++) {
        if (isupper(password[i])) {
            hasUpperCase = true;
        }
        else if (islower(password[i])) {
            hasLowerCase = true;
        }
        else if (ispunct(password[i])) {
            hasSpecialChar = true;
        }
    }

    return (strlen(password) <= 10) && hasUpperCase && hasLowerCase && hasSpecialChar;
}

// Function to get password securely with hidden input
void getPassword(char *password, int maxLength) {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    fgets(password, maxLength, stdin);
    // Remove the newline character
    password[strcspn(password, "\n")] = '\0';

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

/*
// Function to hash the password using SHA-256

void hashPassword(const char *inputPassword, unsigned char *outputHash) {
    EVP_MD_CTX *mdctx;
    const EVP_MD *md = EVP_sha256();

    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit(mdctx, md);
    EVP_DigestUpdate(mdctx, inputPassword, strlen(inputPassword));
    EVP_DigestFinal(mdctx, outputHash, NULL);
    EVP_MD_CTX_free(mdctx);
}
*/

int main(){
	int choice = 0;
	struct Admin newAdmin;
	newAdmin.admin_id = getNewAdmin();
	int fd = open("/home/vraj-naik/sslab/SS_Project/VidyaSynergy/data/admin.data", O_RDWR | O_CREAT | O_APPEND, 0744);
        if (fd == -1) {
              perror("Error opening the file");
              return 1;
        }
	printf("Please enter the name of the admin: ");
	scanf(" %[^\n]",newAdmin.username);
	char tempPassword[11]; // Temporary variable for password input
        do {
              printf("Please Enter the password (max 10 characters) with at least one uppercase, one lowercase, and one special character: ");
              getPassword(tempPassword, sizeof(tempPassword));

              if (isValidPassword(tempPassword)) {
                  newAdmin.password = tempPassword;
                  break;
              } 
              else {
                  printf("Invalid password. Password must meet the criteria.\n");
              }
        } while (1);
	
	printf("Hey!!! Congratulations....!!! Your ID is : %d\n", newAdmin.admin_id);
        write(fd, &newAdmin, sizeof(struct Admin));
        
    
	printf("Do you want to continue(0/1)? ");
	
	scanf("%d", &choice);
	while(choice) {
		newAdmin.admin_id = getNewAdmin();
		printf("Please enter the name of the admin: ");
		scanf(" %[^\n]",newAdmin.username);
		char tempPassword[11]; // Temporary variable for password input
                do {
                        printf("Please Enter the password (max 10 characters) with at least one uppercase, one lowercase, and one special character: ");
                        getPassword(tempPassword, sizeof(tempPassword));

                        if (isValidPassword(tempPassword)) {
                                newAdmin.password = tempPassword;
                                break;
                        } 
                        else {
                                printf("Invalid password. Password must meet the criteria.\n");
                        }
                } while (1);
		printf("Hey!!! Your ID is : %d\n", newAdmin.admin_id);
                write(fd, &newAdmin, sizeof(struct Admin));

                printf("Do you want to continue (1 for Yes, 0 for No)? ");
                scanf("%d", &choice);
	}
	close(fd);

	return 0;
}
/*
int main() {
    int choice;
    struct Admin newAdmin;
    newAdmin.admin_id = getNewAdmin();
    int fd = open("/home/vraj-naik/sslab/SS_Project/VidyaSynergy/data/admin.data", O_RDWR | O_CREAT | O_APPEND, 0744);
    
    if (fd == -1) {
        perror("Error opening the file");
        return 1;
    }

    printf("Please Enter the name of the admin: ");
    scanf(" %[^\n]", newAdmin.username);

    // Ensure password meets criteria
    char tempPassword[11]; // Temporary variable for password input
    do {
        printf("Please Enter the password (max 10 characters) with at least one uppercase, one lowercase, and one special character: ");
        getPassword(tempPassword, sizeof(tempPassword));

        if (isValidPassword(tempPassword)) {
            break;
        } else {
            printf("Invalid password. Password must meet the criteria.\n");
        }
    } while (1);

    // Hash the password
    //hashPassword(tempPassword, newAdmin.password);

    printf("Hey!!! Congratulations....!!! Your ID is : %d\n", newAdmin.admin_id);
    write(fd, &newAdmin, sizeof(struct Admin));

    printf("Do you want to continue (1 for Yes, 0 for No)? ");
    scanf("%d", &choice);
    
    while (choice) {
        newAdmin.admin_id = getNewAdmin();
        printf("Please Enter the name of the admin: ");
        scanf(" %[^\n]", newAdmin.username);

        // Ensure password meets criteria
        do {
            printf("Please Enter the password (max 10 characters) with at least one uppercase, one lowercase, and one special character: ");
            getPassword(tempPassword, sizeof(tempPassword));

            if (isValidPassword(tempPassword)) {
                break;
            } else {
                printf("Invalid password. Password must meet the criteria.\n");
            }
        } while (1);

        // Hash the password
        hashPassword(tempPassword, newAdmin.password);

        printf("Hey!!! Your ID is : %d\n", newAdmin.admin_id);
        write(fd, &newAdmin, sizeof(struct Admin));

        printf("Do you want to continue (1 for Yes, 0 for No)? ");
        scanf("%d", &choice);
    }

    close(fd);
    return 0;
}
*/
