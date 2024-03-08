#ifndef RANKING_H // Include guard to prevent multiple inclusions
#define RANKING_H
#include <vector>
 
 
int royalFlush(std::vector<int> sevenCards);
int straightFlush(std::vector<int> sevenCards);
int fourOfAKind(std::vector<int> sevenCards);
int fullHouse(std::vector<int> sevenCards);
int flush(std::vector<int> sevenCards);
int straight(std::vector<int> sevenCards);
int threeOfAKind(std::vector<int> sevenCards);
int twoPair(std::vector<int> sevenCards);
int pair(std::vector<int> sevenCards);
int highCard(std::vector<int> sevenCards);
// bool highCardTieBreaker(std::vector<int> cardSet1, std::vector<int> cardSet2, bool & tied);

#endif //RANKING_H
