//
// Created by kaappo on 2.12.2021.
//


#pragma once


#include "mytypes.h"
#include "Bitboard.h"


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

    Bitboard getRookMoveBoard (Bitboard blockerBoard, const Square& square) const;
    Bitboard getBishopMoveBoard (Bitboard blockerBoard, const Square& square) const;

    SlidingPieceAttacks ();
};

class Attacks {
private:
    const KnightAttacks knightAttackGenerator;
    const SlidingPieceAttacks slidingPieceAttackGenerator;

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
};