# CS 513 Software Systems Mini Project 

## Title: Design and Development of Course Registration Portal. EdConnect


### Description

The project aims to develop a Academia Portal that is user-friendly and multifunctional.

1. All Student and Faculty details and Course information are stored in files..
2. Account holders have to pass through a login system to enter their accounts, and all these User accounts will be managed by the Administrator.
3. Roles to implement: Faculty, Student, Admin.
4. The application should possess password-protected administrative access, thus preventing the whole management system from unauthorized access
5. Once the Admin connects to the server, He/She should get a login and password prompt.
6. After successful login, you should get menu for example:
   
    Do you want to:

    - Add Student
    - Add Faculty
    - Activate/Deactivate Student 
    - Update Student/Faculty details
    - Exit
      
7. Once the Student connects to the server, He/She should get a login and password prompt.
8. After successful login, He/She should get menu for example:

    Do you want to:

     - Enroll to new Courses
     - Unenroll from already enrolled Courses
     - View enrolled Courses
     - Password Change
     - Exit
      
9. Once the Faculty connects to the server, He/She should get a login and password prompt.
10. After successful login, He/She should get menu for example:

   Do you want to:
   
   - Add new Course
   - Remove offered Course
   - View enrollments in Courses
   - Password Change
   - Exit
     
11. If you want to view the Course details then read lock is to be set else if you want to Enroll or Unenroll then write lock should be used to protect the critical data section.
12. Use socket programming – Server maintains the database and serves multiple clients concurrently. Clients can connect to the server and access their specific academic details.
13. Use System calls instead of Library functions wherever it is possible in the project: Process Management, File Management, File Locking, Semaphores, Multithreading and Inter Process   Communication Mechanisms.

***
<br> 
<i>Report by, </i> <br/>
Vraj Naik <br/>
MTech CSE [MT2023050] <br/>
International Institute of Information Technology, Bangalore. <br/>
