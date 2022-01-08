//
// Created by kaappo on 7.1.2022.
//

#pragma once


#include <unordered_map>
#include <cstdint>
#include "../Board.h"

enum class TranspositionTableHitType {
    LOWER_BOUND, EXACT, UPPER_BOUND
};

struct TranspositionTableEntry {
    TranspositionTableEntry (TranspositionTableHitType hitType, int depth, int positionValue, Move bestMove);
    TranspositionTableEntry ();

    TranspositionTableHitType hitType;
    int depth;
    int positionValue;
    Move bestMove;
};


class TranspositionTable {
private:
    std::unordered_map<uint64_t, TranspositionTableEntry> table;
public:
    TranspositionTable ();

    bool hasEntry (const Board& board, int depth);
    TranspositionTableEntry getEntry(const Board& board);

    void store (const Board& board, TranspositionTableEntry entry);

    int collisions;
    int hits;

    std::vector<std::string> getPrincipalVariation (Board& board, int depth);
};


