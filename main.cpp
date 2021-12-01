#include <iostream>
#include "BitboardOperations.h"
#include "Board.h"
#include "SlidingPieceRays.h"
#include "KnightAttacks.h"
#include "KingAttacks.h"

int main () {
    std::cout << d5 << std::endl;


    Board board;

    Move move = {board, e2, e4};
    board.executeMove(move);

    Move move2 = {board, d7, d5};
    board.executeMove(move2);

    Move move3 = {board, e4, d5};
    board.executeMove(move3);

    std::cout << board << std::endl;

    board.unmakeMove();
    std::cout << board << std::endl;

    board.unmakeMove();
    std::cout << board << std::endl;

    board.unmakeMove();
    std::cout << board << std::endl;

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
