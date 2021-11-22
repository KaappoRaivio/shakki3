//
// Created by kaappo on 20.11.2021.
//

#include <iostream>
#include "PieceSet.h"



//PieceSet::PieceSet () {
//    rooks   = 0b10000001;
//    knights = 0b01000010;
//    bishops = 0b00100100;
//    queen   = 0b00001000;
//    king    = 0b00010000;
//    pawns   = 0b1111111100000000;
//
//    if (color == PieceColor::BLACK) {
//        rooks ^= 63;
//        knights ^= 63;
//        bishops ^= 63;
//        queen ^= 63;
//        king ^= 63;
//        pawns ^= 63;
//    }
//}

//const std::string& PieceSet::burn (std::string& buffer) const {
//    buffer[rooks & (-rooks)] = 'r';
//    return buffer;
//}

PieceSet::PieceSet (const PieceColor& color) : color{color} {
    rooks   = 0b10000001;
    knights = 0b01000010;
    bishops = 0b00100100;
    queen   = 0b00001000;
    king    = 0b00010000;
    pawns   = 0b1111111100000000;

    if (color == PieceColor::BLACK) {
        rooks = BitboardOperations::flipVertical(rooks);
        knights = BitboardOperations::flipVertical(knights);
        bishops = BitboardOperations::flipVertical(bishops);
        queen = BitboardOperations::flipVertical(queen);
        king = BitboardOperations::flipVertical(king);
        pawns = BitboardOperations::flipVertical(pawns);
    }
}




std::string& PieceSet::burn (std::string& buffer) const {
    BitboardOperations::burnBitboard(buffer, rooks, color == WHITE ? 'r' : 'R');
    BitboardOperations::burnBitboard(buffer, knights, color == WHITE ? 'n' : 'N');
    BitboardOperations::burnBitboard(buffer, bishops, color == WHITE ? 'b' : 'B');
    BitboardOperations::burnBitboard(buffer, queen, color == WHITE ? 'q' : 'Q');
    BitboardOperations::burnBitboard(buffer, king, color == WHITE ? 'k' : 'K');
    BitboardOperations::burnBitboard(buffer, pawns, color == WHITE ? 'p' : 'P');

    return buffer;
}

std::ostream& operator<< (std::ostream& os, const PieceSet& set) {
    os << "color: " << set.color;
    return os;
}
