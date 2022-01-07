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
    uint64_t currentHash;


    BoardState (PieceColor turn, Move_raw previousMove, Piece capturedPiece, int plysSinceFiftyMoveReset, int wholeMoveCount, CastlingStatus castlingStatus, uint64_t currentHash);

    friend std::ostream& operator<< (std::ostream& os, const BoardState& state);
};

class BoardStateHistory {
private:
    std::stack<BoardState> states;

public:
    BoardStateHistory ();

    BoardState getCurrentFrame () const;
    BoardState& setCurrentFrame ();
    void pushState (BoardState newFrame);

    void createNewFrame ();
    const BoardState& popFrame ();
};

class Board;
namespace MoveGeneration {
    void addBishopMoves (std::vector<Move>& moves, const Board& context, PieceColor color, const Bitboard& checkMask, const Bitboard& pinMaskHV, const Bitboard& pinMaskD12, bool captureOnly);
    void addQueenMoves (std::vector<Move>& moves, const Board& context, PieceColor color, const Bitboard& checkMask, const Bitboard& pinMaskHV, const Bitboard& pinMaskD12, bool captureOnly);
    void addRookMoves (std::vector<Move>& moves, const Board& context, PieceColor color, const Bitboard& checkMask, const Bitboard& pinMaskHV, const Bitboard& pinMaskD12, bool captureOnly);
    void addKnightMoves (std::vector<Move>& moves, const Board& context, PieceColor color, const Bitboard& checkMask, const Bitboard& pinMask, bool captureOnly);
    void addPawnMoves (std::vector<Move>& moves, const Board& context, PieceColor color, const Bitboard& checkMask, const Bitboard& pinMaskHV, const Bitboard& pinMaskD12, bool captureOnly);
    void addKingMoves (std::vector<Move>& moves, const Board& context, PieceColor color, Bitboard attackMask, bool captureOnly);

}

