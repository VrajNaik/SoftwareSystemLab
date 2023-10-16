#ifndef CUSTOMER_FUNCTIONS
#define CUSTOMER_FUNCTIONS

#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include "../record-struct/Enrollment.h"
#include "../record-struct/Course.h"

struct Student loggedInCustomer;
int semIdentifier;

// Function Prototypes =================================

bool student_operation_handler(int connFD);
bool lock_critical_section(struct sembuf *semOp);
bool unlock_critical_section(struct sembuf *semOp);
bool change_password(int connFD);


struct Enrollment enroll;

int Logout(int connFD);
int view_all_courses(int connFD);
int enroll_course(int connFD);
int view_enrolled_courses(int connFD);
int drop_course(int connFD);

// =====================================================

// Function Definition =================================

bool student_operation_handler(int connFD)
{
    if (login_handler(1, connFD, &loggedInCustomer, NULL))
    {
        ssize_t writeBytes, readBytes;            // Number of bytes read from / written to the client
        char readBuffer[1000], writeBuffer[1000]; // A buffer used for reading & writing to the client

        // Get a semaphore for the user
        key_t semKey = ftok(STUDENT_FILE, loggedInCustomer.id); // Generate a key based on the account number hence, different customers will have different semaphores

        union semun
        {
            int val; // Value of the semaphore
        } semSet;

        int semctlStatus;
        semIdentifier = semget(semKey, 1, 0); // Get the semaphore if it exists
        if (semIdentifier == -1)
        {
            semIdentifier = semget(semKey, 1, IPC_CREAT | 0700); // Create a new semaphore
            if (semIdentifier == -1)
            {
                perror("Error while creating semaphore!");
                _exit(1);
            }

            semSet.val = 1; // Set a binary semaphore
            semctlStatus = semctl(semIdentifier, 0, SETVAL, semSet);
            if (semctlStatus == -1)
            {
                perror("Error while initializing a binary sempahore!");
                _exit(1);
            }
        }

        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, CUSTOMER_LOGIN_SUCCESS);
        while (1)
        {
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, CUSTOMER_MENU);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing CUSTOMER_MENU to client!");
                return false;
            }
            bzero(writeBuffer, sizeof(writeBuffer));

            bzero(readBuffer, sizeof(readBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error while reading client's choice for CUSTOMER_MENU");
                return false;
            }
          
            int choice = atoi(readBuffer);
            switch (choice)
            {
                case 1:
                    view_all_courses(connFD);
                    break;
                case 2:
                    enroll_course(connFD);
                    break;
                case 3: 
                    drop_course(connFD);
                    break;
                case 4:
                    view_enrolled_courses(connFD);
                    break;
                case 5:
                    change_password(connFD);
                    break;
                case 6:
                    Logout(connFD);
                    break;    
                default:
                    writeBytes = write(connFD,"wrong choice ^",14);
                    readBytes = read(connFD,readBuffer,sizeof(readBuffer));
                    break;
            }
        }
    }
    else
    {
        // CUSTOMER LOGIN FAILED
        return false;
    }
    return true;
}

bool change_password(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000], hashedPassword[1000];

    char newPassword[1000];

    // Lock the critical section
    struct sembuf semOp = {0, -1, SEM_UNDO};
    int semopStatus = semop(semIdentifier, &semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while locking critical section");
        return false;
    }

    writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS, strlen(PASSWORD_CHANGE_OLD_PASS));
    if (writeBytes == -1)
    {
        perror("Error writing PASSWORD_CHANGE_OLD_PASS message to client!");
        unlock_critical_section(&semOp);
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading old password response from client");
        unlock_critical_section(&semOp);
        return false;
    }

    if (strcmp(crypt(readBuffer, SALT_BAE), loggedInCustomer.password) == 0)
    {
        // Password matches with old password
        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS, strlen(PASSWORD_CHANGE_NEW_PASS));
        if (writeBytes == -1)
        {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS message to client!");
            unlock_critical_section(&semOp);
            return false;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading new password response from client");
            unlock_critical_section(&semOp);
            return false;
        }

        strcpy(newPassword, crypt(readBuffer, SALT_BAE));

        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_RE, strlen(PASSWORD_CHANGE_NEW_PASS_RE));
        if (writeBytes == -1)
        {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS_RE message to client!");
            unlock_critical_section(&semOp);
            return false;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading new password reenter response from client");
            unlock_critical_section(&semOp);
            return false;
        }

        if (strcmp(crypt(readBuffer, SALT_BAE), newPassword) == 0)
        {
            // New & reentered passwords match

            strcpy(loggedInCustomer.password, newPassword);

            int customerFileDescriptor = open(STUDENT_FILE, O_WRONLY);
            if (customerFileDescriptor == -1)
            {
                perror("Error opening student file!");
                unlock_critical_section(&semOp);
                return false;
            }

            off_t offset = lseek(customerFileDescriptor, loggedInCustomer.id * sizeof(struct Student), SEEK_SET);
            if (offset == -1)
            {
                perror("Error seeking to the student record!");
                unlock_critical_section(&semOp);
                return false;
            }

            struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Student), getpid()};
            int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
            if (lockingStatus == -1)
            {
                perror("Error obtaining write lock on student record!");
                unlock_critical_section(&semOp);
                return false;
            }

            writeBytes = write(customerFileDescriptor, &loggedInCustomer, sizeof(struct Student));
            if (writeBytes == -1)
            {
                perror("Error storing updated student password into student record!");
                unlock_critical_section(&semOp);
                return false;
            }

            lock.l_type = F_UNLCK;
            lockingStatus = fcntl(customerFileDescriptor, F_SETLK, &lock);

            close(customerFileDescriptor);

            writeBytes = write(connFD, PASSWORD_CHANGE_SUCCESS, strlen(PASSWORD_CHANGE_SUCCESS));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

            unlock_critical_section(&semOp);

            return true;
        }
        else
        {
            // New & reentered passwords don't match
            writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_INVALID, strlen(PASSWORD_CHANGE_NEW_PASS_INVALID));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        }
    }
    else
    {
        // Password doesn't match with old password
        writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS_INVALID, strlen(PASSWORD_CHANGE_OLD_PASS_INVALID));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    }

    unlock_critical_section(&semOp);

    return false;
}

bool lock_critical_section(struct sembuf *semOp)
{
    semOp->sem_flg = SEM_UNDO;
    semOp->sem_op = -1;
    semOp->sem_num = 0;
    int semopStatus = semop(semIdentifier, semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while locking critical section");
        return false;
    }
    return true;
}

bool unlock_critical_section(struct sembuf *semOp)
{
    semOp->sem_op = 1;
    int semopStatus = semop(semIdentifier, semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while operating on semaphore!");
        _exit(1);
    }
    return true;
}

int drop_course(int connFD){

  ssize_t readBytes, writeBytes;             
  char readBuffer[1000], writeBuffer[10000];
  int enrollFileDescriptor, courseFileDescriptor;
  bool flag;
  struct Enrollment enroll;
  struct Course course;
  int enrollID,n;
  struct flock lock = {F_WRLCK, SEEK_SET, 0, sizeof(struct Enrollment), getpid()};
  
  // checking whether student has access for courses or not
  if(!loggedInCustomer.status){
    write(connFD,"You are blocked by admin ^",26);
    readBytes = read(connFD,readBuffer,sizeof(readBuffer));
    return 0;
  }

  //else asking for courseid
  writeBytes = write(connFD,"Enter the course-id you want to drop:",37);
  readBytes = read(connFD,readBuffer,sizeof(readBuffer));
  if(readBytes==-1){
    perror("unable to get unenroll courseid response from client");
    return 0;
  }
  
  enrollFileDescriptor = open(ENROLL_FILE,O_RDONLY);
  while((n = read(enrollFileDescriptor, &enroll, sizeof(struct Enrollment))) > 0) {
    if((strcmp(enroll.status,"enrolled")==0) && (strcmp(enroll.studentid,loggedInCustomer.login)==0) && (strcmp(enroll.courseid,readBuffer)==0)){              
        enrollID= enroll.id;
        flag=true;
        break;  
    }
  }
  close(enrollFileDescriptor);
  
  // if student is not enrolled to drop (or) course already unerolled or course not present then throw error
  if(flag==false){
    write(connFD,"Invalid course id ^",20);
    readBytes = read(connFD,readBuffer,sizeof(readBuffer));
    return 0;
  } 

  // seeking to the record  
  enrollFileDescriptor = open(ENROLL_FILE,O_RDONLY);
  int offset = lseek(enrollFileDescriptor,(enrollID-1)*sizeof(struct Enrollment),SEEK_SET);
  if (offset == -1){
    perror("Error while seeking to required course record!");
    return 0;
  }

  lock.l_type = F_RDLCK;
  lock.l_start = offset;
  int lockingStatus = fcntl(enrollFileDescriptor, F_SETLKW, &lock);
  if (lockingStatus == -1){
      perror("Error while obtaining read lock on course record!");
      return 0;
  }
  readBytes = read(enrollFileDescriptor, &enroll, sizeof(struct Enrollment));
  if(readBytes == -1){
    perror("Error while reading enrollment record from the file!");
    return 0;
  }

  // if student is not enrolled throw error
  if(strcmp(enroll.status,"unenrolled")==0){
      write(connFD,"Not enrolled ^",14);
      readBytes = read(connFD,readBuffer,sizeof(readBuffer));
      return 0;
  }
  lock.l_type = F_UNLCK;
  lockingStatus = fcntl(courseFileDescriptor, F_SETLK, &lock);   
  close(enrollFileDescriptor);

  // change status to unenrolled
  strcpy(enroll.status,"unenrolled");
  
  //updating record
  enrollFileDescriptor = open(ENROLL_FILE,O_WRONLY);
  if (enrollFileDescriptor == -1){
        perror("Error while opening enrollment file");
        return 0;
  }

  offset = lseek(enrollFileDescriptor, (enrollID-1) * sizeof(struct Enrollment), SEEK_SET);
  if(offset == -1){
    perror("Error while seeking to required enrollment record!");
    return 0;
  }

  lock.l_type = F_WRLCK;
  lock.l_start = offset;
  lockingStatus = fcntl(enrollFileDescriptor, F_SETLKW, &lock);
  if(lockingStatus == -1){
    perror("Error while obtaining write lock on enrollment record!");
    return 0;
  }
  writeBytes = write(enrollFileDescriptor, &enroll, sizeof(struct Enrollment));
  if (writeBytes == -1){
    perror("Error while writing update enrollment info into file");
  }

  lock.l_type = F_UNLCK;
  fcntl(enrollFileDescriptor, F_SETLKW, &lock);
  close(enrollFileDescriptor);
  
  int courseID;

  //Increasing no of seats of that particular course if student dropped the course successfully
  courseFileDescriptor = open(COURSE_FILE,O_RDONLY);
  while((n = read(courseFileDescriptor, &course, sizeof(struct Course))) > 0) {
    if((strcmp(course.status,"notactive")!=0) && (strcmp(course.courseid,readBuffer)==0)){              
        courseID= course.id;
        break;  
    }
  }
  close(courseFileDescriptor);
  
  //reading the record of the course
  courseFileDescriptor = open(COURSE_FILE,O_RDONLY);
  offset = lseek(courseFileDescriptor,(courseID-1)*sizeof(struct Course),SEEK_SET);
  if (offset == -1){
    perror("Error while seeking to required course record!");
    return 0;
  }
  lock.l_type = F_RDLCK;
  lock.l_start = offset;
  lockingStatus = fcntl(courseFileDescriptor, F_SETLKW, &lock);
  if (lockingStatus == -1){
      perror("Error while obtaining read lock on course record!");
      return 0;
  }
  readBytes = read(courseFileDescriptor, &course, sizeof(struct Course));
  if(readBytes == -1){
    perror("Error while reading course record from the file!");
    return 0;
  }
  lock.l_type = F_UNLCK;
  lockingStatus = fcntl(courseFileDescriptor, F_SETLK, &lock);   
  close(enrollFileDescriptor);

  // Increasing the no of available seats
  course.no_of_available_seats=course.no_of_available_seats+1;
  
  //updating the course record after increasing
  courseFileDescriptor = open(COURSE_FILE,O_WRONLY);
  if (courseFileDescriptor == -1){
        perror("Error while opening course file");
        return 0;
  }
  offset = lseek(courseFileDescriptor, (courseID-1) * sizeof(struct Course), SEEK_SET);
  if(offset == -1){
    perror("Error while seeking to required course record!");
    return 0;
  }
  lock.l_type = F_WRLCK;
  lock.l_start = offset;
  lockingStatus = fcntl(courseFileDescriptor, F_SETLKW, &lock);
  if(lockingStatus == -1){
    perror("Error while obtaining write lock on course record!");
    return 0;
  }
  writeBytes = write(courseFileDescriptor, &course, sizeof(struct Course));
  if (writeBytes == -1){
    perror("Error while writing update course info into file");
  }

  lock.l_type = F_UNLCK;
  fcntl(courseFileDescriptor, F_SETLKW, &lock);
  close(courseFileDescriptor);

  // success message
  writeBytes = write(connFD, ADD_UNENROLL_SUCCESS,strlen(ADD_UNENROLL_SUCCESS));
  if(writeBytes == -1){
      perror("Error writing ADD_UNENROLL_SUCCESS message to client!");
      return false;
  }
  readBytes = read(connFD,readBuffer,sizeof(readBuffer));
  return 0;
}

// viewing course details
int view_enrolled_courses(int connFD){
    ssize_t readBytes, writeBytes;             
    char readBuffer[1000], writeBuffer[10000]; 
    char tempBuffer[1000];
    char temp1[15][10];
    int i,n;
    int enrollFileDescriptor;
    int courseFileDescriptor;
    struct Enrollment enroll;
    struct Course course;
    bool flag;
    
    //checking whether the student have access or not
    if(!loggedInCustomer.status){
       write(connFD,"You are blocked by admin ^",26);
       readBytes = read(connFD,readBuffer,sizeof(readBuffer));
       return 0;
    }

    enrollFileDescriptor = open(ENROLL_FILE,O_RDONLY);
    while((n = read(enrollFileDescriptor, &enroll, sizeof(struct Enrollment))) > 0) {
        if((strcmp(enroll.status,"enrolled")==0) && (strcmp(enroll.studentid,loggedInCustomer.login)==0)){              
           flag=true;
           strcpy(temp1[i],enroll.courseid);
           i++;   
        }
    }

    // if there are no enrolled courses available
    if(flag==false){
       write(connFD,"No enrolled courses ^",21);
       readBytes = read(connFD,readBuffer,sizeof(readBuffer));  
    }

    bool magic[20]={false};

    //Displaying the enrolled courses
    for(int i=0;i<15;i++){
        courseFileDescriptor = open(COURSE_FILE,O_RDONLY);
        while((n = read(courseFileDescriptor, &course, sizeof(struct Course))) > 0) {
            if(magic[course.id]==false && (strcmp(course.status,"notactive")!=0) && (strcmp(course.courseid,temp1[i])==0)){                 
                bzero(writeBuffer,sizeof(writeBuffer));
                magic[course.id]=true;   
                sprintf(writeBuffer, " ^ ********* Course Details *********  \n\tName: %s\n\tDepartment : %s\n\tNo of Seats: %d\n\tCredits : %d\n\tNo of available seats: %d\n\tCourse-id: %s\n", course.name, course.department,course.no_of_seats,course.credits,course.no_of_available_seats,course.courseid);
                writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
                if(writeBytes == -1){
                    perror("Error writing course info to client!");
                    return 0;
                }
                readBytes = read(connFD,readBuffer,sizeof(readBuffer));  //dummy read
                break;
            } 
        }
        close(courseFileDescriptor);
    }
    return 1;   
}

// Enrolling new course
int enroll_course(int connFD){
    ssize_t readBytes, writeBytes;             
    char readBuffer[1000], writeBuffer[10000]; 
    char tempBuffer[1000];
    struct Course fetchcourse;
    int courseFileDescriptor;
    struct flock lock = {F_WRLCK, SEEK_SET, 0, sizeof(struct Course), getpid()};
    
    // checking whether student has access to courses or not
    if(!loggedInCustomer.status){
       write(connFD,"You are blocked by admin ^",26);
       readBytes = read(connFD,readBuffer,sizeof(readBuffer));
       return 0;
    }

    courseFileDescriptor = open(COURSE_FILE, O_RDONLY);
    if (courseFileDescriptor == -1)
    {
        // Course File doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "course file id doesn't exists");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes ==-1)
        {
            perror("Error while writing COURSE_ID_DOESNT_EXIT message to client!");
            return 0;
        }
        return 0;
    }

    int n;
    int i=1;
    char temp1[15][10];

    //Displaying available courses
    strcpy(readBuffer,"\n........Available Courses.......\n");
    while((n = read(courseFileDescriptor, &fetchcourse, sizeof(struct Course))) > 0) {
        char temp[200];
        if(strcmp(fetchcourse.status,"notactive")==0)
           continue;
        strcpy(temp1[i],fetchcourse.courseid);
        i++;   
        strcpy(temp,fetchcourse.courseid);
        strcat(temp,"\t--\t");
        strcat(temp,fetchcourse.name);
        strcat(temp,"\n");   
        strcat(readBuffer,temp);   
    }
    close(courseFileDescriptor);

    //Asking for course id to enroll
    strcat(readBuffer,"\nEnter course id you want to enroll:");
    writeBytes = write(connFD, readBuffer, strlen(readBuffer));
    if(writeBytes == -1){
        perror("Error writing course info to client!");
        return 0;
    }
    bzero(readBuffer,sizeof(readBuffer));
    readBytes= read(connFD,readBuffer,sizeof(readBuffer));
    if(readBytes==-1){
        perror("unable to read choice from client");
        return 0;
    }

    int courseID;

    // We already stored available course codes in temp go the matched record and make it enrolled
    for(int i=0;i<15;i++){
        if(strcmp(readBuffer,temp1[i])==0){

            //fetching id from coursecode
            char *ftPosition = strstr(readBuffer, "C-");
            char *numberStart = NULL;
            if(ftPosition!=NULL) {
               numberStart = ftPosition + strlen("C-");
               courseID = atoi(numberStart);
            }

            struct Course course;
            courseFileDescriptor = open(COURSE_FILE, O_RDONLY);
            int offset = lseek(courseFileDescriptor, (courseID-1) * sizeof(struct Course), SEEK_SET);
            if (offset == -1){
               perror("Error while seeking to required course record!");
               return 0;
            }
            
            lock.l_type = F_RDLCK;
            lock.l_start = offset;
            int lockingStatus = fcntl(courseFileDescriptor, F_SETLKW, &lock);
            if (lockingStatus == -1){
              perror("Error while obtaining read lock on course record!");
              return 0;
            }

            readBytes = read(courseFileDescriptor, &course, sizeof(struct Course));
            if(readBytes == -1){
                perror("Error while reading course record from the file!");
                return 0;
            }
            lock.l_type = F_UNLCK;
            lockingStatus = fcntl(courseFileDescriptor, F_SETLK, &lock);
            
            // If there are no seats present in the course cant enroll
            if(course.no_of_available_seats==0){
               write(connFD,"No available seats to enroll ^",30);
               close(courseFileDescriptor);
               readBytes = read(connFD,readBuffer,sizeof(readBuffer));
               return 0;
            }
            else{
                int n,enrollID;
                bool flag=false;

                int enrollfd = open(ENROLL_FILE,O_RDONLY,0700);
                struct Enrollment enroll; 
                
                while((n = read(enrollfd,&enroll, sizeof(struct Enrollment)))> 0){

                   //checking whether he already enrolled or not 
                   if((strcmp(readBuffer,enroll.courseid)==0) && (strcmp(loggedInCustomer.login,enroll.studentid)==0) && (strcmp(enroll.status,"enrolled")==0)){
                       writeBytes = write(connFD, "Already enrolled ^",18);
                       readBytes = read(connFD,readBuffer,sizeof(readBuffer));
                       return 0;                
                   }

                   //if particular record is already present in enrollment file then fetch the id so that we can change the status to enrolled 
                   else if((strcmp(readBuffer,enroll.courseid)==0) && (strcmp(loggedInCustomer.login,enroll.studentid)==0) && (strcmp(enroll.status,"unenrolled")==0)){
                      flag=true;
                      enrollID= enroll.id;
                      break;                           
                   }       
                }

                // If that particular studentid+courseid record already present 
                if(flag==true){

                    close(enrollfd);
                    enrollfd = open(ENROLL_FILE,O_RDONLY);
                    int offset = lseek(enrollfd,(enrollID-1)*sizeof(struct Enrollment),SEEK_SET);
                    if(offset == -1){
                        perror("Error while seeking to required course record!");
                        return 0;
                    }
                    
                    lock.l_type = F_RDLCK;
                    lock.l_start = offset;
                    int lockingStatus = fcntl(enrollfd, F_SETLKW, &lock);
                    if(lockingStatus == -1){
                        perror("Error while obtaining read lock on enrollment record!");
                        return 0;
                    }
                    readBytes = read(enrollfd, &enroll, sizeof(struct Enrollment));
                    if(readBytes == -1){
                        perror("Error while reading enrollment record from the file!");
                        return 0;
                    }
                    lock.l_type = F_UNLCK;
                    lockingStatus = fcntl(enrollfd, F_SETLK, &lock);   
                    close(enrollfd);

                    // change the status of that record to enrolled
                    strcpy(enroll.status,"enrolled");

                    //also maintaining time of enrollment
                    enroll.enroll_time= time(NULL);

                    //updating the record details
                    enrollfd = open(ENROLL_FILE,O_WRONLY);
                    if (enrollfd == -1){
                        perror("Error while opening enrollment file");
                        return 0;
                    }

                    offset = lseek(enrollfd, (enrollID-1) * sizeof(struct Enrollment), SEEK_SET);
                    if(offset == -1){
                      perror("Error while seeking to required enrollment record!");
                      return 0;
                    }

                    lock.l_type = F_WRLCK;
                    lock.l_start = offset;
                    lockingStatus = fcntl(enrollfd, F_SETLKW, &lock);
                    if(lockingStatus == -1){
                        perror("Error while obtaining write lock on enrollment record!");
                        return 0;
                    }
                    writeBytes = write(enrollfd, &enroll, sizeof(struct Enrollment));
                    if (writeBytes == -1){
                        perror("Error while writing update enrollment info into file");
                    }

                     lock.l_type = F_UNLCK;
                     fcntl(enrollfd, F_SETLKW, &lock);
                     close(enrollfd);
                } 

                // if the record of particular courseid+studentid not present create a new record and update the status as enrolled
                else{
                    if(enrollfd== -1){
                     // Enrollment file was never created
                     enroll.id = 1;
                    }
                    else{

                        //fetching id of previous record
                        int offset = lseek(enrollfd,-sizeof(struct Enrollment),SEEK_END);
                        if(offset == -1){
                            perror("Error seeking to last enrollment record!");
                            return 0;
                        }
                        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Enrollment), getpid()};
                        int lockingStatus = fcntl(enrollfd, F_SETLKW, &lock);
                        if(lockingStatus == -1){
                            perror("Error obtaining read lock on enroll record!");
                            return 0;
                        }
                        struct Enrollment previousenroll;
                        readBytes = read(enrollfd,&previousenroll, sizeof(struct Enrollment));
                        if(readBytes == -1){
                            perror("Error while reading enrollment record from file!");
                            return 0;
                        }
                        lock.l_type = F_UNLCK;
                        fcntl(enrollfd, F_SETLK, &lock);
                        close(enrollfd);
 
                        enroll.id = previousenroll.id + 1;
                    }

                    // status---> enrolled
                    strcpy(enroll.courseid,readBuffer);
                    strcpy(enroll.studentid,loggedInCustomer.login);
                    strcpy(enroll.status,"enrolled");
                    enroll.enroll_time= time(NULL);

                    //creating new enrollment
                    enrollfd = open(ENROLL_FILE,O_CREAT|O_APPEND|O_WRONLY,S_IRWXU);
                    if(enrollfd == -1){
                        perror("Error while opening enroll file!");
                        return 0;
                    }
                    writeBytes = write(enrollfd, &enroll,sizeof(struct Enrollment));
                    if(writeBytes == -1){
                        perror("Error while writing enroll record to file!");
                        return 0;
                    }
                } 
            }
            close(courseFileDescriptor);
            
            //After enrollment decreasing the no of available seats
            course.no_of_available_seats=course.no_of_available_seats-1;                 
            
            //updating the course record
            courseFileDescriptor = open(COURSE_FILE, O_WRONLY);
            if(courseFileDescriptor == -1){
                perror("Error while opening course file");
                return 0;
            }
            offset = lseek(courseFileDescriptor, (courseID-1) * sizeof(struct Course), SEEK_SET);
            if (offset == -1){
               perror("Error while seeking to required course record!");
               return false;
            }

            lock.l_type = F_WRLCK;
            lock.l_start = offset;
            lockingStatus = fcntl(courseFileDescriptor, F_SETLKW, &lock);
            if (lockingStatus == -1){
                perror("Error while obtaining write lock on course record!");
                return false;
            }
            writeBytes = write(courseFileDescriptor, &course, sizeof(struct Course));
            if (writeBytes == -1){
                 perror("Error while writing update course info into file");
            }
            lock.l_type = F_UNLCK;
            fcntl(courseFileDescriptor, F_SETLKW, &lock);
            close(courseFileDescriptor);

            //success message
            writeBytes = write(connFD, ADD_ENROLL_SUCCESS,strlen(ADD_ENROLL_SUCCESS));
            if(writeBytes == -1){
                perror("Error writing ADD_ENROLL_SUCCESS message to client!");
                return false;
            }
            readBytes = read(connFD,readBuffer,sizeof(readBuffer));
            return 0;
        }
    }
    writeBytes= write(connFD,"Invalid course ^",16);
    if(writeBytes==-1){
        perror("unable to write invalid course error message to client");
        return 0;
    }
    readBytes = read(connFD,readBuffer,sizeof(readBuffer)); //dummy read
    return 1;
}

// Viewing course details
int view_all_courses(int connFD){
    
    ssize_t readBytes, writeBytes;             
    char readBuffer[1000], writeBuffer[10000]; 
    char tempBuffer[1000];
    struct Course fetchcourse;
    int courseFileDescriptor;
    struct flock lock = {F_RDLCK, SEEK_SET, 0, sizeof(struct Course), getpid()};

    //checking whether student has access to courses or not
    if(!loggedInCustomer.status){
       write(connFD,"You are blocked by admin ^",26);
       readBytes = read(connFD,readBuffer,sizeof(readBuffer));
       return 0;
    }

    courseFileDescriptor = open(COURSE_FILE, O_RDONLY);
    if (courseFileDescriptor == -1)
    {
        // Course File doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "course file id doesn't exists");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes ==-1)
        {
            perror("Error while writing COURSE_ID_DOESNT_EXIT message to client!");
            return 0;
        }
        return 0;
    }

    int n;
    
    //displaying available courses
    while((n = read(courseFileDescriptor, &fetchcourse, sizeof(struct Course))) > 0) {
        strcpy(readBuffer,fetchcourse.status);
        
        // if the course is not present in the catalog continue to further
        if(strcmp(readBuffer,"notactive")==0)
           continue;
        bzero(writeBuffer,sizeof(writeBuffer));

        sprintf(writeBuffer, " ^ ********* Course Details *********  \n\tName: %s\n\tDepartment : %s\n\tNo of Seats: %d\n\tCredits : %d\n\tNo of available seats: %d\n\tCourse-id: %s\n", fetchcourse.name, fetchcourse.department,fetchcourse.no_of_seats,fetchcourse.credits,fetchcourse.no_of_available_seats,fetchcourse.courseid);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if(writeBytes == -1){
          perror("Error writing course info to client!");
          return 0;
        }
        readBytes = read(connFD,readBuffer,sizeof(readBuffer));
    }
    
    bzero(writeBuffer,sizeof(writeBuffer));
    strcat(writeBuffer, "\n\n You'll now be redirected to the Student menu ^");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if(writeBytes == -1){
        perror("Error writing course info to client!");
        return 0;
    }
    readBytes = read(connFD,readBuffer,sizeof(readBuffer));  //dummy read
    return 1;   
}

// logging out
int Logout(int connFD){
    ssize_t readBytes, writeBytes;             
    char readBuffer[1000], writeBuffer[10000];
    char tempBuffer[1000];
    write(connFD,LOG_OUT,strlen(LOG_OUT));
    close(connFD);
    return 0;
}

#endif
