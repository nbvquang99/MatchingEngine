#ifndef ORDER
#define ORDER
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include "Utils.hpp"

struct Order {
    int orderId;
    bool isBuy;
    int quantity;
    int price;
    Order *nextOrder;
    Order *prevOrder;
};

Order** genBuyOrder(int num) {
    Order **buyArrays = (Order**) calloc(num, sizeof(Order*));
    while (num--) {
        buyArrays[num] = (Order*) malloc(sizeof(Order));
        buyArrays[num]->isBuy = true;
        buyArrays[num]->orderId = num+1;
        buyArrays[num]->price = getRandomLR(LOWER_PRICE, UPPER_PRICE);
        buyArrays[num]->quantity = getRandomLR(LOWER_QUANT, UPPER_QUANT);
        buyArrays[num]->nextOrder = nullptr;
        buyArrays[num]->prevOrder = nullptr;
    }
    return buyArrays;
}

Order** genSellOrderBaseOnBuyOrder(int sellNum, int buyNum) {
    Order **sellArrays = (Order**) calloc(sellNum, sizeof(Order*));
    while (sellNum--) {
        sellArrays[sellNum] = (Order*) malloc(sizeof(Order));
        sellArrays[sellNum]->isBuy = false;
        sellArrays[sellNum]->orderId = sellNum + buyNum + 1;
        sellArrays[sellNum]->price = getRandomLR(LOWER_PRICE, UPPER_PRICE);
        sellArrays[sellNum]->quantity = getRandomLR(LOWER_QUANT, UPPER_QUANT);
        sellArrays[sellNum]->nextOrder = nullptr;
        sellArrays[sellNum]->prevOrder = nullptr;
    }
    return sellArrays;
}

void freeOrder(Order* order)
{
    free(order);
}

std::string orderToString(Order* order) {
    std::string res = std::to_string(order->orderId) + " ";
    res += std::to_string(order->isBuy ? 1 : 0) + " ";
    res += std::to_string(order->price) + " ";
    res += std::to_string(order->quantity) + " ";
    return res;
}

#endif