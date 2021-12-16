//
// Created by kaappo on 22.11.2021.
//
#include <string>
#include <stdexcept>
#include "BitboardOperations.h"

using namespace BitboardOperations::Directions;

std::string& BitboardOperations::burnBitboard (std::string& buffer, Bitboard_raw pieces, char legend) {
    while (pieces != 0) {
        buffer[__builtin_ctzl(pieces)] = legend;
        pieces &= pieces - 1;
    }

    return buffer;
}

std::string BitboardOperations::bitboardToString (Bitboard_raw b) {
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

    for (size_t i = 0; i < result.size(); i += 2) {
        result.insert(i, " ");
    }

    return result;
}

Bitboard_raw BitboardOperations::flipVertical (Bitboard_raw x) {
    return ((x << 56)) |
           ((x << 40) & (Bitboard_raw) (0x00ff000000000000)) |
           ((x << 24) & (Bitboard_raw) (0x0000ff0000000000)) |
           ((x << 8) & (Bitboard_raw) (0x000000ff00000000)) |
           ((x >> 8) & (Bitboard_raw) (0x00000000ff000000)) |
           ((x >> 24) & (Bitboard_raw) (0x0000000000ff0000)) |
           ((x >> 40) & (Bitboard_raw) (0x000000000000ff00)) |
           ((x >> 56));
}


Bitboard_raw BitboardOperations::shift (Bitboard_raw b, int shift) {
    if (shift == 0) {
        return b;
    } else if (shift > 0) {
        return b << shift;
    } else {
        return b >> -shift;
    }
}

//Bitboard_raw BitboardOperations::move (Bitboard_raw b, RayDirection direction) {
//    return move(b, direction, 1);
//}
//
//Bitboard_raw BitboardOperations::move(Bitboard_raw b, RayDirection direction, uint8_t amount) {
//    return move(b, direction, amount, true);
//}
//
//
//Bitboard_raw BitboardOperations::move (Bitboard_raw b, RayDirection direction, uint8_t amount, bool wrapAround) {
//
//}

int8_t BitboardOperations::rayDirectionToShift (RayDirection d, PieceColor perspective) {
    int8_t shift;
    switch (d) {
        case NORTH:
            shift = 8;
            break;
        case NORTH_EAST:
            shift = 9;
            break;
        case EAST:
            shift = 1;
            break;
        case SOUTH_EAST:
            shift = -7;
            break;
        case SOUTH:
            shift = -8;
            break;
        case SOUTH_WEST:
            shift = -9;
            break;
        case WEST:
            shift = -1;
            break;
        case NORTH_WEST:
            shift = 7;
            break;
        default:
            throw std::runtime_error("Shouldn't happen");
    }

    if (perspective == WHITE) return shift;
    else if (perspective == BLACK) return -shift;
    else {
        throw std::runtime_error("Invalid color!");
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
    Bitboard fileA = 0x0101010101010101;
    Bitboard fileB = 0x0202020202020202;
    Bitboard fileC = 0x0404040404040404;
    Bitboard fileD = 0x0808080808080808;
    Bitboard fileE = 0x1010101010101010;
    Bitboard fileF = 0x2020202020202020;
    Bitboard fileG = 0x4040404040404040;
    Bitboard fileH = 0x8080808080808080;

    Bitboard files[8] = {
            fileA, fileB, fileC, fileD, fileE, fileF, fileG, fileH
    };

    Bitboard rank1 = 0x00000000000000FF;
    Bitboard rank2 = 0x000000000000FF00;
    Bitboard rank3 = 0x0000000000FF0000;
    Bitboard rank4 = 0x00000000FF000000;
    Bitboard rank5 = 0x000000FF00000000;
    Bitboard rank6 = 0x0000FF0000000000;
    Bitboard rank7 = 0x00FF000000000000;
    Bitboard rank8 = 0xFF00000000000000;

    Bitboard a1h8Diagonal = 0x8040201008040201;
    Bitboard h1a8AntiDiagonal = 0x0102040810204080;
    Bitboard lightSquares = 0x55AA55AA55AA55AA;
    Bitboard darkSquares = 0xAA55AA55AA55AA55;
}
