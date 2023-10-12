#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>  
#include<arpa/inet.h>  
#include<unistd.h>
#include<stdbool.h>
#include<strings.h>
#include<string.h>

#include "./Structs/Admin.h"
#include "./Structs/getPass.h"

int option, currUserID;

void chooseOption(int sd){
	printf("1 : Student Login\n");
	printf("2 : Faculty Login\n");
	printf("3 : Admin Login\n");
	printf("Choose an option : ");
	scanf("%d", &option);
	printf("Option : %d\n", option);

	switch(option) {
		case 1 :
			attemptStudentLogin(sd);
			break;
		case 2 :
			attemptFacultyLogin(sd);
			break;
		case 3 :
			attemptAdminLogin(sd);
			break;
		default :
			printf("Invalid option!\n\n");
			chooseOption(sd);
			break;
	}
	return;
}

void attemptStudentLogin(int sd) {
	bool result;
	struct Student currUser;
	printf("User ID : ");
	scanf("%d", &currUser.student_id);
	currUserID = currUser.student_id;
	printf("Password : ");
	char* pass = getpass("");
	strcpy(currUser.password, pass);

	getchar();
	write(sd, &option, sizeof(int));
	write(sd, &currUser, sizeof(struct Student));

	read(sd, &result, sizeof(result));

	if(!result) {
		printf("Invalid login!\nEither your Password didn't match OR your Account is Deleted\n");
		chooseOption(sd);
	}
	else {
		printf("Succesfully logged in!\n\n");
	}
	return;
}
void attemptAdminLogin(int sd){
	bool result;
	struct admin currUser;
	printf("User ID : ");
	scanf("%d", &currUser.admin_id);
	currUserID = currUser.admin_id;
	printf("Password : ");
	char* pass = getpass("");
	strcpy(currUser.password, pass);

	write(sd, &option, sizeof(int));
	write(sd, &currUser, sizeof(struct admin));

	read(sd, &result, sizeof(result));

	if(!result) {
		printf("Invalid login!\nEither your Password didn't match OR your Account is Deleted\n");
		chooseOption(sd);
	}
	else {
		printf("Succesfully logged in!\n\n");
	}
	return;
}
void addAccount(int sd){
	int select = 1;
	int type;
	bool result;
	char rdBuff[1000];
	bzero(rdBuff, sizeof(rdBuff));

	write(sd, &select, sizeof(int));

	printf("Enter the type account\n1: Student\n2: Faculty\n");
	scanf("%d", &type);
	write(sd, &type, sizeof(int));
	if(type == 1) {
		struct Student addUser1;
		printf("Name of the Student : ");
		scanf(" %[^\n]",addUser1.name);
		printf("Password(max 10 characters) : ");
		char* pass = getpass("");
        	strcpy(addUser1.password, pass);
		write(sd, &addUser1, sizeof(struct Student));
		read(sd, rdBuff, sizeof(rdBuff));
	}

	if(type == 2) {
		struct Faculty addUser2;
		printf("Name of the Faculty : ");
		scanf(" %[^\n]", addUser2.name1);
		printf("Password(max 10 characters) : ");
		char* pass = getpass("");
                strcpy(addUser2.password, pass);
		write(sd, &addUser2, sizeof(struct Faculty));
		read(sd, rdBuff, sizeof(rdBuff));
	}

	read(sd, &result, sizeof(result));

	if(!result) {
		printf("Error adding the account!\n\n");
	}
	else {
		printf("Succesfully added the account!\n\n");
	}
	printf("%s\n", rdBuff);
	showMenu(sd);
	return;
}

int main() {
	system("clear");
	printf("Welcome to VidyaSynergy\n\n");
	
	struct sockaddr_in server;    
	int sd, msgLength;
	char buff[50];
	char result;

	sd = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(5555);

	connect(sd, (struct sockaddr *)&server, sizeof(server));
	chooseOption(sd);
	close(sd);
	return 0;
}
