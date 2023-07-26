#ifndef UTILS
#define UTILS
#define LOWER_PRICE 1
#define UPPER_PRICE 100
#define LOWER_QUANT 1
#define UPPER_QUANT 100
#define HASH_SIZE 100007
#include <random>
#include <chrono>

int getRandomLR(int l, int r){
    std::mt19937 generator(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int>  distr(l, r);
    return distr(generator);
}
#endif