//
// Created by kaappo on 6.1.2022.
//

#include "BoardEvaluator.h"
#include "../BoardAnalysis.h"

int BoardEvaluator::evaluateSimpleOneSide (const Board& board, PieceColor perspective) {


    int value = 0;
    auto pinMask = BoardAnalysis::getPinMask<BOTH>(board, perspective);
    for (int pieceType = 0; pieceType < PieceTypes::KING; ++pieceType) {
        value += board.getPieces(perspective).boards[pieceType].popCount() * pieceValues[pieceType];
//        value += (board.getPieces(perspective).boards[pieceType] & ~pinMask).popCount() * pieceValues[pieceType];
//        value += (board.getPieces(perspective).boards[pieceType] & pinMask).popCount() * pieceValues[pieceType] * 0.5;
    }



    return value;
//    - pinMask.popCount();
}

int BoardEvaluator::evaluateSimple (const Board& board) {
    return evaluateSimpleOneSide(board, board.getTurn()) - evaluateSimpleOneSide(board, flip(board.getTurn()));
}
