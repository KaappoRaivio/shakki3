//
// Created by kaappo on 29.11.2021.
//

#include "mytypes.h"


PieceType::PieceType (char symbol, uint8_t index) : symbol{symbol}, index{index} {}

char PieceType::getSymbol (PieceColor color) const {
    if (color == WHITE) {
        return std::tolower(symbol);
    } else if (color == BLACK) {
        return std::toupper(symbol);
    } else {
        return '.';
    }
}

PieceType::operator int () const {
    return index;
}

std::ostream& operator<< (std::ostream& os, const PieceType& type) {
    os << "PieceType{symbol: " << type.symbol << " index: " << (int) type.index << "}";
    return os;
}

namespace PieceTypes {
    const PieceType KNIGHT{'n', 0};
    const PieceType BISHOP{'b', 1};
    const PieceType ROOK  {'r', 2};
    const PieceType QUEEN {'q', 3};
    const PieceType PAWN  {'p', 4};
    const PieceType KING  {'k', 5};
    const PieceType NO_PIECE {'.', 255};
    const std::vector<PieceType> pieces = {KNIGHT, BISHOP, ROOK, QUEEN, PAWN, KING};
}

namespace Pieces {
    Piece NO_PIECE {PieceTypes::NO_PIECE, EMPTY};
}

Piece::Piece (PieceType type, PieceColor color) : type{type}, color{color} {
    if (((type == PieceTypes::NO_PIECE) ^ (color == EMPTY)) != 0) {
        throw std::runtime_error("Must be either both NO_PIECE and EMPTY, or none");
    }
}

bool Piece::operator== (const Piece& rhs) const {
    return type == rhs.type &&
           color == rhs.color;
}

bool Piece::operator!= (const Piece& rhs) const {
    return !(rhs == *this);
}

std::ostream& operator<< (std::ostream& os, const Piece& piece) {
    os << piece.type.getSymbol(piece.color);
    return os;
//    os << "Piece{type: " << piece.type << " color: " << piece.color << "}";
//    return os;
}

PieceColor flip (const PieceColor color) {
    switch (color) {
        case WHITE:
            return BLACK;
        case BLACK:
            return WHITE;
        case EMPTY:
            std::cerr << "Probably shouldn't invert EMPTY color!" << std::endl;
            return EMPTY;
        default:
            throw std::runtime_error("Shouldn't happen!");
    }
}

Piece::operator bool () const {
    return *this != Pieces::NO_PIECE;
}
