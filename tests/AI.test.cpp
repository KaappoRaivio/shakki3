//
// Created by kaappo on 1/8/23.
//
#include "catch2/catch.hpp"
//#include "../Board.h"
#include "../Player.h"
//#include "../Move.h"
//#include "../Square.h"

TEST_CASE("Move output regression tests pass") {
    SECTION("Lasker-Reichhelm") {
        Board board = Board::fromFEN("8/k7/3p4/p2P1p2/P2P1P2/8/8/K7 w - - 0 1");
        const auto& aiPlayer = std::make_unique<AIPlayer>(26);
        REQUIRE(aiPlayer->getMove(board) == Move{board, a1, b1});
    }

    SECTION ("Non-obvious #7") {
        Board board = Board::fromFEN("8/1K6/8/4q2P/8/8/5k2/8 b - - 3 2");
        const auto& aiPlayer = std::make_unique<AIPlayer>(13);
        REQUIRE(aiPlayer->getMove(board) == Move{board, f2, e3});
    }
}