#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include "./Structs/Admin.h"
#include "./Structs/Faculty.h"
#include "./Structs/Student.h"
#include "./Structs/Course.h"

void serverTask(int nsd){
	int msgLength, select, type, option, accType, currUserID;
	bool result;
	while(1){
		read(nsd, &option, sizeof(option));
		printf("Option : %d\n", option);

		if(option == 1) {
			struct Student currUser1;
			msgLength = read(nsd, &currUser1, sizeof(struct Student));
			printf("Username : %d\n", currUser1.userID);
			printf("Password : %s\n", currUser1.password);
			currUserID = currUser1.userID;
			result = checkNormalUser(currUser1);
			write(nsd, &result, sizeof(result));
		}
		else if(option == 3) {
			struct Admin currUser3;
			accType = 3;
			msgLength = read(nsd, &currUser3, sizeof(struct Admin));
			currUserID = currUser3.userID;
			printf("Username : %d\n", currUser3.userID);
			printf("Password : %s\n", currUser3.password);
			result = checkAdmin(currUser3);
			write(nsd, &result, sizeof(result));
		}
		else { 
			result = false;
			write(nsd, &result, sizeof(result));
		}
		if(result)	
		        break;
	}
	  close(nsd);
	  write(1, "Client session Ended\n", sizeof("Client session Ended\n"));
	  return;
}

int main(){
	struct sockaddr_in server, client;
	int sd, nsd, clientLen;
	bool result;
	sd = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;    
	server.sin_addr.s_addr = INADDR_ANY; 
	server.sin_port = htons(5555);
	bind(sd, (struct sockaddr *)&server, sizeof(server));
	listen(sd, 5);	     
   	printf("Welcome to VidyaSynergy\n");
	printf("Waiting for Client to connect\n");
	while(1) {
		clientLen = sizeof(client);
		nsd = accept(sd, (struct sockaddr *)&client, &clientLen);

		write(1, "Connected to the client\n", sizeof("Connected to the client\n"));
		if(!fork()) {
			close(sd);
			serverTask(nsd);
			exit(0);
		}
		else {
			close(nsd);
		}
	}
	return 0;
}
