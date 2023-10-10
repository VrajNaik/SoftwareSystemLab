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
	int msgLength,select,type,option,accType,currUserID;
	bool result;
	while(1){
		read(nsd,&option,sizeof(option));
		printf("Option : %d\n",option);

		if(option==1){
			struct normalUser currUser1;
			accType=1;
			msgLength=read(nsd,&currUser1,sizeof(struct normalUser));
			printf("Username : %d\n",currUser1.userID);
			printf("Password : %s\n",currUser1.password);
			currUserID=currUser1.userID;
			result=checkNormalUser(currUser1);
			write(nsd,&result,sizeof(result));
		}
		else if(option==2){
			struct jointUser currUser2;
			accType=2;
			msgLength=read(nsd,&currUser2,sizeof(struct jointUser));
			currUserID=currUser2.userID;
			printf("Username : %d\n",currUser2.userID);
			printf("Password : %s\n",currUser2.password);
			result=checkJointUser(currUser2);
			write(nsd,&result,sizeof(result));
		}
		else if(option==3){
			struct admin currUser3;
			accType=3;
			msgLength=read(nsd,&currUser3,sizeof(struct admin));
			currUserID=currUser3.userID;
			printf("Username : %d\n",currUser3.userID);
			printf("Password : %s\n",currUser3.password);
			result=checkAdmin(currUser3);
			write(nsd,&result,sizeof(result));
		}
		else{
			result=false;
			write(nsd,&result,sizeof(result));
		}
		if(result)	break;
	}

	while(1){
		read(nsd,&select,sizeof(int));
		if(option==1 || option==2){
			if(select==1){
				float amt;
				read(nsd,&amt,sizeof(float));
				result=depositMoney(accType,currUserID,amt);
				write(nsd,&result,sizeof(result));
			}
			else if(select==2){
				float amt;
				read(nsd,&amt,sizeof(float));
				result=withdrawMoney(accType,currUserID,amt);
				write(nsd,&result,sizeof(result));
			}
			else if(select==3){
				float amt;
				amt=getBalance(accType,currUserID);
				write(nsd,&amt,sizeof(float));
			}
			else if(select==4){
				char oldPwd[10];
				char newPwd[10];
				read(nsd,oldPwd,sizeof(oldPwd));
				read(nsd,newPwd,sizeof(newPwd));
				result=alterPassword(accType,currUserID,newPwd,oldPwd);
				write(nsd,&result,sizeof(result));
			}
			else if(select==5){
				if(option==1){
					struct normalUser user1=getNormalUser(currUserID);
					write(nsd,&user1,sizeof(struct normalUser));
				}
				else if(option==2){
					struct jointUser user2=getJointUser(currUserID);
					write(nsd,&user2,sizeof(struct jointUser));
				}
			}
			else if(select==6)	break;
		}
		else if(option==3){
			read(nsd,&type,sizeof(int));
			if(select==1){
				if(type==1){
					struct normalUser newUser1;
					read(nsd,&newUser1,sizeof(struct normalUser));
					result=addNormalUser(newUser1, nsd);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					struct jointUser newUser2;
					read(nsd,&newUser2,sizeof(struct jointUser));
					result=addJointUser(newUser2, nsd);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(select==2){
				if(type==1){
					int delUserID1;
					read(nsd,&delUserID1,sizeof(int));
					result=deleteNormalUser(delUserID1);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					int delUserID2;
					read(nsd,&delUserID2,sizeof(int));
					result=deleteJointUser(delUserID2);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(select==3){
				if(type==1){
					struct normalUser modUser1;
					read(nsd,&modUser1,sizeof(struct normalUser));
					result=modifyNormalUser(modUser1);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					struct jointUser modUser2;
					read(nsd,&modUser2,sizeof(struct jointUser));
					result=modifyJointUser(modUser2);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(select==4){
				if(type==1){
					struct normalUser searchUser1;
					int userID1;
					read(nsd,&userID1,sizeof(int));
					searchUser1=getNormalUser(userID1);
					write(nsd,&searchUser1,sizeof(struct normalUser));
				}
				else if(type==2){
					struct jointUser searchUser2;
					int userID2;
					read(nsd,&userID2,sizeof(int));
					searchUser2=getJointUser(userID2);
					write(nsd,&searchUser2,sizeof(struct jointUser));
				}
			}
			else if(select==5)	break;
		}
	}
	close(nsd);
	write(1,"Client session Ended\n",sizeof("Client session Ended\n"));
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
    	//red();
   	printf("Welcome to ");
    	//yellow();
    	printf("VidyaSynergy\n");
    	//reset();
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
