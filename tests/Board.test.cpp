//
// Created by kaappo on 27.12.2021.
//

#include <unordered_set>
#include "catch2/catch.hpp"
#include "../Board.h"

#include "TestHelpers.h"

TEST_CASE ("Board should implement piece moving", "[board]") {
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

    SECTION("Board should make an unmake moves") {
        SECTION("quiet moves") {
            Board board;
            Move move{board, e2, e4};
            board.executeMove(move);
            TestHelpers::verify(move, board);

            board.unmakeMove();
            REQUIRE(board.toFEN() == Board{}.toFEN());
        }

        SECTION ("capture moves") {
            Board board = Board::fromFEN("5B2/P1N1N3/P1P2P2/8/3RP2r/6kn/2qP1p2/K7 b - - 0 1");
            Board board2 = Board::fromFEN("5B2/P1N1N3/P1P2P2/8/3RP2r/6kn/2qP1p2/K7 b - - 0 1");
            Move move{board, c2, c3};
            board.executeMove(move);

            TestHelpers::verify(move, board);

            board.unmakeMove();
            REQUIRE(board.toFEN() == board2.toFEN());
        }

        SECTION ("castling moves") {
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

        SECTION ("promotion moves") {
            Board board = Board::fromFEN("K4n2/5PP1/1p6/8/1N3Rp1/kP5p/P6B/N2r2Q1 w - - 0 1");

            Move promotionMove{board, g7, g8, PieceTypes::BISHOP};

            board.executeMove(promotionMove);
            std::cout << board << std::endl;
            std::cout << promotionMove.isPromotion() << std::endl;
//            REQUIRE(false);
        }

    }

}

TEST_CASE("Board should implement move generation", "[board]") {
    SECTION("Board generates moves") {
        std::vector<std::pair<std::string, std::unordered_set<std::string>>> testCases = {
                {"r3k2r/5r2/8/8/8/8/8/R3K2R w KQkq - 0 1",                               {"A1B1", "A1C1", "A1D1", "A1A2", "A1A3",  "A1A4", "A1A5", "A1A6", "A1A7", "A1A8", "H1F1", "H1G1", "H1H2", "H1H3", "H1H4", "H1H5", "H1H6", "H1H7", "H1H8", "E1D1", "E1D2", "E1E2", "E1C1"}},
                {"1n1b1n2/N1k5/p1NRPq1p/p1pPP1r1/3p1pQp/K2P2P1/bRP1PPBp/2r1B3 b - - 0 1",
                                                                                         {"A2B1", "A2B3", "A2C4", "A2D5", "D8E7",  "C1A1", "C1B1", "C1D1", "C1E1", "C1C2", "G5G4", "G5E5", "G5F5", "G5H5", "G5G6", "G5G7", "G5G8", "F6E5", "F6F5", "F6E6", "F6G6", "F6E7", "F6F7", "F6G7", "F6H8", "B8C6", "B8D7", "F8E6", "F8G6", "F8D7", "F8H7",   "H2H1Q", "H2H1R", "H2H1B", "H2H1N", "F4F3",   "H4H3",   "A5A4",
                                                                                                                                                                                                                                                                                                                                                                                                                           "C5C4", "H6H5", "H4G3", "F4G3"}},
                {"N4B2/2Qp1pR1/R2nPPp1/1nPK2N1/pP1pPrb1/P2Pkp1P/pr3p1q/1B4b1 w - - 0 1",
                                                                                         {"D5E5", "E6E7", "C5C6", "A6B6", "E6F7",  "E6D7", "G7H7", "A6A4", "G7F7", "A6A5", "G7G6", "E4E5", "C7B8", "H3H4", "F8E7", "C7D8", "A6D6", "F8D6", "G5H7", "A6A7", "A6C6", "C7A5", "B1C2", "B1A2", "C7B6", "G5F7", "C7C6", "C7D6", "C7A7", "C7B7", "G7G8",   "C7C8",   "C7D7",   "G5F3",   "A8B6",   "C5D6",   "H3G4"}},
                {"3B4/2R2npB/2P1K1Pp/1nP2p2/N4k2/4p3/3p2PP/N2b4 b - - 0 1",              {"B5D6", "B5D4", "F4G4", "D1A4", "D1F3",  "D1B3", "B5C7", "D1C2", "B5A7", "B5A3", "F7H8", "D1E2", "F4E4", "F7G5", "D1H5", "F7E5", "F7D6", "D1G4", "F7D8", "B5C3", "E3E2", "H6H5"}},
                {"4qk2/P1p4P/n1p1pBPn/P1p1p1Pp/B1P2pp1/1bP1bR2/2P2NrQ/R3KNr1 w Q - 0 1", {"F2H3", "F2D3", "F2H1", "H2G2", "H2G1",  "F3H3", "F3G3", "A1B1", "H2F4", "F2G4", "F3E3", "H2H1", "A4B3", "F2E4", "F6E7", "A1C1", "F3F4", "F6E5", "A4B5", "F6G7", "F2D1", "F6D8", "H2H3", "F6H8", "A1D1", "A1A3", "C2B3", "H2H5", "G6G7", "A4C6", "A7A8Q", "A7A8R", "A7A8B", "A7A8N", "H7H8Q", "H7H8R", "H7H8B", "H7H8N", "H2H4", "A1A2", "G5H6", "E1D1", "H2G3", "E1E2"}},
                {"rnbqkbnr/pppppppp/8/8/8/N7/PPPPPPPP/R1BQKBNR b KQkq - 0 1",            {"B7B6", "D7D5", "B7B5", "C7C5", "C7C6",  "A7A6", "E7E6", "A7A5", "H7H6", "G8F6", "E7E5", "G7G6", "B8C6", "F7F5", "F7F6", "B8A6", "G7G5", "D7D6", "H7H5", "G8H6"}},
                {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1",                                 {"H8H3", "H8H2", "H8H7", "A8D8", "E8G8", "H8H1", "A8B8", "A8A3", "A8A7", "A8A6", "E8F7", "A8A4", "A8C8", "A8A2", "A8A5", "E8E7", "A8A1", "H8H4", "E8D7", "H8H5", "H8H6", "E8C8",  "H8F8", "H8G8", "E8D8", "E8F8"}},
//                {"rnbqkbnr/pppppp1p/6p1/7P/8/8/PPPPPPP1/RNBQKBNR b KQkq - 0 4", {}},
                {"rnbqkbnr/ppppp1pp/8/1B3p2/4P3/8/PPPP1PPP/RNBQK1NR b KQkq - 1 4", {"B7B6", "F5E4", "E7E5", "G7G6", "G7G5", "C7C5", "E7E6", "A7A6", "G8H6", "H7H6", "C7C6", "G8F6", "A7A5", "B8C6", "B8A6", "F5F4", "H7H5", "E8F7" }},
                {"rnbqkbnr/1ppppppp/p7/1B6/4P3/8/PPPP1PPP/RNBQK1NR b KQkq - 1 4", { "F7F6", "E7E6", "B7B6", "H7H6", "E7E5", "F7F5", "G7G5", "C7C5", "A8A7", "G8H6", "G8F6", "G7G6", "B8C6", "H7H5", "C7C6", "A6A5", "A6B5" }},
                {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K1R1 b Qkq - 1 2", { "H3G2", "B6C4", "F6G8", "B6A4", "A6B7", "B4C3", "H8G8", "H8F8", "E7C5", "B6D5", "H8H6", "F6D5", "G7H6", "E7F8", "G7F8", "A8C8", "F6H5", "F6G4", "B6C8", "E8D8", "H8H7", "A6C8", "A8B8", "A6E2", "E7D8", "D7D6", "E8C8", "F6E4", "H8H5", "F6H7", "B4B3", "E7D6", "G6G5", "C7C5", "H8H4", "C7C6", "E6D5", "A8D8", "A6C4", "E8F8", "A6D3", "A6B5", "E8G8" }},
//                {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/4P3/2N2Q1p/pPPBBPPP/2KR2R1 w kq - 0 5", {}}
        };

//        for (const auto& testCase : testCases) {

        for (size_t i = 0 ; i < testCases.size() ; ++i) {
            if (i == 0)
                std::cout << "debug" << std::endl;
            const auto& testCase = testCases[i];
            const Board& board = Board::fromFEN(testCase.first);
            auto moves = board.getMoves();

            std::unordered_set<std::string> correctMoves = TestHelpers::HelperEngineInterface{}.getMoves(board.toFEN());
            TestHelpers::verifyMoveList(moves, testCase.second, board, i);
            TestHelpers::verifyMoveList(correctMoves, testCase.second, board, i);
        }
    }

    SECTION("Random tests") {
        Board board = Board::fromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

        std::string moves[] = {
                "H1G1",
                "B4B3",
                "E1C1",
                "B3A2"
//                {board, h1, g1},
//                {board, b4, b3},
//                {board, e1, c1},
//                {board, b3, a2}
        };

        for (const auto& move : moves) {
            board.executeMove(Move::fromString(move, board));
        }

        std::cout << board.toFEN() << std::endl;

        auto generatedMoves = board.getMoves();
        std::unordered_set<std::string> correctMoves = TestHelpers::HelperEngineInterface{}.getMoves(board.toFEN());

//        std::cout << TestHelpers::vectorToString(generatedMoves, "\t") << std::endl;

        TestHelpers::verifyMoveList(generatedMoves, correctMoves, board, 0);
    }

    SECTION ("Qperft is correct") {
        Board board = Board::fromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
        thc::ChessRules cr;
        cr.Forsyth("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
//        board.executeMove(Move{board, e2, e4});
//        board.executeMove(Move{board, e7, e6});

//        TestHelpers::HelperEngineInterface{}.getMoves(board.toFEN());

        TestHelpers::analyzePerftProblem(board, cr, 4);


////
//        REQUIRE(TestHelpers::perft(board, 2) == 420);
//        REQUIRE(TestHelpers::perftTHC(cr, 2) == 420);
//        REQUIRE(TestHelpers::perft(board, 3) == 9322);
//        REQUIRE(TestHelpers::perftTHC(cr, 3) == 9322);

//        REQUIRE(TestHelpers::perft(board, 5));
//        REQUIRE(TestHelpers::perftTHC(cr, 5));
//        REQUIRE(TestHelpers::perft(board, 5) == TestHelpers::perftTHC(cr, 5));
    }

}
