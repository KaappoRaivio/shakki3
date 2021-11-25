//
// Created by kaappo on 25.11.2021.
//

#include "Move.h"

Move::Move (const Board& context, Square from, Square to, PieceType pieceToPromoteTo= KING) : move{0} {
    move |= from.getValue() << 10;
    move |= to.getValue() << 4;

    PieceColor color = context.getColorAt(from);

    bool isCapture = context.getColorAt(to) == flip(color);
    if (isCapture) {
        move |= 1 << MoveBitmasks::CAPTURE;
        if (context.is(PAWN, from)) {

        }
    }
    move |= 0 << MoveBitmasks::EN_PASSANT;

    bool isPromotion = bool(Bitboard{to} & BitboardOperations::SquareMasks::rank8.asColor(color));
    if (isPromotion) {
        if (pieceToPromoteTo == KING) {
            throw std::runtime_error("You must set pieceToPromote for a promotion move!");
        }
        move |= 1 << MoveBitmasks::PROMOTION;
        move |= pieceToPromoteTo;
    }
}
