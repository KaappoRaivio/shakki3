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

    Move previousMove {context.getHistory().getCurrentFrame().previousMove};
    bool isEnPassant = previousMove.isDoublePawnPush()
                       && previousMove.getOrigin().getX() == to.getX()
                       && from.diffX(to) == 1
                       && BitboardOperations::SquareMasks::rank5.asColor(color, false) & from
                       && context.is(PAWN, from)
                       && context.getPieceAt(Square{from.getY(), to.getX()}) == Pieces::pieces[PAWN][flip(color)];
    if (isEnPassant) {
        move |= MoveBitmasks::EN_PASSANT;
    } else {
        move &= ~MoveBitmasks::EN_PASSANT;
    }


    bool isCapture = context.getColorAt(to) == flip(color) || isEnPassant;
    if (isCapture) {
        move |= MoveBitmasks::CAPTURE;
    }

    if (context.is(PAWN, from) && from.diffY(to) == 2) {
        move |= MoveBitmasks::DOUBLE_PAWN_PUSH;

    }

    bool isPromotion = context.is(PAWN, from) && bool(BitboardOperations::SquareMasks::rank8.asColor(color, false) & to);
    if (isPromotion) {
        if (pieceToPromoteTo == NO_PIECE) {
            throw std::runtime_error("You must set pieceToPromote for a promotion move!");
        }
        move |= MoveBitmasks::PROMOTION;
        move |= pieceToPromoteTo;
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
    return move & MoveBitmasks::CAPTURE;
}

bool Move::isPromotion () const {
    return move & MoveBitmasks::PROMOTION;
}

bool Move::isDoublePawnPush () const {
    if (isPromotion() || isCapture()) return false;
    return move & MoveBitmasks::DOUBLE_PAWN_PUSH;
}



bool Move::operator== (const Move& rhs) const {
    return move == rhs.move;
}

bool Move::operator!= (const Move& rhs) const {
    return !(rhs == *this);
}

std::ostream& operator<< (std::ostream& os, const Move& move) {
//    if (move.isCastling(MoveBitmasks::KING_CASTLE)) return os << "O-O";
//    if (move.isCastling(MoveBitmasks::QUEEN_CASTLE)) return os << "O-O-O";
    os << move.getOrigin() << move.getDestination();

    if (move.isPromotion()) {
        os << move.getPromotedPiece().getSymbol(WHITE);
    }

    return os;
}

PieceType Move::getPromotedPiece () const {
    if (isPromotion()) {
        return PieceTypes::pieces[move & 0b11];
    } else {
        return NO_PIECE;
    }
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

CastlingStatus Move::getNewCastlingStatus (const Board& context, const CastlingStatus& oldStatus, const Piece& possiblyCapturedPiece) const {
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

    if (possiblyCapturedPiece.type == ROOK) {
        if (getDestination() == Square{h1}.asColorFlip(flip(color))) {
            newStatus.setCanCastle(flip(color), MoveBitmasks::KING_CASTLE, false);
        }
        if (getDestination() == Square{a1}.asColorFlip(flip(color))) {
            newStatus.setCanCastle(flip(color), MoveBitmasks::QUEEN_CASTLE, false);
        }
    }

    if (movingPiece.type == KING) {
        if (getOrigin() == Square{e1}.asColorFlip(color)) {
            newStatus.setCanCastle(color, MoveBitmasks::KING_CASTLE, false);
            newStatus.setCanCastle(color, MoveBitmasks::QUEEN_CASTLE, false);
        }
    }

    return newStatus;
}

Move Move::fromString (std::string moveString, const Board& context) {
    if (moveString.length() < 4) {
        return Moves::NO_MOVE;
    }
    const Square& from = Square::fromString(moveString.substr(0, 2));
    const Square& to = Square::fromString(moveString.substr(2, 2));

    PieceType pieceToPromoteTo;
    if (moveString.size() == 5) {
        pieceToPromoteTo = Pieces::parsePiece(moveString.at(4)).type;
    } else {
        pieceToPromoteTo = NO_PIECE;
    }

    return {context, from, to, pieceToPromoteTo};
}

bool Move::isEnPassant () const {
    if (!isCapture()) return false;
    return move & MoveBitmasks::EN_PASSANT;
}
