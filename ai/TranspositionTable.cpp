//
// Created by kaappo on 7.1.2022.
//

#include "TranspositionTable.h"

TranspositionTable::TranspositionTable () : table{}, collisions{0} {
    table.reserve(1e6);
}

TranspositionTableEntry TranspositionTable::getEntry (const Board& board) {
    return table[board.hash()];
}

bool TranspositionTable::hasEntry (const Board& board, int depth) {
    bool hasEntry = table.contains(board.hash()) && table[board.hash()].depth >= depth;
//    if (hasEntry) ++hits;
//    if (hasEntry) {
//        std::cout << table[board.hash()].depth << std::endl;
//    }
    return hasEntry;
}

void TranspositionTable::store (const Board& board, TranspositionTableEntry entry) {
    if (hasEntry(board, entry.depth)) {
        ++collisions;
    }
    table[board.hash()] = entry;
}

std::vector<std::string> TranspositionTable::getPrincipalVariation (Board& board, int depth) {
    if (depth == 0) return {};
    std::vector<std::string> principalVariation;

    for (const Move& move : board.getMoves()) {
        board.executeMove(move);
        if (hasEntry(board, 0)) {
            const TranspositionTableEntry& entry = getEntry(board);
            if (entry.hitType != TranspositionTableHitType::EXACT) std::cerr << "Problem!" << std::endl;

            principalVariation.push_back(MyUtils::toString(entry.bestMove));
        } else {
            std::cerr << "Problem" << std::endl;
        }

        std::vector<std::string> rest = getPrincipalVariation(board, depth - 1);
        principalVariation.insert(principalVariation.begin(), rest.begin(), principalVariation.end());
    }

    return principalVariation;
}

TranspositionTableEntry::TranspositionTableEntry (TranspositionTableHitType hitType, int depth, int positionValue, Move bestMove) : hitType{hitType}, depth{depth}, positionValue{positionValue}, bestMove(bestMove) {}

TranspositionTableEntry::TranspositionTableEntry () : hitType{TranspositionTableHitType::EXACT}, depth{255}, positionValue{0}, bestMove{Moves::NO_MOVE} {

}
