//
// Created by kaappo on 26.12.2021.
//
#include "catch2/catch.hpp"

#include "../mytypes.h"
#include "../Bitboard.h"
#include "../Square.h"
#include "../BitboardOperations.h"


TEST_CASE ("Bitboard works correctly", "[unit][bitboard]") {
    SECTION ("Bitboard should implement initialisation from multiple objects") {
        REQUIRE(Bitboard{234});
        REQUIRE(Bitboard{Square{2}});
        REQUIRE(Bitboard{-1ull} == 18446744073709551615ull);
    }SECTION ("Bitboard should implement operator== and friends", "[unit][bitboard]") {
        Bitboard board{5497355813888};
        REQUIRE(board == 5497355813888);

        Bitboard board2{1024};
        REQUIRE(board2 == 1024);
        REQUIRE(board2 == Square{10});
        REQUIRE(board2 != Square{43});
    }

    SECTION ("Bitboard should implement setwise operations", "[unit][bitboard]") {
        Bitboard board1 = 1196337404067840;
        Bitboard board2 = 17361641481138401520ull;

        REQUIRE ((board1 & board2) == 70437463670784);
        REQUIRE ((board1 & board2) == (board2 & board1));

        REQUIRE ((board1 ^ board2) == 17362696943615127792ull);
        REQUIRE ((board1 ^ board2) == (board2 ^ board1));

        REQUIRE ((board1 | board2) == 17362767381078798576ull);
        REQUIRE ((board1 | board2) == (board2 | board1));

        REQUIRE(~board1 == 18445547736305483775ull);
    }

    SECTION ("Bitboard should implement reset", "[unit][bitboard]") {
        Bitboard bitboard = 1231245;
        WHEN("Bitboard is reset") {
            bitboard.reset();
            THEN("it should be zero") {
                REQUIRE(bitboard == 0);
            }
        }
    }

    SECTION ("Bitboard should implement shift operations", "[unit][bitboard]") {
        Bitboard board1 = 4096;

        REQUIRE((board1 >> 1) == 2048);
        REQUIRE((board1 << 1) == 8192);
        REQUIRE((board1 >> 10) == 4);
    }

    SECTION ("Bitboard should implement move operations", "[unit][bitboard]") {
        namespace Directions = BitboardOperations::Directions;
        Bitboard board1 = 72567767437312ull;

        REQUIRE(board1.move(Directions::WEST) == 36283883718656);
        REQUIRE(board1.move(Directions::EAST) == 145135534874624);
        REQUIRE(board1.move(Directions::WEST, WHITE, 10) == 70866960388ull);
    }

    SECTION ("Bitboard should implement flipping and rotating itself, also when given color perspectives", "[unit][bitboard]") {
        Bitboard board = 4468913471492ull;

        REQUIRE(board.flipVertical() == 288230926176223232ull);
        REQUIRE(board.rotate180() == 2305843013644976128ull);

        REQUIRE(board.asColor(WHITE) == 4468913471492ull);
        REQUIRE(board.asColor(BLACK) == 2305843013644976128ull);
    }

    SECTION("Bitboard should implement iteration", "[unit][bitboard]") {
        Bitboard board = 4468913479684ull;

        std::vector<int> positions;
        for (const auto& position : board) {
            positions.push_back(position.getValue());
        }

        REQUIRE(positions == std::vector<int>{2, 13, 31, 36, 42});
    }
}