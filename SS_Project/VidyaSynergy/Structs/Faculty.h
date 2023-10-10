#define MAX_NAME_LEN 100
#define MAX_TAUGHT_COURSES 10 
#define MAX_PASSWORD_LEN 50

typedef struct {
    int faculty_id;
    char name[MAX_NAME_LEN];
    char password[MAX_PASSWORD_LEN];
    int taught_courses[MAX_TAUGHT_COURSES]; // Store course IDs
    int num_taught_courses; // Keep track of the number of taught courses
} Faculty;
