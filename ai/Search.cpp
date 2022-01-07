//
// Created by kaappo on 6.1.2022.
//

#include "Search.h"
#include "BoardEvaluator.h"

int Search::negamaxSearch (Board positionToSearch, int depth) {
    using namespace EvaluationConstants;
    return negamaxSearch(positionToSearch, depth, LOSE, WIN);
}

int Search::negamaxSearch (Board& positionToSearch, int depth, int alpha, int beta) {
    const std::vector<Move>& moves = positionToSearch.getMoves();

    if (moves.size() == 0) {
        if (positionToSearch.isCheck()) {
            return EvaluationConstants::LOSE;
        } else {
            return EvaluationConstants::DRAW;
        }
    }

    if (depth == 0) {
        return BoardEvaluator::evaluateSimple(positionToSearch);
    }



    int positionValue = EvaluationConstants::LOSE;
    for (const auto& move : moves) {
        positionToSearch.executeMove(move);
        positionValue = std::max(positionValue, -negamaxSearch(positionToSearch, depth - 1, -beta, -alpha));
        positionToSearch.unmakeMove();

        alpha = std::max(alpha, positionValue);
        if (alpha >= beta) {
            break;
        }
    }

    return positionValue;
}

Move Search::getBestMove (Board position, int searchDepth) {
    const auto& moves = position.getMoves();

    std::vector<int> values;

    for (auto move : moves) {
        position.executeMove(move);
        values.push_back(-negamaxSearch(position, searchDepth - 1));
        position.unmakeMove();
    }

    return moves[std::distance(values.begin(),std::max_element(values.begin(), values.end()))];
}
