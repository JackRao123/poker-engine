#ifndef RANKING_H // Include guard to prevent multiple inclusions
#define RANKING_H
#include <vector>
 
 
bool royalFlush(std::vector<int> sevenCards);
bool straightFlush(std::vector<int> sevenCards);
bool fourOfAKind(std::vector<int> sevenCards);
bool fullHouse(std::vector<int> sevenCards);
bool flush(std::vector<int> sevenCards);
bool straight(std::vector<int> sevenCards);
bool threeOfAKind(std::vector<int> sevenCards);
bool twoPair(std::vector<int> sevenCards);
bool pair(std::vector<int> sevenCards);

#endif //RANKING_H
