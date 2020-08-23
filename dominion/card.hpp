//
//  card.hpp
//  dominion
//
//  Created by Kyle Hess on 8/19/20.
//  Copyright © 2020 Kyle Hess. All rights reserved.
//

#ifndef card_hpp
#define card_hpp

#include <set>
#include <string>
using namespace std;

struct BoardState;

struct Card {
    string name;
    int cost;
    set<string> type;
    
    void onPlay(BoardState* board_state) {}
    void onTrash(BoardState* board_state) {}
    void onGain(BoardState* board_state) {}
    void onBuy(BoardState* board_state) {}
    void onDiscard(BoardState* board_state) {}
    void onReveal(BoardState* board_state) {}
    // The way this is evaluated is as follows:
    //   - We go in order from the player to the left of the player whome the game ended on clockwise
    //   - For each player's deck and each card in the deck, we call the onGameEnd function for that card
    //   - TODO: Give players choice about order to evaluate game-ending cards in
    void onGameEnd(BoardState* board_state) {}
};

struct Estate : Card {
    void onGameEnd(BoardState* board_state);
};

struct Duchy : Card {
    void onGameEnd(BoardState* board_state);
};

struct Province : Card {
    void onGameEnd(BoardState* board_state);
};

struct Copper : Card {
    void onPlay(BoardState* board_state);
};

struct Silver : Card {
    void onPlay(BoardState* board_state);
};

struct Gold : Card {
    void onPlay(BoardState* board_state);
};

struct Curse : Card {
    void onGameEnd(BoardState* board_state);
};

struct Laboratory : Card {
    void onPlay(BoardState* board_state);
};

struct Village : Card {
    void onPlay(BoardState* board_state);
};

#endif /* card_hpp */

















































