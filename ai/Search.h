//
// Created by kaappo on 6.1.2022.
//

#include "../Board.h"
#include "TranspositionTable.h"

#pragma once


namespace EvaluationConstants {
    constexpr int WIN = 32000;
    constexpr int LOSE = -32000;
    constexpr int DRAW = 0;
}

class Search {
private:
    TranspositionTable table;

    int tableHits = 0;
    int nodesSearched = 0;
    int cutoffs = 0;

    int negamaxSearch (Board& positionToSearch, int depth, int alpha, int beta, std::vector<Move>& principalVariation);
    int quiescenceSearch (Board& positionToSearch, int alpha, int beta);
public:
    int negamaxSearch (Board positionToSearch, int depth, std::vector<Move>& principalVariation);

    Move getBestMove (Board position, int searchDepth);

    Move getMove (Board& position, int searchDepth, std::vector<Move>& principalVariation);

    void orderMoves (Board& positionToSearch, std::vector<Move>& moves, int depth);

};



