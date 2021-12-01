//
// Created by kaappo on 25.11.2021.
//

#include "Move.h"

using namespace PieceTypes;

Move::Move (const Board& context, Square from, Square to, PieceType pieceToPromoteTo) : move{0} {
//    std::cout << from << ", " << to << std::endl;
//    std::cout << context << std::endl;
    move |= from.getValue() << 10;
    move |= to.getValue() << 4;

    PieceColor color = context.getColorAt(from);

    bool isCapture = context.getColorAt(to) == flip(color);
    if (isCapture) {
        move |= MoveBitmasks::CAPTURE;
    }

    if (context.is(PAWN, from) && from.diffY(to) == 2) {
        move |= MoveBitmasks::DOUBLE_PAWN_PUSH;
    }

    move &= ~MoveBitmasks::EN_PASSANT;

    bool isPromotion = bool(Bitboard{to} & BitboardOperations::SquareMasks::rank8.asColor(color));
    if (isPromotion) {
        if (pieceToPromoteTo == KING) {
            throw std::runtime_error("You must set pieceToPromote for a promotion move!");
        }
        move |= MoveBitmasks::PROMOTION;
        move |= pieceToPromoteTo;
    }
}

Square Move::getOrigin () const {
    return (move >> 10) & 0b0000000000111111;
}

Square Move::getDestination () const {
    return (move >> 4) & 0b0000000000111111;
}

bool Move::isCapture () const {
    return move & (1 << MoveBitmasks::CAPTURE);
}

Move::Move (bool NO_MOVE) {
    if (NO_MOVE) {
        move = 0xffffull;
    } else {
        throw std::runtime_error("NO_MOVE must be true!");
    }
}

bool Move::operator== (const Move& rhs) const {
    return move == rhs.move;
}

bool Move::operator!= (const Move& rhs) const {
    return !(rhs == *this);
}

std::ostream& operator<< (std::ostream& os, const Move& move) {
    os << "Move{from:" << move.getOrigin() << ", to:" << move.getDestination() << "}";
    return os;
}

Move_raw Move::raw () const {
    return move;
}

Move::Move (Move_raw move) : move{move} {

}

Move::Move (const Move& other) = default;

namespace Moves {
    Move NO_MOVE{true};
}
