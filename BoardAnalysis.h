//
// Created by kaappo on 17.12.2021.
//

#pragma once

#include "Board.h"

namespace BoardAnalysis {
    bool isSquareAttacked (const Board& board, const Square& square, PieceColor color);

    Bitboard getAttackMask (const Board& board, PieceColor color);

    Bitboard getPinMask (const Board& board, PieceColor color);

    Bitboard getCheckMask (const Board& context, PieceColor color);
}