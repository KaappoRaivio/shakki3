//
// Created by kaappo on 26.12.2021.
//
#include "catch2/catch.hpp"
#include "../Board.h"
#include "../BoardAnalysis.h"

TEST_CASE ("Attack mask generation works correctly", "[integration][attacks]") {
    SECTION("Attack mask generation includes every possible move", "[integration][attacks]") {
        Board board = Board::fromFEN("rn1qkbnr/pppbpppp/3p4/1B5Q/4P3/8/PPPP1PPP/RNB1K1NR w KQkq - 0 1");
//        REQUIRE(BoardAnalysis::getAttackMask(board, WHITE) == 47504944365812586ull);

        Board board2 = Board::fromFEN("rn1qkbnr/pppbpppp/3p4/1B5Q/4P3/8/PPPP1PPP/RNB1K1NR b KQkq - 0 1");
//        REQUIRE(BoardAnalysis::getAttackMask(board, BLACK) == 9150750626362163200ull);

        Board board3 = Board::fromFEN("N4B2/2Qp1pR1/R2nPPp1/1nPK2N1/pP1pPrb1/P2Pkp1P/pr3p1q/1B4b1 w - - 0 1");
        REQUIRE(BoardAnalysis::getAttackMask(board3, BLACK) == 1452266442207526354ull);
//        REQUIRE(false); // TODO off by one blockers!!!
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

    SECTION ("When there is double check, checkmask is zero", "[integration][checks]") {
        Board boardWithDoubleCheck = Board::fromFEN("8/4k3/6N1/1r6/8/4R3/8/K7 b - - 0 1");
        REQUIRE(BoardAnalysis::getCheckMask(boardWithDoubleCheck, BLACK) == 0);

        Board boardWithDoubleCheck2 = Board::fromFEN("8/4k3/5P2/1r6/8/4R3/8/K7 b - - 0 1");
        REQUIRE(BoardAnalysis::getCheckMask(boardWithDoubleCheck2, BLACK) == 0);

//        REQUIRE(false);
    }
}

TEST_CASE ("Pinmask generation works correctly", "[integration][pins]") {
    SECTION("When no pieces are pinned, pinmask should be 0", "[integration][pins]") {
        Board b = Board::fromFEN("K7/3ppP2/P3b3/2k2BQ1/p4P2/5r2/4n2p/5R1n w - - 0 1");
        REQUIRE(BoardAnalysis::getPinMask<BOTH>(b, WHITE) == 0);
        REQUIRE(BoardAnalysis::getPinMask<HV>(b, WHITE) == 0);
        REQUIRE(BoardAnalysis::getPinMask<D12>(b, WHITE) == 0);
    }

    SECTION("Bishop pins are not included in HV pinmasks", "[integration][pins]") {
        REQUIRE(BoardAnalysis::getPinMask<HV>(Board::fromFEN("k4nR1/8/2r5/3B4/8/2K5/8/8 b - - 0 1"), BLACK) == 9079256848778919936);
    }

    SECTION("HV pinned rooks can move on the axis they are pinned on", "[integration][pins]") {
        REQUIRE(BoardAnalysis::getPinMask<HV>(Board::fromFEN("k2r3R/8/2r5/3B4/8/2K5/8/8 b - - 0 1"), BLACK) == 18302628885633695744ull);
    }

    SECTION("D12 pinned rooks cannot move") {
        REQUIRE(BoardAnalysis::getPinMask<D12>(Board::fromFEN("k2r3R/8/2r5/3B4/8/2K5/8/8 b - - 0 1"), BLACK) == 567382359670784);
    }

    SECTION("Pieces are pinned even if in check") {
        REQUIRE(BoardAnalysis::getPinMask<BOTH>(Board::fromFEN("k6R/8/2r5/8/8/2K2B2/8/8 b - - 0 1"), BLACK) == 567382630203392);
    }

    SECTION("When there are no pins, no pinmask is generated") {
        REQUIRE(BoardAnalysis::getPinMask<BOTH>(Board::fromFEN("k7/8/2r5/8/4r3/2K2B2/8/8 b - - 0 1"), BLACK) == 0);
        REQUIRE(BoardAnalysis::getPinMask<BOTH>(Board::fromFEN("8/1r6/2R5/8/4K3/8/6b1/k7 w - - 0 1"), WHITE) == 0);
        REQUIRE(BoardAnalysis::getPinMask<BOTH>(Board::fromFEN("8/R4P1p/4n3/2k2P2/7p/Bnppp3/3p2KP/4b3 b - - 0 1"), BLACK) == 0);

    }
}

