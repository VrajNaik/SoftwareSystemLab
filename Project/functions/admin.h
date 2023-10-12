#ifndef ADMIN_FUNCTIONS
#define ADMIN_FUNCTIONSs =================================
#include "./common.h"

bool admin_operation_handler(int connFD);
bool add_student(int cvoidD);
bool add_faculty(int cvoidD);
//bool delete_account(int connFD);
//bool modify_customer_info(int connFD);

// =====================================================

// Function Definition =================================

// =====================================================

bool admin_operation_handler(int connFD)
{

    if (login_handler(0, connFD, NULL, NULL))
    {
        ssize_t writeBytes, readBytes;            // Number of bytes read from / written to the client
        char readBuffer[1000], writeBuffer[1000]; // A buffer used for reading & writing to the client
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ADMIN_LOGIN_SUCCESS);
        while (1)
        {
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, ADMIN_MENU);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing ADMIN_MENU to client!");
                return false;
            }
            bzero(writeBuffer, sizeof(writeBuffer));

            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error while reading client's choice for ADMIN_MENU");
                return false;
            }

            int choice = atoi(readBuffer);
            switch (choice)
            {
            case 1:
                add_student(connFD);
                //get_customer_details(connFD, -1);
                break;
            case 2:
                add_faculty(connFD);
                //get_account_details(connFD, NULL);
                break;
            case 3: 
                //get_transaction_details(connFD, -1);
                break;
            case 4:
                add_student(connFD);
                break;
            case 5:
                //delete_account(connFD);
                break;
            case 6:
                //modify_customer_info(connFD);
                break;
            default:
                writeBytes = write(connFD, ADMIN_LOGOUT, strlen(ADMIN_LOGOUT));
                return false;
            }
        }
    }
    else
    {
        // ADMIN LOGIN FAILED
        return false;
    }
    return true;
}

bool add_student(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Student newCustomer, previousCustomer;

    int customerFileDescriptor = open(STUDENT_FILE, O_RDONLY);
    if (customerFileDescriptor == -1 && errno == ENOENT)
    {
        // Customer file was never created
        newCustomer.id = 0;
    }
    else if (customerFileDescriptor == -1)
    {
        perror("Error while opening STUDENT file");
        return -1;
    }
    else
    {
        int offset = lseek(customerFileDescriptor, -sizeof(struct Student), SEEK_END);
        if (offset == -1)
        {
            perror("Error seeking to last Customer record!");
            return false;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Student), getpid()};
        int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on Student record!");
            return false;
        }

        readBytes = read(customerFileDescriptor, &previousCustomer, sizeof(struct Student));
        if (readBytes == -1)
        {
            perror("Error while reading Student record from file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(customerFileDescriptor, F_SETLK, &lock);

        close(customerFileDescriptor);

        newCustomer.id = previousCustomer.id + 1;
    }
    
    sprintf(writeBuffer, "%s%s", ADMIN_ADD_CUSTOMER_PRIMARY, ADMIN_ADD_CUSTOMER_NAME);

    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing ADMIN_ADD_CUSTOMER_NAME message to client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer name response from client!");
        ;
        return false;
    }

    strcpy(newCustomer.name, readBuffer);

    writeBytes = write(connFD, ADMIN_ADD_CUSTOMER_GENDER, strlen(ADMIN_ADD_CUSTOMER_GENDER));
    if (writeBytes == -1)
    {
        perror("Error writing ADMIN_ADD_CUSTOMER_GENDER message to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer gender response from client!");
        return false;
    }

    if (readBuffer[0] == 'M' || readBuffer[0] == 'F' || readBuffer[0] == 'O')
        newCustomer.gender = readBuffer[0];
    else
    {
        writeBytes = write(connFD, ADMIN_ADD_CUSTOMER_WRONG_GENDER, strlen(ADMIN_ADD_CUSTOMER_WRONG_GENDER));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, ADMIN_ADD_CUSTOMER_AGE);
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing ADMIN_ADD_CUSTOMER_AGE message to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer age response from client!");
        return false;
    }

    int customerAge = atoi(readBuffer);
    if (customerAge == 0)
    {
        // Either client has sent age as 0 (which is invalid) or has entered a non-numeric string
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ERRON_INPUT_FOR_NUMBER);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    newCustomer.age = customerAge;


    strcpy(newCustomer.login, newCustomer.name);
    strcat(newCustomer.login, "-");
    sprintf(writeBuffer, "%d", newCustomer.id);
    strcat(newCustomer.login, writeBuffer);

    char hashedPassword[1000];
    strcpy(hashedPassword, crypt(AUTOGEN_PASSWORD, SALT_BAE));
    strcpy(newCustomer.password, hashedPassword);

    customerFileDescriptor = open(STUDENT_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (customerFileDescriptor == -1)
    {
        perror("Error while creating / opening student file!");
        return false;
    }
    writeBytes = write(customerFileDescriptor, &newCustomer, sizeof(newCustomer));
    if (writeBytes == -1)
    {
        perror("Error while writing Customer record to file!");
        return false;
    }

    close(customerFileDescriptor);

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "%s%s-%d\n%s%s", ADMIN_ADD_CUSTOMER_AUTOGEN_LOGIN, newCustomer.name, newCustomer.id, ADMIN_ADD_CUSTOMER_AUTOGEN_PASSWORD, AUTOGEN_PASSWORD);
    strcat(writeBuffer, "^");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error sending customer loginID and password to the client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

}

bool add_faculty(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Faculty newCustomer, previousCustomer;

    int customerFileDescriptor = open(FACULTY_FILE, O_RDONLY);
    if (customerFileDescriptor == -1 && errno == ENOENT)
    {
        // Customer file was never created
        newCustomer.id = 0;
    }
    else if (customerFileDescriptor == -1)
    {
        perror("Error while opening FACULTY file");
        return -1;
    }
    else
    {
        int offset = lseek(customerFileDescriptor, -sizeof(struct Faculty), SEEK_END);
        if (offset == -1)
        {
            perror("Error seeking to last Customer record!");
            return false;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Faculty), getpid()};
        int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on Faculty record!");
            return false;
        }

        readBytes = read(customerFileDescriptor, &previousCustomer, sizeof(struct Faculty));
        if (readBytes == -1)
        {
            perror("Error while reading Faculty record from file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(customerFileDescriptor, F_SETLK, &lock);

        close(customerFileDescriptor);

        newCustomer.id = previousCustomer.id + 1;
    }
    
    sprintf(writeBuffer, "%s%s", ADMIN_ADD_FACULTY, ADMIN_ADD_FACULTY_NAME);

    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing ADMIN_ADD_FACULTY_NAME message to client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading faculty name response from client!");
        
        return false;
    }

    strcpy(newCustomer.name, readBuffer);

    writeBytes = write(connFD, ADMIN_ADD_FACULTY_GENDER, strlen(ADMIN_ADD_FACULTY_GENDER));
    if (writeBytes == -1)
    {
        perror("Error writing ADMIN_ADD_FACULTY_GENDER message to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading faculty gender response from client!");
        return false;
    }

    if (readBuffer[0] == 'M' || readBuffer[0] == 'F' || readBuffer[0] == 'O')
        newCustomer.gender = readBuffer[0];
    else
    {
        writeBytes = write(connFD, ADMIN_ADD_FACULTY_WRONG_GENDER, strlen(ADMIN_ADD_FACULTY_WRONG_GENDER));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    bzero(writeBuffer, sizeof(writeBuffer));
    
    strcpy(writeBuffer, ADMIN_ADD_FACULTY_DEPARTMENT);
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing ADMIN_ADD_FACULTY_DEPARTMENT message to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading faculty department response from client!");
        return false;
    }
    strcpy(newCustomer.department, readBuffer);


    strcpy(newCustomer.login, newCustomer.name);
    strcat(newCustomer.login, "-");
    sprintf(writeBuffer, "%d", newCustomer.id);
    strcat(newCustomer.login, writeBuffer);

    char hashedPassword[1000];
    strcpy(hashedPassword, crypt(AUTOGEN_PASSWORD, SALT_BAE));
    strcpy(newCustomer.password, hashedPassword);

    customerFileDescriptor = open(FACULTY_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (customerFileDescriptor == -1)
    {
        perror("Error while creating / opening faculty file!");
        return false;
    }
    writeBytes = write(customerFileDescriptor, &newCustomer, sizeof(newCustomer));
    if (writeBytes == -1)
    {
        perror("Error while writing Faculty record to file!");
        return false;
    }

    close(customerFileDescriptor);

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "%s%s-%d\n%s%s", ADMIN_ADD_FACULTY_AUTOGEN_LOGIN, newCustomer.name, newCustomer.id, ADMIN_ADD_FACULTY_AUTOGEN_PASSWORD, AUTOGEN_PASSWORD);
    strcat(writeBuffer, "^");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error sending faculty loginID and password to the client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

}
#endif
