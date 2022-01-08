//
// Created by kaappo on 7.1.2022.
//

#include "TranspositionTable.h"

TranspositionTable::TranspositionTable () : table{}, collisions{0} {
    table.reserve(1e6);
}

TranspositionTableEntry TranspositionTable::getEntry (const Board& board) const {
    const auto& iterator = table.find(board.hash());
    return iterator->second;
}

bool TranspositionTable::hasEntry (const Board& board, int depth) const {
    const auto& iterator = table.find(board.hash());
    if (iterator == table.end()) return false;
    else {
        const auto& value = iterator->second;
        return value.depth >= depth;
    }
}

void TranspositionTable::store (const Board& board, TranspositionTableEntry entry) {
    if (hasEntry(board, 0)) {
        if (getEntry(board).depth >= entry.depth) {
            ++collisions;
            return;
        }
    }
    table[board.hash()] = entry;
}

void TranspositionTable::getPrincipalVariation (Board& board, int depth, std::vector<std::string>& principalVariation) const {
    if (depth == 0) return;

//    std::cout << board << std::endl;

    if (hasEntry(board, 0)) {
        const TranspositionTableEntry& entry = getEntry(board);
        if (entry.hitType != TranspositionTableHitType::EXACT) std::cerr << "Problem!" << std::endl;

        principalVariation.push_back(MyUtils::toString(entry.bestMove));
        board.executeMove(entry.bestMove);
        getPrincipalVariation(board, depth - 1, principalVariation);
    } else {
        std::cerr << "Problem not found" << std::endl;
        principalVariation.emplace_back("Not found");
    }
}

TranspositionTableEntry::TranspositionTableEntry (TranspositionTableHitType hitType, int depth, int positionValue, Move bestMove) : hitType{hitType}, depth{depth}, positionValue{positionValue}, bestMove(bestMove) {}

TranspositionTableEntry::TranspositionTableEntry () : hitType{TranspositionTableHitType::EXACT}, depth{255}, positionValue{0}, bestMove{Moves::NO_MOVE} {

}
