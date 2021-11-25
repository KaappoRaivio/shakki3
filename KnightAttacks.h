//
// Created by kaappo on 22.11.2021.
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

    Bitboard getAttackAt (Square_raw square);
    Bitboard getAttackAt (int y, int x);
};


