//
//  utility.cpp
//  dominion
//
//  Created by Kyle Hess on 8/24/20.
//  Copyright © 2020 Kyle Hess. All rights reserved.
//

#include "card.hpp"
#include "states.hpp"
#include "utility.hpp"

double getRand(double ceiling)  {
    return ceiling * ((double) rand()) / ((double) RAND_MAX);
}

vector<Card*> getStartingPlayerDeck() {
    vector<Card*> startingDeck;
    for (int i = 0; i < 7; ++i) {
        startingDeck.push_back(new Copper());
    }
    for (int i = 0; i < 3; ++i) {
        startingDeck.push_back(new Estate());
    }
    
    return startingDeck;
}


































