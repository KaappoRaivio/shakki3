//
// Created by kaappo on 27.12.2021.
//

#include "catch2/catch.hpp"
#include "../Board.h"

#include "TestHelpers.h"

TEST_CASE ("Board should implement chess rules", "[board]") {
    SECTION("Board should be able to make FENs") {
        std::vector<std::string> randomFENs = {
                "1K1R4/8/8/p1Pp1P2/1PP2r1B/3P1k2/2p1NP2/2b5 w - - 0 1",
                "r1Q3n1/2p3P1/1P2b3/2P2p1k/8/1p2K2N/3R2P1/3N4 w - - 3 45",
                "2r5/3p1kB1/8/PK5P/2r2Pp1/1n4P1/1Q1p1pp1/8 w Kq - 0 1",
                "R3Q3/2p2P1q/1P6/1P6/2P3kr/p2R4/1P4PB/4K3 w KQk - 0 6",
                "1b1q4/kp1bN3/2P1B1N1/2RpP2p/P2p1ppP/nK1P3R/P2P1ppP/4Br1Q w - - 0 1",
                "5N1B/1PprP1P1/PP1k1P1p/5qP1/K1p1pNQR/nppB1n2/b1RP4/6br w - - 0 1"
        };

        for (std::string FEN : randomFENs) {
            Board board = Board::fromFEN(FEN);

            REQUIRE(board.toFEN() == FEN);
        }
    }

    SECTION("Board should make and unmake quiet moves") {
        Board board;
        Move move{board, e2, e4};
        board.executeMove(move);
        TestHelpers::verify(move, board);

        board.unmakeMove();
        REQUIRE(board.toFEN() == Board{}.toFEN());
    }

    SECTION ("Board should make and unmake capture moves") {
        Board board = Board::fromFEN("5B2/P1N1N3/P1P2P2/8/3RP2r/6kn/2qP1p2/K7 b - - 0 1");
        Board board2 = Board::fromFEN("5B2/P1N1N3/P1P2P2/8/3RP2r/6kn/2qP1p2/K7 b - - 0 1");
        Move move{board, c2, c3};
        board.executeMove(move);

        TestHelpers::verify(move, board);

        board.unmakeMove();
        REQUIRE(board.toFEN() == board2.toFEN());
    }

    SECTION ("Board should make and unmake castling moves") {
        Board boardWhite = Board::fromFEN("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
        Board boardBlack = Board::fromFEN("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
        Move move{boardWhite, e1, g1};
        boardWhite.executeMove(move);

        TestHelpers::verify(move, boardWhite);

        boardWhite.unmakeMove();

        REQUIRE(boardWhite.toFEN() == "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

        Move move2{boardWhite, e1, c1};
        boardWhite.executeMove(move2);

        TestHelpers::verify(move2, boardWhite);

        boardWhite.unmakeMove();

        REQUIRE(boardWhite.toFEN() == "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

        Move move3{boardBlack, e8, g8};
        boardBlack.executeMove(move3);

        TestHelpers::verify(move3, boardBlack);

        boardBlack.unmakeMove();

        REQUIRE(boardBlack.toFEN() == "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");

        Move move4{boardBlack, e8, c8};
        boardBlack.executeMove(move4);

        TestHelpers::verify(move4, boardBlack);

        boardBlack.unmakeMove();

        REQUIRE(boardBlack.toFEN() == "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
    }
}

