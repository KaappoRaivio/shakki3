//
// Created by kaappo on 7.1.2022.
//

#include <random>
#include "ZobristHash.h"
#include "mytypes.h"
#include "Square.h"
#include "CastlingStatus.h"

void ZobristHash::populateBitStrings () {
    std::random_device device;
    std::mt19937_64 generator(device()) ;
    std::uniform_int_distribution<uint64_t> distribution;

    for (auto & bitString : pieceBitstrings) {
        for (auto & color : bitString) {
            for (uint64_t& square : color) {
                square = distribution(generator);
            }
        }
    }

    for (auto & bitstring : castlingRightBitstrings) {
        bitstring = distribution(generator);
    }

    for (auto & bitString : sideToMoveBitstrings) {
        bitString = distribution(generator);
    }
}

ZobristHash::ZobristHash () : pieceBitstrings{}, sideToMoveBitstrings{}, castlingRightBitstrings{} {
    populateBitStrings();
}
uint64_t ZobristHash::getHashFor (const Piece& piece, const Square& square) const {
    if (piece.type == PieceTypes::NO_PIECE) return 0;
    return pieceBitstrings[piece.type][piece.color][square];
}

uint64_t ZobristHash::getHashFor (const CastlingStatus& status) const {
    return castlingRightBitstrings[status.status.to_ulong()];
}

uint64_t ZobristHash::getHashFor (PieceColor turn) const {
    return sideToMoveBitstrings[turn];
}