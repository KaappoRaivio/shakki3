//
// Created by kaappo on 29.11.2021.
//

#include <sstream>
#include "mytypes.h"
#include "TerminalColors.h"


//constexpr PieceType::PieceType (const char& symbol, const uint8_t& index) : symbol{symbol}, index{index} {
////    this->symbol = symbol;
////    this->index = index;
//}

char PieceType::getSymbol (PieceColor color) const {
    if (color == BLACK) {
        return std::tolower(symbol);
    } else if (color == WHITE) {
        return std::toupper(symbol);
    } else {
        return '.';
    }
}




std::string PieceType::getSymbolFancy (PieceColor color) const {
//    return fancySymbols[color];
    return "lol";
}


PieceType::operator int () const {
    return index;
}


std::ostream& operator<< (std::ostream& os, const PieceType& type) {
    os << "PieceType{symbol: " << type.symbol << " index: " << +type.index << "}";
    return os;
}


bool PieceType::operator== (const PieceType& rhs) const {
    return symbol == rhs.symbol &&
           index == rhs.index;
}


bool PieceType::operator!= (const PieceType& rhs) const {
    return !(rhs == *this);
}


namespace PieceTypes {
//    const PieceType KNIGHT{'n', 0};
//    const PieceType BISHOP{'b', 1};
//    const PieceType ROOK  {'r', 2};
//    const PieceType QUEEN {'q', 3};
//    const PieceType PAWN  {'p', 4};
//    const PieceType KING  {'k', 5};
//    const PieceType NO_PIECE {'.', 255};
//    const std::vector<PieceType> pieces = {KNIGHT, BISHOP, ROOK, QUEEN, PAWN, KING};
}

namespace Pieces {
    Piece NO_PIECE {PieceTypes::NO_PIECE, EMPTY};

    Piece parsePiece (char asChar) {
        const PieceType* type = nullptr;

        switch (tolower(asChar)) {
            case 'p':
                type = &PieceTypes::PAWN;
                break;
            case 'r':
                type = &PieceTypes::ROOK;
                break;
            case 'n':
                type = &PieceTypes::KNIGHT;
                break;
            case 'b':
                type = &PieceTypes::BISHOP;
                break;
            case 'q':
                type = &PieceTypes::QUEEN;
                break;
            case 'k':
                type = &PieceTypes::KING;
                break;
            default:
                std::cerr << "invalid character" << asChar << "!" << std::endl;
                throw std::runtime_error("invalid character!");
        }

        PieceColor color;
        if (islower(asChar)) {
            color = BLACK;
        } else {
            color = WHITE;
        }

        return {*type, color};
    }
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
//    os << "Piece{TYPE: " << piece.TYPE << " color: " << piece.color << "}";
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
