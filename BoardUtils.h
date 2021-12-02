//
// Created by kaappo on 30.11.2021.
//


#pragma once


#include <stack>
#include <ostream>
#include "mytypes.h"
#include "Move.h"

class Move;

struct BoardState {
    int plysSinceFiftyMoveReset;
    PieceColor turn;
    Move_raw previousMove;
    Piece capturedPiece;

    BoardState (PieceColor turn, Move_raw previousMove, Piece capturedPiece, int plysSinceFiftyMoveReset);

    friend std::ostream& operator<< (std::ostream& os, const BoardState& state);
};

class BoardStateHistory {
private:
    std::stack<BoardState> states;

public:
    BoardStateHistory ();

    const BoardState& getCurrentFrame () const;
    void pushState (BoardState newFrame);

    void createNewFrame ();
    const BoardState& popFrame ();
};

class Board;
namespace MoveGeneration {
    void addBishopMoves (std::vector<Move>& moves, const Board& context, PieceColor color);
    void addRookMoves (std::vector<Move>& moves, const Board& context, PieceColor color);
    void addKnightMoves (std::vector<Move>& moves, const Board& context, PieceColor color);
}