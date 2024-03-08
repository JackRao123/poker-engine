#ifndef GAME_H // Include guard to prevent multiple inclusions
#define GAME_H
  

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
#include <string>

class Deck{
public:
    std::vector<int> cards;
    static std::map<int,std::string> suits;
    static std::map<int, std::string> ranks;
    Deck();
    Deck deepcopy();
    int draw();
    void shuffle();
    void printDeck();
    void dealSpecific(int card);
    static std::string cardToString(int card);
};

class Player{
public:
    int card1;
    int card2;
    Player();
    Player(int card1, int card2);
};

class Game {
public:
    int num_players;
    Deck deck;
    std::vector<Player> players; 
    Game(int num_players);
    bool betterHand(std::vector<int> cardSet1, std::vector<int> cardSet2, bool & tied);
    int calculateWinner(std::vector<int> board);
};
 
#endif //GAME_H
