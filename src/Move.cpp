//
// Created by kaappo on 25.11.2021.
//

#include "Move.h"
#include "Board.h"

using namespace PieceTypes;

Move::Move(const Board &context, const Square &from, const Square &to, const PieceType &pieceToPromoteTo) : move{0} {
    move |= from.getValue() << 10;
    move |= to.getValue() << 4;

    PieceColor color = context.getColorAt(from);

    Move previousMove{context.getHistory().getCurrentFrame().previousMove};
    bool isEnPassant = previousMove.isDoublePawnPush()
                       && previousMove.getOrigin().getX() == to.getX()
                       && from.diffX(to) == 1
                       && BitboardOperations::SquareMasks::rank5.asColor(color, false) & from
                       && context.is(PAWN, from)
                       && context.getPieceAt(Square{from.getY(), to.getX()}) == Pieces::pieces[PAWN][flip(color)];
    if (isEnPassant) {
        move |= Specials::EN_PASSANT;
    } else {
        move &= ~Specials::EN_PASSANT;
    }


    bool isCapture = isEnPassant or context.getColorAt(to) == flip(color);
    if (isCapture) {
        move |= MoveBitmasks::CAPTURE;
    }

    if (context.is(PAWN, from) && from.diffY(to) == 2) {
        move |= Specials::DOUBLE_PAWN_PUSH;

    }

    bool isPromotion =
            context.is(PAWN, from) && bool(BitboardOperations::SquareMasks::rank8.asColor(color, false) & to);
    if (isPromotion) {
        if (pieceToPromoteTo == NO_PIECE) {
            throw std::runtime_error("You must set pieceToPromote for a promotion move!");
        }
        move |= MoveBitmasks::PROMOTION;
        move |= pieceToPromoteTo;
    }

    if (context.is(KING, from) && from.diffX(to) == 2) {
        move |= from.getDirection(to, ROOK) == BitboardOperations::Directions::EAST ? Specials::KING_CASTLE
                                                                                    : Specials::QUEEN_CASTLE;
    }

}

Square Move::getOrigin() const {
    return (move >> 10) & 0b0000000000111111;
}

Square Move::getDestination() const {
    return (move >> 4) & 0b0000000000111111;
}

bool Move::isCapture() const {
    return move & MoveBitmasks::CAPTURE;
}

bool Move::isPromotion() const {
    return move & MoveBitmasks::PROMOTION;
}

bool Move::isDoublePawnPush() const {
    if (isPromotion() || isCapture()) return false;
    return (move & MoveBitmasks::SPECIAL_MASK) == Specials::DOUBLE_PAWN_PUSH;
}


bool Move::operator==(const Move &rhs) const {
    return move == rhs.move;
}

bool Move::operator!=(const Move &rhs) const {
    return !(rhs == *this);
}

std::string Move::toStringSimple () const {
    std::stringstream ss;
    ss << getOrigin() << getDestination();

    if (isPromotion()) {
        ss << getPromotedPiece().getSymbol(WHITE);
    }

    return ss.str();
}

std::ostream &operator<<(std::ostream &os, const Move &move) {
//    if (move.isCastling(Specials::KING_CASTLE)) return os << "O-O";
//    if (move.isCastling(Specials::QUEEN_CASTLE)) return os << "O-O-O";
    return os << move.toStringSimple();
}

PieceType Move::getPromotedPiece() const {
    if (isPromotion()) {
        return PieceTypes::pieces[move & 0b11];
    } else {
        return NO_PIECE;
    }
}

Move_raw Move::raw() const {
    return move;
}

Move::Move(Move_raw move) : move{move} {

}

const Piece &Move::getMovingPiece(const Board &context) const {
    const Piece &piece = context.getPieceAt(getOrigin());
    if (piece) return piece;

    const Piece &piece2 = context.getPieceAt(getDestination());
    if (piece2) return piece2;


    std::stringstream ss;
    ss << "Problem" << std::endl;
    ss << context << *this << std::endl;
    ss << context.toFEN() << std::endl;
    throw std::runtime_error(ss.str());
}

CastlingStatus Move::getNewCastlingStatus(const Board &context, const CastlingStatus &oldStatus,
                                          const Piece &possiblyCapturedPiece) const {
    CastlingStatus newStatus{oldStatus};

    const Piece &movingPiece = getMovingPiece(context);
    PieceColor color = movingPiece.color;
    if (isCastling(Specials::KING_CASTLE) || isCastling(Specials::QUEEN_CASTLE)) {
        newStatus.setCanCastle(color, Specials::KING_CASTLE, false);
        newStatus.setCanCastle(color, Specials::QUEEN_CASTLE, false);
    }

    if (movingPiece.type == ROOK) {
        if (getOrigin() == Square{h1}.asColorFlip(color)) newStatus.setCanCastle(color, Specials::KING_CASTLE, false);
        if (getOrigin() == Square{a1}.asColorFlip(color)) newStatus.setCanCastle(color, Specials::QUEEN_CASTLE, false);
    }

    if (possiblyCapturedPiece.type == ROOK) {
        if (getDestination() == Square{h1}.asColorFlip(flip(color))) {
            newStatus.setCanCastle(flip(color), Specials::KING_CASTLE, false);
        }
        if (getDestination() == Square{a1}.asColorFlip(flip(color))) {
            newStatus.setCanCastle(flip(color), Specials::QUEEN_CASTLE, false);
        }
    }

    if (movingPiece.type == KING) {
        if (getOrigin() == Square{e1}.asColorFlip(color)) {
            newStatus.setCanCastle(color, Specials::KING_CASTLE, false);
            newStatus.setCanCastle(color, Specials::QUEEN_CASTLE, false);
        }
    }

    return newStatus;
}

Move Move::fromString(std::string moveString, const Board &context) {
    if (moveString.length() < 4) {
        return Moves::NO_MOVE;
    }
    const Square &from = Square::fromString(moveString.substr(0, 2));
    const Square &to = Square::fromString(moveString.substr(2, 2));

    PieceType pieceToPromoteTo;
    if (moveString.size() == 5) {
        pieceToPromoteTo = Pieces::parsePiece(moveString.at(4)).type;
    } else {
        pieceToPromoteTo = NO_PIECE;
    }

    return {context, from, to, pieceToPromoteTo};
}

bool Move::isEnPassant() const {
    if (!isCapture()) return false;
    return (move & MoveBitmasks::SPECIAL_MASK) == Specials::EN_PASSANT;
}

bool Move::isCastling(CastlingSide SIDE) const {
    if (!isPromotion() && !isCapture()) {
        return (move & MoveBitmasks::SPECIAL_MASK) == SIDE;
    }
    return false;
}

Move::operator int() const {
    return move;
}

std::string Move::toShortAlgebraic(Board context) const {
    if (isCastling(Specials::KING_CASTLE)) {
        return "O-O";
    } else if (isCastling(Specials::QUEEN_CASTLE)) {
        return "O-O-O";
    }

    std::stringstream ss;
    MOVES legalMoves = context.getMoves(false);

    bool needsFileDisambiguation = false;
    bool needsRankDisambiguation = false;

    for (auto move: legalMoves) {
        if (move.getDestination() != getDestination() or move.getOrigin() == getOrigin()) continue;

        bool canAnotherPieceReach = move.getMovingPiece(context) == getMovingPiece(context);
        if (canAnotherPieceReach) {
            if (getOrigin().getX() != move.getOrigin().getX()) needsFileDisambiguation = true;
            else needsRankDisambiguation = true;
        }
    }
    bool isPawnMove = getMovingPiece(context).type == PAWN;

    if (not isPawnMove) {
        ss << getMovingPiece(context).type.getSymbol(WHITE);
    }

    if (needsFileDisambiguation or (isPawnMove and isCapture())) {
        ss << MyUtils::toString(getOrigin())[0];
    }

    if (needsRankDisambiguation and not isPawnMove) {
        ss << MyUtils::toString(getOrigin())[1];
    }

    if (isCapture()) {
        ss << "x";
    }

    ss << MyUtils::toString(getDestination());

    if (isPromotion()) {
        ss << "=";
        ss << getPromotedPiece().getSymbol(WHITE);
    }

    context.executeMove(*this);
    if (context.isCheckmate()) {
        ss << "#";
    } else if (context.isCheck()) {
        ss << "+";
    }

    return ss.str();
}


std::string MoveUtils::movesToString(const std::vector<Move> &moves, Board context) {
    if (moves.empty()) return "";
    std::stringstream ss;


    int startingFullMoveCount = context.getHistory().getCurrentFrame().fullMoveCount;
    int currentFullMoveCount = startingFullMoveCount;

    bool blackStartsSequence = context.getTurn() == BLACK;


//    std::cout << "Transforming " << MyUtils::toString(moves) << " on board " << context.toFEN() << std::endl;



    try {
        if (blackStartsSequence) {
            ss << startingFullMoveCount << ". ";
            ss << "... ";
            ss << moves[0].toShortAlgebraic(context) << " ";
            currentFullMoveCount += 1;
            context.executeMove(moves[0]);


            for (size_t i = 1; i < moves.size(); i++) {
                auto move = moves[i];
                if (i % 2 == 1) {
                    ss << currentFullMoveCount << ". ";
                    currentFullMoveCount += 1;
                }

                ss << move.toShortAlgebraic(context) << " ";
                context.executeMove(move);
            }
        } else {
            for (size_t i = 0; i < moves.size(); i++) {
                auto move = moves[i];
                if (i % 2 == 0) {
                    ss << currentFullMoveCount << ". ";
                    currentFullMoveCount += 1;
                }

                ss << move.toShortAlgebraic(context) << " ";
                context.executeMove(move);
            }
        }
    } catch (...) {
        ss << "<illegal (TT shenanigans)>";
    }


    return ss.str();
}

