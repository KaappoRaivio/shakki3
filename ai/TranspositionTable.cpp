//
// Created by kaappo on 7.1.2022.
//

#include "TranspositionTable.h"

TranspositionTable::TranspositionTable () : table{}, collisions{0}, hits{0} {
    table.reserve(1e6);
}

const TranspositionTableEntry& TranspositionTable::getEntry (const Board& board, int plysFromRoot) const {
    const auto& iterator = table.find(board.hash());
    std::cout << (iterator == table.end() ? TranspositionTableEntries::INVALID :
                  iterator->second.depth <= plysFromRoot ? iterator->second :
                  TranspositionTableEntries::INVALID).depth << std::endl;

    return iterator == table.end() ? TranspositionTableEntries::INVALID :
           iterator->second.depth <= plysFromRoot ? iterator->second :
           TranspositionTableEntries::INVALID;
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
    if (const TranspositionTableEntry& other = getEntry(board, entry.depth)) {
        if (other.hitType == TranspositionTableHitType::EXACT) {
            return;
        } else if (entry.hitType != TranspositionTableHitType::EXACT) {
            return;
        }
    }
    table[board.hash()] = entry;
}

void TranspositionTable::getPrincipalVariation (Board& board, int depth, std::vector<std::string>& principalVariation) const {
    if (depth == 0) return;

//    std::cout << board << std::endl;

    if (hasEntry(board, 0)) {
        const TranspositionTableEntry& entry = getEntry(board, 0);
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

TranspositionTableEntry::operator bool () const {
    std::cout << "At operator bool" << (depth != TranspositionTableEntries::INVALID.depth) << " " << TranspositionTableEntries::INVALID.depth << " " << depth << std::endl;
    return depth != TranspositionTableEntries::INVALID.depth;
}

