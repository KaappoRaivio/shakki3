//
// Created by kaappo on 22.11.2021.
//
#include <string>
#include <stdexcept>
#include "BitboardOperations.h"

using namespace BitboardOperations::Directions;

std::string& BitboardOperations::burnBitboard (std::string& buffer, Bitboard pieces, char legend) {
    while (pieces != 0) {
        buffer[__builtin_ctzl(pieces)] = legend;
        pieces &= pieces - 1;
    }

    return buffer;
}

std::string BitboardOperations::bitboardToString (Bitboard b) {
    std::string buffer{"................................................................"};
    BitboardOperations::burnBitboard(buffer, b, '1');
    std::string result;
    result.append(buffer.substr(56, 8));
    result.append(buffer.substr(48, 8));
    result.append(buffer.substr(40, 8));
    result.append(buffer.substr(32, 8));
    result.append(buffer.substr(24, 8));
    result.append(buffer.substr(16, 8));
    result.append(buffer.substr(8, 8));
    result.append(buffer.substr(0, 8));

    result.insert(0, "\n");
    result.insert(9, "\n");
    result.insert(18, "\n");
    result.insert(27, "\n");
    result.insert(36, "\n");
    result.insert(45, "\n");
    result.insert(54, "\n");
    result.insert(63, "\n");

    for (int i = 0; i < result.size(); i += 2) {
        result.insert(i, " ");
    }

    return result;
}

Bitboard BitboardOperations::flipVertical (Bitboard x) {
    return  ( (x << 56)                           ) |
            ( (x << 40) & (Bitboard)(0x00ff000000000000) ) |
            ( (x << 24) & (Bitboard)(0x0000ff0000000000) ) |
            ( (x <<  8) & (Bitboard)(0x000000ff00000000) ) |
            ( (x >>  8) & (Bitboard)(0x00000000ff000000) ) |
            ( (x >> 24) & (Bitboard)(0x0000000000ff0000) ) |
            ( (x >> 40) & (Bitboard)(0x000000000000ff00) ) |
            ( (x >> 56) );
}

Bitboard BitboardOperations::move (Bitboard b, ::RayDirection direction) {
    int8_t shift = rayDirectionToShift(direction);
    if (shift == 0) {
        return b;
    } else if (shift > 0) {
        return b << shift;
    } else {
        return b >> -shift;
    }
}

int8_t BitboardOperations::rayDirectionToShift (::RayDirection d) {
    switch (d) {
        case NORTH:
            return 8;
        case NORTH_EAST:
            return 9;
        case EAST:
            return 1;
        case SOUTH_EAST:
            return -7;
        case SOUTH:
            return -8;
        case SOUTH_WEST:
            return -9;
        case WEST:
            return -1;
        case NORTH_WEST:
            return 7;

        default:
            throw std::runtime_error("Shouldn't happen");

    }
}

namespace BitboardOperations::Directions {
//    uint8_t NORTH = 0;
//    uint8_t EAST = 1;
//    uint8_t SOUTH = 2;
//    uint8_t WEST = 3;
//    uint8_t NORTH_EAST = 4;
//    uint8_t SOUTH_EAST = 5;
//    uint8_t SOUTH_WEST = 6;
//    uint8_t NORTH_WEST = 7;
};

namespace BitboardOperations::SquareMasks {
        Bitboard aFile            = 0x0101010101010101;
        Bitboard hFile            = 0x8080808080808080;
        Bitboard firstRank        = 0x00000000000000FF;
        Bitboard eighthRank       = 0xFF00000000000000;
        Bitboard a1h8Diagonal     = 0x8040201008040201;
        Bitboard h1a8AntiDiagonal = 0x0102040810204080;
        Bitboard lightSquares     = 0x55AA55AA55AA55AA;
        Bitboard darkSquares      = 0xAA55AA55AA55AA55;
    }
