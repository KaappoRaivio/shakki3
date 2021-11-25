//
// Created by kaappo on 24.11.2021.
//

#include "KingAttacks.h"
#include "BitboardOperations.h"

namespace Directions = BitboardOperations::Directions;

void KingAttacks::initializeKingAttacks () {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Bitboard king = 1ull << (8 * y + x);
//            king |= BitboardOperations::move(king, Directions::EAST, 1, false)
//                    | BitboardOperations::move(king, Directions::WEST, 1, false);

            king |= king.move(Directions::EAST, WHITE, 1, false)
                    | king.move(Directions::WEST, WHITE, 1, false);

            king |= king.move(Directions::NORTH)
                    | king.move(Directions::SOUTH);


            attacks[8 * y + x] = king;
        }
    }
}

KingAttacks::KingAttacks () : attacks{} {
    initializeKingAttacks();
}

Bitboard KingAttacks::getKingAttackAt (Square_raw square) {
    return attacks[square];
}

Bitboard KingAttacks::getKingAttackAt (int y, int x) {
    return attacks[8 * y + x];
}
