#ifndef FACULTY_FUNCTIONS
#define FACULTY_FUNCTIONS

// Semaphores are necessary joint account due the design choice I've made
#include <sys/ipc.h>
#include <sys/sem.h>
#include <ctype.h>
#include <time.h>
#include "server-constants.h"
#include "../record-struct/Course.h"


struct Faculty loggedInCustomer2;
int semIdentifier;

// Function Prototypes =================================

bool faculty_operation_handler(int connFD);
bool change_password1(int connFD);
bool lock_critical_section1(struct sembuf *semOp);
bool unlock_critical_section1(struct sembuf *sem_op);
int add_course(int connFD);
// =====================================================

// Function Definition =================================

bool faculty_operation_handler(int connFD)
{
    if (login_handler(2, connFD, NULL, &loggedInCustomer2))
    {
        ssize_t writeBytes, readBytes;            // Number of bytes read from / written to the client
        char readBuffer[1000], writeBuffer[1000]; // A buffer used for reading & writing to the client

        // Get a semaphore for the user
        key_t semKey = ftok(FACULTY_FILE, loggedInCustomer2.id); // Generate a key based on the account number hence, different customers will have different semaphores

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
        strcpy(writeBuffer, FACULTY_LOGIN_SUCCESS);
        while (1)
        {
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, FACULTY_MENU);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing FACULTY_MENU to client!");
                return false;
            }
            bzero(writeBuffer, sizeof(writeBuffer));

            bzero(readBuffer, sizeof(readBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error while reading client's choice for FACULTY_MENU");
                return false;
            }
          
            int choice = atoi(readBuffer);
            switch (choice)
            {
            case 1:
                get_faculty_details(connFD, loggedInCustomer2.id);
                break;
            case 2:
                add_course(connFD);
                break;
            case 3:
                //withdraw(connFD);
                break;
            case 4:
                change_password1(connFD);
                //get_balance(connFD);
                break;
            default:
                writeBytes = write(connFD, FACULTY_LOGOUT, strlen(FACULTY_LOGOUT));
                return false;
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

bool change_password1(int connFD)
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
        unlock_critical_section1(&semOp);
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading old password response from client");
        unlock_critical_section1(&semOp);
        return false;
    }

    if (strcmp(crypt(readBuffer, SALT_BAE), loggedInCustomer2.password) == 0)
    {
        // Password matches with old password
        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS, strlen(PASSWORD_CHANGE_NEW_PASS));
        if (writeBytes == -1)
        {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS message to client!");
            unlock_critical_section1(&semOp);
            return false;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading new password response from client");
            unlock_critical_section1(&semOp);
            return false;
        }

        strcpy(newPassword, crypt(readBuffer, SALT_BAE));

        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_RE, strlen(PASSWORD_CHANGE_NEW_PASS_RE));
        if (writeBytes == -1)
        {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS_RE message to client!");
            unlock_critical_section1(&semOp);
            return false;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading new password reenter response from client");
            unlock_critical_section1(&semOp);
            return false;
        }

        if (strcmp(crypt(readBuffer, SALT_BAE), newPassword) == 0)
        {
            // New & reentered passwords match

            strcpy(loggedInCustomer2.password, newPassword);

            int facultyFileDescriptor = open(FACULTY_FILE, O_WRONLY);
            if (facultyFileDescriptor == -1)
            {
                perror("Error opening faculty file!");
                unlock_critical_section1(&semOp);
                return false;
            }

            off_t offset = lseek(facultyFileDescriptor, loggedInCustomer2.id * sizeof(struct Faculty), SEEK_SET);
            if (offset == -1)
            {
                perror("Error seeking to the faculty record!");
                unlock_critical_section1(&semOp);
                return false;
            }

            struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Faculty), getpid()};
            int lockingStatus = fcntl(facultyFileDescriptor, F_SETLKW, &lock);
            if (lockingStatus == -1)
            {
                perror("Error obtaining write lock on faculty record!");
                unlock_critical_section1(&semOp);
                return false;
            }

            writeBytes = write(facultyFileDescriptor, &loggedInCustomer2, sizeof(struct Faculty));
            if (writeBytes == -1)
            {
                perror("Error storing updated faculty password into faculty record!");
                unlock_critical_section1(&semOp);
                return false;
            }

            lock.l_type = F_UNLCK;
            lockingStatus = fcntl(facultyFileDescriptor, F_SETLK, &lock);

            close(facultyFileDescriptor);

            writeBytes = write(connFD, PASSWORD_CHANGE_SUCCESS, strlen(PASSWORD_CHANGE_SUCCESS));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

            unlock_critical_section1(&semOp);

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

    unlock_critical_section1(&semOp);

    return false;
}

int add_course(int connFD){
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];
    struct Course newCourse;
    struct Course prevCourse;


    int courseFileDescriptor = open(COURSE_FILE, O_RDONLY);
    if (courseFileDescriptor == -1 && errno == ENOENT)
    {
        // Course file was never created
        newCourse.id =1;
    }
    else if (courseFileDescriptor == -1)
    {
        perror("Error while opening course file");
        return 0;
    }
    else
    {
        // getting the id of last record
        int offset = lseek(courseFileDescriptor, -sizeof(struct Course), SEEK_END);
        if (offset == -1)
        {
            perror("Error seeking to last Course record!");
            return 0;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Course), getpid()};
        int lockingStatus = fcntl(courseFileDescriptor, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on Course record!");
            return 0;
        }

        readBytes = read(courseFileDescriptor, &prevCourse, sizeof(struct Course));
        if (readBytes == -1)
        {
            perror("Error while reading Course record from file!");
            return 0;
        }

        lock.l_type = F_UNLCK;
        fcntl(courseFileDescriptor, F_SETLK, &lock);

        close(courseFileDescriptor);

        newCourse.id = prevCourse.id+1;
    }

    // course name 
    writeBytes = write(connFD, ADD_COURSE_NAME,strlen(ADD_COURSE_NAME));
    if (writeBytes == -1)
    {
        perror("Error writing ADD_COURSE_NAME message to client!");
        return 0;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading course name from client!");
        return 0;
    }

    //validation for course name
    for(int i = 0;readBuffer[i]!='\0';i++) {
        if(!isalpha(readBuffer[i]) && !isspace(readBuffer[i])) {
            write(connFD,"Invalid  course name ^",21);
            readBytes= read(connFD,readBuffer,sizeof(readBuffer));
            return 0;
        }
    }
    strcpy(newCourse.name,readBuffer);


    // course department 
    writeBytes = write(connFD, ADD_COURSE_DEPARTMENT,strlen(ADD_COURSE_DEPARTMENT));
    if (writeBytes == -1)
    {
        perror("Error writing ADD_COURSE_DEPARTMENT message to client!");
        return 0;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading course department  from client!");
        return 0;
    }

    //validation for course department
    for(int i = 0;readBuffer[i]!='\0';i++) {
        if(!isalpha(readBuffer[i]) && !isspace(readBuffer[i])) {
            write(connFD,"Invalid dept name ^",19);
            readBytes= read(connFD,readBuffer,sizeof(readBuffer));
            return 0;
        }
    }
    strcpy(newCourse.department,readBuffer);

    // no of seats
    writeBytes = write(connFD, ADD_COURSE_SEATS,strlen(ADD_COURSE_SEATS));
    if (writeBytes == -1)
    {
        perror("Error writing ADD_COURSE_SEATS message to client!");
        return 0;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading no of seats from client!");
        return 0;
    }
    int seats = atoi(readBuffer);

    // validation for no of seats
    if(seats<=0){
        write(connFD,"Invalid no of seats ^",21);
        readBytes = read(connFD,readBuffer,sizeof(readBuffer));
        return 0;
    }
    newCourse.no_of_seats= seats;


    // credits
    writeBytes = write(connFD, ADD_COURSE_CREDITS,strlen(ADD_COURSE_CREDITS));
    if (writeBytes == -1)
    {
        perror("Error writing ADD_COURSE_CREDITS message to client!");
        return 0;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading credits from client!");
        return 0;
    }
    int credits = atoi(readBuffer);

    // validation for credits
    if(credits<=0){
        write(connFD,"Invalid credits ^",17);
        readBytes = read(connFD,readBuffer,sizeof(readBuffer));
        return 0;
    }
    newCourse.credits= credits;
     
    
    // no of available seats
    newCourse.no_of_available_seats=seats;

    //coursecode
    char y[4];
    strcpy(newCourse.courseid,"C-");
    sprintf(y ,"%d",newCourse.id);
    strcat(newCourse.courseid, y);

    //course faculty name
    strcpy(newCourse.facultyloginid,loggedInCustomer2.login);

    //course status
    strcpy(newCourse.status,"active");
    
    // creating course record in file
    courseFileDescriptor = open(COURSE_FILE, O_CREAT|O_APPEND|O_WRONLY,S_IRWXU);
    if (courseFileDescriptor == -1)
    {
        perror("Error while creating / opening course file!");
        return 0;
    }

    writeBytes = write(courseFileDescriptor, &newCourse, sizeof(struct Course));
    if (writeBytes == -1)
    {
        perror("Error while writing Course record to file!");
        return 0;
    }

    close(courseFileDescriptor);
    bzero(writeBuffer, sizeof(writeBuffer));
    
    //success message
    sprintf(writeBuffer, "%s%s%d", ADD_COURSE_SUCCESS, newCourse.courseid,newCourse.id);
    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    
    readBytes = read(connFD,readBuffer,sizeof(readBuffer)); //dummy read
    return 1;
}

bool lock_critical_section1(struct sembuf *semOp)
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

bool unlock_critical_section1(struct sembuf *semOp)
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
#endif
