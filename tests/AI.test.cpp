//
// Created by kaappo on 1/8/23.
//
#include "catch2/catch_all.hpp"
//#include "../Board.h"
#include "../src/Player.h"
//#include "../Move.h"
//#include "../Square.h"

TEST_CASE("Move output regression tests pass", "[ai]") {
//    SECTION("AI is performant") {
////        Board board = Board::fromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
////        Board board = Board::fromFEN("2k5/pp1bq3/2p3rp/5p2/3Qp3/1BP1PpR1/PP3P1K/3R4 b - - 0 28");
//        Board board = Board::fromFEN("r2qk2r/2pbbpp1/3p1nnp/p2Pp3/4P3/PB2BNNP/2P2PP1/2RQ1RK1 b kq - 0 15");
//        const auto &player = std::make_unique<AIPlayer>(3, std::chrono::seconds{10});
//
//        std::cout << player->getMove(board) << std::endl;
//
//        BENCHMARK ("asd") {
//                              return player->getMove(board);
//                          };
//    }

    SECTION ("Knight isn't blundered") {
        Board board = Board::fromFEN("8/1kp3p1/7p/R4p1n/p5P1/P1Brr3/1KR5/8 b - - 0 41");

        const auto &player = std::make_unique<AIPlayer>(20, std::chrono::seconds{10});
        player->getSearch().setUseQuiescenceSearch(true);

        const Move &aiMove = player->getMove(board);
        REQUIRE((aiMove.getOrigin() == h5 or aiMove.getDestination() == g4));
    }

    SECTION ("Pawn isn't blundered") {
        Board board = Board::fromFEN("rnbqkb1r/ppp2ppp/3p1n2/4p3/3PP3/5Q2/PPP2PPP/RNB1KBNR w KQkq - 0 4");
        const auto &player = std::make_unique<AIPlayer>(20, std::chrono::seconds{10});
        player->getSearch().setUseQuiescenceSearch(true);
        player->getSearch().setUseTranspositionTable(false);

        REQUIRE(player->getMove(board) != Move::fromString("c1g5", board));
    }

    SECTION ("Rook isn't blundered") {
        Board board = Board::fromFEN("r2r2k1/1bqn1ppp/8/pB2p3/8/PQn1BN2/2P2PPP/3R1RK1 w - - 0 22");
        const auto &player = std::make_unique<AIPlayer>(3, std::chrono::seconds{10});
        player->getSearch().setUseTranspositionTable(true);
        REQUIRE(player->getMove(board) != Move::fromString("b5c4", board));
    }

    SECTION("Lasker-Reichhelm") {
        Board board = Board::fromFEN("8/k7/3p4/p2P1p2/P2P1P2/8/8/K7 w - - 0 1");
        const auto &aiPlayer = std::make_unique<AIPlayer>(21, std::chrono::seconds{10000000});
        aiPlayer->getSearch().setUseQuiescenceSearch(false);
        REQUIRE(aiPlayer->getMove(board) == Move{board, a1, b1});
    }

    SECTION ("Non-obvious #7") {
        std::cout << "moi" << std::endl;
        Board board = Board::fromFEN("8/1K6/8/4q2P/8/8/5k2/8 b - - 3 2");
        const auto &aiPlayer = std::make_unique<AIPlayer>(13, std::chrono::seconds{10000000});
        aiPlayer->getSearch().setUseQuiescenceSearch(false);
        REQUIRE(aiPlayer->getMove(board) == Move{board, f2, e3});
    }

    SECTION ("Quiescence search works correctly") {
        Board board = Board::fromFEN("8/3np1b1/8/3KP2k/8/8/1B6/8 b - - 0 1");


        const auto &playerWithoutQuiescenceSearch = std::make_unique<AIPlayer>(20, std::chrono::seconds{10});
        playerWithoutQuiescenceSearch->getSearch().setUseQuiescenceSearch(false);

        const auto &playerWithQuiescenceSearch = std::make_unique<AIPlayer>(20, std::chrono::seconds{10});
        playerWithQuiescenceSearch->getSearch().setUseQuiescenceSearch(true);

        const Move &badMove = playerWithoutQuiescenceSearch->getMove(board);
        const Move &supposedBetterMove = playerWithQuiescenceSearch->getMove(board);
        std::cout << "Bad: " << badMove << ", Good:" << supposedBetterMove << std::endl;
        REQUIRE(badMove.getDestination() == e5);
        REQUIRE(supposedBetterMove.getDestination() != e5);
    }
}