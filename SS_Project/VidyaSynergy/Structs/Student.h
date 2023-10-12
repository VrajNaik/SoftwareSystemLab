#define MAX_NAME_LEN 100
#define MAX_PASSWORD_LEN 50
#define MAX_ENROLLED_COURSES 20 

struct Student{
    int student_id;
    char username[MAX_NAME_LEN];
    char password[MAX_PASSWORD_LEN];
    int enrolled_courses[MAX_ENROLLED_COURSES]; // Store course IDs
    int num_enrolled_courses; // Keep track of the number of enrolled courses
};

/*
struct Student getStudent(int ID){
        int i = ID;
        struct Student currUser;
        int fd = open("/home/vraj-naik/sslab/SS_Project/VidyaSynergy/data/Student.data", O_RDONLY, 0744);

        int fl1;
        struct flock lock;        
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(struct Student);   
        lock.l_len = sizeof(struct Student);    
        lock.l_pid = getpid();

        fl1=fcntl(fd, F_SETLKW, &lock); 

        lseek(fd, (i) * sizeof(struct Student), SEEK_SET);
        read(fd, &currUser, sizeof(struct Student));

        lock.l_type = F_UNLCK;
	printf("Press Enter to relese Lock from View Details\n");
	getchar();
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return currUser;
}

bool checkStudent(struct Student currUser){
        int i = currUser.userID;
        int fd = open("/home/vraj-naik/sslab/SS_Project/VidyaSynergy/data/Student.data", O_RDONLY, 0744);
        bool result;
        struct Student temp;

        int fl1;
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(struct Student);  
        lock.l_len = sizeof(struct Student);         
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock);

        lseek(fd, (i) * sizeof(struct Student), SEEK_SET);
        read(fd, &temp, sizeof(struct Student));
        if(!strcmp(temp.password, currUser.password) && !strcmp(temp.status, "ACTIVE"))   
                result = true;
        else                                            
                result = false;

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
}
*/
