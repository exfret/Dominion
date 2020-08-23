//
//  card.cpp
//  dominion
//
//  Created by Kyle Hess on 8/19/20.
//  Copyright © 2020 Kyle Hess. All rights reserved.
//

#include "card.hpp"
#include "states.hpp"

void Estate::onGameEnd(BoardState* board_state) {
    board_state->addVictoryPoints(1);
}

void Duchy::onGameEnd(BoardState* board_state) {
    board_state->addVictoryPoints(3);
}

void Province::onGameEnd(BoardState* board_state) {
    board_state->addVictoryPoints(6);
}

void Copper::onPlay(BoardState* board_state) {
    board_state->addTreasure(1);
}

void Silver::onPlay(BoardState* board_state) {
    board_state->addTreasure(2);
}

void Gold::onPlay(BoardState* board_state) {
    board_state->addTreasure(3);
}

void Curse::onGameEnd(BoardState* board_state) {
    
}

void Village::onPlay(BoardState* board_state) {
    board_state->drawCards(1);
    board_state->addActions(2);
}

void Laboratory::onPlay(BoardState* board_state) {
    board_state->drawCards(2);
    board_state->addActions(1);
}














































