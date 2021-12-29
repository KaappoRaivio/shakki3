//
// Created by kaappo on 2.12.2021.
//


#pragma once


#include "mytypes.h"
#include "Bitboard.h"
#include "PieceSet.h"

#include "Board.h"

using namespace BitboardOperations::Directions;
namespace SquareMasks = BitboardOperations::SquareMasks;

class KnightAttacks {
    static constexpr RayDirection noNoEa = 17;
    static constexpr RayDirection noEaEa = 10;
    static constexpr RayDirection soEaEa = -6;
    static constexpr RayDirection soSoEa = -15;
    static constexpr RayDirection soSoWe = -17;
    static constexpr RayDirection soWeWe = -10;
    static constexpr RayDirection noWeWe = 6;
    static constexpr RayDirection noNoWe = 15;
private:
    Bitboard knightAttacks[64];
    void initializeKnightAttacks ();

public:
    KnightAttacks ();

    Bitboard getAttackAt (const Square& square) const;
    Bitboard getAttackAt (int y, int x);

    Bitboard getAttacksAt (const Board& context, const Bitboard& knights, PieceColor color) const;
};

using namespace BitboardOperations;
using namespace BitboardOperations::Directions;

template <PieceType TYPE>
constexpr int getClosestBitPosition (const Bitboard& blockers, RayDirection direction) {
    if (direction == NORTH || direction == EAST || direction == NORTH_EAST || direction == NORTH_WEST) {
        return blockers.ls1b();
    } else {
        return blockers.ms1b();
    }
}

template<PieceType TYPE>
RayDirection getIterationStartDirection () {
    if (TYPE == PieceTypes::ROOK || TYPE == PieceTypes::QUEEN) {
        return NORTH;
    } else if (TYPE == PieceTypes::BISHOP) {
        return NORTH_EAST;
    }
//    return TYPE == PieceTypes::ROOK ? NORTH : NORTH_EAST;
}

template<PieceType TYPE>
RayDirection getIterationEndDirection () {
    if (TYPE == PieceTypes::ROOK) {
        return WEST;
    } else if (TYPE == PieceTypes::BISHOP || TYPE == PieceTypes::QUEEN) {
        return NORTH_WEST;
    }
//    return TYPE == PieceTypes::ROOK ? WEST : NORTH_WEST;
}


template <PieceType TYPE>
class SlidingPieceAttacks {
private:
    Bitboard slides[8][64];

    void populateSlides () {
        // rook slides
        for (int direction = NORTH ; direction <= WEST ; direction++) {
            for (int y = 0 ; y < 8 ; ++y) {
                for (int x = 0 ; x < 8 ; ++x) {
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
                    slides[direction][y * 8 + x] = start ^ originalStart;
                }
            }
        }

        // bishop slides
        for (int direction = NORTH_EAST ; direction <= NORTH_WEST ; direction++) {
            for (int y = 0 ; y < 8 ; ++y) {
                for (int x = 0 ; x < 8 ; ++x) {
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
                    slides[direction][y * 8 + x] = start ^ originalStart;
                }
            }
        }

    }

    Bitboard getSlideAt (RayDirection direction, int square) const {
        return slides[direction][square];
    }

public:

    Bitboard getRayTo (const Square& square, const Bitboard& occupancy, RayDirection direction) const {
        Bitboard blockerMask = getSlideAt(direction, square);

        auto blockers = blockerMask & occupancy;

        if (blockers) {
            int firstBlockPosition = getClosestBitPosition<TYPE>(blockers, direction);
            blockerMask ^= getSlideAt(direction, firstBlockPosition);
        }

        return blockerMask;
    };
    Bitboard getRayTo (const Board& context, const Square& square, RayDirection direction) const {
        return getRayTo(square, context.getOccupancy(false), direction);
    }


    Bitboard getRaysToAllDirections (const Board& context, const Square& square, PieceColor color, bool includeKingInOccupancy) const {
        return getRaysToAllDirectionsFromOccupancy(context.getOccupancy(includeKingInOccupancy), 0, square);
    }

    Bitboard getRaysToAllDirectionsFromOccupancy (const Bitboard& occupancy, const Bitboard& blockers, const Square& square) const {
        Bitboard result{0};
        for (RayDirection direction = getIterationStartDirection<TYPE>() ;
             direction <= getIterationEndDirection<TYPE>() ;
             ++direction) {
            Bitboard blockerMask = getRayTo(square, occupancy, direction);
            result |= blockerMask;
        }

        return result & ~blockers;
    }

    Bitboard getRaysToAllDirectionsAllPieces (const Board& context, const Bitboard& pieces, PieceColor color, bool includeKingInOccupancy) const {
        Bitboard result{0};

        for (const Square& square : pieces) {
            result |= getRaysToAllDirections(context, square, color, includeKingInOccupancy);
        }

        return result;
    }

    Bitboard getCapturesToAllDirections (const Board& context, const Square& square, PieceColor color) const {
        Bitboard result{0};
        for (RayDirection direction = getIterationStartDirection<TYPE>() ; direction <= getIterationEndDirection<TYPE>() ; ++direction) {
            Bitboard blockerMask = getSlideAt(direction, square);

            auto blockers = blockerMask & context.getOccupancy(false);

            if (blockers) {
                int firstBlockPosition = getClosestBitPosition<TYPE>(blockers, direction);

                result |= (Square) firstBlockPosition;
            }
        }

        return result & ~context.getBlockers(color, false);
    }

    Bitboard getRaysToAllDirectionsXRay (const Board& context, const Square& square, PieceColor color) const {
        Bitboard occupancy = context.getOccupancy(false);
        Bitboard blockers = context.getBlockers(color, false);


        const Bitboard& attacks = getRaysToAllDirectionsFromOccupancy(occupancy, 0, square);
        blockers &= attacks;
        return attacks ^ getRaysToAllDirectionsFromOccupancy(occupancy ^ blockers, 0, square);
    }



    SlidingPieceAttacks () {
        populateSlides();
    }
};

class PawnAttacks {
private:
    Bitboard possiblePawnPushesOnEmptyBoard[2][64];
    Bitboard possiblePawnCapturesOnEmptyBoard[2][64];

    void populatePossiblePawnPushes ();
    void populatePossiblePawnCaptures ();
public:
    Bitboard getPawnPushes (const Bitboard& occupancy, PieceColor color, const Bitboard& pawns) const;
    Bitboard getCaptures (const Board& context, const Bitboard& pawns, PieceColor color) const;

    const Bitboard& getPossiblePushesOnEmptyBoard (PieceColor color, const Square& square) const;
    const Bitboard& getPossibleCapturesOnEmptyBoard (PieceColor color, const Square& square) const;

    PawnAttacks ();

};

class KingAttacks {
private:
    Bitboard attacks[64];
    void initializeKingAttacks();

public:
    KingAttacks ();
    Bitboard getAttacksAt (const Board& context, const Square& square, PieceColor color) const;
    Bitboard getKingAttackAt(int y, int x);
};


class Attacks {
private:
    const KnightAttacks knightAttackGenerator;
    const SlidingPieceAttacks<PieceTypes::ROOK> rookAttackGenerator;
    const SlidingPieceAttacks<PieceTypes::BISHOP> bishopAttackGenerator;
    const SlidingPieceAttacks<PieceTypes::QUEEN> queenAttackGenerator;
    const PawnAttacks pawnAttackGenerator;
    const KingAttacks kingAttackGenerator;

public:
    Attacks ();

public:
    const KnightAttacks& getKnightAttacks () const;

//    template <PieceType TYPE>
//    const SlidingPieceAttacks<TYPE>& getSlidingPieceAttackGenerator () const {
//        switch (TYPE) {
//            case PieceTypes::ROOK.index:
//                return rookAttackGenerator;
//            case PieceTypes::BISHOP.index:
//                return bishopAttackGenerator;
//            case PieceTypes::QUEEN.index:
//                return queenAttackGenerator;
//            default:
//                throw std::runtime_error("Invalid piece type!");
//        }
//    }

    SlidingPieceAttacks<PieceTypes::BISHOP> getBishopAttacks () const;
    SlidingPieceAttacks<PieceTypes::ROOK> getRookAttacks () const;
    SlidingPieceAttacks<PieceTypes::QUEEN> getQueenAttacks () const;


    Attacks(Attacks const&)        = delete;
    void operator=(Attacks const&) = delete;

    static Attacks& getInstance () {
        static Attacks instance;
        return instance;
    }

    const PawnAttacks& getPawnAttacks () const;

    const KingAttacks& getKingAttacks () const;
};