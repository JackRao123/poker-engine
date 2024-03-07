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
#include <set>

 
#include "game.h"
#include "helper.h"


// class Conditions{
// //if some condition is satisfied by Deck d return true, else false
// public:
//     static bool firstIsTwoDiamonds(Deck & d){
//         return d.cards[0] == 0 && d.cards[1] == 1;
//     }
// };
 
using namespace std;
//Monte carlo - figures out the equity of each hand.
vector<float> monteCarloGameWinner(Game & g, int iterations, int numThreads = thread::hardware_concurrency()){
    Deck copy = g.deck.deepcopy();

    vector<int> wins(g.num_players, 0);

    vector<int> board;

    //Flop
    board.push_back(copy.draw());
    board.push_back(copy.draw());
    board.push_back(copy.draw());

    //Burn
    copy.draw();
    
    //Turn
    board.push_back(copy.draw());
    
    //Burn
    copy.draw();

    //River
    board.push_back(copy.draw());
    
    g.calculateWinner(board);
    

    return {};
}

//Monte carlo for a deck configuration - how many times will (condition) occur in a deck of 52 cards.
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
    Game g  =Game(4);
    monteCarloGameWinner(g, 10);
    // cout<< Game::straightFlush({ ACE, TWO, THREE, FOUR, FIVE+13}) << endl;


    // cout << monteCarlo(10000000, Conditions::firstIsTwoDiamonds) <<endl;
    return 0;
}