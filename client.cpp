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
#include "Order.hpp"
#include <chrono>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

std::string generateBuffer(int nBuy, int nSell) {
    // time measurement
    auto start = std::chrono::high_resolution_clock::now();

    Order** buyArr = genBuyOrder(nBuy);
    Order** sellArr = genSellOrderBaseOnBuyOrder(nSell, nBuy);
    int indexBuy = 0;
    int indexSell = 0;
    std::string buffer = "";

    for (int i=1; i <= nBuy + nSell; i++) {
        if (indexSell == nSell || getRandomLR(0, 1)==1 && indexBuy < nBuy) {
            buffer += orderToString(buyArr[indexBuy++]);
        } else {
            buffer += orderToString(sellArr[indexSell++]);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    printf("Generated %d orders.\n", nBuy+nSell);
    printf("Time taken by generating %lu (bytes) of the buffer: %lld (ms)\n", buffer.size()+1, duration.count());
    return buffer;
}

int main(int argc, char *argv[])
{
    int socketFd, portNo;
    struct sockaddr_in serverAddress;
    struct hostent *server;

    if (argc < 5) {
        error("ERROR not enough arguments: hostname port numberOfBuyOrder numberOfSellOrder");
    }

    // generating orders
    int nBuy = atoi(argv[3]);
    int nSell = atoi(argv[4]);
    std::string orderBuffer = generateBuffer(nBuy, nSell);
    orderBuffer+="\n";

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
    printf("Start to send orders.\n");
    auto start = std::chrono::high_resolution_clock::now();
    
    char* charArr = new char[orderBuffer.size()+1];
    strcpy(charArr, orderBuffer.c_str());

    // send the total length first
    std::string totalLength = std::to_string(orderBuffer.size());
    while (totalLength.size() < 8) totalLength+=" ";
    if (write(socketFd, totalLength.c_str(), 8) < 0) {
        error("ERROR writing total length to socket");
    }

    // send the buffer, 1000 bytes each
    int check;
    for (int i=0; i<orderBuffer.size(); i+=1000) {
        if (orderBuffer.size() -  i >= 1000) {
            check = write(socketFd, charArr+i, 1000);
        } else {
            check = write(socketFd, charArr+i, orderBuffer.size() -  i);
        }
        if (check < 0) {
            error("ERROR writing to socket");
        }
    }
    
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    printf("Time taken by sending %lu (bytes) of the buffer: %lld (ms)\n", orderBuffer.size(), duration.count());
    close(socketFd);
    return 0;
}