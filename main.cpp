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
 
using namespace std;
class Deck {
private:
    
     
public:
vector<int> cards;
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
 
    static string cardToString(int card){
        //2 3 4 5 6 7 8 9 10 J Q K A (diamonds)
        //2 3 4 5 6 7 8 9 10 J Q K A (clubs)
        //2 3 4 5 6 7 8 9 10 J Q K A (hearts)
        //2 3 4 5 6 7 8 9 10 J Q K A (spades)

        string suit = suits[card/13];
        string rank = ranks[card%13];

        return rank +suit;
    }
     
    void printDeck() {
        for(int card : cards) {
            cout << cardToString(card) << endl;
        }
    }

     


    int draw(){
        int drawn = cards.back();
        cards.pop_back();

        return drawn;
    }

    void dealSpecific(int card) {
        auto it = std::find(cards.begin(), cards.end(), card);
        if (it != cards.end()) {
            cards.erase(it);  
        } else { 
            throw std::runtime_error("Card not found in the deck.");
        }
    }

    Deck deepcopy(){
        Deck copy = Deck();
        copy.cards = this->cards;

        return copy;
    }
};

map<int,string> Deck::suits = {
    {0, "♦️"},
    {1, "♣️"},
    {2, "♥️"},
    {3, "♠️"}
};

map<int,string> Deck::ranks = {
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

class Conditions{
//if some condition is satisfied by Deck d return true, else false
public:
    static bool firstIsTwoDiamonds(Deck & d){
        return d.cards[0] == 0 && d.cards[1] == 1;
    }
};

class Player{
    
public:
int card1;
    int card2;
    Player(): card1(NOT_DEALT), card2(NOT_DEALT){

    }

    Player(int card1, int card2): card1(card1), card2(card2){

    }
};





void printArray(vector<int> arr){
    for(auto c: arr){
        cout << c << " ";
    }

    cout << endl;
}
bool contains(vector<int> array, int value){
    auto it = find(array.begin(), array.end(), value);

    return (it!= array.end());
}

class Game{
public:
    int num_players;
    Deck deck;
    vector<Player> players;

    Game(int num_players): num_players(num_players){
        deck = Deck();
        deck.shuffle();
        

        for(int i=0; i<num_players; i++){
            int c1 = deck.draw();
            int c2 = deck.draw();  
            players.push_back(Player(c1,c2));

            cout << "Player " << i << " dealt " <<Deck::cardToString(c1) <<" " << Deck::cardToString(c2) << endl;
        }
    }
    //returns 1-10
    //1 = rflush
    //10 = hcard
    
    static bool royalFlush(vector<int> sevenCards){
        vector<int> faceCards = {TEN, JACK, QUEEN, KING, ACE};
        
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

    static bool fourOfAKind(vector<int> sevenCards){
        map<int,int> rankCounter;//counts how many cards of each rank.

        for(auto c: sevenCards){
            rankCounter[c%13]++;
            if(rankCounter[c%13] >= 4){
                return true;
            }
        }

        return false;
    }
    static bool straightFlush(vector<int> sevenCards){
        vector<vector<int>> buckets(4,vector<int>(0,0));//sorted into suits

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

    static bool fullHouse(vector<int> sevenCards){
        map<int,int> rankCounter;//counts how many cards of each rank.

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
    static bool flush(vector<int> sevenCards){
        map<int,int> suitCounter;

        for(auto c: sevenCards){
            suitCounter[c/13]++;

            if(suitCounter[c/13] >= 5){
                return true;
            }
        }

        return false;
    }
    static bool straight(vector<int> sevenCards){
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

    static bool threeOfAKind(vector<int> sevenCards){
        map<int,int> rankCounter;//counts how many cards of each rank.

        for(auto c: sevenCards){
            rankCounter[c%13]++;
            if(rankCounter[c%13] >= 3){
                return true;
            }
        }

        return false;
    }
 
    static bool twoPair(vector<int> sevenCards){
        map<int,int> rankCounter;//counts how many cards of each rank.

        for(auto c: sevenCards){
            rankCounter[c%13]++;
        }

        int numPairs = 0;
        for(std::pair<int,int> p: rankCounter){
            numPairs = numPairs + (p.second >= 2);
        } 

        return (numPairs >= 2);
    }
 
    static bool pair(vector<int> sevenCards){
        map<int,int> rankCounter;//counts how many cards of each rank.

        for(auto c: sevenCards){
            rankCounter[c%13]++;
            if(rankCounter[c%13] >= 2){
                return true;
            }
        }

        return false;
    }

    int handRanking(vector<int> sevenCards){
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

    int calculateWinner(vector<int> board){ 
        //return the index of the winner.
        vector<int> handRankings;
        for(int i=0; i<num_players; i++){
            vector<int> sevenCards = board;
            sevenCards.push_back(players[i].card1);
            sevenCards.push_back(players[i].card2);
            
            handRankings.push_back(handRanking(sevenCards));
        }
        
        cout << "Board: ";
        for(auto c : board){
            cout << Deck::cardToString(c) << " ";
        }
        cout << endl;


        cout << "Hand Rankings: ";
        printArray(handRankings);
        cout << endl;

        //stub
        return -1;
    }
};

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