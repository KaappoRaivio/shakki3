//
// Created by kaappo on 22.11.2021.
//


#include "KnightAttacks.h"
#include "mytypes.h"
#include "BitboardOperations.h"

using namespace BitboardOperations::SquareMasks;
namespace Directions = BitboardOperations::Directions;

void KnightAttacks::initializeKnightAttacks () {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Bitboard piece = 1ULL << (8 * y + x);

            Bitboard result = 0;

//            Bitboard east = BitboardOperations::move(piece, Directions::EAST, 1, false);
            Bitboard east = piece.move(Directions::EAST, WHITE, 1, false);
            Bitboard west = piece.move(Directions::WEST, WHITE, 1, false);

//            result |= BitboardOperations::move(east | west, Directions::NORTH, 2)
//                      | BitboardOperations::move(east | west, Directions::SOUTH, 2);
            result |= (east | west).move(Directions::NORTH, WHITE, 2)
                      | (east | west).move(Directions::SOUTH, WHITE, 2);

            east = piece.move(Directions::EAST, WHITE, 1, false);
            west = piece.move(Directions::WEST, WHITE, 1, false);

            result |= (east | west).move(Directions::NORTH)
                      | (east | west).move(Directions::SOUTH);


            knightAttacks[8 * y + x] = result;
        }
    }
}

KnightAttacks::KnightAttacks () : knightAttacks{} {
    initializeKnightAttacks();
}

Bitboard KnightAttacks::getAttackAt (Square_raw square) {
    return knightAttacks[square];
}

Bitboard KnightAttacks::getAttackAt (int y, int x) {
    return knightAttacks[y * 8 + x];
}

