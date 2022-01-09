//
// Created by kaappo on 6.1.2022.
//

#include "Search.h"
#include "BoardEvaluator.h"
#include "TranspositionTable.h"
#include "../BoardAnalysis.h"

#define USE_TT 0
#define ORDER_MOVES 0


int Search::negamaxSearch (Board positionToSearch, int depth, std::vector<Move>& principalVariation) {
    using namespace EvaluationConstants;
    return negamaxSearch(positionToSearch, depth, LOSE, WIN, principalVariation);
}

int Search::negamaxSearch (Board& positionToSearch, int depth, int alpha, int beta, std::vector<Move>& principalVariation) {
    ++nodesSearched;
    int originalAlpha = alpha;

#if USE_TT
    if (table.hasEntry(positionToSearch, depth)) {
        ++tableHits;
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

        if (alpha >= beta) {
//            ++tableHits;
            return entry.positionValue;
        }
    }
#endif


    std::vector<Move> moves = positionToSearch.getMoves();

    if (moves.empty()) {
        if (positionToSearch.isCheck()) {
//            std::cout << "CHECKMATE!" << std::endl;
//            return EvaluationConstants::LOSE;
            return EvaluationConstants::LOSE * (depth + 1);
        } else {
            return EvaluationConstants::DRAW;
        }
    }

    if (depth == 0) {
        return BoardEvaluator::evaluateSimple(positionToSearch, depth);
//        return quiescenceSearch(positionToSearch, alpha, beta);
    }

#if ORDER_MOVES
    orderMoves(positionToSearch, moves, depth);
#endif

    int positionValue = EvaluationConstants::LOSE;
    size_t bestMoveIndex = -1;
    for (size_t moveIndex = 0; moveIndex < moves.size(); ++moveIndex) {
        std::vector<Move> childPrincipalVariation;

        auto move = moves[moveIndex];


        positionToSearch.executeMove(move);
        int newValue = -negamaxSearch(positionToSearch, depth - 1, -beta, -alpha, childPrincipalVariation);
        positionToSearch.unmakeMove();




        if (newValue > positionValue) {
            positionValue = newValue;
            bestMoveIndex = moveIndex;

//            principalVariation.clear();
//            principalVariation.push_back(move);
        }

//        if (positionValue > alpha && positionValue < beta) {
//            principalVariation.clear();
//            principalVariation.push_back(move);
//            std::copy(childPrincipalVariation.begin(), childPrincipalVariation.end(), std::back_inserter(principalVariation));
//        }

        alpha = std::max(alpha, positionValue);
        if (alpha >= beta) {
            ++cutoffs;
            break;
        }
    }

#if USE_TT
    TranspositionTableHitType hitType = positionValue <= originalAlpha ?
                                        TranspositionTableHitType::UPPER_BOUND : positionValue >= beta ?
                                                                                 TranspositionTableHitType::LOWER_BOUND :
                                                                                 TranspositionTableHitType::EXACT;


    TranspositionTableEntry entry{hitType, depth, positionValue, moves[bestMoveIndex]};
    table.store(positionToSearch, entry);
#endif


    return positionValue;
}

Move Search::getBestMove (Board position, int searchDepth) {
    Move bestMoveSoFar = Moves::NO_MOVE;
    for (int depth = 1 ; depth <= searchDepth ; ++depth) {
        std::cout << "Iterative deepening for depth: " << depth << std::endl;


        std::vector<Move> principalVariation;
        bestMoveSoFar = getMove(position, depth, principalVariation);
        std::cout << "\tBest move so far: " << bestMoveSoFar << std::endl;

        std::cout << "\tPrincipal variation: " <<  MyUtils::toString(principalVariation) << std::endl;
        principalVariation.clear();
    }

    return bestMoveSoFar;

}

Move Search::getMove (Board& position, int searchDepth, std::vector<Move>& principalVariation) {
    const auto& moves = position.getMoves();

    std::vector<int> values;
    std::vector<std::vector<Move>> principalVariations;
    nodesSearched = 0;
    tableHits = 0;
    cutoffs = 0;
    table.hits = 0;
    table.collisions = 0;

    for (size_t index = 0; index < moves.size(); ++index) {
        position.executeMove(moves[index]);

        if (MyUtils::toString(moves[index]) == "D6D8") {
            std::cout << "Debug" << std::endl;
        }

        principalVariations.emplace_back();
        values.push_back(
                -negamaxSearch(position, searchDepth - 1, principalVariations[index])
                );
        position.unmakeMove();
    }
    long index = std::distance(values.begin(), std::max_element(values.begin(), values.end()));

    std::cout << "\tTable hits: " << tableHits << std::endl;
    std::cout << "\tNodes searched " << nodesSearched << std::endl;
    std::cout << "\tCutoffs " << cutoffs << std::endl;
    std::cout << "\tCollisions " << table.collisions << std::endl;
    std::cout << "\tHits " << table.hits << std::endl;

    std::cout << "\tBest move value: " << values[index] << std::endl;
    table.store(position, TranspositionTableEntry{TranspositionTableHitType::EXACT, searchDepth, values[index], moves[index]});
    principalVariation.clear();
    principalVariation.push_back(moves[index]);
    std::copy(principalVariations[index].begin(), principalVariations[index].end(), std::back_inserter(principalVariation));

    return moves[index];
}

int Search::quiescenceSearch (Board& positionToSearch, int alpha, int beta) {
    int standing_pat = BoardEvaluator::evaluateSimple(positionToSearch, 0);
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


// MVV_LVA[victim][attacker]
constexpr int MVV_LVA[7][7] = {
        {0,  0,   0,   0,   0,   0,   0},       // victim K, attacker K, Q, R, B, N, P, None
        {10, 100, 180, 300, 300, 900, 0}, // victim Q, attacker K, Q, R, B, N, P, None
        {6,  56,  100, 166, 166, 500, 0}, // victim R, attacker K, Q, R, B, N, P, None
        {3,  33,  60,  100, 100, 300, 0}, // victim B, attacker K, Q, R, B, N, P, None
        {3,  33,  60,  100, 100, 300, 0}, // victim N, attacker K, Q, R, B, N, P, None
        {1,  11,  20,  33,  33,  100, 0}, // victim P, attacker K, Q, R, B, N, P, None
        {0,  0,   0,   0,   0,   0,   0},       // victim None, attacker K, Q, R, B, N, P, None
};

int scoreMove (const Board& context, const Move& move, TranspositionTable& transpositionTable) {
    int moveScoreGuess = 0;

    if (transpositionTable.hasEntry(context, 0)) {
        const auto& entry = transpositionTable.getEntry(context);
//        std::cout << "Found!!" << std::endl;
        transpositionTable.hits++;

        if (entry.bestMove == move) {
            switch (entry.hitType) {
                case TranspositionTableHitType::LOWER_BOUND:
                case TranspositionTableHitType::UPPER_BOUND:
                    moveScoreGuess += 1000;
                    break;
                case TranspositionTableHitType::EXACT:
                    ++transpositionTable.hits;
                    moveScoreGuess += 10000;
                    break;
            }
        }
    }
    moveScoreGuess += MVV_LVA[context.getPieceAt(move.getDestination()).type][context.getPieceAt(move.getOrigin()).type];

    moveScoreGuess += BoardEvaluator::pieceValues[move.getPromotedPiece()];

    if (Attacks::getInstance()
                .getPawnAttacks()
                .getCaptures(context,
                             context.getPieces()[flip(context.getTurn())]
                                     .boards[PieceTypes::PAWN], flip(context.getTurn()))
        & move.getDestination()) {
            moveScoreGuess -= BoardEvaluator::pieceValues[move.getMovingPiece(context).type];
    }


    return moveScoreGuess;
}

void Search::orderMoves (Board& positionToSearch, std::vector<Move>& moves, int depth) {
    std::sort(moves.begin(), moves.end(), [&] (const Move& move1, const Move& move2) {
        return scoreMove(positionToSearch, move1, table) > scoreMove(positionToSearch, move2, table);
    });
}
