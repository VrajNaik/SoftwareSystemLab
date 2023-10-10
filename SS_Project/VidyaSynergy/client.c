#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>  
#include<arpa/inet.h>  
#include<unistd.h>
#include<stdbool.h>
#include<strings.h>
#include<string.h>

#include "./Structs/Admin.h"
#include "./Structs/getPass.h"

void chooseOption(int sd){
	printf("1 : Student Login\n");
	printf("2 : Faculty Login\n");
	printf("3 : Admin Login\n");
	printf("Choose an option : ");
	//red();
	scanf("%d", &option);
	//yellow();
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
void attemptStudentLogin(int sd){
	bool result;
	struct Student currUser;
	printf("User ID : ");
	scanf("%d", &currUser.userID);
	currUserID = currUser.userID;
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

int main(){
	system("clear");
	yellow();
	printf("Welcome to ");
        printf("VidyaSynergy\n\n");
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
	//showMenu(sd);
	close(sd);
	return 0;
}
