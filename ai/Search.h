//
// Created by kaappo on 6.1.2022.
//

#include "../Board.h"
#include "TranspositionTable.h"

#pragma once


namespace EvaluationConstants {
    constexpr int WIN = 1e9;
    constexpr int LOSE = -1e9;
    constexpr int DRAW = 0;
}

class Search {
private:
    TranspositionTable table;

    int negamaxSearch(Board& positionToSearch, int depth, int alpha, int beta);
    int quiescenceSearch (Board& positionToSearch, int alpha, int beta);
public:
    int negamaxSearch (Board positionToSearch, int depth);

    Move getBestMove (Board position, int searchDepth);
};



