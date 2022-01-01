#include <iostream>
#include "BitboardOperations.h"
#include "Board.h"
#include "Attacks.h"
#include "StringUtils.h"
#include "MyMath.h"
#include "BoardAnalysis.h"
#include "InBetween.h"

int main () {

    Board board = Board::fromFEN("4qk2/P1p4P/n1p1pBPn/P1p1p1Pp/B1P2pp1/1bP1bR2/2P2NrQ/R3KNr1 w Q - 0 1");
    std::cout << board << std::endl;

    Move move{board, h7, h8, PieceTypes::KNIGHT};
    board.executeMove(move);

    std::cout << board << std::endl;
    board.unmakeMove();
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


//    for (const auto& m : board.getMoves()) {
//        std::cout << m << ", ";
//    }
//
//    std::cout << board.getMoves().size() << std::endl;


//    std::cout << Attacks::getInstance().getKnightAttackGenerator().getAttackAt((Square) 1) << std::endl;

//    std::exit(0);
}
