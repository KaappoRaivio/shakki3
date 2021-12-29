//
// Created by kaappo on 25.11.2021.
//

#include "Move.h"
#include "Board.h"

using namespace PieceTypes;

Move::Move (const Board& context, const Square& from, const Square& to, const PieceType& pieceToPromoteTo) : move{0} {
    move |= from.getValue() << 10;
    move |= to.getValue() << 4;

    PieceColor color = context.getColorAt(from);

    bool isCapture = context.getColorAt(to) == flip(color);
    if (isCapture) {
        move |= MoveBitmasks::CAPTURE;
    }

    if (context.is(PAWN, from) && from.diffY(to) == 2) {
        move |= MoveBitmasks::DOUBLE_PAWN_PUSH;

        bool isPromotion = bool(BitboardOperations::SquareMasks::rank8.asColor(color, false) & to);
        if (isPromotion) {
            if (pieceToPromoteTo == NO_PIECE) {
                throw std::runtime_error("You must set pieceToPromote for a promotion move!");
            }
            move |= MoveBitmasks::PROMOTION;
            move |= pieceToPromoteTo;
        }
    }

    if (context.is(KING, from) && from.diffX(to) == 2) {
        move |= from.getDirection(to, ROOK) == BitboardOperations::Directions::EAST ? MoveBitmasks::KING_CASTLE : MoveBitmasks::QUEEN_CASTLE;
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

bool Move::isPromotion () const {
    return move & (1 << MoveBitmasks::PROMOTION);
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
    if (move.isCastling(MoveBitmasks::KING_CASTLE)) return os << "O-O";
    if (move.isCastling(MoveBitmasks::QUEEN_CASTLE)) return os << "O-O-O";
    return os << move.getOrigin() << move.getDestination();

}

Move_raw Move::raw () const {
    return move;
}

Move::Move (Move_raw move) : move{move} {

}

const Piece& Move::getMovingPiece (const Board& context) const {
    const Piece& piece = context.getPieceAt(getOrigin());
    if (piece) return piece;

    const Piece& piece2 = context.getPieceAt(getDestination());
    if (piece2) return piece2;

    std::cerr << context << *this << std::endl;
    throw std::runtime_error("Problem");
}

CastlingStatus Move::getNewCastlingStatus (const Board& context, const CastlingStatus& oldStatus) const {
    CastlingStatus newStatus{oldStatus};

    const Piece& movingPiece = getMovingPiece(context);
    PieceColor color = movingPiece.color;
    if (isCastling(MoveBitmasks::KING_CASTLE) || isCastling(MoveBitmasks::QUEEN_CASTLE)) {
        newStatus.setCanCastle(color, MoveBitmasks::KING_CASTLE, false);
        newStatus.setCanCastle(color, MoveBitmasks::QUEEN_CASTLE, false);
    }

    if (movingPiece.type == ROOK) {
        if (getOrigin() == Square{h1}.asColorFlip(color)) newStatus.setCanCastle(color, MoveBitmasks::KING_CASTLE, false);
        if (getOrigin() == Square{a1}.asColorFlip(color)) newStatus.setCanCastle(color, MoveBitmasks::QUEEN_CASTLE, false);
    }

    return newStatus;
}


namespace Moves {
    Move NO_MOVE{true};
}
