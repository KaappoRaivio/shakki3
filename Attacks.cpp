//
// Created by kaappo on 2.12.2021.
//

#include "Attacks.h"
#include "BitboardOperations.h"
#include "PieceSet.h"


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

Bitboard KnightAttacks::getAttackAt (const Bitboard& knights) const {
    Bitboard result{0};

    for (const auto& square : knights) {
        result |= getAttackAt(square);
    }

    return result;
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

Bitboard SlidingPieceAttacks::getRookMoveBoard (const Board& context, const Square& square, PieceColor color) const {
    Bitboard occupancy = context.getPieces()[WHITE].all | context.getPieces()[BLACK].all;
    Bitboard result{0};
    for (RayDirection direction = NORTH; direction <= WEST; ++direction) {
        Bitboard blockerMask = getRookSlideAt(direction, square);

        auto blockers = blockerMask & occupancy;

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

    return result & ~context.getPieceSet(color).all;
}

Bitboard SlidingPieceAttacks::getRookMoveBoard (const Board& context, const Bitboard& rooks, PieceColor color) const {
    Bitboard result{0};

    for (const Square& square : rooks) {
        result |= getRookMoveBoard(context, square, color);
    }

    return result;
}

Bitboard SlidingPieceAttacks::getBishopMoveBoard (const Board& context, const Square& square, PieceColor color) const {
    Bitboard occupancy = context.getPieces()[WHITE].all | context.getPieces()[BLACK].all;
    Bitboard result{0};
    for (RayDirection direction = NORTH_EAST; direction <= NORTH_WEST; ++direction) {
        Bitboard blockerMask = getBishopSlideAt(direction, square);

        auto blockers = blockerMask & occupancy;

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

    return result & ~context.getPieceSet(color).all;
}

Bitboard SlidingPieceAttacks::getBishopMoveBoard (const Board& context, const Bitboard& bishops, PieceColor color) const {
    Bitboard result{0};

    for (const auto& square : bishops) {
        result |= getBishopMoveBoard(context, square, color);
    }

    return result;
}


Bitboard SlidingPieceAttacks::getQueenMoveBoard (const Board& context, const Square& square, PieceColor color) const {
    return getBishopMoveBoard(context, square, color) | getRookMoveBoard(context, square, color);
}

Bitboard SlidingPieceAttacks::getQueenMoveBoard (const Board& context, const Bitboard& queens, PieceColor color) const {
    return getBishopMoveBoard(context, queens, color) | getRookMoveBoard(context, queens, color);
}


Attacks::Attacks () : knightAttackGenerator{}, slidingPieceAttackGenerator{}, pawnAttackGenerator{}, kingAttackGenerator{} {

}

const KnightAttacks& Attacks::getKnightAttackGenerator () const {
    return knightAttackGenerator;
}

const SlidingPieceAttacks& Attacks::getSlidingPieceAttackGenerator () const {
    return slidingPieceAttackGenerator;
}

const PawnAttacks& Attacks::getPawnAttackGenerator () const {
    return pawnAttackGenerator;
}

const KingAttacks& Attacks::getKingAttackGenerator () const {
    return kingAttackGenerator;
}

PawnAttacks::PawnAttacks () : possiblePawnPushesOnEmptyBoard{}, possiblePawnCapturesOnEmptyBoard{} {
    populatePossiblePawnPushes();
    populatePossiblePawnCaptures();
}

Bitboard PawnAttacks::getPawnPushes (const Bitboard& occupancy, PieceColor color, const Bitboard& pawns) const {
    const auto& singlePush = pawns.move(NORTH, color) & ~occupancy;
    const auto& doublePush = singlePush.move(NORTH, color) & SquareMasks::rank4 & ~occupancy;

    return doublePush | singlePush;
}

Bitboard PawnAttacks::getPawnCaptures (const Board& context, const Bitboard& pawns, PieceColor color) const {
    const auto& east = pawns.move(NORTH_EAST, color, 1, false) & context.getPieces()[flip(color)].all;
    const auto& west = pawns.move(NORTH_WEST, color, 1, false) & context.getPieces()[flip(color)].all;

    return east | west;
}

void PawnAttacks::populatePossiblePawnPushes () {
    // white
    for (int y = 1; y < 7; ++y) { // only from rank 2 to 6 since no pawns can exist on rank 1 nor 8
        for (int x = 0; x < 8; ++x) {
            const Square& square = Square{y, x};
            Bitboard start = square;
            start |= start.move(NORTH);
            if (y == 1) start |= start.move(NORTH);

            possiblePawnPushesOnEmptyBoard[WHITE][square] = start;
        }
    }

    // black
    for (int y = 6; y >= 1; --y) { // only from rank 2 to 6 since no pawns can exist on rank 1 nor 8
        for (int x = 0; x < 8; ++x) {
            const Square& square = Square{y, x};
            Bitboard start = square;
            start |= start.move(SOUTH);
            if (y == 6) start |= start.move(SOUTH);

            possiblePawnPushesOnEmptyBoard[BLACK][square] = start;
        }
    }
}

void PawnAttacks::populatePossiblePawnCaptures () {
    // white
    for (int color = WHITE; color < EMPTY; ++color) {
        for (int y = 1; y < 7; ++y) { // only from rank 2 to 6 since no pawns can exist on rank 1 nor 8
            for (int x = 0; x < 8; ++x) {
                const Square& square = Square{y, x};
                const Bitboard& start = square;

                const Bitboard& east = start.move(NORTH_EAST, static_cast<PieceColor>(color));
                const Bitboard& west = start.move(NORTH_WEST, static_cast<PieceColor>(color));

                possiblePawnCapturesOnEmptyBoard[color][square] = east | west;
            }
        }
    }
}

const Bitboard& PawnAttacks::getPossiblePushesOnEmptyBoard (PieceColor color, const Square& square) const {
    return possiblePawnPushesOnEmptyBoard[color][square];
}

const Bitboard& PawnAttacks::getPossibleCapturesOnEmptyBoard (PieceColor color, const Square& square) const {
    return possiblePawnCapturesOnEmptyBoard[color][square];
}




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

Bitboard KingAttacks::getKingAttackAt (const Board& context, const Square& square, PieceColor color) const {
    const Bitboard& occupancy = context.getPieces()[color].all;
    return attacks[square] & ~occupancy;
}

Bitboard KingAttacks::getKingAttackAt (int y, int x) {
    return attacks[8 * y + x];
}
