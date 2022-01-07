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
    TranspositionTableEntry (TranspositionTableHitType hitType, uint8_t depth, int positionValue);
    TranspositionTableEntry ();

    TranspositionTableHitType hitType;
    uint8_t depth;
    int positionValue;
};


class TranspositionTable {
private:
    std::unordered_map<uint64_t, TranspositionTableEntry> table;
public:
    TranspositionTable ();

    bool hasEntry (const Board& board, int depth);
    TranspositionTableEntry getEntry(const Board& board);

    void store (const Board& board, const TranspositionTableEntry& entry);
};



