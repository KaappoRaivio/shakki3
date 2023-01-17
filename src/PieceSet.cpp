//
// Created by kaappo on 20.11.2021.
//

#include <iostream>
#include "PieceSet.h"

using namespace PieceTypes;

PieceSet::PieceSet (const PieceColor& color) : color{color}, boards{}, all{} {
    boards[ROOK]   = 0b10000001;
    boards[KNIGHT] = 0b01000010;
    boards[BISHOP] = 0b00100100;
    boards[QUEEN]  = 0b00001000;
    boards[KING]   = 0b00010000;
    boards[PAWN]  = 0b1111111100000000;

    updateOccupancy();

    if (color == PieceColor::BLACK) {
        for (auto & board : boards) {
            board = board.flipVertical();
        }

        all = all.flipVertical();
    }
}




std::string& PieceSet::burn (std::string& buffer) const {
//    BitboardOperations::burnBitboard(buffer, rooks, color == WHITE ? 'r' : 'R');
//    BitboardOperations::burnBitboard(buffer, knights, color == WHITE ? 'n' : 'N');
//    BitboardOperations::burnBitboard(buffer, bishops, color == WHITE ? 'b' : 'B');
//    BitboardOperations::burnBitboard(buffer, queen, color == WHITE ? 'q' : 'Q');
//    BitboardOperations::burnBitboard(buffer, king, color == WHITE ? 'k' : 'K');
//    BitboardOperations::burnBitboard(buffer, pawns, color == WHITE ? 'p' : 'P');
    for (int i = 0; i < AMOUNT_OF_BOARDS; ++i) {
        boards[i].burnTo(buffer, PieceTypes::pieces[i].getSymbol(color));
    }

//    rooks.burnTo(buffer, color == WHITE ? 'r' : 'R');
//    knights.burnTo(buffer, color == WHITE ? 'n' : 'N');
//    bishops.burnTo(buffer, color == WHITE ? 'b' : 'B');
//    queen.burnTo(buffer, color == WHITE ? 'q' : 'Q');
//    king.burnTo(buffer, color == WHITE ? 'k' : 'K');
//    pawns.burnTo(buffer, color == WHITE ? 'p' : 'P');

    return buffer;
}

std::ostream& operator<< (std::ostream& os, const PieceSet& set) {
    os << "color: " << set.color;
    return os;
}

bool PieceSet::hasPiece (Square square) const {
    return bool((all | boards[KING]) & square);
}

void PieceSet::updateOccupancy () {
    all ^= all;
    for (auto board : boards) {
        all |= board;
    }
    all ^= boards[KING];
}

Bitboard PieceSet::allWithoutKing () const {
    return all ^ boards[KING];
}
