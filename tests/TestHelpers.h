//
// Created by kaappo on 29.12.2021.
//

#pragma once


#include <unordered_set>
#include "../Move.h"
#include "../Board.h"

namespace TestHelpers {
    void verify (const Move& previousMove, const Board& board);

    template <typename T>
    std::string vectorToString (const std::vector<T>& items, const std::string& separator) {
        std::stringstream s;

        for (const auto& item : items) {
            s << separator << item << std::endl;
        }
//        s << items.size() << std::endl;

        return s.str();
    }

    void verifyMoveList (const std::vector<Move>& moves, const std::unordered_set<std::string>& supposedMoves, const Board& context, int index);
    void qperft (Board& board, int depth, int& out);
}