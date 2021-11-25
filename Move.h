//
// Created by kaappo on 25.11.2021.
//


#pragma once


#include <cstdint>
#include "mytypes.h"
#include "Board.h"


// 000000 000000 0         0       0        0
// from   to     promotion capture special1 special2
namespace MoveBitmasks {
    constexpr int CAPTURE = 1 << 3;
    constexpr int PROMOTION = 1 << 4;
    constexpr int DOUBLE_PAWN_PUSH = 1;
    constexpr int KING_CASTLE = 2;
    constexpr int QUEEN_CASTLE = 3;
    constexpr int EN_PASSANT = 1;
}


class Move {
private:
    uint16_t move;

public:
    Move (const Board& context, Square from, Square to, PieceType pieceToPromoteTo);

};


