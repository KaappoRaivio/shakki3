//
// Created by kaappo on 26.12.2021.
//
#include "catch2/catch.hpp"
#include "../Board.h"
#include "../BoardAnalysis.h"

TEST_CASE ("Attack mask generation works correctly", "[integration][attacks]") {
    SECTION("Attack mask generation includes every possible move", "[integration][attacks]") {
        Board board = Board::fromFEN("rn1qkbnr/pppbpppp/3p4/1B5Q/4P3/8/PPPP1PPP/RNB1K1NR w KQkq - 0 1");
        REQUIRE(BoardAnalysis::getAttackMask(board, WHITE) == 47504935775834152ull);

        Board board2 = Board::fromFEN("rn1qkbnr/pppbpppp/3p4/1B5Q/4P3/8/PPPP1PPP/RNB1K1NR b KQkq - 0 1");
        REQUIRE(BoardAnalysis::getAttackMask(board, BLACK) == 1441432489234006016ull);
    }

    SECTION ("Attack mask generation doesn't include the king as an occupant", "[integration][attacks]") {
        Board board = Board::fromFEN("8/5k2/1r6/8/2B5/5R2/1K6/8 w - - 0 1");
        REQUIRE(BoardAnalysis::getAttackMask(board, WHITE) == 6926590283905709351ull);

        Board board2 = Board::fromFEN("8/5k2/1r6/8/2B5/5R2/1K6/8 b - - 0 1");
        REQUIRE(BoardAnalysis::getAttackMask(board2, BLACK) == 8237924853479506434ull);
    }
}

TEST_CASE ("Checkmask generation works correctly", "[integration][checks]") {
    SECTION("When there's not check, checkmask is -1", "[integration][checks]") {
        Board board = Board::fromFEN("6K1/6p1/p2k2p1/2R2qP1/3P4/2P2Pp1/5b1P/1r1b4 w - - 0 1");
        REQUIRE(BoardAnalysis::getCheckMask(board, WHITE) == -1);

        Board board2 = Board::fromFEN("p7/8/3kp1pK/2R2qP1/3P4/2P2Pp1/5b1P/1r1b4 w - - 0 1");
        REQUIRE(BoardAnalysis::getCheckMask(board2, WHITE) == -1);
    }

    SECTION ("When there is check, checkmask is correct", "[integration][checks]") {
        Board board = Board::fromFEN("p7/8/3kp1pK/2R3P1/3P4/2P2Ppq/5b1P/1r1b4 w - - 0 1");
        REQUIRE(BoardAnalysis::getCheckMask(board, WHITE) == 551911686144ull);
    }

    SECTION ("When there is double check, checkmask only includes the checking pieces", "[integration][checks]") {
        Board boardWithDoubleCheck = Board::fromFEN("8/4k3/6N1/1r6/8/4R3/8/K7 b - - 0 1");
        REQUIRE(BoardAnalysis::getCheckMask(boardWithDoubleCheck, BLACK) == Bitboard{70368745226240ull});

        Board boardWithDoubleCheck2 = Board::fromFEN("8/4k3/5P2/1r6/8/4R3/8/K7 b - - 0 1");
        REQUIRE(BoardAnalysis::getCheckMask(boardWithDoubleCheck2, BLACK) == 35184373137408ull);

//        REQUIRE(false);
    }
}

