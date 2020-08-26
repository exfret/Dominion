//
//  utility.hpp
//  dominion
//
//  Created by Kyle Hess on 8/24/20.
//  Copyright © 2020 Kyle Hess. All rights reserved.
//

// NOTE: Only declarations can go in here since this file is included in another header file
// (Otherwise there will be multiple includes of the same function definition)

#ifndef utility_hpp
#define utility_hpp

#include <cstdlib>
#include <vector>
using namespace std;

struct Card;

const int RANDOM_SEED = 0;
const string STARTING_PHASE = "action";

// Return a random value between the zero and the ceiling value
double getRand(double ceiling = 1);

vector<Card*> getStartingPlayerDeck();

/*inline Card* castCard(Card* cardToCast) {
    if (cardToCast == nullptr) {
        return cardToCast;
    }
    if (dynamic_cast<>cardToCast)
}*/

#endif /* utility_hpp */





































