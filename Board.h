//
// Created by kaappo on 20.11.2021.
//


#pragma once

#include <cstdint>
#include <ostream>
#include <sstream>
#include "PieceSet.h"


class Board {
public:
    Board ();

    PieceSet getPieceSet (PieceColor color) const;

    friend std::ostream& operator<< (std::ostream& os, const Board& board);

    PieceColor getColorAt (Square square) const;
    bool is(PieceType type, Square at) const;

private:
    const PieceSet pieces[2];
//    const PieceSet white;
//    const PieceSet black;

    const PieceSet& getWhite () const;

    const PieceSet& getBlack () const;
};


