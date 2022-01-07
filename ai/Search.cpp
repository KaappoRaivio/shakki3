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

    if (moves.empty()) {
        if (positionToSearch.isCheck()) {
            return EvaluationConstants::LOSE;
        } else {
            return EvaluationConstants::DRAW;
        }
    }

    if (depth == 0) {
//        return BoardEvaluator::evaluateSimple(positionToSearch);
        return quiescenceSearch(positionToSearch, alpha, beta);
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

    return moves[std::distance(values.begin(), std::max_element(values.begin(), values.end()))];
}

int Search::quiescenceSearch (Board& positionToSearch, int alpha, int beta) {
    int standing_pat = BoardEvaluator::evaluateSimple(positionToSearch);
    if (standing_pat >= beta) { return beta; }
    if (standing_pat > alpha) { alpha = standing_pat; }


    for (const Move& captureMove : positionToSearch.getMoves(true)) {
        positionToSearch.executeMove(captureMove);
        int score = -quiescenceSearch(positionToSearch, -beta, -alpha);
        if (score >= beta) {
            return beta;
        } else if (score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}
