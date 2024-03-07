#include <vector>
#include <string>
#include <map>  
#include <random>  
#include <chrono>  
#include <algorithm>  
#include <iostream>
#include <functional>
#include <thread> 
#include <atomic>
using namespace std;
 

class Deck {
private:
    vector<int> cards;
     
public:
    static map<int,string> suits;
    static map<int, string> ranks;
    Deck() {
        for(int i=0; i<52; i++){
            cards.push_back(i);
        }      
    }

    void shuffle() {
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(cards.begin(), cards.end(), default_random_engine(seed));
    }
 
    string cardToString(int card){
        //2 3 4 5 6 7 8 9 10 J Q K A (diamonds)
        //2 3 4 5 6 7 8 9 10 J Q K A (clubs)
        //2 3 4 5 6 7 8 9 10 J Q K A (hearts)
        //2 3 4 5 6 7 8 9 10 J Q K A (spades)

        string suit = suits[card/13];
        string rank = ranks[card%13];

        return rank + " of " + suit;
    }
     
    void printDeck() {
        for(int card : cards) {
            cout << cardToString(card) << endl;
        }
    }

    int & operator[](int index){
        return cards[index];
    } 
};

map<int,string> Deck::suits = {
    {0, "Diamonds"},
    {1, "Clubs"},
    {2, "Hearts"},
    {3, "Spades"}
};

map<int,string> Deck::ranks = {
    {0, "Two"},
    {1, "Three"},
    {2, "Four"},
    {3, "Five"},
    {4, "Six"},
    {5, "Seven"},
    {6, "Eight"},
    {7, "Nine"},
    {8, "Ten"},
    {9, "Jack"},
    {10, "Queen"},
    {11, "King"},
    {12, "Ace"}
};

class Conditions{
//if some condition is satisfied by Deck d return true, else false
public:
    static bool firstIsTwoDiamonds(Deck & d){
        return d[0] == 0 && d[1] == 1;
    }
};



float monteCarlo(int iterations, function<bool(Deck&)> condition, int numThreads = thread::hardware_concurrency()) {
    cout << "Beginning Monte-Carlo simulations for " << iterations << " iterations and " << numThreads << " threads.\n";
    atomic<int> successes(0);
    vector<thread> threads;

    // Lambda function - thread work fx
    auto work = [&](int threadIterations) {
        Deck d = Deck();
        for (int i = 0; i < threadIterations; i++) {
            d.shuffle();
            if (condition(d)) {
                successes++;
            }
        }
    };

    int baseIterations = iterations / numThreads;
    int extraIterations = iterations % numThreads;
 
    for (int i = 0; i < numThreads; ++i) {
        int threadIterations = baseIterations + (i < extraIterations ? 1 : 0);
        threads.emplace_back(work, threadIterations);
    }

    //  Wait for finish
    for (auto& thread : threads) {
        thread.join();
    }

    // Output
    cout << "Successes = " << successes.load() << endl;
    cout << "Total = " << iterations << endl;

    return static_cast<float>(successes) / static_cast<float>(iterations);
}


int main(){ 
    Deck d = Deck();
    d.shuffle(); 
   
    cout << monteCarlo(1000000000, Conditions::firstIsTwoDiamonds) <<endl;
    return 0;
}