//
// Created by kaappo on 8.1.2022.
//

#include "Player.h"

Move HumanPlayer::getMove (Board board) {
    return ui->getMove();
}

HumanPlayer::HumanPlayer (UI* ui) : ui{ui} {}

bool HumanPlayer::isHumanPlayer() {
    return true;
}

Move AIPlayer::getMove (Board board) {
    MOVES principalVariation;
    std::cout << "Calculating move!" << std::endl;
    return search.getBestMove(board, searchDepth, allowedTime);
}

Search& AIPlayer::getSearch () {
    return search;
}



bool AIPlayer::isHumanPlayer() {
    return false;
}
