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
//Monte carlo - figures out the equity of each hand?
std::vector<float> monteCarloGameWinner(Game &g, int iterations, int numThreads = std::thread::hardware_concurrency()) {
    std::vector<float> wins(g.num_players, 0);

    // Thread FX
    auto threadFunc = [&]() {
        Deck copy;

        //Deal.
        for (int i = 0; i < iterations / numThreads; i++) {
            copy = g.deck.deepcopy();
            copy.shuffle();
            std::vector<int> board;

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

            int winner = g.calculateWinner(board);
            wins[winner]++; 
 
        }
    };

    // Threading
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(threadFunc);
    }

    //Wait for threads to done
    for (auto& thread : threads) {
        thread.join();
    }
 
    for (int i = 0; i < wins.size(); i++) {
        std::cout << i << " won " << wins[i] << " times = " << 100 * wins[i] / iterations << "%.\n";
    }

    return wins;
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

//todo
//make cards assignable    //done
//make seed assignable (reproducibility)
//multithreading    //done
//optimisation
//account for draws

int main(){ 

    Game g  =Game(2);
    // g.dealRandom();

    g.manualDeal({
        {"2c", "2s"},
        {"Kh", "Kd"} 
        });
    // g.manualDeal({
    //     {"8d", "Jc"}, 
    //     {"9d", "8s"}, 
    //     {"3h", "Qs"}, 
    //     {"5d", "7d"}, 
    //     {"Kh", "Ks"}, 
    //     {"7h", "Ac"}
    //     });



    monteCarloGameWinner(g, 100000);
    // cout<< Game::straightFlush({ ACE, TWO, THREE, FOUR, FIVE+13}) << endl;
 
    // cout << monteCarlo(10000000, Conditions::firstIsTwoDiamonds) <<endl;
    return 0;
}