//
// Created by kaappo on 8.1.2022.
//

#include "UI.h"

#include <utility>
#include <algorithm>

void TtyUI::updateValues (Board board) {
    this->board = board;
    std::cout << (board.getTurn() == PieceColor::WHITE ? "WHITE" : "BLACK") << std::endl;
    std::cout << board << std::endl;
    std::cout << board.toFEN() << std::endl;
    std::cout << "Possible moves: " << MyUtils::toString(board.getMoves()) << std::endl;
}

Move TtyUI::getMove () {

    while (true) {
        std::cout << "Please enter move >> ";
        std::string moveString;

        std::cin >> moveString;

        try {

            const Move& move = Move::fromString(moveString, board);
            MOVES legalMoves = board.getMoves();
            if (std::find(legalMoves.begin(), legalMoves.end(), move) != legalMoves.end()) {
                return move;
            } else {
                continue;
            }
        } catch (...) {
            continue;
        }
    }
}

TtyUI::TtyUI (Board board) : board(std::move(board)) {}

