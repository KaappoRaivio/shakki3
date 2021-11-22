//
// Created by kaappo on 20.11.2021.
//

#include <iostream>
#include "Board.h"

Board::Board () : white{PieceColor::WHITE}, black{PieceColor::BLACK} {}

std::ostream& operator<< (std::ostream& os, const Board& board) {
    std::string base{"................................................................"};
    board.white.burn(base);
    board.black.burn(base);

    for (int y = 7; y >= 0; --y) {
        for (int x = 0; x < 8; ++x) {
            os << base[8 * y + x] << " ";
        }
        os << std::endl;
    }

    return os;
}

