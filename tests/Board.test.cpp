//
// Created by kaappo on 27.12.2021.
//

#include <unordered_set>

#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include <catch2/catch_all.hpp>

#include "../src/Board.h"

#include "TestHelpers.h"
#include "../src/Attacks.h"

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

    SECTION ("Board hashes itself") {
        SECTION ("Obvious tests") {
            Board board;

            Board startingPositionFEN = Board::fromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            REQUIRE(board.hash() == startingPositionFEN.hash());
//            std::cout << board.hash() << std::endl;

            Board reference = board;

            Move move{board, e2, e4};
            board.executeMove(move);
//            std::cout << board.hash() << std::endl;
            Move move2{board, d7, d5};
            board.executeMove(move2);
//            std::cout << board.hash() << std::endl;
            board.unmakeMove();
//            std::cout << board.hash() << std::endl;
            board.unmakeMove();
//            std::cout << board.hash() << std::endl;

            REQUIRE(reference.hash() == board.hash());
        }

        SECTION ("More tests") {
            Board starting = Board::fromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            Board startingWithoutCastling = Board::fromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1");
            Board startingWithDifferentTurn = Board::fromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");

            REQUIRE(starting.hash() != startingWithoutCastling.hash());
            REQUIRE(starting.hash() != startingWithDifferentTurn.hash());
        }

        SECTION ("More tests2") {
            Board reference = Board::fromFEN("rnbq2nr/pppp1kpp/8/2b1p3/4P3/8/PPPP1PPP/RNBQK1NR w KQ - 0 1");
            Board board;
            board.executeSequenceOfMoves({"e2e4", "e7e5", "f1c4", "f8c5", "c4f7", "e8f7"});

            std::cout << reference;
            std::cout << board;
            REQUIRE(board.hash() == reference.hash());
        }

        SECTION ("More tests3") {
            Board board1 = Board::fromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1");
            Board board2 = Board::fromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1");

            board1.executeSequenceOfMoves({"h3g2", "f3g2", "b4c3", "b2c3"});
            board2.executeSequenceOfMoves({"b4c3", "b2c3", "h3g2", "f3g2"});

            REQUIRE(board1.hash() == board2.hash());
        }
    }

    SECTION("Board should make and unmake moves") {
        SECTION("quiet moves") {
            Board board;
            Move move{board, e2, e4};
            REQUIRE(!move.isEnPassant());
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
//            std::cout << board << std::endl;
            std::cout << promotionMove.isPromotion() << std::endl;
//            REQUIRE(false);
        }

        SECTION ("en passant") {
            Board board = Board::fromFEN("r3k2r/p2pqpb1/bn2pnp1/2pPN3/4P3/1pN2Q1p/PPPBBPPP/2KR2R1 w kq c6 0 5");
            std::cout << Move{board.getHistory().getCurrentFrame().previousMove} << std::endl;
//            std::cout << board << std::endl;
            Move enPassantMove{board, d5, c6};
            REQUIRE(enPassantMove.isEnPassant());
            board.executeMove(enPassantMove);
//            std::cout << board << std::endl;
            board.unmakeMove();
//            std::cout << board << std::endl;
//            REQUIRE(false);
        }

    }

}

TEST_CASE("Board should implement move generation", "[board]") {
    SECTION("Board generates moves") {
        std::vector<std::pair<std::string, std::unordered_set<std::string>>> testCases = {
                {"r3k2r/5r2/8/8/8/8/8/R3K2R w KQkq - 0 1",                               {"A1B1", "A1C1", "A1D1", "A1A2", "A1A3", "A1A4", "A1A5", "A1A6", "A1A7", "A1A8", "H1F1", "H1G1", "H1H2", "H1H3", "H1H4", "H1H5", "H1H6", "H1H7", "H1H8", "E1D1", "E1D2", "E1E2", "E1C1"}},
                {"1n1b1n2/N1k5/p1NRPq1p/p1pPP1r1/3p1pQp/K2P2P1/bRP1PPBp/2r1B3 b - - 0 1",
                                                                                         {"A2B1", "A2B3", "A2C4", "A2D5", "D8E7", "C1A1", "C1B1", "C1D1", "C1E1", "C1C2", "G5G4", "G5E5", "G5F5", "G5H5", "G5G6", "G5G7", "G5G8", "F6E5", "F6F5", "F6E6", "F6G6", "F6E7", "F6F7", "F6G7", "F6H8", "B8C6", "B8D7", "F8E6", "F8G6", "F8D7", "F8H7",  "H2H1Q", "H2H1R", "H2H1B", "H2H1N", "F4F3",  "H4H3",  "A5A4",
                                                                                                                                                                                                                                                                                                                                                                                                                  "C5C4", "H6H5", "H4G3", "F4G3"}},
                {"N4B2/2Qp1pR1/R2nPPp1/1nPK2N1/pP1pPrb1/P2Pkp1P/pr3p1q/1B4b1 w - - 0 1",
                                                                                         {"D5E5", "E6E7", "C5C6", "A6B6", "E6F7", "E6D7", "G7H7", "A6A4", "G7F7", "A6A5", "G7G6", "E4E5", "C7B8", "H3H4", "F8E7", "C7D8", "A6D6", "F8D6", "G5H7", "A6A7", "A6C6", "C7A5", "B1C2", "B1A2", "C7B6", "G5F7", "C7C6", "C7D6", "C7A7", "C7B7", "G7G8",  "C7C8",  "C7D7",  "G5F3",  "A8B6",  "C5D6",  "H3G4"}},
                {"3B4/2R2npB/2P1K1Pp/1nP2p2/N4k2/4p3/3p2PP/N2b4 b - - 0 1",              {"B5D6", "B5D4", "F4G4", "D1A4", "D1F3", "D1B3", "B5C7", "D1C2", "B5A7", "B5A3", "F7H8", "D1E2", "F4E4", "F7G5", "D1H5", "F7E5", "F7D6", "D1G4", "F7D8", "B5C3", "E3E2", "H6H5"}},
                {"4qk2/P1p4P/n1p1pBPn/P1p1p1Pp/B1P2pp1/1bP1bR2/2P2NrQ/R3KNr1 w Q - 0 1", {"F2H3", "F2D3", "F2H1", "H2G2", "H2G1", "F3H3", "F3G3", "A1B1", "H2F4", "F2G4", "F3E3", "H2H1", "A4B3", "F2E4", "F6E7", "A1C1", "F3F4", "F6E5", "A4B5", "F6G7", "F2D1", "F6D8", "H2H3", "F6H8", "A1D1", "A1A3", "C2B3", "H2H5", "G6G7", "A4C6", "A7A8Q", "A7A8R", "A7A8B", "A7A8N", "H7H8Q", "H7H8R", "H7H8B", "H7H8N", "H2H4", "A1A2", "G5H6", "E1D1", "H2G3", "E1E2"}},
                {"rnbqkbnr/pppppppp/8/8/8/N7/PPPPPPPP/R1BQKBNR b KQkq - 0 1",            {"B7B6", "D7D5", "B7B5", "C7C5", "C7C6", "A7A6", "E7E6", "A7A5", "H7H6", "G8F6", "E7E5", "G7G6", "B8C6", "F7F5", "F7F6", "B8A6", "G7G5", "D7D6", "H7H5", "G8H6"}},
                {"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1",                                 {"H8H3", "H8H2", "H8H7", "A8D8", "E8G8", "H8H1", "A8B8", "A8A3", "A8A7", "A8A6", "E8F7", "A8A4", "A8C8", "A8A2", "A8A5", "E8E7", "A8A1", "H8H4", "E8D7", "H8H5", "H8H6", "E8C8", "H8F8", "H8G8", "E8D8", "E8F8"}},
                {"rnbqkbnr/ppppp1pp/8/1B3p2/4P3/8/PPPP1PPP/RNBQK1NR b KQkq - 1 4",       {"B7B6", "F5E4", "E7E5", "G7G6", "G7G5", "C7C5", "E7E6", "A7A6", "G8H6", "H7H6", "C7C6", "G8F6", "A7A5", "B8C6", "B8A6", "F5F4", "H7H5", "E8F7"}},
                {"rnbqkbnr/1ppppppp/p7/1B6/4P3/8/PPPP1PPP/RNBQK1NR b KQkq - 1 4",        {"F7F6", "E7E6", "B7B6", "H7H6", "E7E5", "F7F5", "G7G5", "C7C5", "A8A7", "G8H6", "G8F6", "G7G6", "B8C6", "H7H5", "C7C6", "A6A5", "A6B5"}},
                {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K1R1 b Qkq - 1 2", {"H3G2", "B6C4", "F6G8", "B6A4", "A6B7", "B4C3", "H8G8", "H8F8", "E7C5", "B6D5", "H8H6", "F6D5", "G7H6", "E7F8", "G7F8", "A8C8", "F6H5", "F6G4", "B6C8", "E8D8", "H8H7", "A6C8", "A8B8", "A6E2", "E7D8", "D7D6", "E8C8", "F6E4", "H8H5", "F6H7", "B4B3",  "E7D6",  "G6G5",  "C7C5",  "H8H4",  "C7C6",  "E6D5",  "A8D8",  "A6C4", "E8F8", "A6D3", "A6B5", "E8G8"}},
                {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/4P3/2N2Q1P/pPPBBP1P/R3K1R1 w Qkq - 0 5",   {"G1G6", "F3G2", "F3F6", "F3D3", "G1F1", "A1D1", "F3G3", "E2F1", "D2C1", "D2E3", "E5C6", "G1G3", "A1A2", "E2C4", "E2D1", "E5G4", "C3D1", "F3G4", "E5D7", "E5D3", "G1G5", "E2D3", "G1G4", "A1B1", "G1H1", "E2A6", "E1C1", "G1G2", "F3F4", "F3H1", "B2B4",  "A1C1",  "F3E3",  "F3F5",  "E1D1",  "C3B5",  "D2F4",  "C3A2",  "H3H4", "C3B1", "C3A4", "E2B5", "E5C4", "E5G6", "E5F7", "B2B3", "D5D6", "D5E6", "D2G5", "D2H6", "F3H5", "E1F1"}},
                {"rnb1kbnr/pp1ppppp/8/q1p5/3P4/7N/PPP1PPPP/RNBQKB1R w KQkq - 1 5",       {"D1D2", "C1D2", "B1C3", "B1D2", "C2C3", "B2B4"}},
                {"r3k2r/p1ppqpb1/b3pnp1/3PN3/1p2P3/2N1nQ1p/PPPB1PPP/R2B1K1R w kq - 4 5", {"F1E1", "F1G1"}},
                {"r3k2r/p2p1pb1/bn1qpnp1/2pPN3/1p2P3/2N2Q1p/PPP1BPPP/R2KB2R w kq - 0 5", {"H1G1", "E1D2", "D1D2", "A1C1", "G2G4", "H1F1", "F3D3", "E5G4", "C3A4", "B2B3", "F3H5", "D1C1", "C3B1", "E5C6", "A1B1", "F3G3", "E5F7", "E5G6", "A2A4", "G2H3", "E5C4", "E2A6", "F3E3", "F3H3", "F3F4", "C3B5", "F3F5", "F3F6", "E5D7", "F3G4", "E5D3",  "E2B5",  "A2A3",  "E2D3",  "E2C4",  "E2F1",  "G2G3"}},
                {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q2/PPPBBP1P/3RK2b w kq - 0 5",    {"F3H3", "C3B1", "F3G2", "F3H1", "F3H5", "F3F5", "D2E3", "D2F4", "E5C6", "D2G5", "F3G3", "D1B1", "E2F1", "D2H6", "E2B5", "F3G4", "E2D3", "F3D3", "E5C4", "E2A6", "D1A1", "E5D3", "E2C4", "E5G4", "C3A4", "E5G6", "H2H4", "E5F7", "D2C1", "A2A4", "A2A3",  "F3F4",  "B2B3",  "H2H3",  "D5D6",  "C3B5",  "F3F6",  "E5D7",  "D5E6", "F3E3", "E1F1", "D1C1"}},
                {"rnbq1bnr/pppQpkpp/8/5p2/8/2P5/PP1PPPPP/RNB1KBNR b KQ - 0 6",           {"B7B5", "G8H6", "H7H6", "F5F4", "G8F6", "B7B6", "D8D7", "B8C6", "G7G5", "B8A6", "A7A5", "C8D7", "C7C5", "A7A6", "B8D7", "D8E8", "C7C6", "G7G6", "H7H5", "F7F6", "F7G6"}},
                {"5rk1/6pp/8/1PQ1N1br/5p2/PKpp4/4P3/7B w - - 0 1",                       {"E5C6", "B3A4", "E5G6", "C5E7", "C5C4", "E5F3", "C5A7", "C5D6", "C5C6", "H1G2", "H1F3", "E5F7", "H1E4", "C5F2", "A3A4", "C5C7", "E5D7", "C5F8", "E5D3", "B3B4", "H1C6", "E5G4", "C5D5", "C5D4", "C5G1", "C5B6", "H1A8", "B5B6", "C5C8", "H1B7", "B3C4",  "E2E3",  "E2E4",  "C5B4",  "E2D3",  "C5E3",  "C5C3",  "B3A2",  "E5C4", "B3C3"}},
                {"8/5k2/8/5p2/8/6K1/8/6Q1 b - - 3 2",                                    {}},
                {"8/8/4k3/5p2/5Q2/8/5K2/8 b - - 1 1",                                    {}},


        };

//        for (const auto& testCase : testCases) {

        for (size_t i = 0 ; i < testCases.size() ; ++i) {
            if (i == 16) {
                std::cout << "debug" << std::endl;
            }
            const auto& testCase = testCases[i];
            const Board& board = Board::fromFEN(testCase.first);
            auto onlyCaptureMoves = board.getMoves(true);
            auto allMoves = board.getMoves(false);

            TestHelpers::verifyMoveList(onlyCaptureMoves, board, i, true);
            TestHelpers::verifyMoveList(allMoves, board, i, false);
//            TestHelpers::verifyMoveList(correctMoves, testCase.second, board, i);
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

        for (
            const auto& move
                : moves) {
            board.
                    executeMove(Move::fromString(move, board)
            );
        }

//        std::cout << board.toFEN() << std::endl;

        auto generatedMoves = board.getMoves();
        std::unordered_set<std::string> correctMoves = TestHelpers::HelperEngineInterface{}.getMoves(board.toFEN(), 0);

//        std::cout << TestHelpers::vectorToString(generatedMoves, "\t") << std::endl;

        TestHelpers::verifyMoveList(generatedMoves, board,
                                    0, 0);
    }

    SECTION ("Random tests2") {
        Board board = Board::fromFEN("r3k2r/p1ppqpb1/Bn2pnp1/3PN3/4P3/1pN2Q1p/PPPB1PPP/R3K1R1 b Qkq - 0 1");

        Move move = {board, h3, g2};
        board.
                executeMove(move);

//        std::cout << board << std::endl;
//        REQUIRE(false);
    }

    SECTION ("Random tests3") {
        Board board = Board::fromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/4P3/1pN2Q1p/PPPBBPPP/R3K1R1 w Qkq - 0 1");
        Move move{board, g2, h3};
//        board.executeMove(move);
        REQUIRE(!move.isEnPassant());
        REQUIRE(!move.isDoublePawnPush());

//        TestHelpers::verifyMoveList(board.getMoves(), {}, board, 0);
    }

    SECTION ("Random tests4") {
        Board board = Board::fromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q2/PPPBBPpP/R3KR2 b Qkq - 0 1");
//        std::cout << board << std::endl;

        const std::vector<Move>& vector = board.getMoves();
//        TestHelpers::verifyMoveList(vector, {}, board, 0);

    }

    SECTION ("Random tests5") {
        Board board = Board::fromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q2/PPPBBPpP/3RK2R b Kkq - 0 1");
        Move move{board, g2, h1, PieceTypes::BISHOP};
        board.
                executeMove(move);
        REQUIRE(!board.getHistory().getCurrentFrame().castlingStatus.canCastle(WHITE, Specials::KING_CASTLE));
    }

    SECTION ("Random tests6") {
        Board board = Board::fromFEN("r3k2r/p1ppqpb1/b3pnp1/3PN3/1pn1P3/1PN2Q1p/P1PBBPPP/R3K2R w KQkq - 1 2");
        Move move{board, b3, c4};
        board.
                executeMove(move);

        Move move2{board, b4, c3};
        REQUIRE(!move2.isEnPassant());
    }

    SECTION ("Random tests7") {
        Board board = Board::fromFEN("rnbqk1nr/ppppppbp/8/6p1/8/1P5N/P1PPPPPP/RNBQKBR1 b Qkq - 0 3");
        Move move = {board, g7, a1};
        board.
                executeMove(move);
//        std::cout << board << std::endl;

    }

    SECTION ("Random tests8") {
        Board board = Board::fromFEN("rnbqkbnr/p1pppppp/8/8/Pp6/R6N/1PPPPPPP/1NBQKB1R w Kkq - 0 3");
        Move move{board, g2, g4};
        board.
                executeMove(move);
//        std::cout << board << std::endl;
        Move move2{board, b4, a3};
        REQUIRE(!move2.isEnPassant());
    }

    SECTION ("Random tests9") {
        Board board = Board::fromFEN("rnbqkbnr/pppp1ppp/8/4p3/8/7N/PPPPPPPP/RNBQKBR1 b Qkq - 1 2");
//        std::cout << board << std::endl;
        const std::vector<Move>& moves = board.getMoves();
        TestHelpers::verifyMoveList(moves, board,
                                    0, 0);
    }

    SECTION ("En passants aren't generated if the destination square isn't in the checkmask") {
        Board board = Board::fromFEN("rn1q1bnr/pp5p/2p2p2/6p1/4pk2/5N2/PPPP1PPP/RNB1R1K1 w - - 3 3");
        board.executeSequenceOfMoves({"d2d4"});
        std::cout << board.toFEN() << std::endl;

        TestHelpers::verifyMoveList(board.getMoves(), board, 0, 0);
    }

    SECTION ("A sequence of moves doesn't crash perft(6)") {
        Board asd = Board::fromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        asd.executeSequenceOfMoves({"e2e4", "d7d6", "d2d4", "g8f6", "d1f3", "e7e5", "c1g5", "e5d4", "a2a4", "f8e7", "f1c4", "e8g8", "b1d2", "c8g4", "f3g3", "d8d7", "g3h4", "h7h6", "g5f6", "e7f6", "h4g3", "f6e5"});
        REQUIRE(TestHelpers::perft(asd, 6) == -960454105);
    }

    SECTION ("Qperft is correct") {
//        Board board = Board::fromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
//        Board board = Board::fromFEN("r3k2r/p1ppqpb1/b3pnp1/3PN3/1pn1P3/2N2Q1p/PPPBBPPP/R4K1R w KQkq - 0 1");

        Board board;
//        Board board = Board::fromFEN("8/8/4k3/5p2/3Q4/8/5K2/8 w - - 0 1");


        thc::ChessRules cr;
//        cr.Forsyth("8/8/4k3/5p2/3Q4/8/5K2/8 w - - 0 1");
//        cr.Forsyth("rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1");
//        cr.Forsyth("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
//        cr.Forsyth("r3k2r/p1ppqpb1/b3pnp1/3PN3/1pn1P3/2N2Q1p/PPPBBPPP/R4K1R w KQkq - 0 1");
//        board.executeMove(Move{board, e2, e4});
//        board.executeMove(Move{board, e7, e6});

//        TestHelpers::HelperEngineInterface{}.getMoves(board.toFEN());

//        TestHelpers::analyzePerftProblem(board, cr, 3);


//        TestHelpers::perft(board, cr, 4);


////
        REQUIRE(TestHelpers::perft(board, 3) == 8902);
        REQUIRE(TestHelpers::perft(board, 4) == 197281);
        REQUIRE(TestHelpers::perft(board, 5) == 4865609);
        REQUIRE(TestHelpers::perft(board, 6) == 119060324);

//        BENCHMARK_ADVANCED("perft(4)") (Catch::Benchmark::Chronometer meter) {
//                Attacks::getInstance();
//                meter.measure([&] { return TestHelpers::perft(board, 4); });
//            };
//        BENCHMARK ("perft(6)") {
//            return TestHelpers::perft(board, 6);
//        };

//        REQUIRE(TestHelpers::perft(board, 5));
//        REQUIRE(TestHelpers::perftTHC(cr, 5));
//        REQUIRE(TestHelpers::perft(board, 5) == TestHelpers::perftTHC(cr, 5));

//        Board asd = Board::fromFEN("rn3rk1/pppq1pp1/3p3p/4b3/P1BpP1b1/6Q1/1PPN1PPP/R3K1NR w KQ - 0 13");
    }

}
