## Table of contents

- [I. Description](#i-description)
- [II. Compile and run](#ii-compile-and-run)
  - [1. Server](#1-server)
  - [2. Client](#2-client)
  - [3. Custom client](#3-custom-client)
- [III. Code \& Idea Description](#iii-code--idea-description)
    - [**Uitls.hpp**](#uitlshpp)
    - [**Order.hpp**](#orderhpp)
    - [**HashTable.hpp**](#hashtablehpp)
    - [**client.cpp**](#clientcpp)
    - [**clientCustom.cpp**](#clientcustomcpp)
    - [**server.cpp**](#servercpp)
- [V. Note](#v-note)
---
# I. Description
- A simple matching engine, accepting new sell/buy orders and match them with a good performance.
- Support up to 2 million order.
- Not support limit order, only match exactly price.

# II. Compile and run
## 1. Server
Compile with
```
  g++ -o server server.cpp
```
To run: `portNumber` is any port number available on your machine, `isLog` is 0 or 1, to config the server to logging the process of matching
```
  ./server portNumber isLog
```

## 2. Client
Compile with
```
  g++ -o client client.cpp
```
To run: 
  - `hostname` is the hostname of the server, I test on local machine so hostname = localhost.
  - `portNumber` is any port number available on your machine.
  - `buyNumber` is the number of buy orders that client will generate and send to the server. It could be up to 1000000.
  - `sellNumber` is the number of sell orders that client will generate and send to the server. It could be up to 1000000.
```
  ./client localhost portNumber buyNumber sellNumber
```

## 3. Custom client
Compile with
```
  g++ -o clientCustom clientCustom.cpp
```
To run: 
  - `hostname` is the hostname of the server, I test on local machine so hostname = localhost.
  - `portNumber` is any port number available on your machine.
  - `orderType` is 1 or 0 respect to buy or sell.
  - `orderPrice` the price value of the order.
  - `orderQuantity` is the quantity of the order.
```
  ./clientCustom hostname portNumber orderType orderPrice orderQuantity
```

# III. Code & Idea Description
### **Uitls.hpp**
- Include some support functions.
### **Order.hpp**
- Define Order struct, each order object is also a node in linked-list itself.
- Include the function to convert an Order to string.
- Include the function to generate sell/buy orders.
### **HashTable.hpp**
- Define the hash table to store the sell order book and buy order book.
- Hash function base on the price value of order.
- Support methods: create a new hash table, insert, delete the head of a bucket, get the current number of items in the hash table.
### **client.cpp**
- Generate sell/buy orders to send to the server, making the database for the server.
- Generating time for 2 million orders is around 17s on MacBook Pro 2019, 16GB, Core i7 2.6GHz x 6.
- Sending time for 2 million orders is around 77ms.
```
  ./client localhost 1304 1000000 1000000

  Generated 2000000 orders.
  Time taken by generating 30569040 (bytes) of the buffer: 17246 (ms)
  Connected to server.
  Start to send orders.
  Time taken by sending 30569040 (bytes) of the buffer: 77 (ms)
```
### **clientCustom.cpp**
- Sending an order to the server with order type, price, and quantity are defined by user.
- OrderID and matching result are generated and returned by the server.
```
  ./clientCustom localhost 1304 1 100 1

  Connected to server.
  Buy OrderId 2000002 - Price 100 - Quantity 1:
          Sell OrderId 2000001 - Price 100 - Quantity 1
          totally matched.
```
### **server.cpp**
- Matching algorithm bases on FIFO.
- Maintaining 2 hash tables: sellBook and buyBook to store sell orders and buy orders respectively.
- Incoming buy orders will be matched with sell orders in sellBook, it will traverse the linked-list in the hash bucket to update the quantity.
- When the quantity of an order reach 0, it will be deleted from the order book and response as totally matched.
- Listenning from 2 kinds of client.
  - client
    - server will read all the bytes send from client, parsing them to Order objects and save into sell order book, buy order book.
    - processing time 2 million order is around 486ms.
    - during the process, the totally matched or partially matched items would be also deleted from the order books.
    - the server can also print the log by set the `isLog=1` when running the server.
    ```
      Server: got connection from 127.0.0.1 port 49818
      Reading 30569040 (bytes) from the client and matching...
      Read finished: 30569040 (bytes).
      Matching finished
      Time taken by receiving, parsing orders and matching them: 486 (ms).
    ```
  - clientCustom
    - when the server receive a new order from this client custom, it will try to find a matched order for it and then return the result.
    - the result can be totally matched, partially matched or not matched.
    ```
      Server: got connection from 127.0.0.1 port 49746
      OrderId 2000002 - Price 100 - Quantity 1:
              OrderId 2000001 - Price 100 - Quantity 1
              totally matched.
    ```
# V. Note
- The correctness: manually tested with clientCustom.
- Reference: 
  - [Socket Programming](https://www.bogotobogo.com/cplusplus/sockets_server_client.php)
- This repository is implemented from scratch by Nguyen Ba Vinh Quang.

