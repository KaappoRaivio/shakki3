//
// Created by kaappo on 1/8/23.
//
#include "catch2/catch_all.hpp"
//#include "../Board.h"
#include "../src/Player.h"
//#include "../Move.h"
//#include "../Square.h"

TEST_CASE("Move output regression tests pass") {
    SECTION("Lasker-Reichhelm") {
        Board board = Board::fromFEN("8/k7/3p4/p2P1p2/P2P1P2/8/8/K7 w - - 0 1");
        const auto& aiPlayer = std::make_unique<AIPlayer>(26, std::chrono::seconds{10000000});
        REQUIRE(aiPlayer->getMove(board) == Move{board, a1, b1});
    }

    SECTION ("Non-obvious #7") {
        std::cout << "moi" << std::endl;
        Board board = Board::fromFEN("8/1K6/8/4q2P/8/8/5k2/8 b - - 3 2");
        const auto& aiPlayer = std::make_unique<AIPlayer>(13, std::chrono::seconds{10000000});
        REQUIRE(aiPlayer->getMove(board) == Move{board, f2, e3});
    }

    SECTION ("Quiescence search works correctly") {
        Board board = Board::fromFEN("8/3n2b1/8/3KP2k/8/8/1B6/8 b - - 0 1");



        const auto& playerWithoutQuiescenceSearch = std::make_unique<AIPlayer>(1, std::chrono::seconds{10});
        playerWithoutQuiescenceSearch->getSearch().setUseQuiescenceSearch(false);

        const auto& playerWithQuiescenceSearch = std::make_unique<AIPlayer>(1, std::chrono::seconds{10});
        playerWithQuiescenceSearch->getSearch().setUseQuiescenceSearch(true);

        const Move &badMove = playerWithoutQuiescenceSearch->getMove(board);
        const Move &supposedBetterMove = playerWithQuiescenceSearch->getMove(board);
        std::cout << "Bad: " << badMove << ", Good:" << supposedBetterMove << std::endl;
        REQUIRE(badMove.getDestination() == e5);
        REQUIRE(supposedBetterMove.getDestination() != e5);
    }
}