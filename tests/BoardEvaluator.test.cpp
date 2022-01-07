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
//        TestHelpers::verifyMoveList(board.getMoves(), {}, board, 0);
        std::cout << Search{}.getBestMove(board, 5) << std::endl;
    }
}