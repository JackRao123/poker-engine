#include "helper.h"
#include <string> 
#include <iostream>
#include <algorithm>
 
void printArray(std::vector<int> arr){
    for(auto c: arr){
        std::cout << c%13 << " ";
    }

    std::cout << std::endl;
}

bool contains(std::vector<int> array, int value){
    auto it = std::find(array.begin(), array.end(), value);

    return (it!= array.end());
}