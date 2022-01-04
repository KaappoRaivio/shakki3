//
// Created by kaappo on 29.12.2021.
//
#include <unordered_set>
#include "catch2/catch.hpp"
#include "TestHelpers.h"

#include "thc/thc.h"

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
    void verifyMoveList (const std::unordered_set<std::string>& movesString, const std::unordered_set<std::string>& supposedMoves, const Board& context, int index) {
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
            std::cerr << context.toFEN() << std::endl;
            FAIL();
        }
//        else {
//            SUCCEED();
//        }

        REQUIRE(std::unordered_set<std::string>{movesString.begin(), movesString.end()} == supposedMoves);
    }

    void verifyMoveList (const std::vector<Move>& moves, const std::unordered_set<std::string>& supposedMoves, const Board& context, int index) {
        std::unordered_set<std::string> movesString;
        std::transform(moves.begin(), moves.end(), std::inserter(movesString, movesString.begin()), [] (const Move& move) {
            std::stringstream s;
            s << move;
            return s.str();
        });

        verifyMoveList(movesString, supposedMoves, context, index);

//        std::cout << board << std::endl << TestHelpers::vectorToString(moves) << std::endl;


    }

    int perft (Board& board, int depth) {
        int out = 0;
        qperft(board, depth, out);
        return out;
    }
    int perftTHC (thc::ChessRules& board, int depth) {
        int out = 0;
        qperftTHC(board, depth, out);
        return out;
    }

    void qperftTHC (thc::ChessRules& board, int depth, int& out) {
        if (depth == 0) return;

        std::vector<thc::Move> moves;
        std::vector<bool> check;
        std::vector<bool> mate;
        std::vector<bool> stalemate;
        board.GenLegalMoveList(moves, check, mate, stalemate);

        for (auto& move : moves) {
            out += 1;
            board.PlayMove(move);
            qperftTHC(board, depth - 1, out);
            board.PopMove(move);
        }
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

    std::unordered_set<std::string> HelperEngineInterface::getMoves (const std::string& FEN) const {
        thc::ChessRules cr;
        cr.Forsyth(FEN.c_str());
//        std::cout << cr.ToDebugStr() << std::endl;
//        std::cout << "List of all legal moves in the current position" << std::endl;

        std::vector<thc::Move> moves;
        std::vector<bool> check;
        std::vector<bool> mate;
        std::vector<bool> stalemate;
        cr.GenLegalMoveList(moves, check, mate, stalemate);


        unsigned int len = moves.size();

        std::unordered_set<std::string> result;
        thc::Move mv;
        for (unsigned int i = 0 ; i < len ; i++) {
            mv = moves[i];
//            std::string mv_txt = mv.NaturalOut(&cr);
            std::stringstream ss;
            if (mv.special == thc::SPECIAL_BK_CASTLING || mv.special == thc::SPECIAL_WK_CASTLING) {
                ss << "O-O";
            } else if (mv.special == thc::SPECIAL_BQ_CASTLING || mv.special == thc::SPECIAL_WQ_CASTLING) {
                ss << "O-O-O";
            } else {
                ss << Square{mv.src}.asColorFlip(BLACK) << Square{mv.dst}.asColorFlip(BLACK);
                if (mv.special == thc::SPECIAL_PROMOTION_QUEEN) {
                    ss << "=Q";
                }
                if (mv.special == thc::SPECIAL_PROMOTION_ROOK) {
                    ss << "=R";
                }
                if (mv.special == thc::SPECIAL_PROMOTION_BISHOP) {
                    ss << "=B";
                }if (mv.special == thc::SPECIAL_PROMOTION_KNIGHT) {
                    ss << "=N";
                }
            }
            auto str = mv.TerseOut();
            std::transform(str.begin(), str.end(),str.begin(), ::toupper);
            result.insert(str);
        }

        return result;
    }

//    std::unordered_set<std::string> HelperEngineInterface::getMovesNative (const std::string& FEN) const {
//        thc::ChessRules cr;
//        cr.Forsyth(FEN.c_str());
//        std::cout << cr.ToDebugStr() << std::endl;
//        std::cout << "List of all legal moves in the current position" << std::endl;
//
//        std::vector<thc::Move> moves;
//        std::vector<bool> check;
//        std::vector<bool> mate;
//        std::vector<bool> stalemate;
//        cr.GenLegalMoveList(moves, check, mate, stalemate);
//
//
//        unsigned int len = moves.size();
//
//        std::unordered_set<thc::Move> result;
//        thc::Move mv;
//        for (unsigned int i = 0 ; i < len ; i++) {
//            mv = moves[i];
////            std::string mv_txt = mv.NaturalOut(&cr);
//            std::stringstream ss;
//            if (mv.special == thc::SPECIAL_BK_CASTLING || mv.special == thc::SPECIAL_WK_CASTLING) {
//                ss << "O-O";
//            } else if (mv.special == thc::SPECIAL_BQ_CASTLING || mv.special == thc::SPECIAL_WQ_CASTLING) {
//                ss << "O-O-O";
//            } else {
//                ss << Square{mv.src}.asColorFlip(BLACK) << Square{mv.dst}.asColorFlip(BLACK);
//                if (mv.special == thc::SPECIAL_PROMOTION_QUEEN) {
//                    ss << "=Q";
//                }
//                if (mv.special == thc::SPECIAL_PROMOTION_ROOK) {
//                    ss << "=R";
//                }
//                if (mv.special == thc::SPECIAL_PROMOTION_BISHOP) {
//                    ss << "=B";
//                }if (mv.special == thc::SPECIAL_PROMOTION_KNIGHT) {
//                    ss << "=N";
//                }
//            }
//            result.insert(ss.str());
//        }
//
//        return result;
//    }

    void analyzePerftProblem (Board& board, thc::ChessRules& rules, int depth) {
        TestHelpers::verifyMoveList(board.getMoves(), HelperEngineInterface{}.getMoves(rules.ForsythPublish()), board, 0);
        if (depth == 0) return;
        else {
            std::unordered_set<std::string> moves = HelperEngineInterface{}.getMoves(rules.ForsythPublish());
            for (const auto& moveString : moves) {
                std::string moveStringLower = moveString;
                std::transform(moveStringLower.begin(), moveStringLower.end(),moveStringLower.begin(), ::tolower);


                thc::Move thcmove;
//                std::cout << moveString << moveStringLower << std::endl;
                thcmove.TerseIn(&rules, moveStringLower.c_str());

                const Move& move = Move::fromString(moveString, board);

//                std::cout << "thc: " << thcmove.TerseOut() << ", " << move << std::endl;

                rules.PlayMove(thcmove);
                board.executeMove(move);
                analyzePerftProblem(board, rules, depth - 1);

                rules.PopMove(thcmove);
                board.unmakeMove();
            }
        }
    }
}
