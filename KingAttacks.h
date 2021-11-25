//
// Created by kaappo on 24.11.2021.
//


#pragma once


#include "mytypes.h"
#include "Bitboard.h"

class KingAttacks {
private:
    Bitboard attacks[64];
    void initializeKingAttacks();

public:
    KingAttacks ();
    Bitboard getKingAttackAt(Square_raw square);
    Bitboard getKingAttackAt(int y, int x);
};


