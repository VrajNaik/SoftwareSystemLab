/*
============================================================================
Name : 33.c
Author : Vraj Jatin Naik
Description : Write a program to communicate between two machines using socket
Date : 6th Oct, 2023.
============================================================================
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void main() {
    int socketFileDescriptor;
    int connectStatus;
    struct sockaddr_in address;

    ssize_t readBytes, writeBytes;
    char dataFromServer[100];
    char dataFromClient[100];

    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor == -1) {
        perror("Error while creating socket!");
        _exit(0);
    }
    printf("Client side socket successfully created!\n");

    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);

    connectStatus = connect(socketFileDescriptor, (struct sockaddr *)&address, sizeof(address));
    if (connectStatus == -1) {
        perror("Error while connecting to the server!");
        _exit(0);
    }
    printf("Client to server connection successfully established!\n");

    while (1) {
        // Client - Server communication
        printf("Enter data to send to the server: ");
        char dataToSend[100];
        fgets(dataToSend, sizeof(dataToSend), stdin);
        writeBytes = write(socketFileDescriptor, dataToSend, strlen(dataToSend));
        if (writeBytes == -1)
            perror("Error while writing to the network via socket!");
        else {
            printf("Data sent to the server!\n");

            readBytes = read(socketFileDescriptor, dataFromServer, 100);
            if (readBytes == -1)
                perror("Error while reading from the network via socket!");
            else
                printf("Data from the server: %s\n", dataFromServer);
        }
    }

    close(socketFileDescriptor);
}


