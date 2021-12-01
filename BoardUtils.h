//
// Created by kaappo on 30.11.2021.
//


#pragma once


#include <stack>
#include "mytypes.h"
#include "Move.h"

class Move;

struct BoardState {
    int plysSinceFiftyMoveReset;
    PieceColor turn;
    Move& previousMove;

    BoardState (PieceColor turn, Move& previousMove, int plysSinceFiftyMoveReset);
};

class BoardStateHistory {
private:
    std::stack<BoardState> states;


public:
    BoardStateHistory ();

    const BoardState& getCurrentFrame () const;
    BoardState& setCurrentFrame ();
    void createNewFrame ();
    void popFrame ();
};


