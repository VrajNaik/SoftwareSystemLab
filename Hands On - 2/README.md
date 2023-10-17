## Hands-on List 2

14. Write a simple program to create a pipe, write to the pipe, read from pipe and display on the monitor.
15. Write a simple program to send some data from parent to the child process.
16. Write a program to send and receive data from parent to child vice versa. Use two way communication.
17. Write a program to execute `ls -l | wc`
    1. Use `dup`
    2. Use `dup2`
    3. Use `fcntl`
18. Write a program to find out total number of directories on the `pwd`. Execute `ls -l | grep ^d | wc`. Use only `dup2`.
19. Create a FIFO file by 
    1. `mknod` command
    2. `mkfifo` command
    3. Use `strace` command to find out, which command (`mknod` or `mkfifo`) is better. 
    4. `mknod` system call 
    5. `mkfifo` library function
20. Write two programs so that both can communicate by FIFO - Use one way communication.
21. Write two programs so that both can communicate by FIFO - Use two way communications.
22. Write a program to wait for data to be written into FIFO within 10 seconds, use `select` system call with FIFO.
23. Write a program to print the maximum number of files can be opened within a process and size of a pipe (circular buffer). 
24. Write a program to create a message queue and print the key and message queue id.
25. Write a program to print a message queue's (use `msqid_ds` and `ipc_perm` structures)
    1. Access permission 
    2. `uid`, `gid` 
    3. Time of last message sent and received 
    4. Time of last change in the message queue
    5. Size of the queue 
    6. Number of messages in the queue
    7. Maximum number of bytes allowed
    8. PID of the `msgsnd` and `msgrcv`
26. Write a program to send messages to the message queue. Check `ipcs -q`
27. Write a program to receive messages from the message queue 
    1. With `0` as a flag 
    2. With `IPC_NOWAIT` as a flag
28. Write a program to change the exiting message queue permission. (Use `msqid_ds` structure)
29. Write a program to remove the message queue.
30. Write a program to create a shared memory. 
    1. Write some data to the shared memory
    2. Attach with `O_RDONLY` and check whether you are able to overwrite. 
    3. Detach the shared memory 
    4. Remove the shared memory
31. Write a program to create a semaphore and initialize value to the semaphore. 
    1. Create a binary semaphore 
    2. Create a counting semaphore
32. Write a program to implement semaphore to protect any critical section. 
    1. Rewrite the ticket number creation program using semaphore
    2. Protect shared memory from concurrent write access 
    3. Protect multiple pseudo resources (may be two) using counting semaphore 
    4. Remove the created semaphore
33. Write a program to communicate between two machines using socket.
34. Write a program to create a concurrent server. 
    1. Use `fork` 
    2. Use `pthread_create`

----
