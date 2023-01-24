//
// Created by kaappo on 6.1.2022.
//

#include <chrono>
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
    int originalDepth;

    int tableHits = 0;
    int nodesSearched = 0;
    int cutoffs = 0;

    MOVES PV;

    bool useTranspositionTable = true;
public:
    bool isUseTranspositionTable() const;

    bool isUseQuiescenceSearch() const;

    bool isUseMoveOrdering() const;

private:
    bool useQuiescenceSearch = true;
    bool useMoveOrdering = true;
public:
    void setUseTranspositionTable(bool useTranspositionTable);

    void setUseQuiescenceSearch(bool useQuiescenceSearch);

    void setUseMoveOrdering(bool useMoveOrdering);

private:

    int negamaxSearch (Board &positionToSearch, int plysFromRoot, int depth, int alpha, int beta);
    int quiescenceSearch (Board &positionToSearch, int alpha, int beta, int plysFromRoot);
public:
    int negamaxSearch (Board positionToSearch, int depth, MOVES& principalVariation);

    Move getBestMove (Board position, int searchDepth, std::chrono::milliseconds allowedTime);

    Move getMove (Board& position, int searchDepth, MOVES& principalVariation, int& bestMoveScore);

    void orderMoves (Board& positionToSearch, MOVES& moves, int depth);

};



