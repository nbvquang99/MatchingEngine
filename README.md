## Table of contents

- [I. Description](#i-description)
- [II. Compile and run](#ii-compile-and-run)
  - [1. Server](#1-server)
  - [2. Client](#2-client)
  - [3. Custom client](#3-custom-client)
  - [4. Custom client 2](#4-custom-client-2)
- [III. Code \& Idea Description](#iii-code--idea-description)
    - [**Uitls.hpp**](#uitlshpp)
    - [**Order.hpp**](#orderhpp)
    - [**HashTable.hpp**](#hashtablehpp)
    - [**client.cpp**](#clientcpp)
    - [**clientCustom.cpp**](#clientcustomcpp)
    - [**clientCustom2.cpp**](#clientcustom2cpp)
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
## 4. Custom client 2
Compile with
```
  g++ -o clientCustom2 clientCustom2.cpp
```
To run: 
  - `hostname` is the hostname of the server, I test on local machine so hostname = localhost.
  - `portNumber` is any port number available on your machine.
  - `numberOfOrder` is the amount of order that you want to read from `orders.txt` and send them one by one.
```
  ./clientCustom2 hostname portNumber numberOfOrder
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
- This client should be run first.
- Generate sell/buy orders to send to the server, making the database for the server.
- All the generated orders will be saved in `orders.txt` file.
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
### **clientCustom2.cpp**
- Sending a number of orders one by one to the server with order type, price, and quantity are read from `orders.txt`.
- Reading the generated orders from `orders.txt` to save the order generation time.
- OrderID and matching result are generated and returned by the server.
```
  ./clientCustom2 localhost 1304 2000000

  Connected to server.
  Time taken by sending order one by one: 5205 (ms).
  -----------------------
```
### **server.cpp**
- Matching algorithm bases on FIFO.
- Maintaining 2 hash tables: sellBook and buyBook to store sell orders and buy orders respectively.
- Incoming buy orders will be matched with sell orders in sellBook, it will traverse the linked-list in the hash bucket to update the quantity.
- When the quantity of an order reach 0, it will be deleted from the order book and response as totally matched.
- Listenning from 3 kinds of client.
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
  - clientCustom2
    - This is similar to the clientCustom, however, a large amount of orders will be received and process one by one.
    - The `isLog` flag should be `0` if the amount of orders is bigger than 100 since the writing log time is remarkable.
    - The time to receive and process **2000000 orders one by one** is around **7 seconds**.
    ```
      // not printing log
      Server: got connection from 127.0.0.1 port 51570
      Time taken by receiving, parsing 2000000 orders and matching them one by one: 6876 (ms).
      -----------------------
      // printing log
      Sell OrderId 2000205 - Price 11 - Quantity 81:
        Buy OrderId 2000007 - Price 11 - Quantity 12
        Buy OrderId 2000031 - Price 11 - Quantity 15
        Buy OrderId 2000107 - Price 11 - Quantity 93
        totally matched.
      Buy OrderId 2000206 - Price 89 - Quantity 68:
              partial (or not yet) matched.
      Sell OrderId 2000207 - Price 54 - Quantity 43:
              partial (or not yet) matched.
      Sell OrderId 2000208 - Price 50 - Quantity 98:
              partial (or not yet) matched.
      Sell OrderId 2000209 - Price 19 - Quantity 36:
              partial (or not yet) matched.
      Buy OrderId 2000210 - Price 42 - Quantity 85:
              partial (or not yet) matched.
      Time taken by receiving, parsing 100 orders and matching them one by one: 1243 (ms).
    ```
# V. Note
- The correctness: manually tested with clientCustom. For example:
  ```
  ./clientCustom localhost 1304 0 100 1
  Connected to server.
  Sell OrderId 2000001 - Price 100 - Quantity 1:
          partial (or not yet) matched.

  ./clientCustom localhost 1304 1 100 2
  Connected to server.
  Buy OrderId 2000002 - Price 100 - Quantity 2:
          Sell OrderId 2000001 - Price 100 - Quantity 1
          partial (or not yet) matched.
                                                              
  ./clientCustom localhost 1304 0 100 1
  Connected to server.
  Sell OrderId 2000003 - Price 100 - Quantity 1:
          Buy OrderId 2000002 - Price 100 - Quantity 1
          totally matched.
  ```
- Reference: 
  - [Socket Programming](https://www.bogotobogo.com/cplusplus/sockets_server_client.php)
- This repository is implemented from scratch by Nguyen Ba Vinh Quang.

