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

#define NOT_DEALT -1
#define TWO  0 
#define THREE 1
#define FOUR 2 
#define FIVE 3 
#define SIX 4 
#define SEVEN 5 
#define EIGHT 6 
#define NINE 7 
#define TEN 8  
#define JACK 9 
#define QUEEN 10 
#define KING 11 
#define ACE 12 
 
std::map<int,std::string> Deck::suits = {
    {0, "♦️"},
    {1, "♣️"},
    {2, "♥️"},
    {3, "♠️"}
};

std::map<int,std::string> Deck::ranks = {
    {TWO, "2"},
    {THREE, "3"},
    {FOUR, "4"},
    {FIVE, "5"},
    {SIX, "6"},
    {SEVEN, "7"},
    {EIGHT, "8"},
    {NINE, "9"},
    {TEN, "T"},
    {JACK, "J"},
    {QUEEN, "Q"},
    {KING, "K"},
    {ACE, "A"}
};
   
 
Game::Game(int num_players): num_players(num_players){
    deck = Deck();
    deck.shuffle();
    

    for(int i=0; i<num_players; i++){
        int c1 = deck.draw();
        int c2 = deck.draw();  
        players.push_back(Player(c1,c2));

        std::cout << "Player " << i << " dealt " <<Deck::cardToString(c1) <<" " << Deck::cardToString(c2) << std::endl;
    }
}
//returns 1-10
//1 = rflush
//10 = hcard

bool royalFlush(std::vector<int> sevenCards){
    std::vector<int> faceCards = {TEN, JACK, QUEEN, KING, ACE};
    
    //For each suit.
    for(int i=0; i<4; i++){
        bool possible = true;
        //Check each card.
        for(int j = 0; j<faceCards.size(); j++){
            if(!contains(sevenCards, faceCards[j] + i*13)){
            possible = false;
            break; 
            }
        }

        if(possible){
            return possible;
        }     
    }

    return false;
}
bool straightFlush(std::vector<int> sevenCards){
    std::vector<std::vector<int>> buckets(4,std::vector<int>(0,0));//sorted into suits

    for(auto c: sevenCards){
        buckets[c/13].push_back(c%13);
    }

    for(auto b: buckets){
        if(straight(b)){
            return true;
        }
    }

    return false;   
}
bool fourOfAKind(std::vector<int> sevenCards){
    std::map<int,int> rankCounter;//counts how many cards of each rank.

    for(auto c: sevenCards){
        rankCounter[c%13]++;
        if(rankCounter[c%13] >= 4){
            return true;
        }
    }

    return false;
}    
bool fullHouse(std::vector<int> sevenCards){
    std::map<int,int> rankCounter;//counts how many cards of each rank.

    for(auto c: sevenCards){
        rankCounter[c%13]++;
    }

    //which trip and which pair
    int tripsCount = 0;
    int pairsCount = 0;

    for(std::pair<int,int> p: rankCounter){
        if(p.second >= 3){
            tripsCount++;
        }else if(p.second >= 2){
            pairsCount++;
        }
    } 

    return (tripsCount >=2) || (tripsCount >= 1 && pairsCount >= 1); 
}
bool flush(std::vector<int> sevenCards){
    std::map<int,int> suitCounter;

    for(auto c: sevenCards){
        suitCounter[c/13]++;

        if(suitCounter[c/13] >= 5){
            return true;
        }
    }

    return false;
}
bool straight(std::vector<int> sevenCards){
    sort(sevenCards.begin(), sevenCards.end());

    //it is impossible to have two straights of different suits in a set of 7 cards.

    std::set<int> cards;

    for(auto c: sevenCards){
        cards.insert(c%13);

        if(c == ACE){
            cards.insert(-1); //A-5 or T-A straight.
        }
    }   

    int straightSize = 1;
    for(auto it = cards.begin(); it!= cards.end(); it++){
        if(*it + 1 == *std::next(it)){
            straightSize++;
        }else{
            straightSize = 1;
        }

        if(straightSize == 5){
            return true;
        }
    }

    return false;

}
bool threeOfAKind(std::vector<int> sevenCards){
    std::map<int,int> rankCounter;//counts how many cards of each rank.

    for(auto c: sevenCards){
        rankCounter[c%13]++;
        if(rankCounter[c%13] >= 3){
            return true;
        }
    }

    return false;
}
bool twoPair(std::vector<int> sevenCards){
    std::map<int,int> rankCounter;//counts how many cards of each rank.

    for(auto c: sevenCards){
        rankCounter[c%13]++;
    }

    int numPairs = 0;
    for(std::pair<int,int> p: rankCounter){
        numPairs = numPairs + (p.second >= 2);
    } 

    return (numPairs >= 2);
}
bool pair(std::vector<int> sevenCards){
    std::map<int,int> rankCounter;//counts how many cards of each rank.

    for(auto c: sevenCards){
        rankCounter[c%13]++;
        if(rankCounter[c%13] >= 2){
            return true;
        }
    }

    return false;
}



int Game::handRanking(std::vector<int> sevenCards){
    if(royalFlush(sevenCards)){
        return 1;
    }

    if(straightFlush(sevenCards)){
        return 2;
    }

    if(fourOfAKind(sevenCards)){
        return 3;
    }

    if(fullHouse(sevenCards)){
        return 4;
    }

    if(flush(sevenCards)){
        return 5;
    }

    if(straight(sevenCards)){
        return 6;
    }

    if(threeOfAKind(sevenCards)){
        return 7;
    }

    if(twoPair(sevenCards)){
        return 8;
    }

    if(pair(sevenCards)){
        return 9;
    }

    //high card.
    return 10; 
}


int Game::calculateWinner(std::vector<int> board){ 
    //return the index of the winner.
    std::vector<int> handRankings;
    for(int i=0; i<num_players; i++){
        std::vector<int> sevenCards = board;
        sevenCards.push_back(players[i].card1);
        sevenCards.push_back(players[i].card2);
        
        handRankings.push_back(handRanking(sevenCards));
    }

    std::cout << "Board: ";
    for(auto c : board){
        std::cout << Deck::cardToString(c) << " ";
    }
    std::cout << std::endl;


    std::cout << "Hand Rankings: ";
    printArray(handRankings);
    std::cout << std::endl;

    //stub
    return -1;
}
 


Deck::Deck() {
    for(int i=0; i<52; i++){
        cards.push_back(i);
    }      
}

void Deck::shuffle() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
}

std::string Deck::cardToString(int card){
    //2 3 4 5 6 7 8 9 10 J Q K A (diamonds)
    //2 3 4 5 6 7 8 9 10 J Q K A (clubs)
    //2 3 4 5 6 7 8 9 10 J Q K A (hearts)
    //2 3 4 5 6 7 8 9 10 J Q K A (spades)

    std::string suit = suits[card/13];
    std::string rank = ranks[card%13];

    return rank +suit;
}
    
void Deck::printDeck() {
    for(int card : cards) {
        std::cout << cardToString(card) << std::endl;
    }
}

int Deck::draw(){
    int drawn = cards.back();
    cards.pop_back();

    return drawn;
}

void Deck::dealSpecific(int card) {
    auto it = std::find(cards.begin(), cards.end(), card);
    if (it != cards.end()) {
        cards.erase(it);  
    } else { 
        throw std::runtime_error("Card not found in the deck.");
    }
}

Deck Deck::deepcopy(){
    Deck copy = Deck();
    copy.cards =  cards;

    return copy;
}


 
Player::Player(): card1(NOT_DEALT), card2(NOT_DEALT){

}
Player::Player(int card1, int card2): card1(card1), card2(card2){

}