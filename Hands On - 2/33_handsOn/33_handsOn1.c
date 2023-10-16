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
    int socketFileDescriptor, connectionFileDescriptor;
    int bindStatus;
    int listenStatus;
    int clientSize;

    struct sockaddr_in address, client;

    ssize_t readBytes, writeBytes;
    char dataFromClient[100];
    char dataFromServer[100];

    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor == -1) {
        perror("Error while creating socket!");
        _exit(0);
    }
    printf("Server side socket successfully created!\n");

    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);

    bindStatus = bind(socketFileDescriptor, (struct sockaddr *)&address, sizeof(address));
    if (bindStatus == -1) {
        perror("Error while binding name to socket!");
        _exit(0);
    }
    printf("Binding to socket was successful!\n");

    listenStatus = listen(socketFileDescriptor, 2);
    if (listenStatus == -1) {
        perror("Error while trying to listen for connections!");
        _exit(0);
    }
    printf("Now listening for connections on a socket!\n");

    clientSize = (int)sizeof(client);
    connectionFileDescriptor = accept(socketFileDescriptor, (struct sockaddr *)&client, &clientSize);
    if (connectionFileDescriptor == -1)
        perror("Error while accepting a connection!");
    else {
        while (1) {
            // Server - Client communication
            readBytes = read(connectionFileDescriptor, dataFromClient, 100);
            if (readBytes == -1)
                perror("Error while reading from network via socket!");
            else {
                printf("Data from client: %s\n", dataFromClient);
                printf("Enter data to send to the client: ");
                char dataToSend[100];
                fgets(dataToSend, sizeof(dataToSend), stdin);
                writeBytes = write(connectionFileDescriptor, dataToSend, strlen(dataToSend));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Data sent to client!\n");
            }
        }
    }

    close(connectionFileDescriptor);
    close(socketFileDescriptor);
}



