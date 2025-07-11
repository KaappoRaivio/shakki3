//
// Created by kaappo on 1/19/23.
//

#include "PieceSquareTables.h"

int PieceSquareTables::getMiddlegameValue(const Piece &piece, const Square &at) {
    if (piece.type == PieceTypes::NO_PIECE) return 0;
    Square normalizedPerspective = at.asColorFlip(piece.color)
            .flip(); // to account for array literal Y indexing

    return middlegamePieceValues[piece.type];// + middlegameTable[piece.type.index][normalizedPerspective];
}

int PieceSquareTables::getEndgameValue(const Piece &piece, const Square &at) {
    if (piece.type == PieceTypes::NO_PIECE) return 0;

    Square normalizedPerspective = at.asColorFlip(piece.color)
            .flip(); // to account for array literal Y indexing

    return endgamePieceValues[piece.type];// + endgameTable[piece.type.index][normalizedPerspective];
}

int PieceSquareTables::getGamePhaseValue(PieceType pieceType) {
    if (pieceType == PieceTypes::NO_PIECE) return 0;

    return gamePhaseValues[pieceType.index];
}
