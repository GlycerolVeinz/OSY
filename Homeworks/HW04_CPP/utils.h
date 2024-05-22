//
// Created by glycerolveinz on 15.11.23.
//

#ifndef HW04_CPP_UTILS_H
#define HW04_CPP_UTILS_H

#include <iostream>
#include <cstdlib>
#include <thread>
#include <semaphore>

struct DataItem {
    int count;
    std::string word;
};

struct ThreadSharedData{

};

long getNumOfCores();

#endif //HW04_CPP_UTILS_H
