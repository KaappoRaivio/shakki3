//
// Created by kaappo on 20.11.2021.
//


#pragma once

#include <cstdint>
#include <ostream>
#include <sstream>
#include <memory>
#include "PieceSet.h"
#include "Move.h"
#include "BoardUtils.h"

class Move;
class BoardStateHistory;

class Board {
public:
    Board ();

    PieceSet getPieceSet (PieceColor color) const;
    friend std::ostream& operator<< (std::ostream& os, const Board& board);

    PieceColor getColorAt (Square square) const;
    bool is(PieceType type, Square at) const;

    void executeMove (const Move& move);
    void unmakeMove ();

    PieceColor getTurn () const;

    std::vector<Move> getMoves () const;

    const Piece& getPieceAt (Square square) const;
    const PieceSet* getPieces () const;

private:
    std::unique_ptr<Piece> letterbox[8 * 8];
    PieceSet pieces[2];
    std::unique_ptr<BoardStateHistory> history;

    void initializeLetterbox ();

    const PieceSet& getWhite () const;
    const PieceSet& getBlack () const;

};


