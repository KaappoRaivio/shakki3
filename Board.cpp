//
// Created by kaappo on 20.11.2021.
//

#include <iostream>
#include "Board.h"

Board::Board () : pieces{PieceSet{WHITE}, PieceSet{BLACK}} {}

std::ostream& operator<< (std::ostream& os, const Board& board) {
    std::string base{"................................................................"};
    board.pieces[WHITE].burn(base);
    board.pieces[BLACK].burn(base);

    for (int y = 7; y >= 0; --y) {
        for (int x = 0; x < 8; ++x) {
            os << base[8 * y + x] << " ";
        }
        os << std::endl;
    }

    return os;
}

PieceSet Board::getPieceSet (PieceColor color) const {
    if (color == EMPTY) throw std::runtime_error("No EMPTY in Board::getPieceSet!!");
    return pieces[color];
}

PieceColor Board::getColorAt (Square square) const {
    if (pieces[WHITE].hasPiece(square)) return WHITE;
    else if (pieces[BLACK].hasPiece(square)) return BLACK;
    else return EMPTY;
}

const PieceSet& Board::getWhite () const {
    return pieces[WHITE];
}

const PieceSet& Board::getBlack () const {
    return pieces[BLACK];
}

bool Board::is (PieceType type, Square at) const {
    PieceColor color = getColorAt(at);
    return bool(pieces[color].boards[type] & Bitboard{at});
}

