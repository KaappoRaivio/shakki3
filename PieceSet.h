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

    bool hasPiece (Square square) const;

    explicit PieceSet (const PieceColor& color);

    friend std::ostream& operator<< (std::ostream& os, const PieceSet& set);

    std::string& burn (std::string& buffer) const;

    void updateOccupancy ();
};




