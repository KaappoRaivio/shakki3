//
// Created by kaappo on 8.1.2022.
//

#include "catch2/catch.hpp"
#include "../Board.h"
#include "../InBetween.h"

TEST_CASE ("InBetween") {
    SECTION("Works") {
        Board board = Board::fromFEN("4k3/8/8/4K3/8/8/8/8 w - - 0 1");
        REQUIRE(InBetween::getInstance().getDistanceToEdge(board.getPieces(WHITE).boards[PieceTypes::KING].ls1b()) == 3);
        REQUIRE(InBetween::getInstance().getDistanceToEdge(board.getPieces(BLACK).boards[PieceTypes::KING].ls1b()) == 0);
    }
}
