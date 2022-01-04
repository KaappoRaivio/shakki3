//
// Created by kaappo on 17.12.2021.
//

#include "BoardAnalysis.h"
#include "Attacks.h"
#include "InBetween.h"

Bitboard BoardAnalysis::getAttackMask (const Board& board, PieceColor color) {
    Bitboard attackMask;

    const Attacks& attacks = Attacks::getInstance();

    attackMask |= attacks.getPawnAttacks().getCaptures(board, board.getPieces()[color].boards[PieceTypes::PAWN], color);
    attackMask |= attacks.getBishopAttacks().getRaysToAllDirectionsAllPieces(board, board.getPieces()[color].boards[PieceTypes::BISHOP], color, false);
    attackMask |= attacks.getRookAttacks().getRaysToAllDirectionsAllPieces(board, board.getPieces()[color].boards[PieceTypes::ROOK], color, false);
    attackMask |= attacks.getQueenAttacks().getRaysToAllDirectionsAllPieces(board, board.getPieces()[color].boards[PieceTypes::QUEEN], color, false);
    attackMask |= attacks.getKnightAttacks().getAttacksAt(board, board.getPieces()[color].boards[PieceTypes::KNIGHT], color);
    attackMask |= attacks.getKingAttacks().getAttacksAt(board, board.getPieces()[color].boards[PieceTypes::KING].ls1b(), color);

    return attackMask;
}

bool BoardAnalysis::isSquareAttacked (const Board& board, const Square& square, PieceColor color) {
    return bool(getAttackMask(board, color) & square);
}

Bitboard BoardAnalysis::getCheckMask (const Board& context, PieceColor const color) {
    Bitboard checkMask;
    const Attacks& attacks = Attacks::getInstance();

    const Square& kingPosition = context.getPieces(color).boards[PieceTypes::KING].ls1b();

    Square potentialFirstAttacker{Square::INVALID};

    using namespace PieceTypes;

    // bishop and queen
    const Bitboard& supposedBishops = attacks.getBishopAttacks().getRaysToAllDirections(context, kingPosition, color, false) & ~context.getBlockers(color, false);
    for (const Square& possibleBishop : supposedBishops) {
        if (context.getPieceAt(possibleBishop) == Pieces::pieces[BISHOP][flip(color)]
            || context.getPieceAt(possibleBishop) == Pieces::pieces[QUEEN][flip(color)]) {
            if (potentialFirstAttacker != Square::INVALID) {
                return 0;
//                checkMask.reset();
//                checkMask |= potentialFirstAttacker;
//                checkMask |= possibleBishop;
//                return checkMask;
            }
            potentialFirstAttacker = possibleBishop;

            RayDirection direction = kingPosition.getDirection(possibleBishop, PieceTypes::BISHOP);
            checkMask |= attacks.getBishopAttacks().getRayTo(context, kingPosition, direction);
        }
    }

    // rook and queen
    const Bitboard& supposedRooks = attacks.getRookAttacks().getRaysToAllDirections(context, kingPosition, color, false) & ~context.getBlockers(color, false);
    for (const Square& possibleRook : supposedRooks) {
        if (context.getPieceAt(possibleRook) == Pieces::pieces[ROOK][flip(color)]
            || context.getPieceAt(possibleRook) == Pieces::pieces[QUEEN][flip(color)]) {
            if (potentialFirstAttacker != Square::INVALID) {
                return 0;
//                checkMask.reset();
//                checkMask |= potentialFirstAttacker;
//                checkMask |= possibleRook;
//                return checkMask;
            }
            potentialFirstAttacker = possibleRook;

            RayDirection direction = kingPosition.getDirection(possibleRook, PieceTypes::ROOK);
            checkMask |= attacks.getRookAttacks().getRayTo(context, kingPosition, direction);
        }
    }

    // knight
    const Bitboard& supposedKnights = attacks.getKnightAttacks().getAttacksAt(context, kingPosition, color) & ~context.getBlockers(color, false);
    for (const Square& possibleKnight : supposedKnights) {
        if (context.getPieceAt(possibleKnight) == Pieces::pieces[KNIGHT][flip(color)]) {
            if (potentialFirstAttacker != Square::INVALID) {
                return 0;
//                checkMask.reset();
//                checkMask |= potentialFirstAttacker;
//                checkMask |= possibleKnight;
//                return checkMask;
            }
            potentialFirstAttacker = possibleKnight;
            checkMask |= possibleKnight;
        }
    }

    // pawns
    const Square& possiblePawn1 = kingPosition.move(Directions::SOUTH_EAST, flip(color));
    if (context.getPieceAt(possiblePawn1) == Pieces::pieces[PAWN][flip(color)]) {
        if (potentialFirstAttacker != Square::INVALID) {
            return 0;
//            checkMask.reset();
//            checkMask |= potentialFirstAttacker;
//            checkMask |= possiblePawn1;
//            return checkMask;
        }
        potentialFirstAttacker = possiblePawn1;
        checkMask |= possiblePawn1;
    }

    const Square& possiblePawn2 = kingPosition.move(Directions::SOUTH_WEST, flip(color));
    if (context.getPieceAt(possiblePawn2) == Pieces::pieces[PAWN][flip(color)]) {
        if (potentialFirstAttacker != Square::INVALID) {
            return 0;
//            checkMask.reset();
//            checkMask |= potentialFirstAttacker;
//            checkMask |= possiblePawn2;
//            return checkMask;
        }
        potentialFirstAttacker = possiblePawn2;
        checkMask |= possiblePawn2;
    }

    if (checkMask == 0) checkMask = ~checkMask;

    return checkMask;
}