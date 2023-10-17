# Mini Project

## Project Title: Design and Development of Course Registration Portal (Academia) - EdCourse

### Project Description

The Academia Portal project aims to create a user-friendly and multifunctional system for managing student and faculty details as well as course information. The system will include features such as a login system for account holders, with user accounts managed by an Administrator. The roles to be implemented in this system are Faculty, Student, and Admin.

### Key Functionalities

#### Admin Module

- **Add Student**: The Admin can add new student details to the system.
- **Add Faculty**: The Admin can add new faculty details.
- **Activate/Deactivate Student**: Admin can control student account status.
- **Update Student/Faculty Details**: Modify student or faculty information.
- **Exit**: Close the Admin session.

#### Student Module

- **Enroll to New Courses**: Students can enroll in available courses.
- **Unenroll from Already Enrolled Courses**: Withdraw from courses.
- **View Enrolled Courses**: See a list of courses in which they are enrolled.
- **Password Change**: Update their password.
- **Exit**: Close the Student session.

#### Faculty Module

- **Add New Course**: Faculty members can add new courses.
- **Remove Offered Course**: Remove courses they are offering.
- **View Enrollments in Courses**: See a list of students enrolled in their courses.
- **Password Change**: Update their password.
- **Exit**: Close the Faculty session.

### Security and Authentication

- The application will have a password-protected administrative login system.
- Administrators, students, and faculty will have separate login systems.
- User accounts are managed by the Administrator, ensuring authorized access.
- When users connect to the server, they are prompted for their login credentials.

### Concurrency and Data Protection

- To view course details, read locks are used.
- To enroll or unenroll from courses, write locks protect the critical data section.

### Technical Implementation

- The project will utilize socket programming.
- The server will maintain the database and serve multiple clients concurrently.
- System calls will be used instead of library functions where possible for Process Management, File Management, File Locking, Semaphores, Multithreading, and Inter Process Communication Mechanisms.

### Execution
- First compile server program, execute the command `gcc server.c -o server.out -lcrypt`
- To run the server program, execute the command `./server.out` in the background.
- Now compile client program, execute the command `gcc client.c -o client.out`
- To run the client program, use the command `./client.out`.
- Default Admin credentials:
  - Login ID: `admin`
  - Password: `vraj`
- Student Login ID Format: `S-{stu-Id.}`
- Faculty Login ID Format: `F-{fac-Id.}`
- Auto Generated Password: `mtech`

### Project Author

- Vraj Jatin Naik
- MTech CSE [MT2023150]
- International Institute of Information Technology, Bangalore.

The Academia Portal project aims to simplify course registration and academic management for students, faculty, and administrators. The user-friendly interface and secure authentication ensure that only authorized users can access the system. The use of system calls and concurrency control mechanisms makes it a robust and efficient solution for academic institutions.
