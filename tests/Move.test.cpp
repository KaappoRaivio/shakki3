//
// Created by kaappo on 29.12.2021.
//
#include "catch2/catch.hpp"
#include "../Board.h"

TEST_CASE("Move works correctly") {
    SECTION("Promotion moves work correctly") {
        SECTION ("Test 1") {
            Board b = Board::fromFEN("1n1b1n2/N1k5/p1NRPq1p/p1pPP1r1/3p1pQp/K2P2P1/bRP1PPBp/2r1B3 b - - 0 1");
            Move m1 = {b, h2, h1, PieceTypes::QUEEN};
            Move m2 = {b, h2, h1, PieceTypes::ROOK};
            Move m3 = {b, h2, h1, PieceTypes::BISHOP};
            Move m4 = {b, h2, h1, PieceTypes::KNIGHT};

            REQUIRE(m1.isPromotion());
            REQUIRE(m1.getPromotedPiece() == PieceTypes::QUEEN);
            REQUIRE(m2.isPromotion());
            REQUIRE(m2.getPromotedPiece() == PieceTypes::ROOK);
            REQUIRE(m3.isPromotion());
            REQUIRE(m3.getPromotedPiece() == PieceTypes::BISHOP);
            REQUIRE(m4.isPromotion());
            REQUIRE(m4.getPromotedPiece() == PieceTypes::KNIGHT);
        }

        SECTION("Test 2") {
            Board b = Board::fromFEN("r3k2r/5r2/8/8/8/8/8/R3K2R w KQkq - 0 1");
            Move m{b, e1, d2};

            REQUIRE(!m.isPromotion());
        }
    }
}
