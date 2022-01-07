//
// Created by kaappo on 17.12.2021.
//

#pragma once

#include "mytypes.h"
#include "Board.h"
#include "Attacks.h"
#include "InBetween.h"

namespace BoardAnalysis {
    bool isSquareAttacked (const Board& board, const Square& square, PieceColor color);

    Bitboard getAttackMask (const Board& board, PieceColor color);

    template<PinType TYPE>
    Bitboard getPinMask (const Board& context, PieceColor color) {
        const Square& kingPosition = context.getPieces(color).boards[PieceTypes::KING].ls1b();

        Bitboard pinned = 0;

        if (TYPE == HV || TYPE == BOTH) {

            //rooks
            Bitboard rookPinners = Attacks::getInstance()
                                           .getRookAttacks()
                                           .getRaysToAllDirectionsXRay(context, kingPosition, color)
                                   & (context.getPieces(flip(color)).boards[PieceTypes::ROOK] | context.getPieces(flip(color)).boards[PieceTypes::QUEEN]);

            for (const Square& pinnerSquare: rookPinners) {
                pinned |= InBetween::getInstance().getPath(pinnerSquare, kingPosition);
            }
        }
        if (TYPE == D12 || TYPE == BOTH) {
            //bishops
            Bitboard bishopPinners = Attacks::getInstance()
                                             .getBishopAttacks()
                                             .getRaysToAllDirectionsXRay(context, kingPosition, color)
                                     & (context.getPieces(flip(color)).boards[PieceTypes::BISHOP] | context.getPieces(flip(color)).boards[PieceTypes::QUEEN]);

            for (const Square& pinnerSquare: bishopPinners) {
                pinned |= InBetween::getInstance().getPath(pinnerSquare, kingPosition);
            }
        }

        return pinned;
    }

    Bitboard getCheckMask (const Board& context, PieceColor color);
    bool isCheck(const Board& context);
}