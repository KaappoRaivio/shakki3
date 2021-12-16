//
// Created by kaappo on 2.12.2021.
//


#pragma once


#include "mytypes.h"
#include "Bitboard.h"
#include "PieceSet.h"
//#include "Board.h"

class Board;

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

    Bitboard getAttackAt (const Board& context, const Bitboard& knights, PieceColor color) const;
};

template <PieceType TYPE>
class SlidingPieceAttacks {
private:
    Bitboard slides[8][64];

    void populateSlides ();
    Bitboard getSlideAt (RayDirection direction, int square) const;
public:
    Bitboard getRayTo (const Square& square, const Bitboard& occupancy, RayDirection direction) const;
    Bitboard getRayTo (const Board& context, const Square& square, RayDirection direction) const;

    Bitboard getRaysToAllDirections (const Board& context, const Square& square, PieceColor color) const;
    Bitboard getRaysToAllDirectionsAllPieces (const Board& context, const Bitboard& pieces, PieceColor color) const;

    Bitboard getCapturesToAllDirections (const Board& context, const Square& square, PieceColor color) const;

    SlidingPieceAttacks ();
};

class PawnAttacks {
private:
    Bitboard possiblePawnPushesOnEmptyBoard[2][64];
    Bitboard possiblePawnCapturesOnEmptyBoard[2][64];

    void populatePossiblePawnPushes ();
    void populatePossiblePawnCaptures ();
public:
    Bitboard getPawnPushes (const Bitboard& occupancy, PieceColor color, const Bitboard& pawns) const;
    Bitboard getPawnCaptures (const Board& context, const Bitboard& pawns, PieceColor color) const;

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
    Bitboard getKingAttackAt (const Board& context, const Square& square, PieceColor color) const;
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
    const KnightAttacks& getKnightAttackGenerator () const;

    template <PieceType TYPE>
    const SlidingPieceAttacks<TYPE>& getSlidingPieceAttackGenerator () const;


    Attacks(Attacks const&)        = delete;
    void operator=(Attacks const&) = delete;

    static Attacks& getInstance () {
        static Attacks instance;
        return instance;
    }

    const PawnAttacks& getPawnAttackGenerator () const;

    const KingAttacks& getKingAttackGenerator () const;
};