//
// Created by kaappo on 6.1.2022.
//

#include "BoardEvaluator.h"
#include "../BoardAnalysis.h"
#include "Search.h"

int evaluateHistoryOneSide (const Board& board, PieceColor perspective, int plysFromRoot) {
    int score = 0;

    for (int plyIndex = 0; plyIndex < plysFromRoot; ++plyIndex) {
        const BoardState& state = board.getHistory().getFrame(plyIndex);

        const Move& move = state.previousMove;
        if (state.turn == flip(perspective) && move.isCapture()) {
            score += plyIndex;
        }
    }

    return score;
}


int BoardEvaluator::evaluateSimpleOneSide (const Board& board, PieceColor perspective, int plysFromRoot) {
//    if (board.isCheckmate()) return EvaluationConstants::LOSE - plysFromRoot;
    if (board.isCheckmate()) return EvaluationConstants::LOSE;

    int value = 0;
    auto pinMask = BoardAnalysis::getPinMask<BOTH>(board, perspective);
    for (int pieceType = 0; pieceType < PieceTypes::KING; ++pieceType) {
        value += board.getPieces(perspective).boards[pieceType].popCount() * pieceValues[pieceType];
//        value += (board.getPieces(perspective).boards[pieceType] & ~pinMask).popCount() * pieceValues[pieceType];
//        value += (board.getPieces(perspective).boards[pieceType] & pinMask).popCount() * pieceValues[pieceType] * 0.5;
    }

    value += InBetween::getInstance().getDistanceToEdge(board.getPieces(perspective).boards[PieceTypes::KING].ls1b()) * 1;
    value += evaluateHistoryOneSide(board, perspective, plysFromRoot);

    return value;
//    - pinMask.popCount();
}

int BoardEvaluator::evaluateSimple (const Board& board, int plysFromRoot, int originalDepth) {
    return evaluateSimpleOneSide(board, board.getTurn(), plysFromRoot) - evaluateSimpleOneSide(board, flip(board.getTurn()), plysFromRoot);
}
