//
// Created by kaappo on 26.12.2021.
//
#include "catch2/catch.hpp"
#include "../Square.h"
#include "../mytypes.h"
#include "../BitboardOperations.h"

TEST_CASE ("Square works correctly", "[square]") {
    SECTION("Square should implement initialisation from multiple arguments", "[square]") {
        REQUIRE(Square{e4});
        REQUIRE(Square{45});
        REQUIRE(Square{0, 4});
    }

    SECTION("Square should implement equality operations == and !=", "[square]") {
        REQUIRE(Square{e4} == e4);
        REQUIRE(Square{e4} == Square{3, 4});
        REQUIRE(Square{e4} == 28);
    }

    SECTION("Square should implement calculating x and y coordinates", "[square]") {
        REQUIRE(Square{e4}.getX() == 4);
        REQUIRE(Square{e4}.getY() == 3);
    }

    SECTION ("Square should implement calculating the y difference", "[square]") {
        REQUIRE(Square{e4}.diffY(e2) == 2);
        REQUIRE(Square{e2}.diffY(e4) == 2);

        REQUIRE(Square{a8}.diffY(f2) == 6);
        REQUIRE(Square{a4}.diffY(e4) == 0);
    }

    SECTION ("Square should implement move operations", "[square]") {
        namespace Directions = BitboardOperations::Directions;
        REQUIRE(Square{c6}.move(Directions::NORTH) == c7);
        REQUIRE(Square{c6}.move(Directions::NORTH, BLACK) == c5);
        REQUIRE(Square{g7}.move(Directions::SOUTH_WEST) == f6);
        REQUIRE(Square{g7}.move(Directions::SOUTH_WEST, BLACK) == h8);

        REQUIRE(Square{h8}.move(Directions::NORTH) == h8); // prevent segfaults
        REQUIRE(Square{a1}.move(Directions::WEST) == a1); // prevent segfaults

    }

    SECTION ("Square should implement direction deduction", "[square]") {
        namespace Directions = BitboardOperations::Directions;
        using namespace PieceTypes;

        REQUIRE(Square{e2}.getDirection(e4, ROOK) == Directions::NORTH);
        REQUIRE(Square{e4}.getDirection(e2, ROOK) == Directions::SOUTH);

        REQUIRE(Square{e6}.getDirection(g6, ROOK) == Directions::EAST);
        REQUIRE(Square{b1}.getDirection(c2, BISHOP) == Directions::NORTH_EAST);
    }

//    SECTION ("Square should implement arithmetic operations +, += and -, -=", "[square]") {
//        Square s1 = e4;
//        Square s2 = a1;
//
//        REQUIRE(s1 + s2 == Square{g5});
//        REQUIRE(1 == 2);
//    }
}