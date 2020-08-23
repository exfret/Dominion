//
//  states.cpp
//  dominion
//
//  Created by Kyle Hess on 8/21/20.
//  Copyright © 2020 Kyle Hess. All rights reserved.
//

#include <cstdlib>
#include "card.hpp"
#include "states.hpp"

void Pile::insertCard(OrderedCard cardToInsert) {
    this->contents.insert(cardToInsert);
    this->highestOrder = max(this->highestOrder, cardToInsert.order);
}

void BoardState::reshuffle() {
    Pile* discard = this->player_states[this->curr_player]->cardsInDiscard;
    Pile* deck = this->player_states[this->curr_player]->cardsInDeck;
    
    // Create a random permutation of the discard pile
    vector<pair<int,int>> weights;
    for (int i = 1; i <= discard->contents.size(); ++i) {
        weights.push_back(make_pair(rand(),i));
    }
    sort(weights.begin(),weights.end());
    
    // Insert all the cards from the discard pile in the given order
    int i = 0;
    // We need to keep track of the original value of highestOrder because this will change dynamically in the loop
    int tempHighestOrder = deck->highestOrder;
    for (auto it = discard->contents.begin(); it != discard->contents.end(); ++it) {
        Card tempCard = *it->card;
        int tempOrder = tempHighestOrder + get<1>(weights[i]);
        
        deck->insertCard(OrderedCard(&tempCard, tempOrder));
        
        ++i;
    }
    
    // TODO: Implement on-reshuffle properties (need to make sure we give them the choice of how to order the abilities
    /*for (auto it = deck->contents.begin(); it != deck->contents.end(); ++it) {
        // Test if this was one of the just-shuffled cards
        if (it->order > tempHighestOrder) {
            
            
            it->card->onReshuffle(board_)
        }
    }*/
    
    // Now empty the discard pile
    for (auto it = discard->contents.begin(); it != discard->contents.end(); ++it) {
        discard->contents.clear();
    }
}

bool BoardState::drawSingleCard() {
    Pile* hand = this->player_states[this->curr_player]->cardsInHand;
    Pile* deck = this->player_states[this->curr_player]->cardsInDeck;
    
    if (deck->contents.empty()) {
        this->reshuffle();
    }
    
    // If it's still empty, then there are no cards left to draw
    if (deck->contents.empty()) {
        return false;
    }
    
    OrderedCard drawnCard = *(deck->contents.begin());
    deck->contents.erase(drawnCard);
    hand->contents.insert(drawnCard);
    
    return true;
}

// TODO: Draw all cards at the same time and then resolve onDraw effects in order that player chooses
int BoardState::drawCards(int numCards) {
    int numCardsDrawn = 0;
    for (int i = 0; i < numCards; ++i) {
        if (this->drawSingleCard()) ++numCardsDrawn;
    }
    return numCardsDrawn;
}

void BoardState::addActions(int numActionsToAdd) {
    this->player_states[this->curr_player]->num_actions += numActionsToAdd;
}

void BoardState::addTreasure(int numTreasureToAdd) {
    this->player_states[this->curr_player]->num_treasure += numTreasureToAdd;
}

void BoardState::addBuys(int numBuysToAdd) {
    this->player_states[this->curr_player]->num_buys += numBuysToAdd;
}

void BoardState::addVictoryPoints(int numVictoryPointsToAdd) {
    this->player_states[this->curr_player]->num_victory_points += numVictoryPointsToAdd;
}













































