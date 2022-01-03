//
// Created by kaappo on 29.12.2021.
//
#include <unordered_set>
#include "catch2/catch.hpp"
#include "TestHelpers.h"

namespace TestHelpers {
    void verify (const Move& previousMove, const Board& board) {
        REQUIRE(board.getPieceAt(previousMove.getDestination()) == previousMove.getMovingPiece(board));
        REQUIRE(board.getPieceAt(previousMove.getOrigin()) == Pieces::NO_PIECE);

        REQUIRE(Move{board.getHistory()->getCurrentFrame().previousMove} == previousMove);

        PieceColor color = previousMove.getMovingPiece(board).color;
        if (previousMove.isCastling(MoveBitmasks::KING_CASTLE)) {
            REQUIRE(board.getHistory()->getCurrentFrame().castlingStatus.canCastle(color, MoveBitmasks::KING_CASTLE) == false);
        }

        if (previousMove.isCastling(MoveBitmasks::QUEEN_CASTLE)) {
            REQUIRE(board.getHistory()->getCurrentFrame().castlingStatus.canCastle(color, MoveBitmasks::QUEEN_CASTLE) == false);
        }
    }
//
//    std::string vectorToString (const std::vector<Move>& moves) {
//
//    }

    void verifyMoveList (const std::vector<Move>& moves, const std::unordered_set<std::string>& supposedMoves, const Board& context, int index) {
        std::unordered_set<std::string> movesString;
        std::transform(moves.begin(), moves.end(), std::inserter(movesString, movesString.begin()), [](const Move& move) {
            std::stringstream s;
            s << move;
            return s.str();
        });

//        std::cout << board << std::endl << TestHelpers::vectorToString(moves) << std::endl;

        if (std::unordered_set<std::string>{movesString.begin(), movesString.end()} != supposedMoves) {
            std::cerr << "FAIL " << index << ": " << std::endl;




            std::vector<std::string> a;
            for (const auto& supposedlyGeneratedMove : supposedMoves) {
                if (!movesString.contains(supposedlyGeneratedMove)) {
//                    std::cerr << '\t' << supposedlyGeneratedMove << std::endl;
                    a.push_back(supposedlyGeneratedMove);
                }
            }
            if (!a.empty()) {
                std::cerr << "following moves should be generated, but are not:" << std::endl;
                std::cerr << vectorToString(a, "\t") << std::endl;
            }

            std::vector<std::string> b;
            for (const auto& generatedMove : movesString) {
                if (!supposedMoves.contains(generatedMove)) {
//                    std::cerr << '\t' << generatedMove << std::endl;
                    b.push_back(generatedMove);
                }
            }

            if (!b.empty()) {
                std::cerr << "following moves are generated, even though they shouldn't be:" << std::endl;
                std::cerr << vectorToString(b, "\t") << std::endl;
            }

            std::cerr << "context: " << std::endl << context << std::endl;
//            FAIL();
        }
//        else {
//            SUCCEED();
//        }

        REQUIRE(std::unordered_set<std::string>{movesString.begin(), movesString.end()} == supposedMoves);
    }

    void qperft (Board& board, int depth, int& out) {
        if (depth == 0) return;

        for (const auto& move : board.getMoves()) {
            out += 1;
            board.executeMove(move);
            qperft(board, depth - 1, out);
            board.unmakeMove();
        }
    }
}
