//
// Created by kaappo on 6.1.2022.
//

#include "../Board.h"

#pragma once


namespace BoardEvaluator {
    constexpr int pieceValues[6] = {
            300,
            300,
            500,
            900,
            100
    };


    int evaluateSimpleOneSide (const Board& board, PieceColor perspective);
    int evaluateSimple (const Board& board);
}



