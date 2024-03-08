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
#include "ranking.h"
 
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


 
//cardSet1.size() = cardSet2.size() = 7
//returns true if cardset1 is better than cardset2
bool Game::betterHand(std::vector<int> cardSet1, std::vector<int> cardSet2, bool & tied){

    std::vector<std::function<int(std::vector<int>)>> fxs = { royalFlush, straightFlush, fourOfAKind, fullHouse, flush, straight, threeOfAKind, twoPair, pair, highCard};//, highCardTiebreaker };
    
    for(int i=0; i<fxs.size(); i++){
        std::function<int(std::vector<int>)> f = fxs[i];
        
        int res1 = f(cardSet1);
        int res2 = f(cardSet2);

        //if they both have something, then the one with the btter one wins.
        //note - modify the functions in ranking.cpp so that two unique hands 
        if(res1 != res2){
            return res1 > res2;
        }else if(res1 != 0){
            //if they are acctually tied
            tied = true;
            return false;
        }
    }

    
    // return highCardTieBreaker(cardSet1, cardSet2, tied);
    printArray(cardSet1);
    printArray(cardSet2);
    throw std::runtime_error("Should have returned" );
    return false;
}


int Game::calculateWinner(std::vector<int> board){ 
    //return the index of the winner.
    std::vector<int> handRankings;
    std::vector<std::vector<int>> sevenCardHands;
    std::vector<std::set<int>> ties;
    
    for(int i=0; i<num_players; i++){
        std::vector<int> sevenCards = board;
        sevenCards.push_back(players[i].card1);
        sevenCards.push_back(players[i].card2); 
        sevenCardHands.push_back(sevenCards);

        handRankings.push_back(i);
    }
     
    //When this array is sorted, traversing it from the front to the end will give the index of the player with the best hand.  
 
    std::sort(handRankings.begin(), handRankings.end(), [this, sevenCardHands, ties](const int &a, const int &b){
        bool tied = false;
        bool aIsBetter=  Game::betterHand(sevenCardHands[a], sevenCardHands[b], tied);

        // if(tied){
        //     bool found = false;
        //     for(std::set<int> tieGroup: ties){
        //         if(tieGroup.find(a) != tieGroup.end() || tieGroup.find(b) != tieGroup.end()){
        //             tieGroup.insert(a);
        //             tieGroup.insert(b);
        //             found = true;
        //             break;
        //         }
        //     }

        //     if(!found){
        //         ties.push_back({a,b});
        //     }
        // }

        return aIsBetter;
    });





    ////////////////////////////////////////////////

    std::cout << "Board: ";
    for(auto c : board){
        std::cout << Deck::cardToString(c) << " ";
    }
    std::cout << std::endl;


    std::cout << "\nHand Rankings: \n";

    for(int i=0; i<handRankings.size(); i++){
        std::cout << handRankings[i] << " ";
        printHand(sevenCardHands[handRankings[i]]);
    }

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