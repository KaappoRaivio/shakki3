//
// Created by kaappo on 6.1.2022.
//

#include "Search.h"
#include "BoardEvaluator.h"
#include "TranspositionTable.h"

int Search::negamaxSearch (Board positionToSearch, int depth) {
    using namespace EvaluationConstants;
    return negamaxSearch(positionToSearch, depth, LOSE, WIN);
}

int Search::negamaxSearch (Board& positionToSearch, int depth, int alpha, int beta) {
    int originalAlpha = alpha;

    if (table.hasEntry(positionToSearch, depth)) {
        const TranspositionTableEntry& entry = table.getEntry(positionToSearch);
        switch (entry.hitType) {
            case TranspositionTableHitType::UPPER_BOUND:
                beta = std::min(beta, entry.positionValue);
                break;
            case TranspositionTableHitType::LOWER_BOUND:
                alpha = std::max(alpha, entry.positionValue);
                break;
            case TranspositionTableHitType::EXACT:
                return entry.positionValue;
        }
    }

    std::vector<Move> moves = positionToSearch.getMoves();

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

    TranspositionTableHitType hitType = positionValue <= originalAlpha ?
                                        TranspositionTableHitType::UPPER_BOUND : positionValue >= beta ?
                                                                                 TranspositionTableHitType::LOWER_BOUND :
                                                                                 TranspositionTableHitType::EXACT;

    TranspositionTableEntry entry {hitType, static_cast<uint8_t>(depth), positionValue};
    table.store(positionToSearch, entry);

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


    const std::vector<Move>& captureMoves = positionToSearch.getMoves(true);
    if (captureMoves.empty()) {
        if (positionToSearch.isCheck()) {
            return EvaluationConstants::LOSE;
        } else {
            return EvaluationConstants::DRAW;
        }
    }
    for (const Move& captureMove : captureMoves) {
        positionToSearch.executeMove(captureMove);
        int score = -quiescenceSearch(positionToSearch, -beta, -alpha);
        positionToSearch.unmakeMove();
        if (score >= beta) {
            return beta;
        } else if (score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}
