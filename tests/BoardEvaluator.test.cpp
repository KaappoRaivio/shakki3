//
// Created by kaappo on 6.1.2022.
//

#include "catch2/catch.hpp"
#include "../Board.h"
#include "../ai/BoardEvaluator.h"
#include "../ai/Search.h"
#include "TestHelpers.h"


TEST_CASE("BoardEvaluator") {
    SECTION("Simple") {
        Board board;

//        std::cout << BoardEvaluator::evaluateSimpleOneSide(board, WHITE) << std::endl;
    }

    SECTION ("Tree search") {
        Board board = Board::fromFEN("5rk1/5Npp/8/1P1Q2br/5p2/PKpp4/4P3/8 w - - 4 3");
//        std::cout << Search{}.getBestMove(board, 1) << std::endl;
    }

    SECTION ("Evaluator test") {
        Board board = Board::fromFEN("8/8/8/5k2/8/8/5K2/8 w - - 0 1");
//        std::cout << BoardEvaluator::evaluateSimple(board, 1) << std::endl;
        REQUIRE(
                BoardEvaluator::evaluateSimple(board, 1) < 0
        );
    }

    SECTION ("Evaluator test 2") {
        Board board = Board::fromFEN("8/7K/8/7P/6q1/8/5k2/8 b - - 1 1");

        std::cout << BoardEvaluator::evaluateSimple(board, 0) << std::endl;
    }
}
