//
// Created by kaappo on 22.12.2021.
//

#include "InBetween.h"

constexpr Bitboard InBetween::inBetween (Square_raw square1, Square_raw square2) {
    const Bitboard_raw m1 = -1;
    const Bitboard_raw a2a7 = 0x0001010101010100;
    const Bitboard_raw b2g7 = 0x0040201008040200;
    const Bitboard_raw h1b7 = 0x0002040810204080;

    Bitboard_raw between = (m1 << square1) ^ (m1 << square2);

    Bitboard_raw file = (square2 & 7) - (square1 & 7);
    Bitboard_raw rank = ((square2 | 7) - square1) >> 3;
    Bitboard_raw line = ((file & 7) - 1) & a2a7; /* a2a7 if same file */
    line += 2 * (((rank & 7) - 1) >> 58); /* b1g1 if same rank */
    line += (((rank - file) & 15) - 1) & b2g7; /* b2g7 if same diagonal */
    line += (((rank + file) & 15) - 1) & h1b7; /* h1b7 if same antidiag */
    line *= between & -between; /* mul acts like shift by smaller square */
    return line & between;   /* return the bits on that line in-between */
}

constexpr void InBetween::populateLookup () {
    for (int sq1 = 0 ; sq1 < 64 ; ++sq1) {
        for (int sq2 = 0 ; sq2 < 64 ; ++sq2) {
            lookup[sq1][sq2] = inBetween(static_cast<Square_raw>(sq1), static_cast<Square_raw>(sq2));
        }
    }
}

InBetween::InBetween () : lookup{{}} {
    populateLookup();
}

Bitboard InBetween::getPath (const Square& from, const Square& to) const {
    return lookup[from][to] | from;
}
