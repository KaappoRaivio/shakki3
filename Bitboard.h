//
// Created by kaappo on 24.11.2021.
//


#pragma once


#include <cstdint>
#include <ostream>
#include "mytypes.h"
#include "Square.h"

class Bitboard {
private:
    Bitboard_raw value;
public:
    Bitboard (uint64_t value); // NOLINT(google-explicit-constructor)
    Bitboard (Square_raw square);
    Bitboard ();

    Bitboard move (RayDirection direction) const;
    Bitboard move (RayDirection direction, PieceColor perspective) const;
    Bitboard move (RayDirection direction, PieceColor perspective, int amount) const;
    Bitboard move (RayDirection direction, PieceColor perspective, int amount, bool wrapAround) const;

    uint8_t ls1b () const;
    uint8_t ms1b () const;

    std::string& burnTo (std::string& buffer, char symbol='1') const;

    Bitboard flipVertical () const;
    Bitboard rotate180 () const;

    Bitboard asColor (PieceColor color) const;

    Bitboard& operator<<=(int shift);
    friend Bitboard operator<< (Bitboard bitboard, int shift);

    Bitboard& operator>>=(int shift);
    friend Bitboard operator>> (Bitboard bitboard, int shift);

    Bitboard& operator&=(Bitboard other);
    friend Bitboard operator& (Bitboard bitboard, Bitboard other);

    Bitboard& operator|=(Bitboard other);
    friend Bitboard operator| (Bitboard bitboard, Bitboard other);

    Bitboard& operator^=(Bitboard other);
    friend Bitboard operator^ (Bitboard bitboard, Bitboard other);

    friend Bitboard operator~ (Bitboard bitboard);
    explicit operator bool() const;

    friend std::ostream& operator<< (std::ostream& os, const Bitboard& bitboard);

    Bitboard (Square square);
};


