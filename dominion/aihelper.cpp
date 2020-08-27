//
//  aihelper.cpp
//  dominion
//
//  Created by Kyle Hess on 8/27/20.
//  Copyright © 2020 Kyle Hess. All rights reserved.
//

#include "aihelper.hpp"
#include "card.hpp"
#include "states.hpp"

set<pair<double, string>> getWeightedChoices(BoardState* game) {
    set<pair<double, string>> weightedChoices;
    Pile* hand = game->getCurrInHand();
    
    if (game->curr_phase == "action") {
        // Right now there are no actions implemented, so no logic needed
    }
    else if (game->curr_phase == "buy - play treasures") {
        // Test for treasures in the player's hand
        for (auto it = hand->contents.begin(); it != hand->contents.end(); ++it) {
            // Test if this is a treasure card; if so, add it to the choices
            if (it->card->type.find("treasure") != it->card->type.end()) {
                string tempString = "play ";
                // Now add the card's id to the string
                for (int i = 0; i < it->order.size(); ++i) {
                    tempString += to_string(it->order[i]);
                    tempString += ".";
                }
                weightedChoices.insert(make_pair(1,tempString));
            }
        }
    }
    else if (game->curr_phase == "buy - buy cards") {
        for (int i = 0; i < game->board.size(); ++i) {
            // Test if the pile is one that can be bought from
            if (!game->board[i]->contents.empty() && game->canBuyCard(*game->board[i]->contents.rbegin())) {
                weightedChoices.insert(make_pair(0.5, "buy " + to_string(i)));
            }
        }
    }
    
    // Don't forget "next" option!
    weightedChoices.insert(make_pair(0.5, "next"));
    
    return weightedChoices;
}




































