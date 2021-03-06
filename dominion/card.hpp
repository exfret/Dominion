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
    Card() : name(""), cost(0), type(set<string>()) {}
    Card(string init_name, int init_cost, set<string> init_type) : name(init_name), cost(init_cost), type(init_type) {}
    
    string name;
    int cost;
    set<string> type;
    
    virtual void onPlay(BoardState* board_state) {}
    virtual void onTrash(BoardState* board_state) {}
    virtual void onGain(BoardState* board_state) {}
    virtual void onBuy(BoardState* board_state) {}
    // onDiscard not yet implemented
    virtual void onDiscard(BoardState* board_state) {}
    virtual void onReveal(BoardState* board_state) {}
    // The way this is evaluated is as follows:
    //   - We go in order from the player to the left of the player whome the game ended on clockwise
    //   - For each player's deck and each card in the deck, we call the onGameEnd function for that card
    //   - TODO: Give players choice about order to evaluate game-ending cards in
    void onGameEnd(BoardState* board_state) {}
};

struct Estate : Card {
    Estate() : Card("estate", 2, {"victory"}) {}
    
    virtual void onGameEnd(BoardState* board_state);
};

struct Duchy : Card {
    Duchy() : Card("duchy", 5, {"victory"}) {}
    
    virtual void onGameEnd(BoardState* board_state);
};

struct Province : Card {
    Province() : Card("province", 8, {"victory"}) {}
    
    virtual void onGameEnd(BoardState* board_state);
};

struct Copper : Card {
    Copper() : Card("copper", 0, {"treasure"}) {}
    
    virtual void onPlay(BoardState* board_state);
};

struct Silver : Card {
    Silver() : Card("silver", 3, {"treasure"}) {}
    
    virtual void onPlay(BoardState* board_state);
};

struct Gold : Card {
    Gold() : Card("gold", 6, {"treasure"}) {}
    
    virtual void onPlay(BoardState* board_state);
};

struct Curse : Card {
    Curse() : Card("curse", 0, {"curse"}) {}
    
    virtual void onGameEnd(BoardState* board_state);
};

struct Cellar : Card {
    Cellar() : Card("cellar", 2, {"action"}) {}
    
    virtual void onPlay(BoardState* board_state);
};

struct Laboratory : Card {
    Laboratory() : Card("laboratory", 5, {"action"}) {}
    
    virtual void onPlay(BoardState* board_state);
};

struct Village : Card {
    Village() : Card("village", 3, {"action"}) {}
    
    virtual void onPlay(BoardState* board_state);
};

#endif /* card_hpp */

















































