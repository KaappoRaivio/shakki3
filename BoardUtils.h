//
// Created by kaappo on 30.11.2021.
//


#pragma once


#include <stack>
#include <ostream>
#include <bitset>
#include "mytypes.h"
#include "Move.h"
#include "Bitboard.h"
#include "CastlingStatus.h"


struct BoardState {
    int plysSinceFiftyMoveReset;
    int fullMoveCount;
    PieceColor turn;
    Move_raw previousMove;
    Piece capturedPiece;
    CastlingStatus castlingStatus;


    BoardState (PieceColor turn, Move_raw previousMove, Piece capturedPiece, int plysSinceFiftyMoveReset, int wholeMoveCount, CastlingStatus castlingStatus);

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
    void addBishopMoves (std::vector<Move>& moves, const Board& context, PieceColor color, const Bitboard& checkMask, const Bitboard& pinMaskHV, const Bitboard& pinMaskD12);
    void addQueenMoves (std::vector<Move>& moves, const Board& context, PieceColor color, const Bitboard& checkMask, const Bitboard& pinMaskHV, const Bitboard& pinMaskD12);
    void addRookMoves (std::vector<Move>& moves, const Board& context, PieceColor color, const Bitboard& checkMask, const Bitboard& pinMaskHV, const Bitboard& pinMaskD12);
    void addKnightMoves (std::vector<Move>& moves, const Board& context, PieceColor color, const Bitboard& checkMask, const Bitboard& pinMask);
    void addPawnMoves (std::vector<Move>& moves, const Board& context, PieceColor color, const Bitboard& checkMask, const Bitboard& pinMaskHV, const Bitboard& pinMaskD12);
    void addKingMoves (std::vector<Move>& moves, const Board& context, PieceColor color, Bitboard attackMask);

}

