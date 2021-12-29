#include <iostream>
#include "BitboardOperations.h"
#include "Board.h"
#include "Attacks.h"
#include "StringUtils.h"
#include "MyMath.h"
#include "BoardAnalysis.h"
#include "InBetween.h"

int main () {

    Board board = Board::fromFEN("r3k2r/5r2/8/8/8/8/8/R3K2R w KQkq - 0 1");
    std::cout << board << std::endl;

//    const Bitboard& checkMask = BoardAnalysis::getCheckMask(board, board.getTurn());
//    std::cout << checkMask << std::endl;
//    std::cout << InBetween::getInstance().getPath(a1, e5) << std::endl;

//    const Bitboard& pinMask = BoardAnalysis::getPinMask<BOTH>  (board, WHITE);
//    std::cout << pinMask << std::endl;
//
//    const Bitboard& checkMask = BoardAnalysis::getCheckMask(board, WHITE);
//    std::cout << checkMask  << std::endl;
//
//    const Bitboard& attackMask = BoardAnalysis::getAttackMask(board, BLACK);
//    std::cout << attackMask  << std::endl;


    for (const auto& m : board.getMoves()) {
        std::cout << m << ", ";
    }

    std::cout << board.getMoves().size() << std::endl;


//    std::cout << Attacks::getInstance().getKnightAttackGenerator().getAttackAt((Square) 1) << std::endl;

    std::exit(0);
}
