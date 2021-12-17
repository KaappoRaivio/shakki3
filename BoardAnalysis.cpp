//
// Created by kaappo on 17.12.2021.
//

#include "BoardAnalysis.h"
#include "Attacks.h"

Bitboard BoardAnalysis::getAttackMask (const Board& board, PieceColor color) {
    Bitboard attackMask;

    const Attacks& attacks = Attacks::getInstance();
//
    attackMask |= attacks.getPawnAttackGenerator().getPawnCaptures(board, board.getPieces()[color].boards[PieceTypes::PAWN], color);
//    attackMask |= attacks..getRaysToAllDirectionsAllPieces(board, board.getPieces()[color].boards[PieceTypes::BISHOP], color);
    attackMask |= attacks.getBishopAttacks().getRaysToAllDirectionsAllPieces(board, board.getPieces()[color].boards[PieceTypes::BISHOP], color);
    attackMask |= attacks.getRookAttacks().getRaysToAllDirectionsAllPieces(board, board.getPieces()[color].boards[PieceTypes::ROOK], color);
    attackMask |= attacks.getQueenAttacks().getRaysToAllDirectionsAllPieces(board, board.getPieces()[color].boards[PieceTypes::QUEEN], color);

    attackMask |= attacks.getKnightAttackGenerator().getAttackAt(board, board.getPieces()[color].boards[PieceTypes::KNIGHT], color);
    attackMask |= attacks.getKingAttackGenerator().getKingAttackAt(board, board.getPieces()[color].boards[PieceTypes::KING].ls1b(), color);

    return attackMask;
}

bool BoardAnalysis::isSquareAttacked (const Board& board, const Square& square, PieceColor color) {
    return bool(getAttackMask(board, color) & square);
}

Bitboard BoardAnalysis::getCheckMask (const Board& context, PieceColor const color) {
    Bitboard checkMask;
    const Attacks& attacks = Attacks::getInstance();

    const Square& kingPosition = context.getPieces(color).boards[PieceTypes::KING].ls1b();

    // bishop and queen
    const Bitboard& supposedBishops = attacks.getBishopAttacks().getRaysToAllDirections(context, kingPosition, color);
    for (const Square& possibleBishop : supposedBishops) {
        if (context.getPieceAt(possibleBishop) == Piece{PieceTypes::BISHOP, flip(color)}
            || context.getPieceAt(possibleBishop) == Piece{PieceTypes::QUEEN, flip(color)}) {

            RayDirection direction = kingPosition.getDirection(possibleBishop, PieceTypes::BISHOP);
            checkMask |= attacks.getBishopAttacks().getRayTo(context, kingPosition, direction);
        }
    }

    // rook and queen
    const Bitboard& supposedRooks = attacks.getRookAttacks().getRaysToAllDirections(context, kingPosition, color);
    for (const Square& possibleRook : supposedRooks) {
        if (context.getPieceAt(possibleRook) == Piece{PieceTypes::ROOK, flip(color)}
            || context.getPieceAt(possibleRook) == Piece{PieceTypes::QUEEN, flip(color)}) {
            RayDirection direction = kingPosition.getDirection(possibleRook, PieceTypes::ROOK);
            checkMask |= attacks.getRookAttacks().getRayTo(context, kingPosition, direction);
        }
    }

    // knight
    const Bitboard& supposedKnights = attacks.getKnightAttackGenerator().getAttackAt(context, kingPosition, color);
    for (const Square& possibleKnight : supposedKnights) {
        if (context.getPieceAt(possibleKnight) == Piece{PieceTypes::KNIGHT, flip(color)}) {
            checkMask |= possibleKnight;
        }
    }

    // pawn
    const Square& possiblePawn1 = kingPosition.move(Directions::SOUTH_EAST, flip(color));
    if (context.getPieceAt(possiblePawn1) == Piece{PieceTypes::PAWN, flip(color)}) {
        checkMask |= possiblePawn1;
    }

    const Square& possiblePawn2 = kingPosition.move(Directions::SOUTH_WEST, flip(color));
    if (context.getPieceAt(possiblePawn2) == Piece{PieceTypes::PAWN, flip(color)}) {
        checkMask |= possiblePawn2;
    }



    return checkMask;
}

Bitboard BoardAnalysis::getPinMask (const Board& board, PieceColor color) {
    const Square& kingPosition = board.getPieces(color).boards[PieceTypes::KING].ls1b();
    return kingPosition; //TODO!!
}
