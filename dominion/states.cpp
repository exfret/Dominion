//
//  states.cpp
//  dominion
//
//  Created by Kyle Hess on 8/21/20.
//  Copyright © 2020 Kyle Hess. All rights reserved.
//

#include <cstdlib>
#include "card.hpp"
#include "event.hpp"
#include "states.hpp"

////////////////////////////////////////////////////////////////////////////////
// Pile - interaction functions
////////////////////////////////////////////////////////////////////////////////

void Pile::transferCard(OrderedCard cardToTransfer, vector<int> newOrder) {
    // First, remove said card from its parent pile
    // Make sure its parent pile exists (otherwise we are adding this card from the void
    if (cardToTransfer.parent != nullptr) {
        cardToTransfer.parent->contents.erase(cardToTransfer);
    }
    
    // Now change the parent to point to the new pile
    cardToTransfer.parent = this;
    
    // And the new ordering of the card
    cardToTransfer.order = newOrder;
    
    // Now insert the card
    contents.insert(cardToTransfer);
}

void Pile::transferCard(OrderedCard cardToTransfer) {
    transferCard(cardToTransfer, {getHighestOrder()[0] + 1});
}

// TODO: Make shuffling part faster and also avoid shuffling in case that withShuffling == false
// TODO: Make numbers smaller for shuffled things (isn't really needed now unless you're playing things from deck)
void Pile::transferTo(Pile* otherPile, bool withShuffling) {
    set<pair<vector<int>, OrderedCard>> cardsToTransfer;
    for (auto it = contents.begin(); it != contents.end(); ++it) {
        vector<int> tempOrder = {otherPile->getHighestOrder()[0] + 1, rand()};
        
        cardsToTransfer.insert(make_pair(tempOrder, *it));
    }
    
    for (auto it = cardsToTransfer.begin(); it != cardsToTransfer.end(); ++it) {
        if (withShuffling) otherPile->transferCard(get<1>(*it), get<0>(*it));
        else otherPile->transferCard(get<1>(*it));
    }
}

////////////////////////////////////////////////////////////////////////////////
// Pile - interface functions
////////////////////////////////////////////////////////////////////////////////

string Pile::getPileString() {
    string pileString = "";
    
    for (auto it = contents.begin(); it != contents.end(); ++it) {
        pileString += it->card->name;
        pileString += ":";
        for (int i = 0; i < it->order.size(); ++i) {
            pileString += to_string(it->order[i]);
            pileString += ".";
        }
        pileString += ",";
    }
    
    return pileString;
}

////////////////////////////////////////////////////////////////////////////////
// PlayerState - interaction functions
////////////////////////////////////////////////////////////////////////////////

void PlayerState::reshuffle() {
    Pile* discard = cardsInDiscard;
    Pile* deck = cardsInDeck;
    
    discard->transferTo(deck, true);
    
    // TODO: Implement on-reshuffle properties (need to make sure we give them the choice of how to order the abilities)
    /*for (auto it = deck->contents.begin(); it != deck->contents.end(); ++it) {
     // Test if this was one of the just-shuffled cards
     if (it->order > tempHighestOrder) {
     
     
     it->card->onReshuffle(board_)
     }
     }*/
}

bool PlayerState::drawSingleCard() {
    Pile* hand = cardsInHand;
    Pile* deck = cardsInDeck;
    
    if (deck->contents.empty()) {
        reshuffle();
    }
    
    // If it's still empty, then there are no cards left to draw
    if (deck->contents.empty()) {
        return false;
    }
    
    OrderedCard drawnCard = *(deck->contents.begin());
    hand->transferCard(drawnCard);
    
    return true;
}

// TODO: Draw all cards at the same time and then resolve onDraw effects in order that player chooses
int PlayerState::drawCards(int numCards) {
    int numCardsDrawn = 0;
    for (int i = 0; i < numCards; ++i) {
        if (drawSingleCard()) ++numCardsDrawn;
    }
    return numCardsDrawn;
}

////////////////////////////////////////////////////////////////////////////////
// PlayerState - interface functions
////////////////////////////////////////////////////////////////////////////////

string PlayerState::getStateString(bool isCurrPlayer) {
    string stateString;
    
    if (isCurrPlayer) {
        stateString += cardsInPlay->getPileString();
        stateString += ";";
        stateString += cardsInHand->getPileString();
        stateString += ";";
    }
    else {
        stateString += to_string(cardsInPlay->contents.size());
        stateString += ";";
        stateString += to_string(cardsInHand->contents.size());
        stateString += ";";
    }
    
    stateString += to_string(cardsInDiscard->contents.size());
    stateString += ";";
    stateString += to_string(cardsInDeck->contents.size());
    stateString += ";";
    
    stateString += to_string(num_actions);
    stateString += ",";
    stateString += to_string(num_treasure);
    stateString += ",";
    stateString += to_string(num_buys);
    stateString += ",";
    stateString += to_string(num_villagers);
    stateString += ",";
    stateString += to_string(num_coffers);
    stateString += ",";
    stateString += to_string(num_victory_points);
    stateString += ",";
    
    return stateString;
}

////////////////////////////////////////////////////////////////////////////////
// BoardState - initialization functions
////////////////////////////////////////////////////////////////////////////////

void BoardState::initializeBoard() {
    vector<Card*> tempCardHolder;
    
    // Add coppers (remember to subtract starting coppers)
    for (int i = 0; i < 60 - 7*player_states.size(); ++i) {
        tempCardHolder.push_back(new Copper());
    }
    board.push_back(new Pile(tempCardHolder));
    tempCardHolder.clear();
    
    // Add silvers
    for (int i = 0; i < 40; ++i) {
        tempCardHolder.push_back(new Silver());
    }
    board.push_back(new Pile(tempCardHolder));
    tempCardHolder.clear();
    
    // Add gold
    for (int i = 0; i < 30; ++i) {
        tempCardHolder.push_back(new Gold());
    }
    board.push_back(new Pile(tempCardHolder));
    tempCardHolder.clear();
    
    // Add estates
    for (int i = 0; i < 12; ++i) {
        tempCardHolder.push_back(new Estate());
    }
    board.push_back(new Pile(tempCardHolder));
    tempCardHolder.clear();
    
    // Add duchies
    for (int i = 0; i < 12; ++i) {
        tempCardHolder.push_back(new Duchy());
    }
    board.push_back(new Pile(tempCardHolder));
    tempCardHolder.clear();
    
    // Add provinces
    for (int i = 0; i < 12; ++i) {
        tempCardHolder.push_back(new Province());
    }
    board.push_back(new Pile(tempCardHolder));
    tempCardHolder.clear();
    
    // Add curses
    for (int i = 0; i < 10*(player_states.size() - 1); ++i) {
        tempCardHolder.push_back(new Curse());
    }
    board.push_back(new Pile(tempCardHolder));
    tempCardHolder.clear();
}

////////////////////////////////////////////////////////////////////////////////
// BoardState - helper functions
////////////////////////////////////////////////////////////////////////////////

PlayerState* BoardState::getCurrPlayerState() {
    return player_states[curr_player];
}
Pile* BoardState::getCurrInPlay() {
    return getCurrPlayerState()->cardsInPlay;
}
Pile* BoardState::getCurrInHand() {
    return getCurrPlayerState()->cardsInHand;
}
Pile* BoardState::getCurrInDiscard() {
    return getCurrPlayerState()->cardsInDiscard;
}
Pile* BoardState::getCurrInDeck() {
    return getCurrPlayerState()->cardsInDeck;
}

void BoardState::reshuffle() {
    getCurrPlayerState()->reshuffle();
}

bool BoardState::drawSingleCard() {
    return getCurrPlayerState()->drawSingleCard();
}

int BoardState::drawCards(int numCards) {
    return getCurrPlayerState()->drawCards(numCards);
}

void BoardState::addActions(int numActionsToAdd) {
    getCurrPlayerState()->num_actions += numActionsToAdd;
}

void BoardState::setActions(int newActionsAmount) {
    getCurrPlayerState()->num_actions = newActionsAmount;
}

void BoardState::addTreasure(int numTreasureToAdd) {
    getCurrPlayerState()->num_treasure += numTreasureToAdd;
}

void BoardState::setTreasure(int newTreasureAmount) {
    getCurrPlayerState()->num_treasure = newTreasureAmount;
}

void BoardState::addBuys(int numBuysToAdd) {
    getCurrPlayerState()->num_buys += numBuysToAdd;
}

void BoardState::setBuys(int newBuyAmount) {
    getCurrPlayerState()->num_buys = newBuyAmount;
}

void BoardState::addVictoryPoints(int numVictoryPointsToAdd) {
    getCurrPlayerState()->num_victory_points += numVictoryPointsToAdd;
}

////////////////////////////////////////////////////////////////////////////////
// BoardState - event functions
////////////////////////////////////////////////////////////////////////////////

void BoardState::addEvent(Event* eventToAdd) {
    set<string> triggers = eventToAdd->triggerBuckets;
    
    for (auto it = triggers.begin(); it != triggers.end(); ++it) {
        events[*it].insert(eventToAdd);
    }
}

void BoardState::removeEvent(Event* eventToRemove) {
    set<string> triggers = eventToRemove->triggerBuckets;
    
    for (auto it = triggers.begin(); it != triggers.end(); ++it) {
        events[*it].erase(eventToRemove);
    }
}

void BoardState::triggerEvents(string trigger, vector<string> currCommand) {
    set<Event*> eventCandidates = events[trigger];
    
    vector<Event*> eventsTriggered;
    for (auto it = eventCandidates.begin(); it != eventCandidates.end(); ++it) {
        if ((*it)->checkIfTriggered(this, currCommand)) {
            eventsTriggered.push_back(*it);
        }
    }
    
    if (eventsTriggered.size() == 1) {
        triggeredEvents.push(eventsTriggered[0]);
    }
    else if (eventsTriggered.size() >= 2) {
        Event* resolveMultiEvents = new Event;
        resolveMultiEvents->eventTypeName = "resolve_multi_event";
        for (int i = 0; i < eventsTriggered.size(); ++i) {
            resolveMultiEvents->data.push_back(eventsTriggered[i]);
        }
        resolveMultiEvents->onCall = [=] (BoardState* board_state, vector<string> command) -> string {
            if (command.size() == 1) {
                return "ERROR: Must supply a valid index of an event to resolve first.";
            }
            
            // TODO: Make sure command[1] is an int
            // TODO: Make sure command[1] is in the proper range
            int eventIndexToResolve = stoi(command[1]);
            vector<void*>& listOfEvents = resolveMultiEvents->data;
            
            // Remove the event that's going to be resolved
            swap(listOfEvents[eventIndexToResolve], listOfEvents[listOfEvents.size() - 1]);
            listOfEvents.pop_back();
            
            // Now test if there's just one more event left and if so, pop this and add that event
            board_state->triggeredEvents.pop();
            board_state->triggeredEvents.push(static_cast<Event*>(listOfEvents[0]));
            
            // Now add the triggered event on top
            board_state->triggeredEvents.push(static_cast<Event*>(listOfEvents[eventIndexToResolve]));
            
            return "Successfully resolved multi-event.";
        };
        resolveMultiEvents->getEventString = [=] (BoardState* board_state) -> string {
            string eventString = "";
            
            eventString += "Event ID: " + to_string((unsigned long)resolveMultiEvents) + "\n";
            
            eventString += "Event of type multi-event with the following subevents:\n";
            for (int i = 0; i < resolveMultiEvents->data.size(); ++i) {
                eventString += "\t" + static_cast<Event*>(resolveMultiEvents->data[i])->getEventString(board_state) + "\n";
            }
            
            return eventString;
        };
        triggeredEvents.push(resolveMultiEvents);
    }
}

////////////////////////////////////////////////////////////////////////////////
// BoardState - interaction functions
////////////////////////////////////////////////////////////////////////////////

void BoardState::playCard(OrderedCard cardToPlay) {
    // First transfer the card into the play area
    Pile* play = getCurrInPlay();
    play->transferCard(cardToPlay);
    
    // Now activate its on play abilities
    cardToPlay.card->onPlay(this);
}

void BoardState::gainCard(OrderedCard cardToGain) {
    Pile* discard = getCurrInDiscard();
    
    // Add the card to the player's discard pile
    discard->transferCard(cardToGain);
    
    // Now activate the card's onGain effect (if it has any)
    cardToGain.card->onGain(this);
}

bool BoardState::canBuyCard(OrderedCard cardToBuy) {
    return getCurrTreasure() >= cardToBuy.card->cost && getCurrBuys() >= 1;
}

void BoardState::buyCard(OrderedCard cardToBuy) {
    addBuys(-1);
    addTreasure(cardToBuy.card->cost);
    
    // Now activate the card's onBuy effect (if it has any)
    cardToBuy.card->onBuy(this);
    
    gainCard(cardToBuy);
}

void BoardState::startTurn() {
    addActions(1);
    addBuys(1);
}

void BoardState::goToNextPlayer() {
    curr_player = (curr_player + 1) % player_states.size();
}

// TODO: Allow player to choose which card to put on top
// For now, I'm just going to make the top card invisible to see
// TODO: Add onDiscard effects
void BoardState::endTurn() {
    Pile* play = getCurrInPlay();
    Pile* hand = getCurrInHand();
    Pile* discard = getCurrInDiscard();
    
    play->transferTo(discard);
    hand->transferTo(discard);
    
    drawCards(5);
    
    setActions(0);
    setTreasure(0);
    setBuys(0);
    
    goToNextPlayer();
    
    curr_phase = STARTING_PHASE;
}

void BoardState::nextTurn() {
    endTurn();
    startTurn();
}

////////////////////////////////////////////////////////////////////////////////
// BoardState - interface functions
////////////////////////////////////////////////////////////////////////////////

string BoardState::getBoardString() {
    string boardString = "";
    
    for (int i = 0; i < board.size(); ++i) {
        boardString += board[i]->getPileString();
        boardString += ";";
    }
    
    return boardString;
}












































