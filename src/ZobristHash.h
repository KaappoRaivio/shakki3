//
// Created by kaappo on 7.1.2022.
//

#pragma once

#include <cstdint>
#include "mytypes.h"
#include "Square.h"
#include "CastlingStatus.h"

class ZobristHash {
private:
    uint64_t pieceBitstrings[6][2][64];
    uint64_t sideToMoveBitstrings[2];
    uint64_t castlingRightBitstrings[16];

    void populateBitStrings ();

public:
    ZobristHash ();

    ZobristHash (ZobristHash const&) = delete;

    void operator= (ZobristHash const&) = delete;

    static ZobristHash& getInstance () {
        static ZobristHash instance;
        return instance;
    }

    uint64_t getHashFor (const Piece& piece, const Square& square) const;
    uint64_t getHashFor (const CastlingStatus& status) const;

    uint64_t getHashFor (PieceColor turn) const;
};

