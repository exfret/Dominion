//
//  states.hpp
//  dominion
//
//  Created by Kyle Hess on 8/21/20.
//  Copyright © 2020 Kyle Hess. All rights reserved.
//

#ifndef states_hpp
#define states_hpp

#include <set>
#include <vector>

struct Card;

struct OrderedCard {
    OrderedCard(Card* init_card, int init_order) : card(init_card), order(init_order) {}
    
    bool operator<(const OrderedCard& other) const {
        return order < other.order;
    }
    
    Card* card;
    int order;
};

struct Pile {
    set<OrderedCard> contents;
    int highestOrder;
    
    void insertCard(OrderedCard cardToInsert);
};

struct PlayerState {
    Pile* cardsInPlay;
    Pile* cardsInHand;
    // Higher order means higher up in discard pile
    Pile* cardsInDiscard;
    // Higher order means lower down in deck
    Pile* cardsInDeck;
    
    int num_actions;
    int num_treasure;
    int num_buys;
    
    int num_villagers;
    int num_coffers;
    int num_victory_points;
};

struct BoardState {
    vector<Pile*> board;
    vector<PlayerState*> player_states;
    int curr_player;
    int curr_phase;
    
    // Shuffle the current player's discard pile into their draw deck
    void reshuffle();
    // Returns whether a card was drawn
    bool drawSingleCard();
    // Returns the number of cards drawn
    int drawCards(int numCards);
    // Adds actions to the current player
    void addActions(int numActionsToAdd);
    // Adds treasure to the current player
    void addTreasure(int numTreasureToAdd);
    // Adds buys to the current player
    void addBuys(int numBuysToAdd);
    // Adds victory points to the current player
    void addVictoryPoints(int numVictoryPointsToAdd);
};

#endif /* states_hpp */















































