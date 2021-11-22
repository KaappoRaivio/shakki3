//
// Created by kaappo on 20.11.2021.
//


#pragma once

#include <string>
#include <ostream>
#include "Square.h"
#include "PieceColor.h"
#include "BitboardOperations.h"
#include "mytypes.h"


class PieceSet {
private:
    PieceColor color;
    Bitboard rooks;
    Bitboard knights;
    Bitboard bishops;
    Bitboard queen;
    Bitboard king;
    Bitboard pawns;

public:
    explicit PieceSet (const PieceColor& color);

    friend std::ostream& operator<< (std::ostream& os, const PieceSet& set);

    std::string& burn (std::string& buffer) const;
};




