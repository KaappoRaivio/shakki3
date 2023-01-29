//
// Created by kaappo on 6.1.2022.
//

#include <chrono>
#include "../Board.h"
#include "TranspositionTable.h"

#pragma once

#define MAX_SEARCH_DEPTH 351

namespace EvaluationConstants {
    constexpr int WIN = 32000;
    constexpr int LOSE = -32000;
    constexpr int DRAW = 0;
}

struct LINE {
    int moveCount;
//    Move moves[MAX_SEARCH_DEPTH];
    std::vector<Move> moves;
};

class Search {
private:
    TranspositionTable table;
    int originalDepth;

    int tableHits = 0;
    int nodesSearched = 0;
    int cutoffs = 0;
//    std::array<Move[], MAX_SEARCH_DEPTH> PV;
    LINE PV;

    bool useTranspositionTable = true;
    bool useQuiescenceSearch = true;
    bool useMoveOrdering = true;

    int negamaxSearch(Board &positionToSearch, int plysFromRoot, int depth, int alpha, int beta, LINE &pline);
    int quiescenceSearch (Board &positionToSearch, int alpha, int beta, int plysFromRoot);
public:
    int negamaxSearch(Board positionToSearch, int depth, LINE &PV);
    Move getBestMove (Board position, int searchDepth, std::chrono::milliseconds allowedTime);
    Move getMove(Board &position, int searchDepth, int &bestMoveScore);
    void orderMoves (Board& positionToSearch, MOVES& moves, int depth);

    void setUseTranspositionTable(bool useTranspositionTable);
    void setUseQuiescenceSearch(bool useQuiescenceSearch);
    void setUseMoveOrdering(bool useMoveOrdering);

    bool isUseTranspositionTable() const;
    bool isUseQuiescenceSearch() const;
    bool isUseMoveOrdering() const;

    static void copyPV(Move *destination, const Move *source, size_t n);
};



