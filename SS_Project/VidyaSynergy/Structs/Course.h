#define MAX_COURSE_NAME_LEN 100
#define MAX_ENROLLED_STUDENTS 200 

typedef struct {
    int course_id;
    char course_name[MAX_COURSE_NAME_LEN];
    int seats_available;
    int enrolled_students[MAX_ENROLLED_STUDENTS]; // Store student IDs
    int num_enrolled_students; // Keep track of the number of enrolled students
    int faculty_id; // ID of the faculty member teaching the course
} Course;
