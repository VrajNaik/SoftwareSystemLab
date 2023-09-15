#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
int main(){
	pid_t pid;
	time_t time_now;
	struct tm exec_time;
	
	exec_time.tm_hour = 23;
	exec_time.tm_min = 35;
	exec_time.tm_sec = 0;
	
	while(1){
		time(&time_now); //Adds current time in time_now
		struct tm* curr_time = localtime(&time_now);
		if(curr_time->tm_hour == exec_time.tm_hour && curr_time->tm_min == exec_time.tm_min){
			pid = fork(); //create a child process to execute the script
			
			if(pid==0){
				execlp("ls","ls","-l",NULL); // we want to excute ls with argument -l..execlp typically used to execute scripts
				exit(1);
			}
			else if(pid>0){
				int status;
				waitpid(pid,&status,0); //wait for child to complete
				if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                   			 printf("Script executed successfully.\n");
                    			 break; // Exit the loop
                		} 	
				else {
                    			printf("Script execution failed.\n");
                		}
			}
		} 
	sleep(1); //sleep for 60 seconds before checking the time again
	}
}
