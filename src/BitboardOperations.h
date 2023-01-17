//
// Created by kaappo on 22.11.2021.
//

#pragma once

#include <cstdint>
#include <string>
#include "mytypes.h"
#include "Bitboard.h"


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

    int8_t rayDirectionToShift (RayDirection d, PieceColor perspective);

//    Bitboard_raw move (Bitboard_raw b, RayDirection direction);
//    Bitboard_raw move (Bitboard_raw b, RayDirection direction, uint8_t amount);
//    Bitboard_raw move (Bitboard_raw b, RayDirection direction, uint8_t amount, bool wrapAround);
    Bitboard_raw shift(Bitboard_raw b, int shift);

    Bitboard_raw flipVertical (Bitboard_raw x);

    std::string& burnBitboard (std::string& buffer, Bitboard_raw pieces, char legend);

    std::string bitboardToString (Bitboard_raw b);


    namespace SquareMasks {
        extern Bitboard fileA;
        extern Bitboard fileB;
        extern Bitboard fileC;
        extern Bitboard fileD;
        extern Bitboard fileE;
        extern Bitboard fileF;
        extern Bitboard fileG;
        extern Bitboard fileH;

        extern Bitboard files[8];

        extern Bitboard rank1;
        extern Bitboard rank2;
        extern Bitboard rank3;
        extern Bitboard rank4;
        extern Bitboard rank5;
        extern Bitboard rank6;
        extern Bitboard rank7;
        extern Bitboard rank8;

        extern Bitboard ranks[8];

        extern Bitboard a1h8Diagonal;
        extern Bitboard h1a8AntiDiagonal;
        extern Bitboard lightSquares;
        extern Bitboard darkSquares;
    }
}