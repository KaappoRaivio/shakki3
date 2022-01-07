//
// Created by kaappo on 29.12.2021.
//

#pragma once


#include <unordered_set>
#include "../Move.h"
#include "../Board.h"

#include "thc/thc.h"

namespace TestHelpers {
    void verify (const Move& previousMove, const Board& board);

    template<typename T>
    std::string vectorToString (const std::vector<T>& items, const std::string& separator) {
        std::stringstream s;

        for (const auto& item : items) {
            s << separator << item << std::endl;
        }
//        s << items.size() << std::endl;

        return s.str();
    }

    void verifyMoveList (const std::vector<Move>& moves, const Board& context, int index, bool captureOnly);
    void verifyMoveList (const std::unordered_set<std::string>& movesString, const Board& context, int index, bool captureOnly);

    void qperftTHC (thc::ChessRules& board, int depth, int& out);
    int perft (Board& board, int depth);
    int perftTHC (thc::ChessRules& board, int depth);


    class HelperEngineInterface {
    public:
        std::unordered_set<std::string> getMoves (const std::string& FEN, bool captureOnly) const;
//        std::unordered_set<std::string> getMovesNative (const std::string& FEN) const;
    };

    void analyzePerftProblem (Board& board, thc::ChessRules& cr, int depth);
}