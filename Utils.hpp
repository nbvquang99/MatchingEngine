#ifndef UTILS
#define UTILS
#define LOWER_PRICE 1
#define UPPER_PRICE 10
#define LOWER_QUANT 1
#define UPPER_QUANT 10
#define HASH_SIZE 107
#include <random>
#include <chrono>

int getRandomLR(int l, int r){
    std::mt19937 generator(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int>  distr(l, r);
    return distr(generator);
}
#endif