#include <iostream>
#include "BitboardOperations.h"
#include "Board.h"
#include "Attacks.h"
#include "StringUtils.h"
#include "MyMath.h"
#include "BoardAnalysis.h"
#include "InBetween.h"

int main () {

    Board board = Board::fromFEN("k7/3r4/8/7b/8/3N1R2/8/3K4 w - - 0 1");
    std::cout << board << std::endl;

//    const Bitboard& checkMask = BoardAnalysis::getCheckMask(board, board.getTurn());
//    std::cout << checkMask << std::endl;
//    std::cout << InBetween::getInstance().getPath(a1, e5) << std::endl;

    const Bitboard& attackMask = BoardAnalysis::getPinMask(board, WHITE);
    std::cout << attackMask << std::endl;


//    std::cout << Attacks::getInstance().getKnightAttackGenerator().getAttackAt((Square) 1) << std::endl;

    std::exit(0);
//    std::cout << "moi" << std::endl;
//    std::cout << moves << std::endl;
//
    Move move = {board, e2, e4};
    board.executeMove(move);

    Move move2 = {board, d7, d5};
    board.executeMove(move2);
    const auto& moves = board.getMoves();

    for (const auto& m: moves) {
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
