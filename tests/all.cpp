//
// Created by kaappo on 24.12.2021.
//

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "../mytypes.h"
#include "../Bitboard.h"
#include "../Square.h"
#include "../BitboardOperations.h"

TEST_CASE ( "Bitboards work correctly", "[BitBoard]" ) {
    SECTION ("Bitboards should be initializable from multiple objects") {
        REQUIRE(Bitboard{234});
        REQUIRE(Bitboard{Square{2}});
        REQUIRE(Bitboard{-1ull} == 18446744073709551615);
    }
    SECTION ("Bitboard should implement operator== and friends") {
        Bitboard board{5497355813888};
        REQUIRE(board == 5497355813888);

        Bitboard board2{1024};
        REQUIRE(board2 == 1024);
        REQUIRE(board2 == Square{10});
        REQUIRE(board2 != Square{43});
    }

    SECTION ("Bitboard should implement setwise operations") {
        Bitboard board1 = 1196337404067840;
        Bitboard board2 = 17361641481138401520;

        REQUIRE ((board1 & board2) == 70437463670784);
        REQUIRE ((board1 & board2) == (board2 & board1));

        REQUIRE ((board1 ^ board2) == 17362696943615127792);
        REQUIRE ((board1 ^ board2) == (board2 ^ board1));

        REQUIRE ((board1 | board2) == 17362767381078798576);
        REQUIRE ((board1 | board2) == (board2 | board1));

        REQUIRE(~board1 == 18445547736305483775);
    }

    SECTION ("Bitboard should implement shift operations") {
        Bitboard board1 = 4096;

        std::cout << (board1 >> 1) << std::endl;

        REQUIRE((board1 >> 1) == 2048);
        REQUIRE((board1 << 1) == 8192);
        REQUIRE((board1 >> 10) == 4);

    }

    SECTION ("Bitboard should implement move operations") {
        namespace Directions = BitboardOperations::Directions;
        Bitboard board1 = 4096;

        std::cout << (board1 >> 1) << std::endl;

        REQUIRE(board1.move(Directions::WEST) == 2048);
        REQUIRE(board1.move(Directions::EAST) == 8192);
        REQUIRE(board1.move(Directions::WEST, WHITE, 10) == 4);

    }
}