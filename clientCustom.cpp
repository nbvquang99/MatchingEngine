#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <math.h>
#include <chrono>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int socketFd, portNo;
    struct sockaddr_in serverAddress;
    struct hostent *server;

    if (argc < 6) {
        error("ERROR not enough arguments: hostname port orderType price quantity");
    }

    // sending throught socket
    portNo = atoi(argv[2]);
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0) {
        error("ERROR opening socket");
    }

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        error("ERROR no such host\n");
    }

    // setup the serverAddress structure
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(portNo);

    if (connect(socketFd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        error("ERROR connecting");
    }
    
    printf("Connected to server.\n");
    
    // send the FLAG first
    if (write(socketFd, "CUST_ORD", 8) < 0) {
        error("ERROR writing total length to socket");
    }

    // send the buffer
    int check;
    std::string order="";
    order+=argv[3];
    order+=" ";
    order+=argv[4];
    order+=" ";
    order+=argv[5];
    while (order.size() < 10) order+=" ";
    if (write(socketFd, order.c_str(), 10) < 0) {
        error("ERROR writing total length to socket");
    }

    char responseLen[8];
    read(socketFd, responseLen, 8);
    int readLen = atoi(responseLen);
    char response[1001];
    read(socketFd, response, readLen);
    std::cout << response;

    close(socketFd);
    return 0;
}