//
// Created by kaappo on 8.1.2022.
//

#include "Player.h"

Move HumanPlayer::getMove (Board board) {
    return ui->getMove();
}

HumanPlayer::HumanPlayer (UI* ui) : ui{ui} {}

Move AIPlayer::getMove (Board board) {
    std::vector<Move> principalVariation;
    std::cout << "Calculating move!" << std::endl;
    return search.getBestMove(board, searchDepth);
}


AIPlayer::AIPlayer (int searchDepth) : search{}, searchDepth{searchDepth} {}
