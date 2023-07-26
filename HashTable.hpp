#ifndef HASH_TABLE
#define HASH_TABLE
#include <string>
#include "Order.hpp"

struct HashTable {
    Order **head;
    Order **tail;
    int count;
    int size;
};

HashTable* createTable(int size) {
    HashTable* table = (HashTable*) malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->head = (Order**) calloc(table->size, sizeof(Order*));
    table->tail = (Order**) calloc(table->size, sizeof(Order*));

    for (int i = 0; i < table->size; i++) {
        table->head[i] = nullptr;
        table->tail[i] = nullptr;
    }

    return table;
}

void freeTable(HashTable* table) {
    for (int i = 0; i < table->size; i++) {
        Order* order = table->head[i];
        if (order != nullptr) freeOrder(order);
        order = table->tail[i];
        if (order != nullptr) freeOrder(order);
    }
    free(table->head);
    free(table->tail);
    free(table);
}

unsigned long hashFunction(HashTable* table, int price) {
    return price % table->size;
}

void insertHashTable(HashTable *table, Order *order) {
    unsigned long key = hashFunction(table, order->price);
    if (table->head[key] == nullptr) {
        table->head[key] = order;
        table->tail[key] = order;
    } else {
        table->tail[key]->nextOrder = order;
        order->prevOrder = table->tail[key];
        table->tail[key] = order;
    }
    table->count++;
}


Order* removeHead(HashTable *table, Order *order) {
    unsigned long key = hashFunction(table, order->price);
    if (table->head[key] == nullptr) return nullptr;
    if (table->head[key] == table->tail[key]) {
        freeOrder(table->head[key]);
        table->head[key] = table->tail[key] = nullptr;
        table->count--;
        return nullptr;
    }
    Order *tmp = table->head[key];
    table->head[key] = tmp->nextOrder;
    table->head[key]->prevOrder = nullptr;
    table->count--;
    freeOrder(tmp);
    return table->head[key];
}

#endif