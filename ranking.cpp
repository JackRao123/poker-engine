
#include <vector>
#include <algorithm>
#include <map>  
#include <set> 
#include "ranking.h"
#include "game.h"
#include "helper.h"
 

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

