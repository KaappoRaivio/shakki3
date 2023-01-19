//
// Created by kaappo on 6.1.2022.
//

#include <valarray>
#include "BoardEvaluator.h"
#include "../BoardAnalysis.h"
#include "Search.h"
#include "eval/PieceSquareTables.h"

int evaluateHistoryOneSide(const Board &board, PieceColor perspective, int plysFromRoot) {
    int score = 0;

    for (int plyIndex = 0; plyIndex < plysFromRoot; ++plyIndex) {
        const BoardState &state = board.getHistory().getFrame(plyIndex);

        const Move &move = state.previousMove;
        if (state.turn == flip(perspective) && move.isCapture()) {
            score += plyIndex;
        }
    }

    return score;
}


int BoardEvaluator::evaluateSimpleOneSide(const Board &board, PieceColor perspective, int plysFromRoot) {
    int value = 0;
    auto pinMask = BoardAnalysis::getPinMask<BOTH>(board, perspective);


    int middlegamePhase = 0;
    int middlegameEval = 0;
    int endgameEval = 0;

    for (int pieceType = 0; pieceType < PieceTypes::KING; ++pieceType) {
        const Bitboard &ownPiecesOnBoard = board.getPieces(perspective).boards[pieceType];
        const Bitboard &opponentPiecesOnBoard = board.getPieces(flip(perspective)).boards[pieceType];

//        value += ownPiecesOnBoard.popCount() * pieceValues[pieceType];

        for (const Square &occupiedSquare: ownPiecesOnBoard) {
            middlegameEval += PieceSquareTables::getMiddlegameValue(Pieces::pieces[pieceType][perspective],
                                                                    occupiedSquare);
            endgameEval += PieceSquareTables::getEndgameValue(Pieces::pieces[pieceType][perspective],
                                                                 occupiedSquare);
        }

        middlegamePhase += PieceSquareTables::getGamePhaseValue(PieceTypes::pieces[pieceType]) *
                           (ownPiecesOnBoard.popCount() + opponentPiecesOnBoard.popCount());
    }

    int opponentKingsDistanceToBoardEdge = InBetween::getInstance().getDistanceToEdge(
            board.getPieces(perspective).boards[PieceTypes::KING].ls1b());

    Square ownKingPos = board.getPieces(perspective).boards[PieceTypes::KING].ls1b();
    Square opponentKingPos = board.getPieces(flip(perspective)).boards[PieceTypes::KING].ls1b();

    int distanceBetweenKings =
            std::abs(ownKingPos.getX() - opponentKingPos.getX()) + std::abs(ownKingPos.getY() - opponentKingPos.getY());

    endgameEval += opponentKingsDistanceToBoardEdge;  // Prefer positions where opponent king is close to
    endgameEval -= distanceBetweenKings;              // edge and the kings are close to each other. This helps in making
                                                      // checkmates on an empty board with limited material.



    middlegamePhase = std::min(24, middlegamePhase); // for the rare case of early promotion which would lead to a negative endgamephase
    int endgamePhase = 24 - middlegamePhase;

    value += (middlegameEval * middlegamePhase + endgameEval * endgamePhase) / 24;


    return value;
}

int BoardEvaluator::evaluateSimple(const Board &board, int plysFromRoot, int originalDepth) {
    return evaluateSimpleOneSide(board, board.getTurn(), plysFromRoot) -
           evaluateSimpleOneSide(board, flip(board.getTurn()), plysFromRoot);
}
