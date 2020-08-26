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
#include "utility.hpp"
using namespace std;

struct Card;

// This needs to be forward declared for OrderedCard to understand what a pile is
struct Pile;

struct OrderedCard {
    OrderedCard(Card* init_card, double init_order, Pile* init_parent) : card(init_card), order(init_order), parent(init_parent) {}
    
    // May need to add an assignment operator to stop from tracking the same card around
    bool operator<(const OrderedCard& other) const {
        return order < other.order;
    }
    
    Card* card;
    // TODO: Change this to something that makes cards easier to select (a string?)
    double order;
    Pile* parent;
};

struct Pile {
    Pile() {}
    Pile(vector<Card*> cards) {
        for (int i = 0; i < cards.size(); ++i) {
            Card* tempCard = cards[i];
            contents.insert(OrderedCard(tempCard, i, this));
        }
    }
    
    set<OrderedCard> contents;
    
    double getHighestOrder() {
        if (contents.empty()) return 0;
        else return contents.rbegin()->order;
    }
    
    // If no order is specified, put the card at the highest order position
    void transferCard(OrderedCard cardToTransfer, double newOrder);
    void transferCard(OrderedCard cardToTransfer);
    void transferTo(Pile* otherPile, bool withShuffling = false);
    
    ////////////////////////////////////////////////////////////////////////////////
    // Interface functions
    ////////////////////////////////////////////////////////////////////////////////
    string getPileString();
};

struct PlayerState {
    PlayerState() : num_actions(0), num_treasure(0), num_buys(0), num_villagers(0), num_coffers(0), num_victory_points(0) {
        // Put the starting player deck inthe discard so that it gets shuffled when new cards are drawn
        cardsInPlay = new Pile();
        cardsInHand = new Pile();
        cardsInDiscard = new Pile(getStartingPlayerDeck());
        cardsInDeck = new Pile();
        drawCards(5);
    }
    
    Pile* cardsInPlay;
    Pile* cardsInHand;
    // Higher order means higher up in discard pile
    Pile* cardsInDiscard;
    // Higher order means lower down in deck (i.e.- lower order gets drawn first)
    Pile* cardsInDeck;
    
    int num_actions;
    int num_treasure;
    int num_buys;
    
    int num_villagers;
    int num_coffers;
    int num_victory_points;
    
    void reshuffle();
    bool drawSingleCard();
    int drawCards(int numCards);
};

struct BoardState {
    BoardState(int numPlayers = 2) : curr_player(0), curr_phase("start of game") {
        for (int i = 0; i < numPlayers; ++i) {
            player_states.push_back(new PlayerState());
        }
        initializeBoard();
    }
    
    int curr_player;
    string curr_phase;
    vector<PlayerState*> player_states;
    // For the board, higher order means higher up in the pile
    vector<Pile*> board;
    
    void initializeBoard();
    
    // Some helper functions to access the current player's member variables faster
    PlayerState* getCurrPlayerState();
    Pile* getCurrInPlay();
    Pile* getCurrInHand();
    Pile* getCurrInDiscard();
    Pile* getCurrInDeck();
    
    int getCurrActions() {return getCurrPlayerState()->num_actions;}
    int getCurrTreasure() {return getCurrPlayerState()->num_treasure;}
    int getCurrBuys() {return getCurrPlayerState()->num_buys;}
    
    int getCurrVillagers() {return getCurrPlayerState()->num_villagers;}
    int getCurrCoffers() {return getCurrPlayerState()->num_coffers;}
    int getCurrVictoryPoints() {return getCurrPlayerState()->num_victory_points;}
    
    // Shuffle the current player's discard pile into their draw deck
    void reshuffle();
    // Returns whether a card was drawn
    bool drawSingleCard();
    // Returns the number of cards drawn
    int drawCards(int numCards);
    // Adds actions to the current player
    void addActions(int numActionsToAdd);
    // Sets actions of the current player
    void setActions(int newActionsAmount);
    // Adds treasure to the current player
    void addTreasure(int numTreasureToAdd);
    // Sets treasure of the current player
    void setTreasure(int newTreasureAmount);
    // Adds buys to the current player
    void addBuys(int numBuysToAdd);
    // Set buys of the current player
    void setBuys(int newBuyAmount);
    // Adds victory points to the current player
    void addVictoryPoints(int numVictoryPointsToAdd);
    
    // TODO: addCoffers and addVillagers
    
    void playCard(OrderedCard cardToPlay);
    void gainCard(OrderedCard cardToGain);
    void buyCard(OrderedCard cardToBuy);
    
    // Resets some player state values for the next turn
    void startTurn();
    // Shift currplayer to next player
    void goToNextPlayer();
    // Complete a player's cleanup phase and go on to the next player
    void endTurn();
    // Completes last player's turn and goes on to the next player
    void nextTurn();
    
    ////////////////////////////////////////////////////////////////////////////////
    // Interface functions
    ////////////////////////////////////////////////////////////////////////////////
    string getBoardString();
};

#endif /* states_hpp */















































