#include "helper.h"
#include "game.h"
#include <string> 
#include <iostream>
#include <algorithm>
 
void printArray(std::vector<int> arr){
    for(auto c: arr){
        std::cout << c << " ";
    }

    std::cout << std::endl;
}


void printCards(std:: vector<int>arr){
    for(auto c: arr){
        std::cout << Deck::cardToString(c) << " ";
    }
 
}

bool contains(std::vector<int> array, int value){
    auto it = std::find(array.begin(), array.end(), value);

    return (it!= array.end());
}