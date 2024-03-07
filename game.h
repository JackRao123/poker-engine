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
    int handRanking(std::vector<int> sevenCards);
    int calculateWinner(std::vector<int> board);
};
 
bool royalFlush(std::vector<int> sevenCards);
bool straightFlush(std::vector<int> sevenCards);
bool fourOfAKind(std::vector<int> sevenCards);
bool fullHouse(std::vector<int> sevenCards);
bool flush(std::vector<int> sevenCards);
bool straight(std::vector<int> sevenCards);
bool threeOfAKind(std::vector<int> sevenCards);
bool twoPair(std::vector<int> sevenCards);
bool pair(std::vector<int> sevenCards);

#endif //GAME_H
