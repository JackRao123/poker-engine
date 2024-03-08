
#include <vector>
#include <algorithm>
#include <map>  
#include <set> 
#include "ranking.h"
#include "game.h"
#include "helper.h"
#include <cmath>

#include <iostream>

int royalFlush(std::vector<int> sevenCards){
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
            return 1;
        }     
    }

    return 0;
}

int straightFlush(std::vector<int> sevenCards){
    std::vector<std::vector<int>> buckets(4,std::vector<int>(0,0));//sorted into suits

    for(auto c: sevenCards){
        buckets[c/13].push_back(c%13);
    }

    //best straight flush ranking (this means score)
    int bestSF = 0;
    for(auto b: buckets){
        bestSF = std::max(bestSF, straight(b));
    }

    return bestSF%13;   
}
 
int fourOfAKind(std::vector<int> sevenCards){
    std::map<int,int> rankCounter;//counts how many cards of each rank.

    int highestQuads = 0; 

    for(auto c: sevenCards){
        rankCounter[c%13]++;
        if(rankCounter[c%13] >= 4){
            highestQuads = std::max(highestQuads, c%13);
        }
    }

    return highestQuads%13;
}    

int fullHouse(std::vector<int> sevenCards){
    std::map<int,int> rankCounter;//counts how many cards of each rank.

    for(auto c: sevenCards){
        rankCounter[c%13]++;
    }

    //which trip and which pair
    int bestTrips = 0;
    int bestPair = 0;

    for(std::pair<int,int> p: rankCounter){
        if(p.second >= 3){
            if(p.first > bestTrips){
                bestPair = std::max(bestPair, bestTrips);
                bestTrips = p.first;
            }else{
                bestPair = std::max(bestPair, p.first);
            }
        }else if(p.second >= 2){
            bestPair = std::max(bestPair, p.first);
        }
    } 

    //best is 12 * 13 +  11= 167 (AAAKK)
    //worst is 0*13 + 1 = 1 (22233)

    if(bestTrips*bestPair != 0){
        return bestTrips*13 + bestPair;
    }
    return 0;
    
}

int flush(std::vector<int> sevenCards){
    std::map<int,int> suitCounter;

    int flushScore = 0x0;
    
    //0b1111100000000 = AKQJT flush
    //0b0000000011111 = 23456 flush


    for(auto c: sevenCards){
        suitCounter[c/13]++;   
    }

    for(std::pair<int,int> p: suitCounter){
        if(p.second >=5){
            int suit = p.first;
            std::sort(sevenCards.begin(), sevenCards.end(), [](int a, int b){return a%13 > b%13;});
            for(auto card: sevenCards){
                if(suit == card/13){
                    flushScore = flushScore + (0x1<<(card%13)); 
                }
            } 
        }
    }
 
 
    // std::cout << "flushscore =  " <<flushScore <<std::endl;
    return flushScore;
}
 
int straight(std::vector<int> sevenCards){
    sort(sevenCards.begin(), sevenCards.end());

    //it is impossible to have two straights of different suits in a set of 7 cards.

    std::set<int> cards;

    for(auto c: sevenCards){
        cards.insert(c%13);

        if(c == ACE){
            cards.insert(-1); //A-5 or T-A straight.
        }
    }   

    int topStraightCard = 0;

    int straightSize = 1;
    for(auto it = cards.begin(); it!= cards.end(); it++){
        if(*it + 1 == *std::next(it)){
            straightSize++;
        }else{
            straightSize = 1;
        }

        if(straightSize >= 5){
            topStraightCard = std::max(topStraightCard, (*it + 13)%13);
        }
    }

    return topStraightCard%13;

}

int threeOfAKind(std::vector<int> sevenCards){
    std::map<int,int> rankCounter;//counts how many cards of each rank.

    int highestTrips = 0; 
    for(auto c: sevenCards){
        rankCounter[c%13]++;
        if(rankCounter[c%13] >= 3){
            highestTrips = c%13;
        }
    }

    //if we dont have trips
    if(highestTrips ==0){
        return 0;
    }

    //tiebreaking
    //we want to find the 2 kickers 
    for(int i=0; i<sevenCards.size(); i++){
        if(sevenCards[i]%13 == highestTrips){
            sevenCards[i] = -1; //invalidate.
        }
    }
    std::sort(sevenCards.begin(), sevenCards.end(), [](int a, int b){return a%13 > b%13;});
    
    return highestTrips*13*13 + 13*(sevenCards[0]%13) + sevenCards[1]%13;
}

int twoPair(std::vector<int> sevenCards){
    //when both people have two pair, whoever has the higher top pair wins
    int bestPair= -1;
    int secondBestPair = -1;

    std::sort(sevenCards.begin(), sevenCards.end(), [](int a, int b){return a%13 > b%13;});
    for(int i=0; i<sevenCards.size() -1; i++){
        if(sevenCards[i]%13 == sevenCards[i+1]%13){
            if(bestPair == -1){
                bestPair = sevenCards[i]%13;
            }else{
                secondBestPair = sevenCards[i]%13;
                break;
            }
            i++;
        }
    }

    //no twopair
    if(bestPair == -1 || secondBestPair == -1){
        return 0;
    }
   
    for(int i=0; i<sevenCards.size(); i++){
        if(sevenCards[i]%13 == bestPair || sevenCards[i]%13 == secondBestPair){
            sevenCards[i] = -1;
        }
    }
    std::sort(sevenCards.begin(), sevenCards.end(), [](int a, int b){return a%13 > b%13;});
    
    return bestPair*13*13  + secondBestPair*13 + sevenCards[0]%13;
}
 
int pair(std::vector<int> sevenCards){
    int bestPairRanking = 0; 
    std::sort(sevenCards.begin(), sevenCards.end(), [](int a, int b){return a%13 > b%13;});
 
    for(int i=0; i<sevenCards.size() -1; i++){
        if(sevenCards[i]%13 == sevenCards[i+1]%13){
            bestPairRanking = sevenCards[i]%13;
            break;
        }
    }
    
    //no pair
    if(bestPairRanking==0){
        return 0;
    }

    //kickers
    int highCardsLeft = 3;
    int score = bestPairRanking*pow(13,3);

    for(auto c: sevenCards){
        if(c!=bestPairRanking){
            score = score + (c%13)*pow(13,highCardsLeft-1);
            highCardsLeft--;
        }
    }

    return score;  
}
 
int highCard(std::vector<int> sevenCards){
    std::sort(sevenCards.begin(), sevenCards.end(), [](int a, int b){return a%13 > b%13;});

    int score = 0;
    for(int i=0; i<5; i++){
        score = score + (sevenCards[i]%13)*pow(13,(4-i));
    }

    return score;
}




void printHand(std::vector<int> sevenCards) {
    if (royalFlush(sevenCards)) {
        std::cout << "Royal flush: ";
        printCards(sevenCards);
        std::cout << std::endl;
    } else if (straightFlush(sevenCards)) {
        std::cout << "Straight flush: ";
        printCards(sevenCards);
        std::cout << std::endl;
    } else if (fourOfAKind(sevenCards)) {
        std::cout << "Four of a kind: ";
        printCards(sevenCards);
        std::cout << std::endl;
    } else if (fullHouse(sevenCards)) {
        std::cout << "Full house: ";
        printCards(sevenCards);
        std::cout << std::endl;
    } else if (flush(sevenCards)) {
        std::cout << "Flush: ";
        printCards(sevenCards);
        std::cout << std::endl;
    } else if (straight(sevenCards)) {
        std::cout << "Straight: ";
        printCards(sevenCards);
        std::cout << std::endl;
    } else if (threeOfAKind(sevenCards)) {
        std::cout << "Three of a kind: ";
        printCards(sevenCards);
        std::cout << std::endl;
    } else if (twoPair(sevenCards)) {
        std::cout << "Two pair: ";
        printCards(sevenCards);
        std::cout << std::endl;
    } else if (pair(sevenCards)) {
        std::cout << "Pair: ";
        printCards(sevenCards);
        std::cout << std::endl;
    } else {
        std::cout << "High card: ";
        printCards(sevenCards);
        std::cout << std::endl;
    }
}



// //Returns true if cardSet1 is better.
// bool highCardTieBreaker(std::vector<int> cardSet1, std::vector<int> cardSet2, bool & tied){   
//     // cassert(cardSet1.size() == cardSet2.size());
  
//     std::sort(cardSet1.begin(), cardSet1.end(), std::less<int>());
//     std::sort(cardSet2.begin(), cardSet2.end(), std::less<int>());

//     for(int i=0; i<cardSet1.size();i++){
//         if(cardSet1[i]%13 == cardSet2[i]%13){
//             continue;
//         }else{
//             return cardSet1[i]%13 > cardSet1[i]%13;
//         }
//     }


//     tied = true;
//     return false; 
// }




//when comparing, go down the list of royalFlush -> pair.
//Each one returns a number
//0 = not achieved
//the higher the number the better


