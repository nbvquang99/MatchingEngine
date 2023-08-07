#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <algorithm>
#include <math.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <chrono>
#include "Order.hpp"
#include "HashTable.hpp"

#define MAX_BUFFER 1009

using namespace std;

HashTable* buyBook = createTable(HASH_SIZE);
HashTable* sellBook = createTable(HASH_SIZE);

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

// try to find a matching order in hashTable1 for newOrder
// if newOrder is not totally resolve then push it in hashTable2
bool matching(Order *newOrder, HashTable* hashTable1, HashTable* hashTable2, bool loggingToken, bool ssToken, std::stringstream& ss) {
    unsigned long key = hashFunction(hashTable1, newOrder->price);
    Order* it = hashTable1->head[key];
    if (loggingToken) std::cout << (newOrder->isBuy ? "Buy " : "Sell ") << "OrderId " << newOrder->orderId << " - Price " << newOrder->price << " - Quantity " << newOrder->quantity << ":\n";
    if (ssToken) ss << (newOrder->isBuy ? "Buy " : "Sell ") << "OrderId " << newOrder->orderId << " - Price " << newOrder->price << " - Quantity " << newOrder->quantity << ":\n";
    
    while (it != nullptr) {      
        if (it->price == newOrder->price) {
            int cross = newOrder->quantity - it->quantity;
            if (loggingToken) std::cout << "\t" << (it->isBuy ? "Buy " : "Sell ")  << "OrderId " << it->orderId << " - Price " << it->price << " - Quantity " << it->quantity << "\n";
            if (ssToken) ss << "\t" << (it->isBuy ? "Buy " : "Sell ") << "OrderId " << it->orderId << " - Price " << it->price << " - Quantity " << it->quantity << "\n";
            if (cross < 0) {
                it->quantity -= newOrder->quantity;
                newOrder->quantity = 0;
                if (loggingToken) std::cout << "\t" << "totally matched." << std::endl;
                if (ssToken) ss << "\t" << "totally matched." << std::endl;
                return true;
            } else if (cross > 0) {
                newOrder->quantity = cross;
                it = removeHead(hashTable1, newOrder);
            } else {
                newOrder->quantity = 0;
                it = removeHead(hashTable1, newOrder);
                if (loggingToken) std::cout << "\t" << "totally matched." << std::endl;
                if (ssToken) ss << "\t" << "totally matched." << std::endl;
                return true;
            }
        } else {
            it = it->nextOrder;
        }
    }
    insertHashTable(hashTable2, newOrder);
    if (loggingToken) std::cout << "\t" << "partial (or not yet) matched." << std::endl;
    if (ssToken) ss << "\t" << "partial (or not yet) matched." << std::endl;
    return false;
}

void processMatching(int sock, int totalLength, bool loggingToken,  std::stringstream& ss) {
    char* buffer = new char[totalLength];
    int readSize = 1000;
    int byteRead = 0;
    
    while (byteRead < totalLength) {
        readSize = min(readSize, totalLength-byteRead);
        byteRead += read(sock, buffer+byteRead, readSize);
    }
    printf("Read finished: %d (bytes).\n", byteRead);

    // matching
    int count=0, number=0;
    int numArr[4];
    // std::cout << buffer;
    // std::cout << sscanf(buffer, "%d%d%d%d", &orderId, &orderType, &price, &quantity) << std::endl;
    for (int i=0; i<byteRead; i++) {
        if (buffer[i] > 47 && buffer[i] < 58) {
            number = number *10 + buffer[i] - 48;
        } else {
            numArr[count] = number;
            number=0;
            count++;
            if (count == 4) {
                count=0;
                Order* order = new Order();
                order->orderId = numArr[0];
                order->isBuy = numArr[1];
                order->price = numArr[2];
                order->quantity = numArr[3];
                order->nextOrder = nullptr;
                order->prevOrder = nullptr;
                if (order->isBuy) {
                    matching(order, sellBook, buyBook, loggingToken, false, ss);
                } else {
                    matching(order, buyBook, sellBook, loggingToken, false, ss);
                }
            }
        }
    }
    printf("Matching finished\n");
}

int main(int argc, char *argv[])
{
    int welcomeSockFd, servingSockFd, portNo;
    bool loggingToken;
    socklen_t clientLen;
    char totalLength[8];
    struct sockaddr_in serverAddress, clientAddress;

    if (argc < 3) {
        error("ERROR not enough arguments\n");
    }
    // create the welcoming socket
    welcomeSockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (welcomeSockFd < 0){
        error("ERROR opening socket");
    }
        
    // init address structure
    bzero((char *) &serverAddress, sizeof(serverAddress));

    // get the port number from argument
    portNo = atoi(argv[1]);

    // get the port number from argument
    loggingToken = atoi(argv[2]);

    /* setup the serverAddress structure*/
    serverAddress.sin_family = AF_INET;  
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    // host-to-network short
    serverAddress.sin_port = htons(portNo);

    // bind the socket to portNo
    bind(welcomeSockFd, (struct sockaddr *) &serverAddress, sizeof(serverAddress));

    // listening to all incoming connection and store into a backlog queue
    listen(welcomeSockFd, 5);
    printf("Listening on port %d.\n", portNo);

    // The accept() call actually accepts an incoming connection
    clientLen = sizeof(clientAddress);
    int startingId = 2000000;
    std::stringstream ss;
    while (true) {
        servingSockFd = accept(welcomeSockFd, (struct sockaddr *) &clientAddress, &clientLen);
        if (servingSockFd < 0) {
            error("ERROR on accept");
        } else {
            auto start = std::chrono::high_resolution_clock::now();
            
            printf("Server: got connection from %s port %d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));

            // read the totalLength first
            read(servingSockFd, totalLength, 8);

            // process customClient
            if (totalLength[0] == 'B') {
                char buffer[11];
                int ordType, price, quantity;
                int ordersCount = 0;
                while (true) {
                    if (read(servingSockFd, buffer, 10) <= 0) break;
                    ordersCount++;
                    sscanf(buffer, "%d%d%d", &ordType, &price, &quantity);
                    Order* order = new Order();
                    order->orderId = ++startingId;
                    order->isBuy = ordType;
                    order->price = price;
                    order->quantity = quantity;
                    order->nextOrder = nullptr;
                    order->prevOrder = nullptr;
                    ss.str(std::string());
                    if (order->isBuy) {
                        matching(order, sellBook, buyBook, loggingToken, true, ss);
                    } else {
                        matching(order, buyBook, sellBook, loggingToken, true, ss);
                    }
                    // string response = ss.str();
                    // string responseLen = to_string(response.size());
                    // while (responseLen.size() < 8) responseLen+=" ";
                    // write(servingSockFd, responseLen.c_str(), 8);
                    // write(servingSockFd, response.c_str(), response.size());
                }
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
                printf("Time taken by receiving, parsing %d orders and matching them one by one: %lld (ms).\n", ordersCount, duration.count());
                printf("-----------------------\n");
            } else if (totalLength[0] == 'C') {
                char buffer[11];
                read(servingSockFd, buffer, 10);
                int ordType, price, quantity;
                sscanf(buffer, "%d%d%d", &ordType, &price, &quantity);
                Order* order = new Order();
                order->orderId = ++startingId;
                order->isBuy = ordType;
                order->price = price;
                order->quantity = quantity;
                order->nextOrder = nullptr;
                order->prevOrder = nullptr;
                ss.str(std::string());
                if (order->isBuy) {
                    matching(order, sellBook, buyBook, true, true, ss);
                } else {
                    matching(order, buyBook, sellBook, true, true, ss);
                }
                string response = ss.str();
                string responseLen = to_string(response.size());
                while (responseLen.size() < 8) responseLen+=" ";
                write(servingSockFd, responseLen.c_str(), 8);
                write(servingSockFd, response.c_str(), response.size());
            } else { // process client
                int bufferLength = stoi(totalLength);
                printf("Reading %d (bytes) from the client and matching...\n", bufferLength);

                // read the orders
                processMatching(servingSockFd, bufferLength, loggingToken, ss);
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
                printf("Time taken by receiving, parsing orders and matching them: %lld (ms).\n", duration.count());
                printf("-----------------------\n");
            }
            close(servingSockFd);
        }
    }

    close(welcomeSockFd);
    return 0; 
}