//
// Created by kaappo on 30.11.2021.
//

#include "BoardUtils.h"
//#include "Board.h"

BoardState::BoardState (PieceColor turn, Move_raw previousMove, Piece capturedPiece, int plysSinceFiftyMoveReset)
                        : plysSinceFiftyMoveReset{plysSinceFiftyMoveReset}, turn{turn}, previousMove{previousMove}, capturedPiece(capturedPiece) {}

std::ostream& operator<< (std::ostream& os, const BoardState& state) {
    os << "BoardState{plysSinceFiftyMoveReset: " << state.plysSinceFiftyMoveReset << " turn: " << state.turn << " previousMove: " << state.previousMove << " capturedPiece: " << state.capturedPiece << "}";
    return os;
}

BoardStateHistory::BoardStateHistory () : states{} {
    createNewFrame();
}

void BoardStateHistory::createNewFrame () {
    if (states.empty()) {
        states.push(BoardState{WHITE, Moves::NO_MOVE.raw(), Pieces::NO_PIECE, 0});
    } else {
        states.push(BoardState{states.top()});
    }
}

const BoardState& BoardStateHistory::getCurrentFrame () const {
    return states.top();
}

const BoardState& BoardStateHistory::popFrame () {
    const BoardState& state = states.top();
    states.pop();
    return state;
}

void BoardStateHistory::pushState (BoardState newFrame) {
    states.push(newFrame);
}