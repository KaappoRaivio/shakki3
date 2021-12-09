#include <iostream>
#include "BitboardOperations.h"
#include "Board.h"
#include "SlidingPieceRays.h"
#include "KnightAttacks.h"
#include "Attacks.h"

int main () {
//    const Bitboard pawns = 0b1110000000000000000000000000000000000000000000000000;
//    const Bitboard occupancy = 0b1110000000100000010000000000000000000000000000000000;


//    std::cout << Attacks::getInstance().getPawnAttackGenerator().getPossiblePushesOnEmptyBoard(BLACK, a5) << std::endl;


//    Board board;
//    Board board = Board::fromFEN("1k6/8/3r4/8/8/3K4/8/8 w - - 0 1");
    Board board = Board::fromFEN("rnbqkbnr/ppp1pppp/3p4/8/2B1P3/8/PPPP1PPP/RNBQK1NR w KQkq - 0 1");
    std::cout << board << std::endl;

//    std::cout << (board.getPieces()[WHITE].all | board.getPieces()[BLACK].all) << std::endl;

//    std::cout << board << std::endl;
//    std::cout << board.getTurn() << std::endl;
//    std::cout << BoardAnalysis::isSquareAttacked(board, board.getPieces()[WHITE].boards[PieceTypes::KING].ls1b(), BLACK) << std::endl;
    const Bitboard& attackMask = BoardAnalysis::getAttackMask(board, WHITE);
    std::cout << attackMask << std::endl;

    std::cout << Attacks::getInstance().getKnightAttackGenerator().getAttackAt((Square )1) << std::endl;

    std::exit(0);
//    std::cout << "moi" << std::endl;
//    std::cout << moves << std::endl;
//
    Move move = {board, e2, e4};
    board.executeMove(move);

    Move move2 = {board, d7, d5};
    board.executeMove(move2);
    const auto& moves = board.getMoves();

    for (const auto& m : moves) {
        std::cout << m << ", ";
    }
    std::cout << std::endl;
    std::cout << board << std::endl;
//
//
//    Move move3 = {board, e4, d5};
//    board.executeMove(move3);
//
//    std::cout << board << std::endl;
//
//    board.unmakeMove();
//    std::cout << board << std::endl;
//
//    board.unmakeMove();
//    std::cout << board << std::endl;
//
//    board.unmakeMove();
//    std::cout << board << std::endl;

//    std::cout << board << std::endl;
//    std::cout << board << std::endl;

//    SlidingPieceRays slidingPieceRays;
//    KingAttacks attacks;

//    Bitboard occupancy = 0x1e4000e640e04800;
//    Bitboard occupancy '= 0b01100110011001100;
//    for (const auto& bit : occupancy) {
//        std::cout << bi
//    }'

//    std::cout << occupancy << std::endl;
//
//    for (const Square& bit : occupancy) {
//        std::cout << bit << std::endl;
//    }

//    std::cout << occupancy << std::endl;
//
//    std::cout << attacks.getKingAttackAt(5, 5) << std::endl;
//    std::cout << "done\n";

    return 0;
}
