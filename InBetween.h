//
// Created by kaappo on 22.12.2021.
//


#pragma once


#include "mytypes.h"
#include "Square.h"
#include "Bitboard.h"

class InBetween {
private:
    Bitboard lookup[64][64];

    constexpr Bitboard inBetween (Square_raw square1, Square_raw square2);
    constexpr void populateLookup();
public:
    InBetween ();

    InBetween(InBetween const&)      = delete;
    void operator=(InBetween const&) = delete;

    static InBetween& getInstance () {
        static InBetween instance;
        return instance;
    }

    Bitboard getPath (const Square& from, const Square& to) const;
};


