//
// Created by kaappo on 2.12.2021.
//

#include "Attacks.h"
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

Bitboard KnightAttacks::getAttackAt (const Square& square) const {
    return knightAttacks[square];
}

Bitboard KnightAttacks::getAttackAt (int y, int x) {
    return knightAttacks[y * 8 + x];
}

//namespace KnightAttack {
//    const KnightAttacks instance;
//}

using namespace BitboardOperations;
using namespace BitboardOperations::Directions;

void SlidingPieceAttacks::populateRookSlides () {
    for (int direction = NORTH; direction <= WEST; direction++) {
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                Bitboard start = 1ULL << (8 * y + x);
                Bitboard originalStart = start;
                Bitboard other = start;
                while (true) {
                    if (direction == NORTH) {
                        if (start & SquareMasks::rank8) {
                            break;
                        }
                    } else if (direction == SOUTH) {
                        if (start & SquareMasks::rank1) {
                            break;
                        }
                    } else if (direction == EAST) {
                        if (start & SquareMasks::fileH) {
                            break;
                        }
                    } else if (direction == WEST) {
                        if (start & SquareMasks::fileA) {
                            break;
                        }
                    } else {
                        throw std::runtime_error("Wrong ray!");
                    }
//                    other = move(other, direction);
                    other = other.move(direction);
                    start |= other;
                }
                rookSlides[direction][y * 8 + x] = start ^ originalStart;
            }
        }
    }
}

void SlidingPieceAttacks::populateBishopSlides () {
    for (int direction = NORTH_EAST; direction <= NORTH_WEST; direction++) {
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                Bitboard start = 1ULL << (8 * y + x);
                Bitboard originalStart = start;
                Bitboard other = start;
                while (true) {
                    if (direction == NORTH_EAST) {
                        if (start & (SquareMasks::rank8 | SquareMasks::fileH)) {
                            break;
                        }
                    } else if (direction == SOUTH_EAST) {
                        if (start & (SquareMasks::rank1 | SquareMasks::fileH)) {
                            break;
                        }
                    } else if (direction == SOUTH_WEST) {
                        if (start & (SquareMasks::fileA | SquareMasks::rank1)) {
                            break;
                        }
                    } else if (direction == NORTH_WEST) {
                        if (start & (SquareMasks::fileA | SquareMasks::rank8)) {
                            break;
                        }
                    } else {
                        throw std::runtime_error("Wrong ray!");
                    }
                    other = other.move(direction);
                    start |= other;
                }
                bishopSlides[direction][y * 8 + x] = start ^ originalStart;
            }
        }
    }
}

Bitboard SlidingPieceAttacks::getRookSlideAt (RayDirection direction, int square) const {
    return rookSlides[direction][square];
}

Bitboard SlidingPieceAttacks::getBishopSlideAt (RayDirection direction, int square) const {
    return bishopSlides[direction][square];
}

SlidingPieceAttacks::SlidingPieceAttacks () : rookSlides{}, bishopSlides{} {
    populateBishopSlides();
    populateRookSlides();
}

Bitboard SlidingPieceAttacks::getRookBlockerMask (int y, int x) {
    return getRookSlideAt(NORTH, 8 * y + x)
           | getRookSlideAt(EAST, 8 * y + x)
           | getRookSlideAt(SOUTH, 8 * y + x)
           | getRookSlideAt(WEST, 8 * y + x);
}

Bitboard SlidingPieceAttacks::getBishopBlockerMask (int y, int x) {
    return getBishopSlideAt(NORTH_EAST, 8 * y + x)
           | getBishopSlideAt(SOUTH_EAST, 8 * y + x)
           | getBishopSlideAt(SOUTH_WEST, 8 * y + x)
           | getBishopSlideAt(NORTH_WEST, 8 * y + x);
}

Bitboard SlidingPieceAttacks::getRookMoveBoard (Bitboard blockerBoard, const Square& square) const {
    Bitboard result = 0;
    for (RayDirection direction = NORTH; direction <= WEST; ++direction) {
        Bitboard blockerMask = getRookSlideAt(direction, square);

        auto blockers = blockerMask & blockerBoard;

        if (blockers) {
            int firstBlockPosition;
            if (direction == NORTH || direction == EAST) {
                firstBlockPosition = blockers.ls1b();
            } else if (direction == SOUTH || direction == WEST) {
                firstBlockPosition = blockers.ms1b();
            } else {
                throw std::runtime_error("Wrong direction, you probably messed up refactoring :)");
            }

            blockerMask ^= getRookSlideAt(direction, firstBlockPosition);
        }

        result |= blockerMask;
    }

    return result;
}

Bitboard SlidingPieceAttacks::getBishopMoveBoard (Bitboard blockerBoard, const Square& square) const {
    Bitboard result = 0;
    for (RayDirection direction = NORTH_EAST; direction <= NORTH_WEST; ++direction) {
        Bitboard blockerMask = getBishopSlideAt(direction, square);

        auto blockers = blockerMask & blockerBoard;

        if (blockers) {
            int firstBlockPosition;
            if (direction == NORTH_EAST || direction == NORTH_WEST) {
                firstBlockPosition = blockers.ls1b();
            } else if (direction == SOUTH_EAST || direction == SOUTH_WEST) {
                firstBlockPosition = blockers.ms1b();
            } else {
                throw std::runtime_error("Wrong direction, you probably messed up refactoring :)");
            }

            blockerMask ^= getBishopSlideAt(direction, firstBlockPosition);
        }

        result |= blockerMask;
    }

    return result;
}

Attacks::Attacks () : knightAttackGenerator{}, slidingPieceAttackGenerator{} {

}

const KnightAttacks& Attacks::getKnightAttackGenerator () const {
    return knightAttackGenerator;
}

const SlidingPieceAttacks& Attacks::getSlidingPieceAttackGenerator () const {
    return slidingPieceAttackGenerator;
}
