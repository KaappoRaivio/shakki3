//
// Created by kaappo on 20.11.2021.
//


#pragma once

#include <string>
#include <ostream>
#include "Square.h"
#include "BitboardOperations.h"
#include "mytypes.h"

#define AMOUNT_OF_BOARDS 6


struct PieceSet {
    PieceColor color;
    Bitboard boards[AMOUNT_OF_BOARDS];
    Bitboard all;

    explicit PieceSet (const PieceColor& color);
    bool hasPiece (Square square) const;

    std::string& burn (std::string& buffer) const;
    friend std::ostream& operator<< (std::ostream& os, const PieceSet& set);

    Bitboard allWithoutKing () const;

    void updateOccupancy ();
};




