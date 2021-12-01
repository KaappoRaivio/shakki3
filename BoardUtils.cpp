//
// Created by kaappo on 30.11.2021.
//

#include "BoardUtils.h"
//#include "Board.h"

BoardState::BoardState (PieceColor turn, Move& previousMove, int plysSinceFiftyMoveReset)
                        : plysSinceFiftyMoveReset{plysSinceFiftyMoveReset}, turn{turn}, previousMove{previousMove} {}

BoardStateHistory::BoardStateHistory () : states{} {
    createNewFrame();
}

void BoardStateHistory::createNewFrame () {
    if (states.empty()) {
        states.push(BoardState{WHITE, Moves::NO_MOVE, 0});
    } else {
        states.push(BoardState{states.top()});
    }
}

const BoardState& BoardStateHistory::getCurrentFrame () const {
//    std::cout << "Getting current frame: " << states.size() << std::endl;
    return states.top();
}

void BoardStateHistory::popFrame () {
    states.pop();
}

BoardState& BoardStateHistory::setCurrentFrame () {
    return states.top();
}
