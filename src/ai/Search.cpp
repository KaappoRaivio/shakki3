//
// Created by kaappo on 6.1.2022.
//

#include <chrono>
#include <thread>
#include "Search.h"
#include "BoardEvaluator.h"
#include "TranspositionTable.h"
#include "../BoardAnalysis.h"
#include <cmath>
#include "../mytypes.h"

int Search::negamaxSearch(Board positionToSearch, int depth) {
    return negamaxSearch(positionToSearch, 0, depth, EvaluationConstants::LOSE, EvaluationConstants::WIN);
}

int Search::negamaxSearch(Board &positionToSearch, int plysFromRoot, int depth, int alpha, int beta) {
    ++nodesSearched;

    int originalAlpha = alpha;
    if (this->useTranspositionTable) {
        auto entry = table.getEntry(positionToSearch, depth - plysFromRoot);
        if (entry != TranspositionTableEntries::INVALID) {
            ++tableHits;
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
    }


    MOVES moves = positionToSearch.getMoves();

    if (moves.empty()) {
        if (positionToSearch.isCheck()) {
            return EvaluationConstants::LOSE;
        } else {
            return EvaluationConstants::DRAW;
        }
    }

    if (plysFromRoot == depth) {
        if (useQuiescenceSearch) {
            return quiescenceSearch(positionToSearch, alpha, beta, plysFromRoot + 1);
        } else {
            return BoardEvaluator::evaluateSimple(positionToSearch, depth, originalDepth);
        }
    }

    if (useMoveOrdering) {
        orderMoves(positionToSearch, moves, depth);
    }

    int positionValue = EvaluationConstants::LOSE - 1;
    size_t bestMoveIndex = -1;


    for (size_t moveIndex = 0; moveIndex < moves.size(); ++moveIndex) {
        const Move &move = moves[moveIndex];
        positionToSearch.executeMove(move);
        int newValue = -negamaxSearch(positionToSearch, plysFromRoot + 1, depth, -beta, -alpha);
        positionToSearch.unmakeMove();

//        positionValue = std::max(positionValue, newValue);
        if (newValue > positionValue) {
            positionValue = newValue;
            bestMoveIndex = moveIndex;
//            std::cout << "Setting depth " << depth << " at " << plysFromRoot << " to " << moves[moveIndex] << std::endl;
            if (depth == 4) {
//                std::cout << MyUtils::toString(moves[bestMoveIndex]) << std::endl;
            }
            PV[depth + 1][plysFromRoot] = moves[bestMoveIndex];
            for (int i = plysFromRoot + 1; i < MAX_SEARCH_DEPTH; ++i) {
                PV[depth + 1][i] = Moves::NO_MOVE;
            }
        }
        alpha = std::max(alpha, newValue);

        if (alpha >= beta) {
            ++cutoffs;
            break;
        }
    }

    if (useTranspositionTable) {
        TranspositionTableHitType hitType;
        if (positionValue <= originalAlpha) {
            hitType = TranspositionTableHitType::UPPER_BOUND;
        } else if (positionValue >= beta) {
            hitType = TranspositionTableHitType::LOWER_BOUND;
        } else {
            hitType = TranspositionTableHitType::EXACT;
        }
        //    TranspositionTableHitType hitType = positionValue <= originalAlpha ?
        //                                        TranspositionTableHitType::UPPER_BOUND : positionValue >= beta ?
        //                                                                                 TranspositionTableHitType::LOWER_BOUND :
        //                                                                                 TranspositionTableHitType::EXACT;


        TranspositionTableEntry newEntry{hitType, depth - plysFromRoot, positionValue, moves[bestMoveIndex]};
        table.store(positionToSearch, newEntry);
    }


    return positionValue;
}


int Search::quiescenceSearch(Board &positionToSearch, int alpha, int beta, int plysFromRoot) {
    int standing_pat = BoardEvaluator::evaluateSimple(positionToSearch, plysFromRoot, originalDepth);
    if (standing_pat >= beta) { return beta; }
    if (alpha < standing_pat) { alpha = standing_pat; }


    const MOVES &captureMoves = positionToSearch.getMoves(true);

    for (const Move &captureMove: captureMoves) {
        positionToSearch.executeMove(captureMove);
        int score = -quiescenceSearch(positionToSearch, -beta, -alpha, plysFromRoot + 1);
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

int
scoreMove(const Board &context, const Move &move, TranspositionTable &transpositionTable, bool useTranspositionTable) {
    int moveScoreGuess = 0;

//    if (transpositionTable.hasEntry(context, 0)) {
    if (useTranspositionTable) {
        auto entry = transpositionTable.getEntry(context, 0);
        if (entry != TranspositionTableEntries::INVALID) {
            if (entry.hitType == TranspositionTableHitType::EXACT) {
//                std::cout << "Hit!" << std::endl;
                moveScoreGuess += 2000;
            } else {
//                std::cout << "No hit!" << std::endl;
            }
        }
    }

    moveScoreGuess += MVV_LVA[context.getPieceAt(move.getDestination()).type][context.getPieceAt(
            move.getOrigin()).type];

    moveScoreGuess += BoardEvaluator::pieceValues[move.getPromotedPiece()];

    if (Attacks::getInstance()
                .getPawnAttacks()
                .getCaptures(context,
                             context.getPieces()[flip(context.getTurn())]
                                     .boards[PieceTypes::PAWN], flip(context.getTurn()))
        & move.getDestination()) {
        moveScoreGuess -= BoardEvaluator::pieceValues[move.getMovingPiece(context).type];
    }

    if (move.isCapture()) moveScoreGuess += 1000;


    return moveScoreGuess;
}

struct move_hash {
    std::size_t operator()(Move const &move) const {
        return std::hash<Move_raw>()(move.raw());
    }
};

void Search::orderMoves(Board &positionToSearch, MOVES &moves, int depth) {
//    std::cout << "Sorting moves with size " << moves.size() << "!"  << std::endl;
    int comparisons = 0;

//    SCORES moveScores;
    std::unordered_map<Move, int, move_hash> moveScores{moves.size()};
    for (size_t i = 0; i < moves.size(); ++i) {
        moveScores[moves[i]] = scoreMove(positionToSearch, moves[i], table, useTranspositionTable);
    }


    std::sort(moves.begin(), moves.end(), [&](const Move &move1, const Move &move2) {
//        ++comparisons;
//        return scoreMove(positionToSearch, move1, table, useTranspositionTable) > scoreMove(positionToSearch, move2, table, useTranspositionTable);
        return moveScores[move1] > moveScores[move2];
    });
//    std::cout << "Sorted moves with " << comparisons << " comparisons performed!" << std::endl;
}

Move Search::getBestMove(Board position, int searchDepth, std::chrono::milliseconds allowedTime) {
    std::cout << "Allowed time (ms): " << allowedTime.count() << std::endl;
    table.clear();
    originalDepth = searchDepth;

    Move bestMoveSoFar = Moves::NO_MOVE;
    int searchedDepth = 0;

    std::chrono::steady_clock::time_point actualBegin = std::chrono::steady_clock::now();

    for (int depth = 1; depth <= searchDepth; ++depth) {
        std::cout << "Iterative deepening for depth: " << depth << std::endl;
        int moveScore;

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        bestMoveSoFar = getMove(position, depth, moveScore);
        searchedDepth = depth;
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        long elapsedMillis = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();


        std::cout << "\tBest move so far: " << bestMoveSoFar << ", took " << elapsedMillis << " ms" << std::endl;

        std::cout << "\tPrincipal variation: "
                  << MyUtils::toString(PV[depth], [](const Move &move) { return move == Moves::NO_MOVE; }) << std::endl;
        if (moveScore >= 32000) {
            std::cout << "Found mate!" << std::endl;
            break;
        }
        auto predictedTimeForNextDepth = (end - begin) * std::sqrt(position.getMoves().size());
        auto timeLeft = allowedTime - (end - actualBegin);
        if (predictedTimeForNextDepth > timeLeft) {
            break;
        }
    }


//    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::cout << "Managed to search to depth " << searchedDepth << ", making move " << bestMoveSoFar << "!";

    return bestMoveSoFar;

}

Move Search::getMove(Board &position, int searchDepth, int &bestMoveScore) {
    auto moves = position.getMoves();
    if (useMoveOrdering) {
        orderMoves(position, moves, searchDepth);
    }

    std::cout << MyUtils::toString(moves) << std::endl;

    std::vector<int> values;
    nodesSearched = 0;
    tableHits = 0;
    cutoffs = 0;
    table.hits = 0;
    table.collisions = 0;

    Move bestMoveSoFar = Moves::NO_MOVE;
    int bestMoveScoreSoFar = EvaluationConstants::LOSE;

    for (size_t index = 0; index < moves.size(); ++index) {
        position.executeMove(moves[index]);

        int moveScore = -negamaxSearch(position, searchDepth - 1);
        values.push_back(moveScore);

        position.unmakeMove();

        if (moveScore > bestMoveScoreSoFar) {
            std::cout << "BestMove changed from: " << bestMoveSoFar << "(" << bestMoveScoreSoFar << "), to: "
                      << moves[index] << "(" << moveScore
                      << ")" << std::endl;
            bestMoveScoreSoFar = moveScore;
            bestMoveSoFar = moves[index];
        }
    }
    long index = std::distance(values.begin(), std::max_element(values.begin(), values.end()));

    std::cout << "\tTable hits: " << tableHits << std::endl;
    std::cout << "\tNodes searched " << nodesSearched << std::endl;
    std::cout << "\tCutoffs " << cutoffs << std::endl;
    std::cout << "\tCollisions " << table.collisions << std::endl;
    std::cout << "\tHits " << table.hits << std::endl;

    std::cout << "\tBest move value: " << values[index] << std::endl;

//    bestMoveScore = values[index];
    bestMoveScore = bestMoveScoreSoFar;
    for (size_t i = 0; i < moves.size(); ++i) {
        std::cout << "\t\t" << moves[i] << ": " << values[i] << std::endl;
    }

//    table.store(position, TranspositionTableEntry{TranspositionTableHitType::EXACT, searchDepth, values[index], moves[index]});

//    return moves[index];
    return bestMoveSoFar;
}

void Search::setUseTranspositionTable(bool useTranspositionTable) {
    this->useTranspositionTable = useTranspositionTable;
}

void Search::setUseQuiescenceSearch(bool useQuiescenceSearch) {
    this->useQuiescenceSearch = useQuiescenceSearch;
}

void Search::setUseMoveOrdering(bool useMoveOrdering) {
    this->useMoveOrdering = useMoveOrdering;
}

bool Search::isUseTranspositionTable() const {
    return useTranspositionTable;
}

bool Search::isUseQuiescenceSearch() const {
    return useQuiescenceSearch;
}

bool Search::isUseMoveOrdering() const {
    return useMoveOrdering;
}
