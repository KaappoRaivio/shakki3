//
// Created by kaappo on 7.1.2022.
//

#include "TranspositionTable.h"

TranspositionTable::TranspositionTable () : table{} {
    table.reserve(1e6);
}

TranspositionTableEntry TranspositionTable::getEntry (const Board& board) {
    return table[board.hash()];
}

bool TranspositionTable::hasEntry (const Board& board, int depth) {
    return table.contains(board.hash()) && table[board.hash()].depth >= depth;
}

void TranspositionTable::store (const Board& board, const TranspositionTableEntry& entry) {
    table[board.hash()] = entry;
}

TranspositionTableEntry::TranspositionTableEntry (TranspositionTableHitType hitType, uint8_t depth, int positionValue) : hitType{hitType}, depth{depth}, positionValue{positionValue} {}

TranspositionTableEntry::TranspositionTableEntry () : hitType{TranspositionTableHitType::EXACT}, depth{255}, positionValue{0} {

}
