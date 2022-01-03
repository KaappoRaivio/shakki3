//
// Created by kaappo on 2.12.2021.
//

#include "Attacks.h"
#include "BitboardOperations.h"
#include "PieceSet.h"
#include "Board.h"

using namespace BitboardOperations::SquareMasks;
namespace Directions = BitboardOperations::Directions;

void KnightAttacks::initializeKnightAttacks () {
    for (int y = 0 ; y < 8 ; ++y) {
        for (int x = 0 ; x < 8 ; ++x) {
            Bitboard piece = 1ULL << (8 * y + x);

            Bitboard result = 0;

//            Bitboard east = BitboardOperations::move(piece, Directions::EAST, 1, false);
            Bitboard east = piece.move(Directions::EAST, WHITE, 1, false);
            Bitboard west = piece.move(Directions::WEST, WHITE, 1, false);

//            result |= BitboardOperations::move(east | west, Directions::NORTH, 2)
//                      | BitboardOperations::move(east | west, Directions::SOUTH, 2);
            result |= (east | west).move(Directions::NORTH, WHITE, 2)
                      | (east | west).move(Directions::SOUTH, WHITE, 2);

            east = east.move(Directions::EAST, WHITE, 1, false);
            west = west.move(Directions::WEST, WHITE, 1, false);

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

Bitboard KnightAttacks::getAttacksAt (const Board& context, const Bitboard& knights, PieceColor color) const {
    Bitboard result{0};

    for (const auto& square : knights) {
        result |= getAttackAt(square);
    }


    return result;
}

Bitboard KnightAttacks::getAttackAt (int y, int x) {
    return knightAttacks[y * 8 + x];
}




//template<PieceType TYPE>
//Bitboard SlidingPieceAttacks<TYPE>::getRayTo (const Square& square, const Bitboard& occupancy, RayDirection direction) const {
////    Bitboard blockerMask = getRookSlideAt(direction, square);
//    Bitboard blockerMask = getSlideAt(direction, square);
//
//    auto blockers = blockerMask & occupancy;
//
//    if (blockers) {
//        int firstBlockPosition = getClosestBitPosition<TYPE>(blockers, direction);
//        blockerMask ^= getSlideAt(direction, firstBlockPosition);
//    }
//
//    return blockerMask;
//}
//

//template<PieceType type>
//void SlidingPieceAttacks<type>::populateSlides () {
//    // rook slides
//    for (int direction = NORTH ; direction <= WEST ; direction++) {
//        for (int y = 0 ; y < 8 ; ++y) {
//            for (int x = 0 ; x < 8 ; ++x) {
//                Bitboard start = 1ULL << (8 * y + x);
//                Bitboard originalStart = start;
//                Bitboard other = start;
//                while (true) {
//                    if (direction == NORTH) {
//                        if (start & SquareMasks::rank8) {
//                            break;
//                        }
//                    } else if (direction == SOUTH) {
//                        if (start & SquareMasks::rank1) {
//                            break;
//                        }
//                    } else if (direction == EAST) {
//                        if (start & SquareMasks::fileH) {
//                            break;
//                        }
//                    } else if (direction == WEST) {
//                        if (start & SquareMasks::fileA) {
//                            break;
//                        }
//                    } else {
//                        throw std::runtime_error("Wrong ray!");
//                    }
//                    //                    other = move(other, direction);
//                    other = other.move(direction);
//                    start |= other;
//                }
//                slides[direction][y * 8 + x] = start ^ originalStart;
//            }
//        }
//    }
//
//    // bishop slides
//    for (int direction = NORTH_EAST ; direction <= NORTH_WEST ; direction++) {
//        for (int y = 0 ; y < 8 ; ++y) {
//            for (int x = 0 ; x < 8 ; ++x) {
//                Bitboard start = 1ULL << (8 * y + x);
//                Bitboard originalStart = start;
//                Bitboard other = start;
//                while (true) {
//                    if (direction == NORTH_EAST) {
//                        if (start & (SquareMasks::rank8 | SquareMasks::fileH)) {
//                            break;
//                        }
//                    } else if (direction == SOUTH_EAST) {
//                        if (start & (SquareMasks::rank1 | SquareMasks::fileH)) {
//                            break;
//                        }
//                    } else if (direction == SOUTH_WEST) {
//                        if (start & (SquareMasks::fileA | SquareMasks::rank1)) {
//                            break;
//                        }
//                    } else if (direction == NORTH_WEST) {
//                        if (start & (SquareMasks::fileA | SquareMasks::rank8)) {
//                            break;
//                        }
//                    } else {
//                        throw std::runtime_error("Wrong ray!");
//                    }
//                    other = other.move(direction);
//                    start |= other;
//                }
//                slides[direction][y * 8 + x] = start ^ originalStart;
//            }
//        }
//    }
//
//}
//
//template<PieceType type>
//Bitboard SlidingPieceAttacks<type>::getSlideAt (RayDirection direction, int square) const {
//    return slides[direction][square];
//}

//template<PieceType type>
//SlidingPieceAttacks<type>::SlidingPieceAttacks () : slides{} {
//    populateSlides();
//}

//template<PieceType TYPE>
//Bitboard SlidingPieceAttacks<TYPE>::getRaysToAllDirections (const Board& context, const Square& square, PieceColor color) const {
//    Bitboard occupancy = context.getPieces()[WHITE].all | context.getPieces()[BLACK].all;
//    Bitboard result{0};
//    for (RayDirection direction = getIterationStartDirection<TYPE>() ;
//         direction <= getIterationEndDirection<TYPE>() ;
//         ++direction) {
//        Bitboard blockerMask = getRayTo(square, occupancy, direction);
//        result |= blockerMask;
//    }
//
//    return result & ~context.getPieces(color).all;
//}

//template<PieceType TYPE>
//Bitboard SlidingPieceAttacks<TYPE>::getRaysToAllDirectionsAllPieces (const Board& context, const Bitboard& pieces, PieceColor color) const {
//    Bitboard result{0};
//
//    for (const Square& square : pieces) {
//        result |= getRaysToAllDirections(context, square, context);
//    }
//
//    return result;
//}



//template<PieceType TYPE>
//Bitboard SlidingPieceAttacks<TYPE>::getRayTo (const Board& context, const Square& square, RayDirection direction) const {
//    Bitboard occupancy = context.getPieces()[WHITE].all | context.getPieces()[BLACK].all;
//    return getRayTo(square, occupancy, direction);
////    return getRookMoveBoard(context, occupancy, direction);
//}

//template<PieceType TYPE>
//Bitboard SlidingPieceAttacks<TYPE>::getCapturesToAllDirections (const Board& context, const Square& square, PieceColor color) const {
//    Bitboard occupancy = context.getPieces()[WHITE].all | context.getPieces()[BLACK].all;
//    Bitboard result{0};
//    for (RayDirection direction = getIterationStartDirection<TYPE>() ; direction <= getIterationEndDirection<TYPE>() ; ++direction) {
//        Bitboard blockerMask = getSlideAt(direction, square);
//
//        auto blockers = blockerMask & occupancy;
//
//        if (blockers) {
//            int firstBlockPosition = getClosestBitPosition<TYPE>(blockers, direction);
////            if (direction == NORTH || direction == EAST) {
////                firstBlockPosition = blockers.ls1b();
////            } else if (direction == SOUTH || direction == WEST) {
////                firstBlockPosition = blockers.ms1b();
////            } else {
////                throw std::runtime_error("Wrong direction, you probably messed up refactoring :)");
////            }
//
//            result |= (Square) firstBlockPosition;
//        }
//    }
//
//    return result & ~context.getPieces(color).all;
//}




const KnightAttacks& Attacks::getKnightAttacks () const {
    return knightAttackGenerator;
}

//template <PieceType TYPE>
//const SlidingPieceAttacks<TYPE>& Attacks::getSlidingPieceAttackGenerator () const {
////    return rookAttackGenerator;
//    switch (TYPE.index) {
//        case PieceTypes::ROOK.index:
//            return rookAttackGenerator;
//        case PieceTypes::BISHOP.index:
//            return bishopAttackGenerator;
//        case PieceTypes::QUEEN.index:
//            return queenAttackGenerator;
//        default:
//            throw std::runtime_error("Invalid piece type!");
//    }
//}

const PawnAttacks& Attacks::getPawnAttacks () const {
    return pawnAttackGenerator;
}

const KingAttacks& Attacks::getKingAttacks () const {
    return kingAttackGenerator;
}

Attacks::Attacks () : knightAttackGenerator{}, rookAttackGenerator{}, bishopAttackGenerator{}, queenAttackGenerator{},
                      pawnAttackGenerator{}, kingAttackGenerator{} {}

SlidingPieceAttacks<PieceTypes::BISHOP> Attacks::getBishopAttacks () const {
    return bishopAttackGenerator;
}

SlidingPieceAttacks<PieceTypes::ROOK> Attacks::getRookAttacks () const {
    return rookAttackGenerator;
}

SlidingPieceAttacks<PieceTypes::QUEEN> Attacks::getQueenAttacks () const {
    return queenAttackGenerator;
}

PawnAttacks::PawnAttacks () : possiblePawnPushesOnEmptyBoard{}, possiblePawnCapturesOnEmptyBoard{} {
    populatePossiblePawnPushes();
    populatePossiblePawnCaptures();
}

Bitboard PawnAttacks::getPawnPushes (const Bitboard& occupancy, PieceColor color, const Bitboard& pawns) const {
    const auto& singlePush = pawns.move(NORTH, color) & ~occupancy;
    const auto& doublePush = singlePush.move(NORTH, color) & SquareMasks::rank4.asColor(color, false) & ~occupancy;

    return doublePush | singlePush;
}

Bitboard PawnAttacks::getCaptures (const Board& context, const Bitboard& pawns, PieceColor color) const {
    const auto& east = pawns.move(NORTH_EAST, color, 1, false);
    const auto& west = pawns.move(NORTH_WEST, color, 1, false);

    return east | west;
}

void PawnAttacks::populatePossiblePawnPushes () {
    // white
    for (int y = 1 ; y < 7 ; ++y) { // only from rank 2 to 6 since no pawns can exist on rank 1 nor 8
        for (int x = 0 ; x < 8 ; ++x) {
            const Square& square = Square{y, x};
            Bitboard start = square;
            start |= start.move(NORTH);
            if (y == 1) start |= start.move(NORTH);

            possiblePawnPushesOnEmptyBoard[WHITE][square] = start;
        }
    }

    // black
    for (int y = 6 ; y >= 1 ; --y) { // only from rank 2 to 6 since no pawns can exist on rank 1 nor 8
        for (int x = 0 ; x < 8 ; ++x) {
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
    for (int color = WHITE ; color < EMPTY ; ++color) {
        for (int y = 1 ; y < 7 ; ++y) { // only from rank 2 to 6 since no pawns can exist on rank 1 nor 8
            for (int x = 0 ; x < 8 ; ++x) {
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
    for (int y = 0 ; y < 8 ; ++y) {
        for (int x = 0 ; x < 8 ; ++x) {
            Bitboard king = 1ull << (8 * y + x);
//            king |= BitboardOperations::move(king, Directions::EAST, 1, false)
//                    | BitboardOperations::move(king, Directions::WEST, 1, false);

            king |= king.move(Directions::EAST, WHITE, 1, false)
                    | king.move(Directions::WEST, WHITE, 1, false);

            king |= king.move(Directions::NORTH)
                    | king.move(Directions::SOUTH);

            king ^= 1ull << (8 * y + x);

            attacks[8 * y + x] = king;
        }
    }
}

KingAttacks::KingAttacks () : attacks{} {
    initializeKingAttacks();
}

Bitboard KingAttacks::getAttacksAt (const Board& context, const Square& square, PieceColor color) const {
    return attacks[square];
}

Bitboard KingAttacks::getKingAttackAt (int y, int x) {
    return attacks[8 * y + x];
}
