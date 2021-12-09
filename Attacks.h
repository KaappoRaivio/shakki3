//
// Created by kaappo on 2.12.2021.
//


#pragma once


#include "mytypes.h"
#include "Bitboard.h"
#include "PieceSet.h"
#include "Board.h"


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

    Bitboard getAttackAt (const Bitboard& knights) const;
};

class SlidingPieceAttacks {
private:
    Bitboard rookSlides[4][64];
    Bitboard bishopSlides[8][64];

    void populateRookSlides ();
    void populateBishopSlides ();

    Bitboard getRookSlideAt(RayDirection direction, int square) const;
    Bitboard getBishopSlideAt(RayDirection direction, int square) const;
public:
    Bitboard getRookBlockerMask(int y, int x);
    Bitboard getBishopBlockerMask(int y, int x);

    Bitboard getRookMoveBoard (const Board& context, const Square& square, PieceColor color) const;
    Bitboard getBishopMoveBoard (const Board& context, const Square& square, PieceColor color) const;

    SlidingPieceAttacks ();

    Bitboard getQueenMoveBoard (const Board& context, const Square& square, PieceColor color) const;

    Bitboard getBishopMoveBoard (const Board& context, const Bitboard& bishops, PieceColor color) const;

    Bitboard getRookMoveBoard (const Board& context, const Bitboard& rooks, PieceColor color) const;

    Bitboard getQueenMoveBoard (const Board& context, const Bitboard& queens, PieceColor color) const;
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
    const SlidingPieceAttacks slidingPieceAttackGenerator;
    const PawnAttacks pawnAttackGenerator;
    const KingAttacks kingAttackGenerator;

    Attacks ();
public:
    const KnightAttacks& getKnightAttackGenerator () const;

    const SlidingPieceAttacks& getSlidingPieceAttackGenerator () const;

    Attacks(Attacks const&)        = delete;
    void operator=(Attacks const&) = delete;

    static Attacks& getInstance () {
        static Attacks instance;
        return instance;
    }

    const PawnAttacks& getPawnAttackGenerator () const;

    const KingAttacks& getKingAttackGenerator () const;
};