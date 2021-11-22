//
// Created by kaappo on 22.11.2021.
//

#pragma once

#include <cstdint>
#include "mytypes.h"


namespace BitboardOperations {
//    typedef uint8_t RayDirection;
    namespace Directions {
        constexpr uint8_t NORTH = 0;
        constexpr uint8_t EAST = 1;
        constexpr uint8_t SOUTH = 2;
        constexpr uint8_t WEST = 3;
        constexpr uint8_t NORTH_EAST = 4;
        constexpr uint8_t SOUTH_EAST = 5;
        constexpr uint8_t SOUTH_WEST = 6;
        constexpr uint8_t NORTH_WEST = 7;
    };

    int8_t rayDirectionToShift (::RayDirection d);

    Bitboard move (Bitboard b, ::RayDirection direction);

    Bitboard flipVertical (Bitboard x);

    std::string& burnBitboard (std::string& buffer, Bitboard pieces, char legend);

    std::string bitboardToString (Bitboard b);


    namespace SquareMasks {
        extern Bitboard aFile;
        extern Bitboard hFile;
        extern Bitboard firstRank;
        extern Bitboard eighthRank;
        extern Bitboard a1h8Diagonal;
        extern Bitboard h1a8AntiDiagonal;
        extern Bitboard lightSquares;
        extern Bitboard darkSquares;
    }
}