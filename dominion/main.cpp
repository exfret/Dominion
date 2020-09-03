//
//  main.cpp
//  dominion
//
//  Created by Kyle Hess on 8/19/20.
//  Copyright © 2020 Kyle Hess. All rights reserved.
//

/*
 
 TODO
   1) Finish implementing base game and base cards
     DONE A: Make function for transitioning to next player's turn
     DONE B: Make a function for buying cards
     DONE C: Make a function for playing cards
     D: Test for victory conditions
   DONE 2) Make interface for interacting with game
   DONE 3) Add types and names for cards
   DONE 4) Make ordered card use vector<int> as an ordering
   DONE 5) Allow viewing other player's info
   6) Add info on cards
   7) Add function to print out all information about game state
   8) Add a function for possible game choices
   9) Make some basic functionality for playing turns for the AI
     - Basically, handhold some part of the decision process for it
   10) Make a log
   11) Create multi-action capability in a single command so that there's less I/O needed
   12) Add undo functionality
   13) Add destructors (to prevent memory leaks)
   14) Make interface to play with bot in a real game and input what's happening in the computer
 
 IDEA: Split up AI into two AI's - a strategic one for buy phase and a tactical one for action phase?
 
 */

#include <iostream>
#include <string>
#include <vector>
#include "aihelper.hpp"
#include "card.hpp"
#include "event.hpp"
#include "states.hpp"
using namespace std;

// TODO: Make more checks for formatting so that Jacob's errors can be caught more easily
// TODO: Change if statement to case statement to speed things up if they are too slow
// TODO: Refactor this code into its own class
int main() {
    srand(RANDOM_SEED);
    
    vector<BoardState*> games;
    int currGame = 0;
    bool debugMode = false;
    
    while (true) {
        string input;
        getline(cin, input);
        
        vector<string> words;
        string tempWord;
        for (int i = 0; i < input.size(); ++i) {
            if (input[i] != ' ') tempWord += input[i];
            else {
                words.push_back(tempWord);
                tempWord = "";
            }
        }
        if (tempWord != "") words.push_back(tempWord);
        
        if (words.empty()) {
            cout << "ERROR: No command given" << endl;
            continue;
        }
        
        // TODO: Change the following massive if statement into a case statement?
        
        ////////////////////////////////////////////////////////////////////////////////
        // Help
        ////////////////////////////////////////////////////////////////////////////////
        
        if (words[0] == "help") {
            cout << "Just ask Kyle via facebook messenger since he's too lazy to type everything out right now." << endl;
            continue;
        }
        
        ////////////////////////////////////////////////////////////////////////////////
        // Game management
        ////////////////////////////////////////////////////////////////////////////////
        
        // Start a new, standard, big money game (and switch focus to that)
        if (words[0] == "newgame") {
            BoardState* newGame = new BoardState();
            games.push_back(newGame);
            
            currGame = games.size() - 1;
            
            cout << "New game successfully initialized!" << endl;
            continue;
        }
        
        if (words[0] == "changegame") {
            if (words.size() <= 1) {
                cout << "ERROR: Must specify which game to change to" << endl;
                continue;
            }
            
            // TODO: check if the supplied words[1] is actually an integer and supply error message if not
            // Also check that the game exists
            currGame = stoi(words[1]);
            
            cout << "Switched focus to game " << words[1] << endl;
            continue;
        }
        
        if (words[0] == "startgame") {
            if (games[currGame]->curr_phase != "start of game") {
                cout << "ERROR: Game is already started!" << endl;
                continue;
            }
            
            games[currGame]->curr_phase = STARTING_PHASE;
            games[currGame]->startTurn();
            cout << "Game successfully put into first player's " << STARTING_PHASE << " phase!" << endl;
            continue;
        }
        
        if (words[0] == "changemode") {
            if (words.size() <= 1) {
                cout << "ERROR: Must specify what mode setting you would like to change" << endl;
                continue;
            }
            
            if (words[1] == "debug") {
                if (words.size() <= 2) {
                    debugMode = !debugMode;
                    cout << "Successfully toggled debug mode to " << debugMode << "!" << endl;
                    continue;
                }
                
                if (words[2] == "true" || words[2] == "1") {
                    debugMode = true;
                    cout << "Successfully toggled debug mode to true!" << endl;
                    continue;
                }
                else if (words[2] == "false" || words[2] == "0") {
                    debugMode = false;
                    cout << "Successfully toggled debug mode to false!" << endl;
                    continue;
                }
                else {
                    cout << "ERROR: Debug mode can only be 'true' or 'false'." << endl;
                    continue;
                }
            }
            else {
                cout << "ERROR: Invalid mode setting name" << endl;
                continue;
            }
        }
        
        // At this point, all commands require a game to be accessed, so...
        // If we aren't looking at a proper game, return an error
        // TODO: Add case checking to give more specific errors?
        if (currGame < 0 || games.size() <= currGame) {
            cout << "ERROR: All the commands I have yet to check require the game in view to be created, but this is not the case. Either you mistyped a command or the game you are looking at hasn't been created." << endl;
            continue;
        }
        
        ////////////////////////////////////////////////////////////////////////////////
        // Viewing functions
        ////////////////////////////////////////////////////////////////////////////////
        
        // TODO: Make a prettier formatting mode
        // TODO: Allow viewing of other people's hands in debug mode
        // TODO: Allow viewing info about other aspects of the game, like current treasure
        // TODO: Make functions that return ALL information at once
        if (words[0] == "info") {
            if (words.size() <= 1) {
                cout << "ERROR: Must specify some type of info to retrieve." << endl;
                continue;
            }
            
            if (words[1] == "all") {
                // TODO: Wrap all this up in the board_states class
                cout << games[currGame]->getBoardString() << "|";
                cout << games[currGame]->curr_phase << "|";
                cout << games[currGame]->curr_player << "|";
                vector<PlayerState*> players = games[currGame]->player_states;
                for (int i = 0; i < players.size(); ++i) {
                    if (i == games[currGame]->curr_player) {
                        cout << players[i]->getStateString(true) << "|";
                    }
                    else {
                        cout << players[i]->getStateString(false) << "|";
                    }
                }
                cout << endl;
                continue;
            }
            else if (words[1] == "board") {
                // Returns list of board piles
                // Piles are separated by commas, and cards inside piles are separated by commas
                cout << games[currGame]->getBoardString() << endl;
                continue;
            }
            else if (words[1] == "choices") {
                set<pair<double, string>> choices = getWeightedChoices(games[currGame]);
                
                for (auto it = choices.begin(); it != choices.end(); ++it) {
                    cout << get<0>(*it) << ":" << get<1>(*it) << "|";
                }
                cout << endl;
                continue;
            }
            else if (words[1] == "events") {
                // TODO: Add a way to differentiate between when an event has multiple triggers and when there are
                // just multiple events
                // TODO: Add information about the event's data in here
                cout << "The following are inactive events:\n";
                for (auto it = games[currGame]->events.begin(); it != games[currGame]->events.end(); ++it) {
                    cout << "Trigger: " << get<0>(*it) << "\n";
                    for (auto it2 = get<1>(*it).begin(); it2 != get<1>(*it).end(); ++it2) {
                        cout << "\t" << (*it2)->getEventString(games[currGame]);
                    }
                }
                
                cout << "The following are active events:\n";
                // Copy by reference so that popping won't actually pop the the triggered events
                stack<Event*> triggeredEvents = games[currGame]->triggeredEvents;
                while (!triggeredEvents.empty()) {
                    cout << triggeredEvents.top()->getEventString(games[currGame]);
                    triggeredEvents.pop();
                }
                cout << endl;
                continue;
            }
            else if (words[1] == "phase") {
                cout << games[currGame]->curr_phase << endl;
                continue;
            }
            else if (words[1] == "player") {
                if (words.size() <= 2) {
                    cout << "ERROR: Must specify some type of player info to retrieve." << endl;
                    continue;
                }
                
                // See if the player to view has been specified
                PlayerState* player = nullptr;
                if (words.size() <= 3) {
                    player = games[currGame]->getCurrPlayerState();
                }
                else {
                    player = games[currGame]->player_states[stoi(words[3])];
                }
                
                // See if the player is requesting limited information
                // TODO: Test for what limitedInfo should be in a smart manner
                // i.e.- if debug mode is off and we are viewing the deck and limitedInfo is not specified,
                // then automatically set limitedInfo to true
                // TODO: Also return the top card of the discard in limitedInfo mode once I get that fixed up
                bool limitedInfo = false;
                if (words.size() >= 5) {
                    if (words[4] == "true" || words[4] == "1") {
                        limitedInfo = true;
                    }
                    else if (words[4] == "false" || words[4] == "1") {
                        limitedInfo = false;
                    }
                    else {
                        cout << "ERROR: limitedInfo setting must be 'true' or 'false'." << endl;
                        continue;
                    }
                }
                
                if (words[2] == "play") {
                    cout << player->cardsInPlay->getPileString() << endl;
                    continue;
                }
                else if (words[2] == "hand") {
                    cout << player->cardsInHand->getPileString() << endl;
                    continue;
                }
                else if (words[2] == "discard") {
                    if (limitedInfo) {
                        cout << player->cardsInDiscard->contents.size() << endl;
                        continue;
                    }
                    else if (debugMode) {
                        cout << player->cardsInDiscard->getPileString() << endl;
                        continue;
                    }
                    else {
                        cout << "ERROR: Looking through your discard pile requires debug mode to be on." << endl;
                        continue;
                    }
                }
                else if (words[2] == "deck") {
                    if (limitedInfo) {
                        cout << player->cardsInDeck->contents.size() << endl;
                        continue;
                    }
                    else if (debugMode) {
                        cout << player->cardsInDeck->getPileString() << endl;
                        continue;
                    }
                    else {
                        cout << "ERROR: Looking through your deck requires debug mode to be on." << endl;
                        continue;
                    }
                }
                else if (words[2] == "actions") {
                    cout << player->num_actions << endl;
                    continue;
                }
                else if (words[2] == "treasure") {
                    cout << player->num_treasure << endl;
                    continue;
                }
                else if (words[2] == "buys") {
                    cout << player->num_buys << endl;
                    continue;
                }
                else {
                    cout << "ERROR: That's not a valid type of player information to get!" << endl;
                    continue;
                }
            }
            else {
                cout << "ERROR: That's not a valid parameter to get information on!" << endl;
                continue;
            }
        }
        
        if (!debugMode && (games[currGame]->curr_phase == "end of game" || games[currGame]->curr_phase == "start of game")) {
            cout << "ERROR: All the commands I have yet to check require either debug mode to be on or the current game to be in progress, but neither of these conditions are true. Either you mistyped a command or the command you are trying to use can't be used in this circumstance." << endl;
            continue;
        }
        
        ////////////////////////////////////////////////////////////////////////////////
        // Event resolution functions
        ////////////////////////////////////////////////////////////////////////////////
        
        if (words[0] == "resolve") {
            if (games[currGame]->triggeredEvents.empty()) {
                cout << "ERROR: No events to resolve." << endl;
                continue;
            }
            
            Event* nextEvent = games[currGame]->triggeredEvents.top();
            cout << "Attempting to resolve event of type " << nextEvent->eventTypeName << "...\n";
            cout << nextEvent->onCall(games[currGame], words) << endl;
            continue;
        }
        
        // If there are events to resolve and there was no attempt to resolve them, spit an error and continue
        if (!debugMode && !games[currGame]->triggeredEvents.empty()) {
            cout << "ERROR: Interaction functions cannot be used when there is an event to resolve!" << endl;
            continue;
        }
        
        ////////////////////////////////////////////////////////////////////////////////
        // Interacting functions
        ////////////////////////////////////////////////////////////////////////////////
        
        // TODO: allow playing of other player's cards in debug mode
        if (words[0] == "play") {
            if (words.size() <= 1) {
                cout << "ERROR: Must supply the id of a card to play." << endl;
                continue;
            }
            
            // Now parse the input vector specifying what to play
            vector<int> cardIndex;
            string tempNumber = "";
            for (int i = 0; i < words[1].size(); ++i) {
                if (words[1][i] == '.') {
                    // TODO: Check case that supplied value is not an int
                    // Make sure this isn't the last decimal point
                    if (tempNumber != "") cardIndex.push_back(stoi(tempNumber));
                    tempNumber = "";
                }
                else {
                    tempNumber += words[1][i];
                }
            }
            // Allow for ommitting the last decimal point
            if (tempNumber != "") cardIndex.push_back(stoi(tempNumber));
            
            // TODO: Add a Pile member function to get cards with a given id
            OrderedCard tempOrderedCard(nullptr, cardIndex, nullptr);
            set<OrderedCard> hand = games[currGame]->getCurrInHand()->contents;
            auto cardToPlay = hand.lower_bound(tempOrderedCard);
            
            if (cardToPlay == hand.end()) {
                cout << "ERROR: The selected card does not exists; please input a value less than or equal to a card's ID to play it." << endl;
                continue;
            }
            
            // Get the types of the given card
            set<string> types = cardToPlay->card->type;
            
            // Make sure the type of the card matches the phase
            // Also check that the player has actions left if needed
            if (!debugMode && games[currGame]->curr_phase == "action" && types.find("action") == types.end()) {
                cout << "ERROR: Debug mode is off and you are attempting to play a non-action card in the action phase!" << endl;
                continue;
            }
            else if (!debugMode && games[currGame]->curr_phase == "action" && games[currGame]->getCurrActions() == 0) {
                cout << "ERROR: Debug mode is off and you are attempting to play an action card without an action!" << endl;
                continue;
            }
            else if (!debugMode && games[currGame]->curr_phase == "buy - play treasures" && types.find("treasure") == types.end()) {
                cout << "ERROR: Debug mode is off and you are attempting to play a non-treasure card in the beginning of the buy phase!" << endl;
                continue;
            }
            
            // First, add playing this card to the event stack
            Event* playCardEvent = new Event;
            playCardEvent->eventTypeName = "play_card_" + cardToPlay->card->name;
            // TODO: Figure out if there's a better way to do the following
            // I have to dereference cardToPlay since the only way to add a pointer from an iterator is to dereference
            // the iterator, but I couldn't figure out how to make a non-const pointer to the dereferenced value without
            // implicitly copying by value. Therefore, I'm just going the copy-by-value route, since it won't matter for
            // OrderedCard anyways as its important member variables are just pointers
            // (this wouldn't work for other classes)
            playCardEvent->data.push_back(new OrderedCard(*cardToPlay));
            playCardEvent->onCall = [=] (BoardState* board_state, vector<string> commands) -> string {
                // We could just specify to play the card cardToPlay, but I want to be able to easily get access
                // to the information of what card is going to play later, so I'm storing it in this event's data
                OrderedCard* playCardEventCardToPlay = static_cast<OrderedCard*>(playCardEvent->data[0]);
                games[currGame]->playCard(*playCardEventCardToPlay);
                // Remember to have this event remove itself from the active event stack
                games[currGame]->triggeredEvents.pop();
                return "Successfully played a " + playCardEventCardToPlay->card->name;
            };
            playCardEvent->getEventString = [=] (BoardState* board_state) -> string {
                string eventString = "";
                
                eventString += "Event ID: " + to_string((unsigned long)playCardEvent) + "\n";
                eventString += "Event of type play " + cardToPlay->card->name + "\n";
                
                return eventString;
            };
            games[currGame]->triggeredEvents.push(playCardEvent);
            
            // Then, trigger all events that happen when you play a card
            // (These will happen first since they are actually higher in the stack from being put in later)
            games[currGame]->triggerEvents(words[0], words);
            
            // Inform the player of
            cout << "Successfully added playing a " << cardToPlay->card->name << " to the event queue." << endl;
            continue;
        }
        
        
        if (words[0] == "buy") {
            if (words.size() <= 1) {
                cout << "ERROR: Need to specify which pile to buy from!" << endl;
                continue;
            }
            
            // Make sure we are in the buy phase
            if (!debugMode && games[currGame]->curr_phase != "buy - buy cards") {
                cout << "ERROR: Not in debug mode and attempting to buy cards in phase other than buy - buy cards phase." << endl;
                continue;
            }
            
            // TODO: Test if second argument is indeed an int
            int pileNumberToBuyFrom = stoi(words[1]);
            if (pileNumberToBuyFrom < 0 || games[currGame]->board.size() <= pileNumberToBuyFrom) {
                cout << "ERROR: That pile does not exist" << endl;
                continue;
            }
            
            Pile* pileToBuyFrom = games[currGame]->board[stoi(words[1])];
            
            // Test if the pile is empty
            if (pileToBuyFrom->contents.empty()) {
                cout << "ERROR: Pile is empty!" << endl;
                continue;
            }
            
            OrderedCard cardToBuy = *pileToBuyFrom->contents.rbegin();
            
            // Test if there are enough buys
            if (!debugMode && games[currGame]->getCurrBuys() < 1) {
                cout << "ERROR: You do not have enough buys and are not in debug mode." << endl;
                continue;
            }
            
            // Test if the player has enough money
            if (!debugMode && games[currGame]->getCurrTreasure() < cardToBuy.card->cost) {
                cout << "ERROR: You do not have enough treasure and are not in debug mode." << endl;
                continue;
            }
            
            // First, add buying this card to the event stack
            Event* buyCardEvent = new Event;
            buyCardEvent->eventTypeName = "buy_card_" + cardToBuy.card->name;
            buyCardEvent->data.push_back(new OrderedCard(cardToBuy));
            buyCardEvent->onCall = [=] (BoardState* board_state, vector<string> command) -> string {
                OrderedCard* buyCardEventCardToBuy = static_cast<OrderedCard*>(buyCardEvent->data[0]);
                // TODO: Don't subtract money and buys and such during debug mode
                games[currGame]->buyCard(*buyCardEventCardToBuy);
                games[currGame]->triggeredEvents.pop();
                return "Successfully bought a " + buyCardEventCardToBuy->card->name;
            };
            buyCardEvent->getEventString = [=] (BoardState* board_state) -> string {
                string eventString = "";
                
                eventString += "Event ID: " + to_string((unsigned long)buyCardEvent) + "\n";
                eventString += "Event of typ buy " + cardToBuy.card->name + "\n";
                
                return eventString;
            };
            games[currGame]->triggeredEvents.push(buyCardEvent);
            
            // Trigger cards that trigger on buy
            games[currGame]->triggerEvents(words[0], words);
            
            cout << "Successfully added playing a " << cardToBuy.card->name << " to the event stack." << endl;
            continue;
        }
        
        // A quick way to go to the next phase/turn
        // TODO: Test for game end conditions when doing this and alert the player if this happens
        // TODO: Wrap the "go to next phase" function inside the board state struct
        if (words[0] == "next") {
            // First, trigger events that happen at the start of a phase
            games[currGame]->triggerEvents(words[0], words);
            
            // Use a reference so that we can assign the value
            string& phase = games[currGame]->curr_phase;
            
            if (phase == "action") {
                phase = "buy - play treasures";
                cout << "Successfully switched from 'action' phase to 'buy - play treasures' phase" << endl;
                continue;
            }
            else if (phase == "buy - play treasures") {
                phase = "buy - buy cards";
                cout << "Successfully switched from 'buy - play treasures' phase to 'buy - buy cards' phase" << endl;
                continue;
            }
            else if (phase == "buy - buy cards") {
                games[currGame]->nextTurn();
                cout << "Successfully ended turn" << endl;
                continue;
            }
            
            cout << "ERROR: Somehow your phase has left the bounds of reality...." << endl;
            continue;
        }
        
        cout << "Unknown command (type 'help' for a list of commands)" << endl;
        continue;
    }
}















































